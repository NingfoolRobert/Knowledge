#!/bin/sh 


svrlist=("AlarmService")

svrpathlist=("/home/nbf/Work/Platform/Monitor/AlarmSvr/conf")

checkAndStart()
{
	echo $1
	#check service 
	PIDS=$(ps -ef|grep $1 |grep -v "grep"| awk '{print $2}')
	if [ -z "$PIDS" ]; then 
		sh -c "nohup setsid "$2/$1" &> /dev/null &"

		echo "start"
	fi 
	#start service 
}



for((i = 0; i < ${#svrlist[@]}; ++i)) do 
	
	checkAndStart  ${svrlist[i]} ${svrpathlist[i]}
done

exit 1
