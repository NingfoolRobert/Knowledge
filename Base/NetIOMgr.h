// 
// Call Center On Demand Product Series
// Copyright (C) 2019 ChannelSoft(Beijing.) Technology Ltd., Co.
// All right reserved
// 
/// @file NetIOMgr.h
/// @brief 
/// @author ningbf,nbf0537@163.com
/// @version 0.1
/// @date 2019-10-24
// $$
// TODO:
// 
// *******************************************************/

#pragma once 

#include "Socket.h"
#include <sys/epoll.h>



class CNetIOMgr 
{
public:
	CNetIOMgr();
	virtual ~CNetIOMgr();
public:
	virtual bool OnInitialUpdate();
	
		
};
