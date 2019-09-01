/*************************************************************************
	> File Name: fullpermutation.cpp
	> Author: Benin
	> Mail: nbf0537@163.com 
	> Created Time: 2019年08月30日 星期五 21时45分22秒
 ************************************************************************/

#include<iostream>
using namespace std;
#include <vector>
#include <stdio.h>


void help(vector<vector<int> >& res,vector<int>&arr,vector<int>&tmp,vector<bool>&flag)
{
	if(arr.size() == tmp.size())
	{
		res.push_back(tmp);
		return;
	}
	for(int i =0; i < arr.size(); ++i)
	{
		if(flag[i])
			continue;
		flag[i] = true;
		tmp.push_back(arr[i]);
		help(res,arr,tmp,flag);
		flag[i] = false;
		tmp.erase(tmp.begin()+tmp.size()-1);
	}
}
vector<vector<int> > fullPermutation(vector<int>& arr)
{
	vector<vector<int> > res;
	vector<int> tmp;
	vector<bool>  flag(arr.size(), false);
	
	help(res,arr,tmp,flag);
	return res;
}

void print(vector<vector<int> >& res)
{
	for(int i = 0; i < res.size(); ++i)
	{
		vector<int> plist = res[i];
		for(int j = 0; j < plist.size(); ++j)
		{
			cout << plist[j] << " ";
		}
		cout << endl;
	}
}

int main(int agrc,char** agrv)
{
	int arr[10]={ 12,23,45,9};
	vector<int> test(arr,arr+4);
	
	vector<vector<int> > res = fullPermutation(test);
	print(res);
}
