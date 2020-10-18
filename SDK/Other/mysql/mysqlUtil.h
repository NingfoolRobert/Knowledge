/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxmysqlUtil.h
 * @brief:xxxmysqlUtil 
 * @author:xxxmysqlUtilnbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-09-07
 */

#pragma once 


#include "cpool.h"
#include <string> 
#include <atomic> 

#define MAX_MYSQL_CONNECTION  256

class CMySqlUtil: 
	public mysqlpp::ConnectionPool 
{
public:
	CMySqlUtil();
	virtual ~CMySqlUtil();
public:
	virtual bool OnInitialUpdate(std::string ip, int port, std::string username, std::string pwd, std::string  dbname);
	
	virtual mysqlpp::Connection* create();
	
	virtual void destory(mysqlpp::Connection* pConn);

	virtual void release(const mysqlpp::Connection* pConn);
	
	virtual void remove(const mysqlpp::Connection* pConn);
public:
	mysqlpp::Connection* GetConn();

protected:
	unsigned int max_idle_time() { return _idle_time; }

private:
	std::string			 _dbname;
	std::string			 _ip;
	std::string			_username;
	std::string			_password;
	int					_port;
	unsigned int		_idle_time;
private:
	std::atomic_int		_max_conn; 
	std::atomic_int		_cur_conn;
};
