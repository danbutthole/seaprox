#!/bin/sh

set -e

HERE=$(dirname ${0})

# Check with gcc
make -C "${HERE}/../src/" clean
make CC=gcc -C "${HERE}/../src/"

# Check with clang
make -C "${HERE}/../src/" clean
make CC=clang -C "${HERE}/../src/"

# Final clean
make -C "${HERE}/../src/" clean_headers
