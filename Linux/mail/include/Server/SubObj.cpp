#include "SubObj.h"
#include "Log.h"
#include "SubService.h"
#include "GlobalVar.h"

#include <thread>
#include <string.h>
#include <string>

CSubObj::CSubObj():m_bStop(true)
{
	m_pUser		= nullptr;
	m_pSubSrv	= nullptr;
}

CSubObj::~CSubObj()
{

}

bool CSubObj::Init(CSubService* pSubSrv, tinyxml2::XMLElement* pSub)
{

	m_pSubSrv = pSubSrv;

	if(!LoadXMLConfig(pSub))
	{
		return false;
	}
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

	LogInfo("Init NetService Success......");

	return true;
}

bool CSubObj::AddSubInfo(std::string strURI, std::string strFilter, int nID/* =0*/)
{
	if(0 == strURI.length() || 0 == strFilter.length())
		return false;
	std::string strKey = strURI + strFilter;
	
	SUBORIGININFO stInfo = { 0 };
	//stInfo.nID = nID;
	//stInfo.strURI = strURI;
	//stInfo.strFilter = strFilter;
	
	//m_listSubInfo[strKey] = stInfo;

	return true;
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

zmsg_t* CSubObj::RecvMsg()
{
	return zmsg_recv(m_sockSub);
}

void CSubObj::SubThread(CSubObj* pSubObj)
{
	while(true)
	{

		if(nullptr == pSubObj || pSubObj->isStop())
		{
			break;
		}
		zmsg_t* msg = pSubObj->RecvMsg();
		pSubObj->push(msg);
	}
}
bool CSubObj::push(zmsg_t* msg) 
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
	std::set<string> setURI;
	for(auto it = m_listSubInfo.begin(); it != m_listSubInfo.end(); ++it)
	{
		if(setURI.count(it->second.strURI) == 0)
		{
			int ret = zsock_connect(m_sockSub, it->second.strURI.c_str());
			if(ret != 0)
			{
				LogError("[E] connect %s fail.",it->second.strURI.c_str());
			}
			LogInfo("Connect %s Success...", it->second.strURI.c_str());
			//
			setURI.insert(it->second.strURI);
			for(auto itFilter = it->second.listFilter.begin(); itFilter != it->second.listFilter.end(); ++itFilter)
			{
				std::string str = *itFilter;
	 			zsock_set_subscribe(m_sockSub,  str.c_str());
			}
		}
	}

	if(m_pUser)
	{
		m_pUser->Start();
	}
	std::thread tr1(CSubObj::SubThread, this);
	tr1.detach();
	return true;
}

BaseHandler* CSubObj::CreateHandler(int nType)
{
	if(nullptr == m_pSubSrv)
		return nullptr;
	return m_pSubSrv->CreateHandler(nType);
}

bool CSubObj::LoadXMLConfig(tinyxml2::XMLElement* pSub)
{

	if(pSub == nullptr)
	{
		return false;
	}

	using namespace tinyxml2;

	m_nType = atoi(pSub->Attribute("Type"));
	
	m_strNote = pSub->Attribute("Note");

	m_pUser = CreateHandler(m_nType);
	if(nullptr == m_pUser)
	{
		LogError("[E] CreateHandler fail. HandlerType: %d, Note: %s", m_nType, m_strNote.c_str());
		return false;
	}

	//订阅对象信息
	SUBORIGININFO stInfo;// = { 0 };
	for(auto pGroup = pSub->FirstChildElement("Group"); pGroup != nullptr; pGroup = pGroup->NextSiblingElement())
	{
		if(nullptr == pGroup)
			break;
		stInfo.clear();
		stInfo.nID = atoi(pGroup->Attribute("ID"));
		stInfo.strURI = pGroup->Attribute("URI"); //str;
		//
		for(auto pFilter = pGroup->FirstChildElement("Filter"); pFilter != nullptr; pFilter = pFilter->NextSiblingElement())
		{
			if(nullptr == pFilter)
				break;
			stInfo.listFilter.insert(pFilter->GetText());
		}
		m_listSubInfo[stInfo.nID] = stInfo;
	}
	return true;
}

void CSubObj::Close()
{
	if(m_sockSub == nullptr)
		return ;

	//zsock_close(&m_sockSub);
	zsock_destroy(&m_sockSub);
}
