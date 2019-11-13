# !/bin/sh

SVRNAME="./AgentService"

usage()
{
	echo "Usage:$0  {start|stop|restart|status}"
}

Test()
{
	pid=$(ps -ef|grep $SVRNAME |grep -v "grep"| awk '{print $2}')
	echo $pid
}

svr_start()
{
	SELFID='id -u'

	PIDS=$(ps -ef|grep $SVRNAME |grep -v "grep"| awk '{print $2}')
	#PIDS=$('ps -ef|grep -u $SELFID -f "$SVRNAME"')
	if [ -z "$PIDS" ]; then 
		sh -c "nohup setsid $SVRNAME &> /dev/null &"
		PIDS=$(ps -ef|grep $SVRNAME |grep -v "$0"|grep -v "grep"| awk '{print $2}')
		echo start $SVRNAME $PIDS 
	else 
		echo $SVRNAME is already running 
	fi 

}


svr_stop()
{
	SELFID='id -u'
	#PIDS="ps -ef|grep "./AlarmService"|grep -v "grep"|awk '{print $2}' "
	PIDS=$(ps -ef|grep $SVRNAME |grep -v "grep"| awk '{print $2}')
	if [ -z $PIDS ]; then 
		echo $SVRNAME is already stopped 
	else 
		sh -c " kill -9  $PIDS"
		echo service $SVRNAME  pid $PIDS  is killed.
		
	fi 
}

svr_restart()
{
	svr_stop  
	
	sleep 1
	
	svr_start
}

svr_status()
{
#	PIDS=$(ps -ef|grep -v )
	PIDS=$(ps -ef|grep $SVRNAME |grep -v "grep"| awk '{print $2}')
	if [ -z "$PIDS" ]; then 
		echo "service $SVRNAME not exist!"
	else 
		echo "service $SVRNAME  pid $PIDS "
	fi
}


case $1 in 
start)
	svr_start 
	;;
stop)
	svr_stop 
	;;
restart)
	svr_restart 
	;;
status)
	svr_status 
	;;
test)
	Test 
	;;
*)
	usage 
	;;
esac
