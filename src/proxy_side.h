/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "utils_ptr.h"

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

typedef int (*proxy_connecting_fn)(struct proxy_side* side);

#define TO_PROXY_SIDE(proxy_side_data) \
	container_of(proxy_side_data, struct proxy_side, proxy_size_data);

struct proxy_side {
	void* proxy_side_data;
	enum proxy_side_type side;
	char* description;		// like "ipv4[1.2.3.4:3234]
	int fd;
	enum proxy_state state;
	proxy_connecting_fn connecting_callback;
};

#endif /* PROXY_SIDE_H */
