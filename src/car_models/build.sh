#!/usr/bin/bash

#export CPLUS_INCLUDE_PATH=/usr/local/include/gazebo-7:$CPLUS_INCLUDE_PATH
#export CPLUS_INCLUDE_PATH=/usr/local/include/sdformat-4.1:$CPLUS_INCLUDE_PATH
#export CPLUS_INCLUDE_PATH=/usr/local/include/ignition:$CPLUS_INCLUDE_PATH
#export CPLUS_INCLUDE_PATH=/usr/local/include/ignition/math2:$CPLUS_INCLUDE_PATH

if [ ! -d build ]; then
   mkdir build
fi

if [ ! -d plugins ]; then
   mkdir plugins
fi

cd build

cmake ../src/ -Wno-dev

make

cd ..

cp build/*.dylib plugins/
