#!/bin/sh

HERE=$(dirname ${0})

for x in ${HERE}/../src/*.{c,h}
do
	clang-format -i --sort-includes "${x}"
done
