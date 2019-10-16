#include "czmq.h"
#include <map>

#include "BaseHandler.h"

class CSubService
{
public:
	CSubService();
	virtual ~CSubService();
public:

	virtual bool OnInitualUpdate();
	
	virtual BaseHandler* CreateHandler(int nType) = 0;

	virtual bool OnTimeOut(struct tm* pTime);

	virtual bool OnTerminate();

public:
	

private:
	std::map<int, SubWorker*> m_mapWorker;
	
};
