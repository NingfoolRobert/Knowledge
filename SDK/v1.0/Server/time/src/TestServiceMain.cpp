


#include "ServiceTpl.h"
#include "TestService.h"


int main(int argc, char **agrv)
{
	CServiceTpl<CTestService>  Service;
	Service.Execute("Test");
	return 0;
}
