#!/bin/sh

set -e

HERE=$(dirname ${0})

# Check with gcc
make -C "${HERE}/../src/" clean
make CC=gcc -C "${HERE}/../src/" headers

# Check with clang
make -C "${HERE}/../src/" clean
make CC=clang -C "${HERE}/../src/" headers

# Final clean
make -C "${HERE}/../src/" clean
