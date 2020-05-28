

#include "mysql++/mysql++.h"


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <iostream> 

#include <vector> 


#define DB_SVR_IP "127.0.0.1"
#define DB_SVR_PORT 3306 
#define DB_USER_NAME	"root"
#define DB_USER_SECRTE	"123456"
#define DB_NAME			"Alarm"

int main()
{
	using namespace mysqlpp;
	


	Connection* pConn = new Connection(DB_NAME, DB_SVR_IP, DB_USER_NAME, DB_USER_NAME, DB_SVR_PORT);
	if(nullptr == pConn)
	{
		printf("error.\n");
		return 0;
	}

	if(pConn->connected())
		printf("sucess.\n");

	Query query = pConn->query("select * from table_user_information");

	StoreQueryResult res = query.store();
	if(res)
	{
		for(auto i = 0u; i < res.num_rows(); ++i)
		{
		//		printf("name: %s, value: %d\n", res[i]["No_"], res[i]["Name_"]);
			std::cout << res[i]["No_"] << " " << res[i]["Name_"] << std::endl;
		}
	}




	return 0;
}
