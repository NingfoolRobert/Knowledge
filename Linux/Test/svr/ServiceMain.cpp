#include "ServiceTpl.h"
//#include "Service.h"

#include "TestService.h"



int main()
{
	CServiceTpl<CTestService> service;
	service.Execute("Test");
	return 0;
}
