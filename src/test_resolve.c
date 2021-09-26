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

int test_resolve_inet(const char *name)
{
	struct sockaddr_in result = { 0 };
	size_t result_len = sizeof(result);
	uint8_t *byte_ptr = NULL;
	int ret = -1;

	int resolve_ret = resolve_inet(name, SOCK_STREAM, TEST_PORT,
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
		byte_ptr = (uint8_t *)&(result.sin_addr.s_addr);
		printf("[%s]: %d.%d.%d.%d\n", name, (int)byte_ptr[0],
		       (int)byte_ptr[1], (int)byte_ptr[2], (int)byte_ptr[3]);
		ret = 0;
	}

	return ret;
}

int test_resolve_inet6(const char *name)
{
	struct sockaddr_in6 result = { 0 };
	size_t result_len = sizeof(result);
	uint16_t *word_ptr = NULL;
	int ret = -1;

	int resolve_ret =
		resolve_inet6(name, SOCK_STREAM, TEST_PORT,
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
		word_ptr = (uint16_t *)&(result.sin6_addr.s6_addr);
		printf("[%s]: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n", name,
		       (int)htons(word_ptr[0]), (int)htons(word_ptr[1]),
		       (int)htons(word_ptr[2]), (int)htons(word_ptr[3]),
		       (int)htons(word_ptr[4]), (int)htons(word_ptr[5]),
		       (int)htons(word_ptr[6]), (int)htons(word_ptr[7]));
		ret = 0;
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
