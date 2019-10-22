#! /bin/bash 

SourceDir='./Bean'
DestDir='./Msg'

PBFILE='PBMessageDefine.h'

#cd $SourceDiri

cd $DestDir 
rm -rf *
cd ..

for fileName in $(ls $SourceDir)
do	
	if [ "${fileName##*.}"x = "proto"x ]; then 
		echo $fileName 
		protoc -I=$SourceDir/  --cpp_out=$DestDir/  $SourceDir/$fileName 
	fi 
done


if [  -f $PBFILE ]; then 
	cat /dev/null > $PBFILE 
else 
	touch $PBFILE 
fi 

#touch $PBFILE

echo "#pragma once" >> $PBFILE
echo -e "\n" >> $PBFILE
for fileName in $(ls $DestDir)
do 
	if [ "${fileName##*.}"x = "h"x ];then 
		echo "#include \"$fileName\"" >> $PBFILE
	fi 
done 

echo >> $PBFILE















