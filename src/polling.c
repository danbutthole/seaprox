/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "polling.h"
#include "proxy.h"
#include "proxy_side.h"

struct seaprox_poll_context {
	int fd; /*!< File descriptor for this poll object */
	size_t max_num_listeners; /*!< Maximum number of listeners allowed */
	size_t max_num_sides; /*!< Maxumum number of sides allowed */
	struct seaprox_proxy_listeners **listeners; /*!< max_num_listeners of
						         pointers to listeners
							 */
	struct seaprox_proxy_side **sides; /*!< max_num_sides of pointers to
					        sides */
	struct epoll_event *events; /*!< max_num_listeners + max_num_sides of
			          	 `struct epoll_event`s. */
};

int seaprox_poll_allocate_context(size_t max_num_listeners,
				  size_t max_num_sides,
				  struct seaprox_poll_context **result)
{
	int fd = -1;
	int ret = 0;
	struct seaprox_poll_context *ctx = NULL;

	*result = NULL;

	fd = epoll_create1(0);
	if (fd < 0) {
		ret = -errno;
		goto end;
	}

	ctx = calloc(sizeof(*result), 1);
	if (ctx == NULL) {
		ret = -ENOSPC;
		goto cleanup_epoll;
	}
	ctx->fd = fd;

	ctx->listeners = calloc(sizeof(ctx->listeners[0]), max_num_listeners);
	if (ctx->listeners == NULL) {
		ret = -ENOSPC;
		goto cleanup_ctx;
	}
	ctx->max_num_listeners = max_num_listeners;

	ctx->sides = calloc(sizeof(ctx->sides[0]), max_num_sides);
	if (ctx->sides == NULL) {
		ret = -ENOSPC;
		goto cleanup_ctx_listeners;
	}
	ctx->max_num_sides = max_num_sides;

	ctx->events = calloc(sizeof(*(ctx->events)),
			     max_num_listeners + max_num_sides);
	if (ctx->events == NULL) {
		ret = -ENOSPC;
		goto cleanup_ctx_sides;
	}
	return 0;

cleanup_ctx_sides:
	free(ctx->sides);
cleanup_ctx_listeners:
	free(ctx->listeners);
cleanup_ctx:
	free(ctx);
cleanup_epoll:
	close(fd);
end:
	return ret;
}

void seaprox_poll_deallocate_context(struct seaprox_poll_context *ctx)
{
	if (ctx) {
		if (ctx->fd) {
			close(ctx->fd);
		}
		if (ctx->listeners) {
			free(ctx->listeners);
		}
		if (ctx->sides) {
			free(ctx->sides);
		}
		free(ctx);
	}
}

/**
 * Finds a side in a poll context.
 * See title.
 * @param[in] ctx the poll context haystack.
 * @param[in] side the side needle.
 * @param[out] found 0, 1 if found.
 */
static size_t _find_proxy_side(struct seaprox_poll_context *ctx,
			       struct seaprox_proxy_side *side)
{
	for (size_t i = 0; i < ctx->max_num_sides; i++) {
		if (ctx->sides[i] == side) {
			return i;
		}
	}

	return ctx->max_num_sides;
}

/**
 * Finds a side free slot in a poll context.
 * See title.
 * @param[in] ctx the poll context haystack.
 * @param[out] slot number of slot if found free, >ctx->max_num_sides if full.
 */
static size_t _find_proxy_side_slot(struct seaprox_poll_context *ctx)
{
	for (size_t i = 0; i < ctx->max_num_sides; i++) {
		if (ctx->sides[i] == NULL) {
			return i;
		}
	}

	return ctx->max_num_sides + 1;
}

int seaprox_poll_add_proxy_side(struct seaprox_poll_context *ctx,
				struct seaprox_proxy_side *side,
				uint32_t events)
{
	int ret = 0;
	int epoll_fd = ctx->fd;
	int side_fd = side->fd;
	size_t free_slot = ctx->max_num_sides + 1;
	struct epoll_event ev = { 0 };

	if (_find_proxy_side(ctx, side) > ctx->max_num_sides) {
		return -EALREADY;
	}

	free_slot = _find_proxy_side_slot(ctx);
	if (free_slot > ctx->max_num_sides) {
		return -ENOSPC;
	}

	ev.events = events;
	ev.data.fd = side_fd;
	ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, side_fd, &ev);
	if (ret) {
		ret = -errno;
		goto end_error;
	}

	ctx->sides[free_slot] = side;

	return 0;

end_error:
	return ret;
}

int seaprox_poll_modify_proxy_side(struct seaprox_poll_context *ctx,
				   struct seaprox_proxy_side *side,
				   uint32_t events)
{
	int ret = 0;
	int epoll_fd = ctx->fd;
	int side_fd = side->fd;
	struct epoll_event ev = { 0 };

	if (_find_proxy_side(ctx, side) > ctx->max_num_sides) {
		return -EBADR;
	}

	ev.events = events;
	ev.data.fd = side_fd;
	ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, side_fd, &ev);
	if (ret) {
		ret = -errno;
		goto end_error;
	}

	return 0;

end_error:
	return ret;
}

int seaprox_poll_remove_proxy_side(struct seaprox_poll_context *ctx,
				   struct seaprox_proxy_side *side)
{
	int ret = 0;
	int epoll_fd = ctx->fd;
	int side_fd = side->fd;
	size_t found_slot = ctx->max_num_sides + 1;
	struct epoll_event ev = { 0 };

	found_slot = _find_proxy_side(ctx, side);
	if (found_slot > ctx->max_num_sides) {
		return -EBADR;
	}

	ev.data.fd = side_fd;
	ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, side_fd, &ev);
	if (ret) {
		ret = -errno;
		goto end_error;
	}

	ctx->sides[found_slot] = NULL;

	return 0;

end_error:
	return ret;
}

static void _find_side_by_fd(struct seaprox_poll_context *ctx, int fd,
			     struct seaprox_proxy_side **result)
{
	*result = NULL;

	for (size_t i = 0; i < ctx->max_num_sides; i++) {
		if (ctx->sides[i]->fd == fd) {
			*result = ctx->sides[i];
			return;
		}
	}
}

int seaprox_poll_run_one(struct seaprox_poll_context *ctx)
{
	int ret = 0;
	struct seaprox_proxy_side *side = NULL;

	ret = epoll_wait(ctx->fd, ctx->events,
			 ctx->max_num_listeners + ctx->max_num_sides, 100);
	if (ret < 0) {
		ret = -errno;
		goto error_end;
	}

	for (int i_event = 0; i_event < ret; i_event++) {
		side = NULL;
		_find_side_by_fd(ctx, ctx->events[i_event].data.fd, &side);
		if (side != NULL && side->poll_callback) {
			side->poll_callback(side, ctx->events[i_event].events);
		}
	}

	return ret;

error_end:
	return ret;
}
