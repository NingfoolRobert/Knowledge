#!/bin/bash 


ulimit -c unlimited


sudo echo "1" > /proc/sys/kernel/core_uses_pid 

if [ ! -d "/home/nbf/corefile" ]; then 
	mkdir -R /home/nbf/corefileone 
else 
	echo "exit"
fi

sysctl -w kernel.core_pattern=/home/nbf/corefileone/core-%e-%p-%t kernel.core_pattern=/home/nbfcorefileone/core-%e-%p-%t
