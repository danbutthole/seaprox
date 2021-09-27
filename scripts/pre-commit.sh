#!/bin/sh

set -e

for x in scripts/pre-commit.d/*.sh
do
	${x}
done
