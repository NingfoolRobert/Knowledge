#!/bin/bash 

if [ ! -d "./proto" ]; then 
	git clone -b proto git@bjitgit3.cicc.com.cn:haoyuxing/ETL-Monitor-Platform.git ./proto
	exit 1	
fi 


#cd ./proto 
#./generate.sh 
#cd ..



if [ ! -d "./build" ];then 
	mkdir build 
else 
	rm -rf ./build/*
fi  

cd ./build 

Dbg="Debug"
Rel="Release"

if [ $# -gt 0 ]; then 
	if [ $1 == "Debug" ]; then 
		Mod=$Debg 
	else 
		Mod=$Rel 
	fi 
else 
	Mod=$Dbg 
fi 
echo $Mod
cmake -DCMAKE_BUILD_TYPE=$Mod ..

echo "start make...."

make 

