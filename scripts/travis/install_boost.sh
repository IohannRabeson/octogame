#!/bin/sh

set -ev

sudo add-apt-repository -y ppa:boost-latest/ppa
sudo apt-get -qq update
sudo apt-get install libboost-test1.55-dev
sudo apt-get install libboost-random1.55-dev
