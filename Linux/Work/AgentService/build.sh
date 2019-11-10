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
		fi 
		
		cd ./Release 
		MOD="-DCMAKE_BUILD_TYPE=Release"
	else 
		if [ ! -d "./Debug" ]; then 
			mkdir ./Debug 
		else 
			rm -rf ./Debug/*
		fi 
		
		cd ./Debug 
		MOD="-DCMAKE_BUILD_TYPE=Debug"
	fi 
else  
	if [ ! -d "./Debug" ]; then 
			mkdir ./Debug 
	else 
			rm -rf ./Debug/*
	fi 	
	
	cd ./Debug 
	MOD="-DCMAKE_BUILD_TYPE=Debug"
fi 



cmake $MOD ../..

make 
 

#case $1 in 
#Release)
#	func_Release 
#	exit 0 
#	;;
#Debug)
#	func_Debug 
#	exit 0
#	;;
#make)
#	func_make 
#	exit 0 
#	;; 
#help)
#	usage
#	exit 0
#	;;
#esac


