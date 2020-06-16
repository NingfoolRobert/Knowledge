#!/bin/sh 

FileList=("/home/nbf/sh"  "/home/nbf/Work/Platform/Monitor/AlarmSvr/conf")


DATE=`date +%Y%m%d%H%M%S`

baklog()
{
	BakPath="$1/_baklog"
	if [ ! -d "$BakPath" ];then 
		mkdir -p  "$BakPath" 
	fi 

	for filename in `ls $1`
	do 
		#echo $filename 
		if [ -f $1/$filename ]; then 
			if [ "${filename##*.}"x = "log"x ];then 
				newfile=$DATE-$filename 
				#echo $newfile 
				cp $1/$filename $BakPath/$newfile 
				echo "" > $1/$filename 
			fi 
		fi 
	done 
}

delttllog()
{
	find "$1/_baklog" -mtime +6 -name "*.log"  -exec rm -rf {} \;
}


for((i = 0; i < ${#FileList[@]}; ++i)) do 
	
	echo ${FileList[i]}
	baklog ${FileList[i]}
	delttllog ${FileList[i]}
done


