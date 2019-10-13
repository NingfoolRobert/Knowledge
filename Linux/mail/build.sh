#!/bin/bash 

if [ ! -d "./build" ];then 
	mkdir build 
else 
	rm -rf ./build/*
fi  

cd ./build 

cmake ..

echo "start make...."

make 

