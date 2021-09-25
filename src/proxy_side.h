/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdint.h>

#ifndef PROXY_SIDE_H
#define PROXY_SIDE_H

struct proxy_side;

enum proxy_state {
	IDLE,
	CONNECTING,
	CONNECTED,
};

enum proxy_side_type {
	SOURCE,
	DEST,
};

/**
 * Function to call on event during CONNECTING state.
 * See title.
 * @param[in] side "this" connection side
 * @param[out] success -errno on error, 0 on done, 1 on more
 */
typedef int (*proxy_connecting_fn)(struct proxy_side *side);

/**
 * Function to call on an epoll event for the side's fd.
 * See title.
 * @param[in] side "this" connection side
 * @param[out] success -errno on error, 0 on success
 */
typedef int (*epoll_handler_fn)(struct proxy_side *side, uint32_t events);

struct proxy_side {
	void *proxy_side_data; /*!< Opaque to implementation */
	enum proxy_side_type side; /*!< Type of this side */
	char *description; /*!< Like "ipv4[1.2.3.4:3234]" */
	int fd; /*!< File descriptor for this side */
	enum proxy_state state; /*!< State of the connection setup */
	proxy_connecting_fn connecting_callback; /*!< Callback for during
						   `CONNECTING` */
	epoll_handler_fn epoll_callback; /*!< Callback for epoll events */
};

#endif /* PROXY_SIDE_H */
