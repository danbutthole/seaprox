/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "proxy_connection.h"
#include "proxy_side.h"
#include "utils_ptr.h"

#ifndef PROXY_STRUCT_HELPERS_H
#define PROXY_STRUCT_HELPERS_H

/**
 * Returns a proxy connection from a side.
 * Based on side type, work out position of connection.
 * @param[in] side the proxy side of interest
 * @param[out] connection the proxy connection in relation to side, NULL if
 *             error.
 */
struct proxy_connection *proxy_side_get_connection(struct proxy_side *side);

/**
 * Returns other side of proxy connection given a side.
 * Based on proxy side type, get the other side of the proxy connection.
 * @param[in] side the proxy side of interest
 * @param[out] other side of the proxy connection, NULL if error.
 */
struct proxy_side *proxy_side_get_other_side(struct proxy_side *side);

/**
 * Returns the proxy side given the proxy side data.
 * See title.
 * @param[in] data data for the side of interest.
 * @param[out] side the proxy side for the given data.
 */
struct proxy_side *proxy_data_get_side(void *data);

#endif /* PROXY_STRUCT_HELPERS_H */
