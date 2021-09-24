#!/bin/sh

CC=${1}
IN=${2}

${CC} -fsyntax-only ${IN}
if [ "${?}" = "0" ]
then
	echo y > "${IN}.ho"
else
	echo "${IN}: failed"
	exit 1
fi
