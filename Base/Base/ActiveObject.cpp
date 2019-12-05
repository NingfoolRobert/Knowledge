#include "ActiveObject.h"
	

CActiveObject::CActiveObject(void)
{

}

CActiveObject::~CActiveObject(void)
{
	
}


bool CActiveObject::Init(int nThreadNum = 1, CActiveObject* pObject/* = nullptr*/)
{
	return true;
}


bool CActiveObject::OnMsg(PMSGHEADER pMsg)
{
	return true;
}

void CActiveObject::Terminate()
{
	
}
 bool CActiveObject::PostMsg(PMSGHEADER pMsg)
{
	if(m_listActiveObj.size() == 1)
	{
		m_listActiveObj[0].PostMsg(pMsg);
		return false;
	}
	else 
	{
		int nSerialGroup  = 1;
		m_listActiveObj[nSerialGroup].PostMsg(pMsg);
	}

	return true;
}
