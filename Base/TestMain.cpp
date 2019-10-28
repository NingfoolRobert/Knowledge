#include "ServiceTpl.h"
#include "TestService.h"

int main()
{
	CServiceTpl<CTestService> service;
	service.Execute("Test");
	return 0;	
}
