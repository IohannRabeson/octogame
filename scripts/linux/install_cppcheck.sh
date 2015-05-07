#!/bin/sh

git clone git://github.com/danmar/cppcheck.git cppcheck-repository
cd cppcheck-repository
echo "Building cppcheck..."
mkdir $PWD/../cppcheck_cfg/
make SRCDIR=build CFGDIR=$PWD/../cppcheck_cfg/ HAVE_RULES=yes
make install CFGDIR=$PWD/../cppcheck_cfg/
cd ..
