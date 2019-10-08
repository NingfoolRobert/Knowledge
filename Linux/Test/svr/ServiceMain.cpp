#include "ServiceTpl.h"
#include "TestService.h"

int main()
{
	CServiceTpl<CTestService> service;
	return service.Execute("Test");
}
