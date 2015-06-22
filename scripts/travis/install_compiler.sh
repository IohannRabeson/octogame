#!/bin/sh

set -e

echo "deb http://llvm.org/apt/precise/ llvm-toolchain-precise main" | tee -a /etc/apt/sources.list
echo "deb http://llvm.org/apt/precise/ llvm-toolchain-precise-3.5 main" | tee -a /etc/apt/sources.list
echo "deb http://ppa.launchpad.net/ubuntu-toolchain-r/test/ubuntu precise main" | tee -a /etc/apt/sources.list
apt-get update -qq
apt-get install -qq --allow-unauthenticated clang-3.5 
