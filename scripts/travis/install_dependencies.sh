#!/bin/sh

set -e

packages="
libboost-test-dev
libboost-random-dev
"

for package in $packages
do
	apt-get install -qq --allow-unauthenticated $package
done
