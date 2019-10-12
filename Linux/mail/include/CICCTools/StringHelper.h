#pragma once
#include <string>
#include <vector>
#include <iconv.h>
#include <errno.h>
#include <cstring>
using namespace std;

namespace CICCTools
{
	static void SplitString(const string& s, vector<string>& v, const string& c)
	{
		string::size_type pos1, pos2;
		pos2 = s.find(c);
		pos1 = 0;
		while (string::npos != pos2)
		{
			v.push_back(s.substr(pos1, pos2 - pos1));

			pos1 = pos2 + c.size();
			pos2 = s.find(c, pos1);
		}
		if (pos1 != s.length())
			v.push_back(s.substr(pos1));
	};

	static string UTF8ToGBK(string strs)
	{
#ifdef WIN32 
		//std::string strs(cchar);
		int len = MultiByteToWideChar(CP_UTF8, 0, strs.c_str(), -1, NULL, 0);
		wchar_t  * wszGBK = new wchar_t[len + 1];
		memset(wszGBK, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strs.c_str(), -1, wszGBK, len);
		len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
		char *szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
		//strUTF8 = szGBK;  
		std::string strTemp(szGBK);
		delete[]szGBK;
		delete[]wszGBK;
		return strTemp;
#else
		return strs;
#endif
	};

	static string GBKToUTF8(string cchar)
	{
#ifdef WIN32 
		//std::string strs(cchar);
		std::string strs(cchar);
		string strOutUTF8 = "";
		WCHAR * str1;
		int n = MultiByteToWideChar(CP_ACP, 0, strs.c_str(), -1, NULL, 0);
		str1 = new WCHAR[n];
		MultiByteToWideChar(CP_ACP, 0, strs.c_str(), -1, str1, n);
		n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
		char * str2 = new char[n];
		WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
		strOutUTF8 = str2;
		delete[]str1;
		str1 = NULL;
		delete[]str2;
		str2 = NULL;
		return strOutUTF8;
#else
		return cchar;
#endif
	};

	static string GB2312ToUtf8Linux(char* pInput)
	{
		iconv_t cd;
		char* pSrc = pInput;
		cd = iconv_open("utf-8", "gb2312");
		if (cd == (iconv_t) - 1)
		{
			return "";
		}


		size_t iSrcLen = strlen(pInput);
		if(iSrcLen<=0)
		{
			return "";
		}
		size_t iOutLen = iSrcLen*2;
		char* output = (char*)malloc(iOutLen);
		memset(output,0,iOutLen);
		if(output == NULL)
		{
			return "";
		}
		char* pDes = output;
		size_t ret = iconv(cd,&pSrc,&iSrcLen,&pDes,&iOutLen);

		if (ret == (size_t) - 1)
		{
			iconv_close(cd);
			return "";
		}

		string retStr(output);
		iconv_close(cd);
		free(output);
		output = NULL;
		return retStr;
	}




}