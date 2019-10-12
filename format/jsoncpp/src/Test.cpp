#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "json/json.h"



using namespace std;

int main()
{
	string message = "{ \"data\" : { \"username\" : \"test\"  }, \"type\" : 6  }";
    Json::Reader reader(Json::Features::strictMode());
	Json::Value parseData;
	if (reader.parse(message.c_str(), parseData))
	{
		int type = parseData.get("type", 0).asInt();
		Json::Value frameData = parseData.get("data", 0);
		string username = frameData.get("username", 0).asString();
		printf("type: %d, username: %s\n", type, username.c_str());
	}

	Json::Value data;
    Json::Value frame;
    Json::FastWriter fastWriter;

    frame["type"] = 66;
    data["username"] = "test";
    frame["data"] = data;

    string result = fastWriter.write(frame);
    printf("result: %s \n", result.c_str());

	return 0;
}
