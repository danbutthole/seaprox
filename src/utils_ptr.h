/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stddef.h>

#ifndef UTIL_PTR_H
#define UTIL_PTR_H

/**
 * Note: container_of _copied_ and modified from the linux kernel,
 *       take this into consideration when copied this code for your
 *       own purposes. For the purpose of seaprox, this is OK.
 */
#define container_of(ptr, type, member)                                        \
	({                                                                     \
		void *__mptr = (void *)(ptr);                                  \
		((type *)(__mptr - offsetof(type, member)))                    \
	})

#endif /* UTIL_PTR_H */
