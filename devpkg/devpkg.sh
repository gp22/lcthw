#!/bin/bash

set -e

# go somewhere safe
cd /tmp

# get the source to base APR 1.7.4
curl -L -O https://dlcdn.apache.org//apr/apr-1.7.4.tar.gz

# extract it and go into the source
tar -xzvf apr-1.7.4.tar.gz
cd apr-1.7.4

# configure, make, make install
./configure
make
sudo make install

# reset and cleanup
cd /tmp
rm -rf apr-1.7.4 apr-1.7.4.targ.gz

# do the same with apr-util
curl -L -O https://dlcdn.apache.org//apr/apr-util-1.6.3.tar.gz

# extract
tar -xzvf apr-util-1.6.3.tar.gz
cd apr-util-1.6.3

# configure, make, make install
./configure --with-apr=/usr/local/apr
# you need that extra parameter to configure because
# apr-util can't really find it because... who knows.

make
sudo make install

# cleanup
cd /tmp
rm -rf apr-util-1.6.3* apr-1.7.4*
