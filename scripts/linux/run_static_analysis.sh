#!/bin/sh

DIRS="Managers Firefly Octo Map Physics Decors Main Managers"

cppcheck --quiet --inline-suppr --enable=warning --error-exitcode=1 --std=c++11 --force --platform=unix64 -I $DIRS -I/usr/local/include/ $DIRS
