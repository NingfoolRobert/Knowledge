#pragma once 

#include "BaseHandler.h"
#include "tinyxml2.h"


#include <czmq.h>
#include <vector>
#include <map>
#include <set>


typedef struct stSubOriginInfo
{
	int								nID;
	std::string						strURI;
	std::set<std::string>			listFilter;

	void clear()
	{
		nID = 0;
		strURI = "";
		listFilter.clear();
	}
}SUBORIGININFO, *PSUBORIGININFO;

class CSubService;

class CSubObj
{
public:
	CSubObj();
	virtual ~CSubObj();
public:
	
public:
	bool Init(CSubService* pSubService, tinyxml2::XMLElement* pSub);
	
	bool AddSubInfo(std::string strURI, std::string strFilter, int nID = 0);

	bool Bind(BaseHandler* pUser);
	
	bool UpdateConfig(const std::string strFilePath);

	bool Start();

	bool push(zmsg_t* msg) ;

	bool isStop() { return m_bStop; }

	zsock_t* Dettach(){ return m_sockSub; }

protected:
	BaseHandler* CreateHandler(int nType);

	bool	LoadXMLConfig(tinyxml2::XMLElement* pSub);

public:
	static void SubThread(CSubObj* pSubObj);

	zmsg_t* RecvMsg();


	void Close();
public:

	int				m_nType;		//
	std::string		m_strNote;		//备注

private:
	BaseHandler*	m_pUser;

	CSubService*		m_pSubSrv;
	
	bool			m_bStop;

	zsock_t*		m_sockSub;

	std::map<int, SUBORIGININFO> m_listSubInfo;
};

