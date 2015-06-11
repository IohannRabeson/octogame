#!/bin/sh

DIRS="Managers Firefly Octo Map Physics Decors Main Managers"

cppcheck --inline-suppr --quiet --enable=all --error-exitcode=1 --force --std=c++11 -q --platform=unix64 -I $DIRS -I /usr/local/include/ -I ./octolib/includes $DIRS 2>&1 | grep -e "warning" -e "error" > cppcheck_results.txt & tail -f cppcheck_results.txt
if [ -s cppcheck_results.txt ]; then
	cat cppcheck_results.txt
	exit 1
fi
