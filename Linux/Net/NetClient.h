
#include "Socket.h"


class CNetClient: public CSocket
{
public:
	CNetClient();
	virtual ~CNetClient();
public:
	virtual  bool   OnMsg(PHEADER  pMsg) = 0;
	virtual  bool	OnConnect()	= 0;
	virtual	 bool	OnBreak() = 0;
	virtual  bool	OnAckMsg(PHEADER pMsg) = 0;
public:
		

private:
	//Buffer
	//

};
