#!/bin/sh

set -e

packages="
libpthread-stubs0-dev
libx11-dev
libgl1-mesa-dev
libxrandr-dev
libfreetype6-dev
libglew1.5-dev
libjpeg8-dev
libsndfile1-dev
libopenal-dev
libudev-dev
libx11-xcb-dev
libxcb-icccm4-dev
libxcb-image0-dev
libxcb-util0-dev
libxcb-randr0-dev
libpcre3-dev
cmake
"

for package in $packages
do
	apt-get install -qq --allow-unauthenticated $package
done

git clone https://github.com/LaurentGomila/SFML.git sfml-repository
cd sfml-repository
git pull origin master
cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=TRUE .
make
make install
ldconfig /usr/local/lib
cd ..
