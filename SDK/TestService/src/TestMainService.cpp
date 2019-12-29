
#include "ServiceTpl.h"
#include "TestService.h"

int main(int argc, char** argv)
{
	ServiceTpl<CTestService> service;
	service.Execute("Test");
	return 0;
}

