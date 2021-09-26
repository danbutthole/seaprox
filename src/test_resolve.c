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

int main(int argc, char **argv)
{
	int resolve_ret = 0;
	struct sockaddr_in result = { 0 };
	size_t result_len = sizeof(result);
	uint8_t *byte_ptr = NULL;

	if (argc < 2) {
		printf("try:\n"
		       "\t%s <name0> [<name1> [<name2> [...]]]\n",
		       argv[0]);
		exit(1);
	}

	for (int i = 1; i < argc; i++) {
		resolve_ret =
			resolve_inet(argv[i], SOCK_STREAM, TEST_PORT,
				     (struct sockaddr *)&result, &result_len);
		if (resolve_ret < 0) {
			printf("error: errno: %s\n", strerror(-resolve_ret));
			exit(1);
		} else if (result_len != sizeof(result)) {
			printf("error: length of result mismatch: %lu vs %lu",
			       result_len, sizeof(result));
			exit(1);
		} else if (result.sin_port != htons(TEST_PORT)) {
			printf("error: incorrect port: %d vs %d",
			       (int)htons(result.sin_port), TEST_PORT);
			exit(1);
		} else {
			byte_ptr = (uint8_t *)&(result.sin_addr.s_addr);
			printf("[%s]: %d.%d.%d.%d\n", argv[i], (int)byte_ptr[0],
			       (int)byte_ptr[1], (int)byte_ptr[2],
			       (int)byte_ptr[3]);
		}
	}

	return 0;
}
