/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "proxy.h"

#ifndef PROXY_STATIC_H
#define PROXY_STATIC_H

/**
 * Makes a proxy listener object from inet prference, address and port details.
 * Read title.
 * @param[in] inet_preference preference of INET or INET6.
 * @param[in] address ip or ip6 address for binding to.
 * @param[in] port tcp port to bind to.
 * @param[in] listener set to new proxy listener or NULL on error
 * @param[out] success 0 on success, -errno on error
 */
int seaprox_make_proxy_static(enum seaprox_bind_preference inet_preference,
			      const char *address, uint16_t port,
			      struct seaprox_proxy_listener **listener);

#endif /* PROXY_STATIC_H */
