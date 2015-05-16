#!/bin/bash


echo 'This script is just a dumb script  to make it easy to get started. '
echo 'It does not address all options that are available with soapybrain, such as:'
echo '-  Oculus support'
echo ' or eventually'
echo '-  Leap motion support'
echo '-  Razor support'

mkdir build
cd build
ln -s ../shaders ./
cmake -DEXAMPLES_BUILD=true ../
make -j
cd -
echo "Build finished, visit ./build/soapybrain-examples to run examples"
