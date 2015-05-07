#!/bin/sh

cppcheck --check-config --error-exitcode=1 --force --std=c++11 -q --platform=unix64 -I /usr/local/include/ -I ./includes/ -I ./octolib/includes ./includes/ ./src/

if [ "$?" != "0" ]; then
	exit 1
fi
