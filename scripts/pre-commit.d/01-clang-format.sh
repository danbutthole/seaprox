#!/bin/sh

BAD_LINT=0

for x in src/*.{c,h}
do
	LINES=$(clang-format --output-replacements-xml --Werror --sort-includes "${x}" | wc -l)
	if ((LINES > 3))
	then
		echo bad lint: ${x}
		BAD_LINT=1
	fi
done

if [ "${BAD_LINT}" != 0 ]
then
	echo run "./scripts/lint.sh"
fi

exit ${BAD_LINT}
