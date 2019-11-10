#!/bin/sh 

if [ ! -d "./build" ]; then 
	mkdir ./build 
fi 

#if [ $# -gt 0]; then 
#	Mod=$1
#else 
#	Mod="Debug"
#fi

usage()
{
	echo "$0 {/Release/Debug/Make}"
}

func_Release()
{
	if [ ! -d "./build/Release" ]; then 
		mkdir ./build/Release 
	else
		rm -rf ./build/Release/*
	fi 
	
	cd ./build/Release 
	cmake -DCMAKE_BUILD_TYPE=Release ../..
	make 
}

func_Debug()
{
	if [ ! -d "./build/Debug" ]; then 
		mkdir ./build/Debug 
	else
		rm -rf ./build/Debug/*
	fi 
	
	cd ./build/Debug 
	cmake  -DCMAKE_BUILD_TYPE=Debug ../..
	make 
}

func_make()
{
	if [ ! -f "./build/Debug/Makefile" ]; then 
		func_Debug
		eixt 0
		;;
	else 
		cd ./build/Debug 
		make 
	fi 
}

case $1 in 
Release)
R)
r)
	func_Release 
	exit 0
	;;
Debug)
D)
d)
	exit 0
	;;
Make)
m)
M)
	func_make;
	exit 0 
	;;
*)
	usage 
	exit 0
	;;
esac
	
