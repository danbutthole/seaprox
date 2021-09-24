#!/bin/sh

HERE=$(dirname ${0})

# Check with gcc
make -C "${HERE}" clean
make CC=gcc -C "${HERE}"

# Check with clang
make -C "${HERE}" clean
make CC=clang -C "${HERE}"
