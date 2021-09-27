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
	size_t num_listeners; /*!< Number of listeners at the moment */
	size_t max_num_sides; /*!< Maxumum number of sides allowed */
	size_t num_sides; /*!< Number of sides at the moment */
	struct seaprox_proxy_listeners **listeners; /*!< max_num_listeners of
						         pointers to listeners
							 */
	struct seaprox_proxy_side **sides; /*!< max_num_sides of pointers to
					        sides */
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
	if (ctx->listeners) {
		ret = -ENOSPC;
		goto cleanup_ctx;
	}
	ctx->max_num_listeners = max_num_listeners;

	ctx->sides = calloc(sizeof(ctx->sides[0]), max_num_sides);
	if (ctx->sides) {
		ret = -ENOSPC;
		goto cleanup_ctx_listeners;
	}
	ctx->max_num_sides = max_num_sides;

	return 0;

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
