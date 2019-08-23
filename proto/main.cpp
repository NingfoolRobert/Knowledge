/*************************************************************************
> File Name:  main.cpp
> Author:	Benin
> Mail:	nbf0537@163.com
> Created Time:	2019年08月12日 星期一 22时54分24秒
************************************************************************/

#include<iostream>

#include "Test.pb.h"
#include <string.h>
#include <fstream>
#include <stdlib.h>


using namespace std;

using namespace lm;
int main(int argc, char** argv)
{

	helloworld example;
	
	example.set_id(10);
	example.set_str("hello");
	
	std::string  filename ="Single_len";
	std::fstream output(filename, std::ios::out | std::ios::trunc | std::ios::binary);
	if(!example.SerializeToOstream(&output))
	{
		std::cerr << "Fail to write example." << std::endl;
		exit(-1);
	}
//	auto& list_person = example.mutable_list_person();
	auto person =	example.add_list_person();
	person->set_id(10001);
	person->set_name("Ning");


	cout << example.id()<<endl;
	cout << example.str()<<endl;

	auto& map = *example.mutable_list_test();
	///string str="TestOne";
	//map->insert(1,32);
	//map->insert(2,64);
	//*(map)[1] = 32;
	//*(map)[2] = 64;
	example.add_list_array(32);	;
	
	example.add_list_array(78);
	
	
	map[1] = 32;
	map[2] =64;
	if(map.empty())
		cout << "empty" <<endl;
	else
		cout << "no empty"<<endl;

	cout << map.size()<<endl;
	//int nSize = example.list_Test_size();
	//cout << "Size :  "<< nSize <<endl;
	
	for(auto it = example.list_test().begin(); it != example.list_test().end(); ++it)
	{
		cout << it->first <<  ":" << it->second <<endl;
	}
//	cout << map[1] <<endl;
//	cout << map[2] <<endl;
//
//	auto it = example.list_test().find(1);
//	if(it == example.list_test().end())
//		cout << "fail..."<<endl;
//	cout << it->second<<endl;

	int nSize = example.list_array_size();
	for(int i=0;i<nSize;++i)
	{
		cout << example.list_array(i) << endl;
	}
//example.S

	const   lm::person&	personOne = example.list_person(0);
	
	cout << " person: " << personOne.id() << endl;
	cout << " person: " << personOne.name() << endl;

	auto personPtr = example.mutable_list_person(0);
	personPtr->set_id(1000010);
	personPtr->set_name("Wang");

	cout << " personptr:" << personPtr->id() <<endl;
	cout << " personptr: " << personPtr->name() << endl;
	return 0;
}

