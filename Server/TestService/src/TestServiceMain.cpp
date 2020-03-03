#include "ServiceTpl.h"
#include "TestService.h"



int main(int argc, char** argv)
{
	CServiceTpl<CTestService> Service; 
	Service.Execute("Test");
	return 0;
}
