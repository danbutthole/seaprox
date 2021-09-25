/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "proxy_side.h"
#include "utils_ptr.h"

#ifndef PROXY_CONNECTION_H
#define PROXY_CONNECTION_H

struct proxy_connection {
	char *description; /*!<Like "http[<src>-><dst>]" */
	struct proxy_side *source; /*!< Source side of the connection */
	struct proxy_side *dest; /*!< Destination side of the connectiond */
};

#endif /* PROXY_CONNECTION_H */
