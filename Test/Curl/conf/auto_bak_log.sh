#!/bin/sh

baklog='/home/nbf/Work/Platform/Monitor/StatAnalyzeService/conf/bak_log'

Path='/home/nbf/Work/Platform/Monitor/StatAnalyzeService/conf'

if [ ! -d "$baklog" ];then 
	mkdir -p  $baklog 
fi 


DATE=`date +%Y%m%d%H%M%S`

#echo $DATE

for filename in `ls $Path`
do 
	echo $filename
	if [ -f $Path/$filename ]; then 
		if [ "${filename##*.}"x = "log"x ];then 
#			echo $filename 
			newfile=$DATE-$filename
			#echo $newfile
			cp $Path/$filename $baklog/$newfile 
			#mv $newfle $baklog 
			echo "" > $Path/$filename
		fi
	fi 

done 

