/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <errno.h>
#include <stdlib.h>

#include "proxy.h"
#include "proxy_static.h"

struct seaprox_proxy_static_context;

/**
 * Allocates and returns a `seaprox_proxy_static_context` object.
 * See title, and `seaprox_proxy_static_deallocate_context`.
 * @param[out] ctx returned pointer to proxy context, or NULL if error.
 */
struct seaprox_proxy_static_context *
_seaprox_proxy_static_allocate_context(void);

/**
 * Deallocates/frees `poll_context` object.
 * See title, and `seaprox_proxy_static_allocate_context`.
 * @param[in] ctx pointer to proxy context to be deallocated.
 */
void _seaprox_proxy_static_deallocate_context(
	struct seaprox_proxy_static_context *ctx);

/**
 * Binds a socket to a ip/port combination for given INET type.
 * Read title.
 * @param[in] ctx proxy static context to bind to address and port.
 * @param[in] inet_preference preference of INET or INET6.
 * @param[in] address ip or ip6 address for binding to.
 * @param[in] port tcp port to bind to.
 * @param[out] success 0 on success, -errno on error.
 */
int _seaprox_proxy_static_bind(struct seaprox_proxy_static_context *ctx,
			       enum seaprox_bind_preference inet_preference,
			       const char *address, uint16_t port);

/**
 * Takes a proxy static converts it to a proxy listener object.
 * Read title.
 * @param[in] ctx proxy static context to bind to address and port.
 * @param[out] success 0 on success, -errno on error.
 */
struct _seaprox_proxy_listener *
_seaprox_make_listener_from_static(struct seaprox_proxy_static_context *ctx);

int seaprox_make_proxy_static(enum seaprox_bind_preference inet_preference,
			      const char *address, uint16_t port,
			      struct seaprox_proxy_listener **listener)
{
	*listener = NULL;
	return -EINVAL;
}
