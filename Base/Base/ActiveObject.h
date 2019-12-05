/** 
 * Copyright (C) 2019 CICC(BJ).
 * 
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * CICC(BJ), except with written permission of CICC(BJ).
 * 
 * @file:		ActiveObject.h
 * @brief:		
 * @author:		ningbf
 * @version:	1.0
 * @date:		2019-12-05
 */

#pragma once 

#include "AsyncObject.h"

typedef bool (*FUNCMSG)(PMSGHEADER pMsg);

class CActiveObject:public CAsyncObject 

{
public:
	CActiveObject(void);
	virtual ~CActiveObject(void);
public:
	bool Init(int nThreadNum = 1, CActiveObject* pObject = nullptr);

	virtual bool PostMsg(PMSGHEADER pMsg);

	virtual bool PostMsg(CBuffer* pBuffer);

	virtual bool OnMsg(PMSGHEADER pMsg);

	virtual void Terminate();

private:
	std::mutex					m_clsLock;
	std::vector<CActiveObject*> m_listActiveObj;
};

