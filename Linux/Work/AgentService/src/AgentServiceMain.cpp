

#include "AgentService.h"
#include "ServiceTpl.h"


int main()
{
	CServiceTpl<CAgentService> service;
	return service.Execute("Agent");
}
