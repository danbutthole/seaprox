/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef RESOLVE_H
#define RESOLVE_H

enum inet_preference {
	RESOLVE_INET,
	RESOLVE_INET6,
};

/**
 * Performs INET and/or INET6 lookup and returns the result.
 * @param[in] name name of the host to resolve/lookup
 * @param[in] preference preference of inet versus inet6 addressing
 * @param[in] result where the `struct sockaddr` result is stored
 * @param[in] result_len the sockaddr_len of the result
 * @param[out] success 0 on success `-errno` on error
 */
int resolve(const char *name, enum inet_preference preference,
	    struct sockaddr *result, size_t *result_len);

/**
 * Performs INET lookup and returns the result.
 * @param[in] name name of the host to resolve/lookup
 * @param[in] result where the `struct sockaddr` result is stored
 * @param[in] result_len the sockaddr_len of the result
 * @param[out] success 0 on success `-errno` on error
 */
int resolve_inet(const char *name, struct sockaddr *result, size_t *result_len);

/**
 * Performs INET6 lookup and returns the result.
 * @param[in] name name of the host to resolve/lookup
 * @param[in] result where the `struct sockaddr` result is stored
 * @param[in] result_len the sockaddr_len of the result
 * @param[out] success 0 on success `-errno` on error
 */
int resolve_inet6(const char *name, struct sockaddr *result,
		  size_t *result_len);

#endif /* RESOLVE_H */
