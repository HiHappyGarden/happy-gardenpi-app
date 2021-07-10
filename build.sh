#!/bin/bash

#export RASPBIAN_ROOTFS=$HOME/raspberrypi/rootfs
#export PATH=/opt/cross-pi-gcc/bin:$PATH
#export RASPBERRY_VERSION=1  
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=$HOME/raspberrypi/toolchain-rpi.cmake ..
make

/usr/bin/rsync -azP .. antoniosalsi@passy-raspberrypi:/projects/happy-gardenpi-app
