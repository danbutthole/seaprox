/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdint.h>

#ifndef PROXY_H
#define PROXY_H

enum seaprox_bind_preference {
	BIND_INET,
	BIND_INET6,
};

struct seaprox_proxy_listener;

/**
 * Callback from poll for proxy listener.
 * See title.
 * @param[in] listener the listener object receiving events.
 * @param[in] events the events for the callback.
 * @param[out] success 0 on success, -errno on error.
 */
typedef int (*seaprox_poll_proxy_accept_callback)(
	struct seaprox_proxy_listener *listener, uint32_t events);

struct seaprox_proxy_listener {
	const char *address; /*!< Address to listen on */
	void *proxy_data; /*!< Opaque to proxy data */
	int fd; /*!< File descriptor bound to interface */
	seaprox_poll_proxy_accept_callback
		poll_callback; /*!< Callback on accept/readable */
};

#endif /* PROXY_H */
