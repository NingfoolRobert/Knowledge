#!/bin/bash 

mkdir ./Service 

if[ ! -d "./build" ]; then 
	echo "build file not exist!"
else 
	if [! -f "./build/bin/AlarmService" ]; then 
		echo "the exe not exist."
	else 
		cp  ./build/bin/AlarmService  ./Server/ 
	fi 
fi 


