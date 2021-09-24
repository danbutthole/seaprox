/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "proxy_connection.h"
#include "proxy_side.h"
#include "utils_ptr.h"

#ifndef PROXY_STRUCT_HELPERS_H
#define PROXY_STRUCT_HELPERS_H

struct proxy_connection *proxy_side_get_connection(struct proxy_side *side);

struct proxy_side *proxy_side_get_other_side(struct proxy_side *side);

struct proxy_side *proxy_data_get_side(void *data);

#endif /* PROXY_STRUCT_HELPERS_H */
