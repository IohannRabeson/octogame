#!/bin/sh

DIRS="Managers Firefly Octo Map Physics Decors Main Managers"

cppcheck -v --check-config --error-exitcode=1 --force --std=c++11 -q --platform=unix64 -I $DIRS /usr/local/include/ -I ./octolib/includes $DIRS

if [ "$?" != "0" ]; then
	exit 1
fi
