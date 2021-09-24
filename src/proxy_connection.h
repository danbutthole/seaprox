/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "proxy_side.h"
#include "utils_ptr.h"

#ifndef PROXY_CONNECTION_H
#define PROXY_CONNECTION_H

struct proxy_connection;

#define TO_PROXY_CONNECTION_FROM_SOURCE(source) \
	container_of(source, struct proxy_connection, source)

#define TO_PROXY_CONNECTION_FROM_DEST(dest) \
	container_of(dest, struct proxy_connection, dest)

struct proxy_connection {
	char* description;	/* Like "http[<src>-><dst>]" */
	struct proxy_side* source;
	struct proxy_side* dest;
};

#endif /* PROXY_CONNECTION_H */
