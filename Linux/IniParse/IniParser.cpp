/*************************************************************************
> File Name:  IniParser.cpp
> Author:	Benin
> Mail:	nbf0537@163.com
> Created Time:	2019年07月28日 星期日 14时02分01秒
************************************************************************/

#include"IniParser.h"



CIniParser::CIniParser(const string strConfFileName):m_strConfFileName(strConfFileName),m_bInit(false)
{
	Init();
}

CIniParser::~CIniParser()
{

}

void CIniParser::Init()
{
	try
	{
		read_ini(m_strConfFileName,m_ptree);
		m_bInit=true;
	}
	catch(std::exception &e)
	{
		//printf("init faile...");
	}
}

int CIniParser::GetConfInt(const string strSection,const string strKey,int nDefault)
{
	if(m_bInit)
	{
		INIMAP tag_settings = m_ptree.get_child(strSection);
		return tag_settings.get<int>(strKey,nDefault);
	}
	return nDefault;
}

std::string CIniParser::GetConfString(const string strSection,const string strKey,string strDefault)
{
	if(m_bInit)
	{
		INIMAP tag_settings = m_ptree.get_child(strSection);
		return tag_settings.get<string>(strKey,strDefault);
	}
	return strDefault;
}



