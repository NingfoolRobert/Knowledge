#!/bin/sh 

PRO_NAME=AgentService
START_PATH=/home/nbf/Work/Platform/Monitor/AgentService/conf/service.sh
Svr_Path=/home/nbf/Work/Platform/Monitor/AgentService/conf/AgentService 
param="start" 
ulimit -c unlimited 

while true 
do 
	NUM=`ps -ux|grep ${PRO_NAME} |wc -l`
	if [ "${NUM}" -lt "2" ]; then
	#ps -fe |grep ${PRO_NAME} |grep -v grep 
	#if [ ! $? -eq 0 ]; then 
		echo ${NUM}
		echo "${PRO_NAME} was Killed"
		sh -c "nohup setsid $PRO_NAME &> /dev/null &"
#		bash ${START_PATH} ${param}
		sleep 1
	fi 
	sleep 3
done 



