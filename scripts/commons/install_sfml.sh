#!/bin/sh

set -e

git clone https://github.com/LaurentGomila/SFML.git sfml-repository
cd sfml-repository
git pull origin master
cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=TRUE .
make
make install
ldconfig /usr/local/lib
cd ..
