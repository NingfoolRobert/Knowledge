/*************************************************************************
	> File Name: CAutoLock.h
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2019年08月19日 星期一 07时22分30秒
 ************************************************************************/

#include"CObjectLock.h"

class CAutoLock
{

public:
	CAutoLock(CObjectLock* obj)
	{
		assert(obj);
		_obj = obj;
		_obj->Lock();
	}
	virtual ~CAutoLock()
	{
		_obj->Unlock();
	}

private:
	CObjectLock*  _obj;
};
