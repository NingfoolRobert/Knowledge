#! /bin/sh 

project_path=$(cd "dirname $0"; pwd)
project_name="${project_path##*/}"

if [ -f "$project_name.tar.gz" ]; then 
	echo "delete $project_name.tar.gz"
	rm $project_name.tar.gz 
fi

if [ ! -d "$project_name" ]; then 
	mkdir $project_name 
else 
	rm ./$project_name 
fi 

if [ ! -f "./build/bin/$project_name" ]; then 
	echo please build the $project_name 
else 

	cp ./build/bin/$project_name ./$project_name 
	cp ./conf/*.xml ./$project_name 
	cp ./conf/service.sh  ./$project_name
	tar zcvf $project_name.tar.gz $project_name 
	rm -r $project_name 
	echo tar success
fi 
