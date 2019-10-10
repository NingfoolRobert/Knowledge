#include "CommonHelper.h"


int main ()
{
	std::vector<std::string> listFileName;

	char szTmp [256]={"/home/nbf/github/Knowledge"};
	GetAllFileName(szTmp,listFileName);
	
	std::cout << listFileName.size() << std::endl;
	
	return 0;
}
