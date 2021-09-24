#!/bin/sh

for x in src/*.{c,h}
do
	clang-format -i ${x}
done
