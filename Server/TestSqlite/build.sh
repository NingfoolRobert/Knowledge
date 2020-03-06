#!/bin/sh 

if [ ! -d "./build/Debug" ]; then 
	mkdir -p ./build/Debug 
fi 

if [ ! -d "./build/Release" ]; then 
	mkdir -p ./build/Release 
fi 

if [ ! -d "./proto/msg" ]; then 
	./proto/generate.sh 
fi 

function build_Debug()
{
	cd ./build/Debug 
	rm -rf *
	cmake -DCMAKE_BUILD_TYPE=Debug ../.. 
	make 
}

function build_Release()
{
	cd ./build/Release 
	rm -rf * 
	cmake -DCMAKE_BUILD_TYPE=Release ../.. 
	make 
}

if [ $# -gt 0 ]; then 
	Mod=$1;
else 
	Mod="Debug"
fi 
echo $Mod 
case $Mod in 
	Release)
		build_Release
		;;
	*)
		build_Debug
		;;
esac

