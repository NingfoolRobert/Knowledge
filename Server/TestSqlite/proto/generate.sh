#!/bin/sh


if [! -d "./msg" ] then 
	mkdir msg 
fi 


protoc -I=./ --cpp_out=./msg/ ./*.proto 



