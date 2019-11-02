#include "ServiceTpl.h"
#include "TestService.h"

int main(int argc, char** argv)
{
	CServiceTpl<CTestService> service;
	service.Execute("Test");
	return 0;	
}
