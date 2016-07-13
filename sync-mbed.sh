#!/bin/bash
# This script is for syncing the microbit-samples repository on GitHub to
# developer.mbed.org
# usage:
# cd microbit-samples
# (optional) git pull etc
# (optional) - update source/microbit.lib to point to the latest microbit.lib on mbed
# ./sync-mbed.sh

set -u
set -x

MBED_USERNAME="LancasterUniversity"
MBED_URL="developer.mbed.org"
MICROBIT_TEAM="teams/microbit"
EXAMPLES_PATH="source/examples"
EXAMPLE_PREPEND="microbit"

GIT_REVISION=$(git describe --tags)
EXAMPLES=$(ls "$EXAMPLES_PATH")
#Or just do a subset:
#EXAMPLES="hello-world invaders logic-gates proximity-heart simple-animation simple-radio-rx simple-radio-tx snake"

#Feeling uncertain? Uncomment this and you get to 'approve' each line
#trap read debug

for example in $EXAMPLES; do
	ENAME="$EXAMPLE_PREPEND-$example"
	cd "$EXAMPLES_PATH/$example" || exit
	CODE="https://$MBED_USERNAME@developer.mbed.org/$MICROBIT_TEAM/code/$ENAME"
	if [ ! -d .hg ]; then
		hg init
	fi
	FILES="$(git ls-files)";
	hg pull $CODE
	hg update
	#clobber everything with the versions in git
	for FILE in $FILES; do
		git checkout "$FILE"
	done
	cp ../../*.lib ./
	hg add *
	hg commit -u $MBED_USERNAME -m "Update to git: $GIT_REVISION"
	PUSH="https://developer.mbed.org/$MICROBIT_TEAM/code/$ENAME"
	hg push "$PUSH"
	rm *.lib
	cd ../../../
done

