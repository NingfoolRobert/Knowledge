#!/bin/sh 


Bean="./util/bean"
MSG="./util/msg"
ENUM="./util"

DstDir="./Msg"

PBFILE="PBMessageDefine.h"

if [ ! -d $DstDir ]; then 
	mkdir $DstDir 
else 
	rm -rf $DstDir/* 
fi 

	protoc -I=./  -I=./util/bean -I=./util/msg -I=./util --cpp_out=$DstDir/ $ENUM/*.proto
	protoc -I=./ --cpp_out=$DstDir/		$Bean/*.proto 
	protoc -I=./ --cpp_out=$DstDir/		$MSG/*.proto
	protoc -I=./ --cpp_out=$DstDir/		./util/*.proto
	protoc -I=./ --cpp_out=$DstDir/		./*.proto


#for fileName in ${Bean}/*
#do 
#	temp_file="$fileName"
#	protoc -I=$Bean/ --cpp_out=$DstDir/ $fileName 
#	echo $temp_file
#done

#for filename in ${MSG}/*
#do 
#	temp_file="filename"
#	protoc -I=$MSG/ --cpp_out=$DstDir/ $filename 
#done 

if [ ! -f $PBFILE ]; then 
	touch $PBFILE 
else 
	cat /dev/null > $PBFILE 
fi 

echo "#pragma once" >> $PBFILE 
echo -e "\n" >>$PBFILE

function getdir(){
	echo $1
	for file in $1/*
	do 
		if test -f $file 
		then 
			if [ "${file##*.}"x = "h"x ]; then 
				echo "#include \"$file\""  >> $PBFILE 
			fi 
		else 
			getdir $file 
		fi 
	done 
}

getdir $DstDir

#for fileName in ${DstDir}/* 
#do 
#	if [ "${fileName##*.}"x = "h"x ]; then 
#		echo "#include \"$basename $fileName\"" >> $PBFILE 
#	fi 
#done 

echo >> $PBFILE 

