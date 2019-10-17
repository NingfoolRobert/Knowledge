#!/bin/bash 

mkdir ./Service 

if[ ! -d "./build" ]; then 
	echo "build file not exist!"
	if [ ! -f "./build.sh" ];
		echo "No find the file: build.sh "
	else 
		/bin/bash ./build.sh 
	fi 
else 
	if [! -f "./build/bin/AlarmService" ]; then 
		echo "the exe not exist."
	else 
		cp  ./build/bin/AlarmService  ./Server/ 
	fi 
fi 


