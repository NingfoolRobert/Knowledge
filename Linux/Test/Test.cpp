/*************************************************************************
> File Name:  Test.cpp
> Author:	Benin
> Mail:	nbf0537@163.com
> Created Time:	2019年08月11日 星期日 13时37分22秒
************************************************************************/

#include<iostream>
using namespace std;

int main(int argc, char** argv)
{

	char szTmp[100] = {0};
	sprintf(szTmp,"%-*s: %s", 20, "ninhao", "hello");
	cout << szTmp <<endl;
  int nums  = 1000;
  cout << nums<<endl;
  cout << "hello world!"<< endl;
	return 0;
}
