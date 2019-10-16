#include "SubObj.h"
#include "Log.h"

CSubObj::CSubObj():m_bStop(true)
{
	m_pUser = nullptr;
}

CSubObj::~CSubObj()
{

}

bool CSubObj::Init()
{
		
	m_sockSub = zsock_new(ZMQ_SUB);
	if(m_sockSub == nullptr)
	{
		LogError("[E] Create Sub object fail...");
		return false;
	}

	zsock_set_tcp_keepalive(m_sockSub, 1);

	zsock_set_tcp_keepalive_cnt(m_sockSub, 3);
	zsock_set_tcp_keepalive_idle(m_sockSub, 60);
	zsock_set_tcp_keepalive_intvl(m_sockSub, 10);
	
	zsock_set_rcvbuf(m_sockSub, 64 * 1024 * 1024);
	zsock_set_sndbuf(m_sockSub, 64 * 1024 * 1024);
	zsock_set_sndhwm(m_sockSub, 0);
	zsock_set_rcvhwm(m_sockSub, 0);


	return true;
}

bool CSubObj::AddSubInfo(std::string strURI, std::string strFilter, int nID =/*0*/)
{
	if(0 == strURI.length() || 0 == strFilter.length())
		return false;
	std::string strKey = strURI + strFilter;
	
	SUBORIGININFO stInfo = { 0 };
	stInfo.nID = nID;
	stInfo.strURI = strURI;
	stInfo.strFilter = strFilter;
	
	m_listSubInfo[strKey] = stInfo;
}

bool CSubObj::Bind(BaseHandler* pUser)
{
	if(pUser == nullptr)
	{
		return false;
	}
	m_pUser = pUser;
	return true;
}

zmg_t* CSubObj::RecvMsg()
{
	return zmsg_recv(m_sockSub);
}

static void CSubObj::SubThread(CSubObj* pSubObj)
{
	while(!pSubObj->isStop())
	{	
		zmg_t* msg = pSubObj->RecvMsg()
		pSubObj->push(msg);
	}
}
bool CSubObj::push(zmg_t* msg) 
{
	if(nullptr == msg)
		return false;

	if(m_pUser)
	{
		m_pUser->Push(msg);
	}
	return true;
}

bool CSubObj::Start()
{
	set<string> URI;
	for(auto it = m_listSubInfo.begin(); it != m_listSubInfo.end(); ++it)
	{
		if(URI.cout(it->second.strURI) == 0)
		{
			int ret = zsock_connect(m_sockSub, it->second.strURI.c_str());
			if(ret != 0)
			{
				LogError("[E] connect %s fail.",it->second.strURI.c_str());
			}		
		}
		zsock_set_subscribe(m_sockSub,  it->second.strFilter.c_str());
	}

	std::thread tr1(CSubObj::SubThread, this);
	tr1.detach();

	return true;
}
