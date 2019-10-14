#include "CommonHelper.h"

using namespace std;
int main ()
{
	std::vector<std::string> listFileName;

	char szTmp [256]={"/home/nbf/github/Knowledge"};
	GetAllFileName(szTmp,listFileName);
	
	std::cout << listFileName.size() << std::endl;

	long lCreateTime, lModifyTime,lAccessTime;
	char szFileNam[256] = {"/home/nbf/github/Knowledge/Linux/Tool/CommonHelper.h"};
	if(!GetFileTime(szFileNam, lCreateTime, lModifyTime, lAccessTime))
	{
		std::cout << "fail..." <<std::endl;
		return -1;
	}
	cout << lCreateTime << endl;
	cout << lModifyTime << endl;
	cout << lAccessTime << endl;
	return 0;
}
