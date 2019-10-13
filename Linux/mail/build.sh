#!/bin/bash 

rm -rf ./build/*

cd ./build 
cmake ..
echo "start make...."
make 

