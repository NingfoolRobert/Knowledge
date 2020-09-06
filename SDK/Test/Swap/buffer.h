/** 
 * Copyright (C) 2020 xxx.
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * xxx, except with written permission of xxx.
 * 
 * @file:xxxbuffer.h
 * @brief:xxxbuffer 
 * @author:xxxbuffernbf,nbf0537@163.com
 * @version:com 1.0
 * @date:com2020-09-06
 */

#pragma once 
#include <stdio.h> 
#include <stdlib.h> 


class CBuffer 
{
public:
	CBuffer();
	virtual ~CBuffer();
public:

	void Set(int nLen, int nCap, int nHeaderLen);
	
	void Swap(CBuffer& rhs);

	void exchange(CBuffer& rhs);

	int GetBufLen(){return m_nLen;}
	int GetCap(){ return m_nCapbility; }
	int GetHeaderLen() { return m_nHeaderLen; }
	
private:
	int			m_nLen;
	int			m_nCapbility;
	int			m_nHeaderLen;

};
