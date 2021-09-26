/**
 * Copyright 2021 - 2021, Dan Farrell and the seaprox contributors
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define TEST_PORT (12345)

#include "resolve.h"

int print_sockaddr_in_in6(struct sockaddr *sa, size_t sa_len)
{
	int ret = 0;
	struct sockaddr_in *cast = NULL;
	struct sockaddr_in6 *cast6 = NULL;
	uint8_t *byte_ptr = NULL;
	uint16_t *word_ptr = NULL;

	switch (sa->sa_family) {
	case AF_INET:
		cast = (struct sockaddr_in *)sa;
		if (sa_len < sizeof(*cast)) {
			printf("error: sockaddr_in does not fit\n");
			ret = -1;
		} else {
			byte_ptr = (uint8_t *)&(cast->sin_addr.s_addr);
			printf("%d.%d.%d.%d", (int)byte_ptr[0],
			       (int)byte_ptr[1], (int)byte_ptr[2],
			       (int)byte_ptr[3]);
		}
		break;
	case AF_INET6:
		cast6 = (struct sockaddr_in6 *)sa;
		if (sa_len < sizeof(*cast6)) {
			printf("error: sockaddr_in does not fit\n");
			ret = -1;
		} else {
			word_ptr = (uint16_t *)&(cast6->sin6_addr.s6_addr);
			printf("%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
			       (int)htons(word_ptr[0]), (int)htons(word_ptr[1]),
			       (int)htons(word_ptr[2]), (int)htons(word_ptr[3]),
			       (int)htons(word_ptr[4]), (int)htons(word_ptr[5]),
			       (int)htons(word_ptr[6]),
			       (int)htons(word_ptr[7]));
		}
		break;
	default:
		printf("error: not inet or inet6\n");
		ret = 0;
	}

	return ret;
}

int test_resolve_inet(const char *name)
{
	struct sockaddr_in result = { 0 };
	size_t result_len = sizeof(result);
	uint8_t *byte_ptr = NULL;
	int ret = -1;

	int resolve_ret = resolve(name, SOCK_STREAM, TEST_PORT, RESOLVE_INET,
				  (struct sockaddr *)&result, &result_len);
	if (resolve_ret < 0) {
		printf("error: errno: %s\n", strerror(-resolve_ret));
	} else if (result_len != sizeof(result)) {
		printf("error: length of result mismatch: %lu vs %lu",
		       result_len, sizeof(result));
	} else if (result.sin_port != htons(TEST_PORT)) {
		printf("error: incorrect port: %d vs %d",
		       (int)htons(result.sin_port), TEST_PORT);
	} else {
		printf("[%s]: ", name);
		ret = print_sockaddr_in_in6((struct sockaddr *)&result,
					    result_len);
		printf("\n");
	}

	return ret;
}

int test_resolve_inet6(const char *name)
{
	struct sockaddr_in6 result = { 0 };
	size_t result_len = sizeof(result);
	uint16_t *word_ptr = NULL;
	int ret = -1;

	int resolve_ret = resolve(name, SOCK_STREAM, TEST_PORT, RESOLVE_INET6,
				  (struct sockaddr *)&result, &result_len);
	if (resolve_ret < 0) {
		printf("error: errno: %s\n", strerror(-resolve_ret));
	} else if (result_len != sizeof(result)) {
		printf("error: length of result mismatch: %lu vs %lu",
		       result_len, sizeof(result));
	} else if (result.sin6_port != htons(TEST_PORT)) {
		printf("error: incorrect port: %d vs %d",
		       (int)htons(result.sin6_port), TEST_PORT);
	} else {
		printf("[%s]: ", name);
		ret = print_sockaddr_in_in6((struct sockaddr *)&result,
					    result_len);
		printf("\n");
	}

	return ret;
}

int main(int argc, char **argv)
{
	int test_ret = 0;

	if (argc < 2) {
		printf("try:\n"
		       "\t%s <name0> [<name1> [<name2> [...]]]\n",
		       argv[0]);
		exit(1);
	}

	for (int i = 1; i < argc; i++) {
		test_ret = test_resolve_inet(argv[i]);
		if (test_ret) {
			exit(1);
		}
		test_ret = test_resolve_inet6(argv[i]);
		if (test_ret) {
			exit(1);
		}
	}

	return 0;
}
