#include "mysqlUtil.h"


	
CMySqlUtil::CMySqlUtil():_max_conn(MAX_MYSQL_CONNECTION),_cur_conn(0)
{

}
	
CMySqlUtil::~CMySqlUtil()
{
}


bool CMySqlUtil::OnInitialUpdate(std::string ip, int port, std::string username, std::string pwd, std::string  dbname)
{

	return true;
}
	
mysqlpp::Connection* CMySqlUtil::GetConn()
{
	return mysqlpp::safe_grab();
}

mysqlpp::Connection*	CMySqlUtil::create()
{
	mysqlpp::Connection* pConn = nullptr;
	
	return pConn;
}

void CMySqlUtil::destory(mysqlpp::Connection* pConn)
{

}

void CMySqlUtil::release(const mysqlpp::Connection* pConn)
{

}
