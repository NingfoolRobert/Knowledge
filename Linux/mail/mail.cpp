#include "mail.h"

CMail::CMail()
{

}

CMail::~CMail()
{

}

bool CMail::OnIntialUpdate(const char* pszConfigFileName)
{
	return true;
}

bool CMail::OnNotify()
{

}



bool CMail::LogOn()
{
	return true;
}

bool CMail::LogOut()
{
	return true;
}

bool CMail::SendMail(std::vector<std::string>& listContactor,const char* pszMailTxt)
{
	return true;
}



bool CMail::SendEmailHead()	//放邮件头部信息
{
	return true;
}

bool CMail::SendEmailBody()	//发送文本信息
{
	return true;
}

bool CMail::SendAttachment()	//发送附件
{
	return true;
}

bool CMail::SendEmailEnd()	//发送邮件结束信息 
{
	return true;
}
