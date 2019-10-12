#include "ServiceTpl.h"
//#include "TestService.h"
#include "AlarmService.h"

int main()
{
	CServiceTpl<CAlarmService> service;
	return service.Execute("Alarm");
}
