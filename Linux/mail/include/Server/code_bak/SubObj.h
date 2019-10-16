
#include "BaseHandler.h"

#include <vector>

typedef struct stSubOriginInfo
{
	int		nID;
	std::string		strURI;
	std::filter		strFilter;

}SUBORIGININFO, *PSUBORIGININFO;

class CSubObj
{
public:
	CSubObj();
	virtual ~CSubObj();
public:
	
public:
	bool Init();
	
	bool AddSubInfo(std::string strURI, std::string strFilter, int nID = 0);

	bool Bind(BaseHandler* pUser);
	
	bool UpdateConfig(const std::string strFilePath);

	bool Start();

	bool push(zmg_t* msg) ;

	bool isStop() { return m_bStop; }

	zsock_t* Dettach(){ return m_sockSub; }
public:
	static void SubThread(CSubObj* pSubObj);

	zmsg_t* RecvMsg();

	//bool Disconnect(std::string strURI);

private:
	BaseHandler*	m_pUser;

	bool			m_bStop;

	zsock_t*		m_sockSub;

	std::map<std::string, SUBORIGININFO> m_listSubInfo;
};

