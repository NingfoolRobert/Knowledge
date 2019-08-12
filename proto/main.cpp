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
	//example.S
	return 0;
}

