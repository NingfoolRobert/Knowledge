/*************************************************************************
> File Name:  IniParser.h
> Author:	Benin
> Mail:	nbf0537@163.com
> Created Time:	2019年07月28日 星期日 13时55分01秒
************************************************************************/

#include<iostream>
#include<string>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/ini_parser.hpp>

using namespace std;

//using boost::property_ptree;

class CIniParser
{
	public:
		typedef boost::property_tree::ptree INIMAP;
		CIniParser(const string strConfFileName);
		virtual ~CIniParser();
		void Init();
		int GetConfInt(const string strSection,const string strKey,int nDefault);
		std::string GetConfString(const string strSection,const string strkey,std::string strDefault);
	private:
		std::string		m_strConfFileName;
		INIMAP			m_ptree;
		bool			m_bInit;
};

