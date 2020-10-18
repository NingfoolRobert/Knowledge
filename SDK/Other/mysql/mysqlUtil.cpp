#include "mysqlUtil.h"


	
CMySqlUtil::CMySqlUtil():_max_conn(MAX_MYSQL_CONNECTION),_cur_conn(0)
{

}
	
CMySqlUtil::~CMySqlUtil()
{
}


bool CMySqlUtil::OnInitialUpdate(std::string ip, int port, std::string username, std::string pwd, std::string  dbname)
{
	_ip = ip;
	_port = port;
	_username = username;
	_password = pwd;
	_dbname = dbname;
	return true;
}
	
mysqlpp::Connection* CMySqlUtil::GetConn()
{
	mysqlpp::Connection* pConn = mysqlpp::safe_grab();
	_cur_conn++;
	return pConn;
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
	if(nullptr == pConn) return;
	
	mysqlpp::ConnectionPool::release(pConn);
}
	
void CMySqlUtil::remove(const mysqlpp::Connection* pConn)
{
	if(pConn == nullptr)  return ;
	mysqlpp::ConnectionPool::remove(pConn);
}
