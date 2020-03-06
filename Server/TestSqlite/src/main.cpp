


#include "NotifitionInfoMgr.h"




int main ()
{
	CNotifitionInfoMgr db; 

	db.Init();
	db.CreateBizTable("fix");
	
	PB::UserInfo stUser;
	stUser.set_userid(23363);
	stUser.set_username("宁贝飞");
	stUser.set_englishname("Beifei.Ning");
	stUser.set_email("Beifei.Ning@cicc.com.cn");
	stUser.set_wechatid("dajf123323");
	stUser.set_level(10);
	stUser.set_departmentid(0);
	stUser.set_departmentname("IT");
	stUser.set_focuslevel(3);
	stUser.set_iscc(true);
	

	db.AddUser2BizTab(stUser, "fix");



	PB::BizInfo* pBiz = new PB::BizInfo;
	
	db.GetNotifier(pBiz, "fix", 3);

	

	db.Terminate();
	return 0;
}
