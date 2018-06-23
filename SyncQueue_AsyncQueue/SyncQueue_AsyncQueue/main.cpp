// SyncQueue_AsyncQueue.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <thread>
#include "SyncQ.h"
#include <iostream>

using namespace std;
struct SAddContext 
{
	SAddContext():a(0),b(0)
	{
	}
	int a;
	int b;
};
struct SAddResult
{
	SAddResult() :a(0), b(0),nResult(0)
	{
	}
	int a;
	int b;
	int nResult;
};
int main()
{
	moon::sync_queue<SAddContext>que1;
	moon::sync_queue<SAddResult>que2;
	std::thread calculate([&que1, &que2]()
	{
		while (true)
		{
			if (que1.size() == 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			auto data = que1.Move();
			for (auto& dat : data)
			{
				SAddResult sar;
				sar.a = dat.a;
				sar.b = dat.b;
				sar.nResult = sar.a + sar.b;
				que2.push_back(sar);
			}
		}
	});
	std::thread PrintThread([&que2]() 
	{
		while (true)
		{
			if (que2.size() == 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			auto data = que2.Move();
			for (auto& dat : data)
			{
				printf(" a = %d\n b = %d\n nResult = %d", dat.a, dat.b, dat.nResult);
			}
		}
		
	});

	int x = 0;
	int y = 0;

	while (cin >> x >> y)
	{
		//scanf("%d,%d", &x, &y);
		//;
		//cout << x << y << endl;
		SAddContext sc;
		sc.a = x;
		sc.b = y;
		cout << sc.a << sc.b << endl;
		que1.push_back(sc);
	}


	system("pause");
    return 0;
}

