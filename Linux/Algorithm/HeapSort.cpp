/*************************************************************************
	> File Name: HeapSort.cpp
	> Author: Benin
	> Mail: nbf0537@163.com 
	> Created Time: 2019年08月30日 星期五 05时55分42秒
 ************************************************************************/
#include<iostream>
#include  <stdio.h>
#include <stdlib.h>
using namespace std;
void swap(int* arr ,int i,int k)
{
	int temp = arr[i];
	arr[i]=arr[k];
	arr[k]=temp;
}
void print (int* arr,int len)
{

	for(int i = 0; i < len; ++i)
		cout << arr[i] <<endl;
}
void adjustHeap(int* arr, int i, int len)
{
	int temp = arr[i];
	
	for(int k= 2*i+1; k < len; k = 2*k+1)
	{
		if(k+1 < len && arr[k] < arr[k+1])	 //创建大顶
			k++;
		if(arr[k]> temp)
		{
			swap(arr,i,k);
			i = k;
		}
		else{
			break;
		}
	}
}
void HeapSort(int* arr, int len)
{
	
	for(int i =len/2-1; i>=0;i--)
		adjustHeap(arr,i,len);				//创建堆

	for(int i = len -1; i > 0; i--)
	{
		swap(arr,0,i);
		adjustHeap(arr,0,i);
	}
}


int main()
{
	int arr[10]={11,23,9,6,90};
	print(arr,5);
	HeapSort(arr,5);
	cout << "Sort after" <<endl;
	print(arr,5);
	return 0;
}
