/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stddef.h>

#include "proxy.h"
#include "proxy_side.h"

#ifndef POLLING_H
#define POLLING_H

struct seaprox_poll_context;

/**
 * Allocates and returns a `poll_context` object.
 * See title, and `seaprox_poll_deallocate_context`.
 * @param[in] max_num_listeners maximum allowed number of proxy listeners
 * @param[in] max_num_sides maximum allowed number of proxy sides
 * @param[in] result returned pointer to poll context, or NULL if error.
 * @param[out] success 0 if success, -errno if error;
 */
int seaprox_poll_allocate_context(size_t max_num_listeners,
				  size_t max_num_sides,
				  struct seaprox_poll_context **result);

/**
 * Deallocates/frees `poll_context` object.
 * See title, and `seaprox_poll_allocate_context`.
 * @param[in] ctx pointer to `poll_context` to be deallocated.
 */
void seaprox_poll_deallocate_context(struct seaprox_poll_context *ctx);

/**
 * Adds a `seaprox_proxy_side` to the polling context for given events.
 * See title.
 * @param[in] ctx `seaprox_poll_context` to which the `seaprox_proxy_side` will
 * 		   be added.
 * @param[in] side `seaprox_proxy_side` to be added to the
 * 		   `seaprox_poll_context`.
 * @param[in] events events of interest to the `seaprox_proxy_side`.
 * @param[out] success 0 on success, -errno on error.
 */
int seaprox_poll_add_proxy_side(struct seaprox_poll_context *ctx,
				struct seaprox_proxy_side *side,
				uint32_t events);

/**
 * Adds a `seaprox_proxy_side` to the polling context for given events.
 * See title.
 * @param[in] ctx `seaprox_poll_context` forwhich the `seaprox_proxy_side` will
 * 		   be modified.
 * @param[in] side `seaprox_proxy_side` to be modified in the
 * 		   `seaprox_poll_context`.
 * @param[in] events events of interest to the `seaprox_proxy_side`.
 * @param[out] success 0 on success, -errno on error.
 */
int seaprox_poll_modify_proxy_side(struct seaprox_poll_context *ctx,
				   struct seaprox_proxy_side *side,
				   uint32_t events);

/**
 * Adds a `seaprox_proxy_side` to the polling context for given events.
 * See title.
 * @param[in] ctx `seaprox_poll_context` from which the `seaprox_proxy_side`
 * 		   will be removed.
 * @param[in] side `seaprox_proxy_side` to be removed from the
 * 		   `seaprox_poll_context`.
 * @param[out] success 0 on success, -errno on error.
 */
int seaprox_poll_remove_proxy_side(struct seaprox_poll_context *ctx,
				   struct seaprox_proxy_side *side);

/**
 * Adds a proxy listener to the polling context.
 * See title.
 * @param[in] ctx `seaprox_poll_context` to which the listener will be added.
 * @param[in] listener listener to be added to the `seaprox_poll_context`.
 * @param[out] success 0 on success, -errno on error.
 */
int seaprox_poll_add_proxy(struct seaprox_poll_context *ctx,
			   struct seaprox_proxy_listener *listener);

/**
 * Removes a proxy listener from the polling context.
 * See title.
 * @param[in] ctx `seaprox_poll_context` from which the listener will be
 * 		  removed.
 * @param[in] listener listener to be removed from the `seaprox_poll_context`.
 * @param[out] success 0 on success, -errno on error.
 */
int seaprox_poll_remove_proxy(struct seaprox_poll_context *ctx,
			      struct seaprox_proxy_listener *listener);

#endif /* POLLING_H */
