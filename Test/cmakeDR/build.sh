#!/bin/sh 


if [ ! -d "./build" ]; then 
	mkdir ./build 
fi 

cd ./build 

if [ $# -gt 0 ]; then 
	if [ "$1" = "Release" ]; then 
		if [ ! -d "./Release" ]; then 
			mkdir ./Release
		else 
			rm -rf ./Release/* 
			cd ./Release 
		fi 
		MOD="-DCMAKE_BUILD_TYPE=Release"
	else 
		if [ ! -d "./Debug" ]; then 
			mkdir ./Debug 
		else 
			rm -rf ./Debug/*
			cd ./Debug 
		fi 
		MOD="-DCMAKE_BUILD_TYPE=Debug"
	fi 
else 
	if [ ! -d "./Debug" ]; then 
			mkdir ./Debug 
	else 
			rm -rf ./Debug/*
			cd ./Debug 
	fi 	
	
	MOD="-DCMAKE_BUILD_TYPE=Debug"
fi 



cmake $MOD ../..

make 
 
	


