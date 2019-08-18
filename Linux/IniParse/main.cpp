/*************************************************************************
> File Name:  main.cpp
> Author:	Benin
> Mail:	nbf0537@163.com
> Created Time:	2019年07月28日 星期日 00时02分05秒
************************************************************************/

#include<iostream>
#include<string>
#include<map>
//#include<boost/property_tree/ptree.hpp>
//#include<boost/property_tree/ini_parser.hpp>

#include "IniParser.h"
#include <unistd.h>

using namespace std;

//void TestBoostIni()
//{
//	string str = "/home/ningbf/Test/IniParser/Svr.conf";
//
//	boost::property_tree::ptree m_pt , tag_settings;
//	
//	try
//	{
//		read_ini(str,m_pt);
//	}
//	catch(std::exception &e)
//	{
//		printf("Read_ini fail...");
//	}
//	tag_settings = m_pt.get_child("Settings");
//
//	string strIP =tag_settings.get<string>("SvrAddress","");
//	cout << "IP:" <<strIP<<endl;
//
//}

void TestBoostIni()
{
	char filepath[256]={0};
	getcwd(filepath,256);
	char ConfName[256]={0};
	sprintf(ConfName,"%s/Svr.conf",filepath);
	cout << ConfName <<endl;
//	CIniParser ConfConf("/home/ningbf/Test/IniParser/Svr.conf");
	CIniParser Conf(ConfName);	
	cout<<"IP: "<< Conf.GetConfString("Settings","SvrAddress","")<< endl;
}

int main()
{
	TestBoostIni();
	return 0;
}
