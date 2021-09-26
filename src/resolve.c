/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "resolve.h"

/**
 * Generic wrapper for `getaddrinfo`.
 * Takes in name, domain and socket type and returns `struct sockaddr`
 * @param[in] name name of the host to resolve/lookup
 * @param[in] domain the domain/family for the address lookup
 * @param[in] type type of socket for a connection
 * @param[in] port port number for the service
 * @param[in] result where the `struct sockaddr` result is stored
 * @param[in] result_len the sockaddr_len of the result
 * @param[out] success 0 on success `-errno` on error
 */
static int _resolve_inetX(const char *name, int domain, int type,
			  struct sockaddr *result, size_t *result_len)
{
	int ret = 0;

	struct addrinfo hints = { 0 };
	struct addrinfo *gai_result = NULL;

	int gai_ret = 0;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = domain;
	hints.ai_socktype = type;
	hints.ai_flags |= AI_CANONNAME;

	gai_ret = getaddrinfo(name, NULL, &hints, &gai_result);
	if (gai_ret < 0 || result == NULL) {
		ret = -errno;
	} else {
		if (*result_len >= gai_result->ai_addrlen) {
			memcpy(result, gai_result->ai_addr,
			       gai_result->ai_addrlen);
			*result_len = gai_result->ai_addrlen;
			ret = 0;
		} else {
			ret = -ENOSPC;
			*result_len = 0;
		}
		freeaddrinfo(gai_result);
	}

	return ret;
}

int resolve_inet(const char *name, int type, uint16_t port,
		 struct sockaddr *result, size_t *result_len)
{
	int ret = 0;
	struct sockaddr_in *cast = NULL;

	ret = _resolve_inetX(name, AF_INET, type, result, result_len);
	if (ret == 0) {
		cast = (struct sockaddr_in *)result;
		cast->sin_port = htons(port);
	}

	return ret;
}

int resolve_inet6(const char *name, int type, uint16_t port,
		  struct sockaddr *result, size_t *result_len)
{
	int ret = 0;
	struct sockaddr_in6 *cast = NULL;

	ret = _resolve_inetX(name, AF_INET6, type, result, result_len);
	if (ret == 0) {
		cast = (struct sockaddr_in6 *)result;
		cast->sin6_port = htons(port);
	}

	return ret;
}
