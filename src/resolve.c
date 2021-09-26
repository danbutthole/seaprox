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

int resolve_inet(const char *name, int type, uint16_t port,
		 struct sockaddr *result, size_t *result_len)
{
	int ret = 0;

	struct addrinfo hints = { 0 };
	struct addrinfo *gai_result = NULL;
	struct sockaddr_in *cast = NULL;

	int gai_ret = 0;
	void *ptr = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = type;
	hints.ai_flags |= AI_CANONNAME;

	gai_ret = getaddrinfo(name, NULL, &hints, &gai_result);
	if (gai_ret < 0 || result == NULL) {
		ret = -errno;
	} else {
		memcpy(result, gai_result->ai_addr, gai_result->ai_addrlen);
		freeaddrinfo(gai_result);
		*result_len = gai_result->ai_addrlen;
		cast = (struct sockaddr_in *)result;
		cast->sin_port = htons(port);
		ret = 0;
	}

	return ret;
}
