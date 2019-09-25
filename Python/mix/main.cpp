#include <iostream>
//#include <boost/python.hpp>

//#include <python3.6m/Python.h>
//#include <python3.6m/pythonrun.h>
#include <python2.7/Python.h>


using namespace std;
//using namespace boost::python;

int main()
{
	Py_Initialize();
	if(!Py_IsInitialized())
	{
			std::cout <<"1" <<std::endl;
		return -1;
	}
  
	PyRun_SimpleString("import sys");
	
	int argc = 1;
	
	char *argv[1];
	
	argv[0] = "/home/nbf/github/Knowledge/Python/mix/Xtest.py";
	
	PySys_SetArgv(argc, argv);

	if(PyRun_SimpleString("execfile('/home/nbf/github/Knowledge/Python/mix/Xtest.py')") == NULL)	
	//if(PyRun_SimpleString("execfile('./Xtest.py')") == NULL)	
	{
//		std::cout <<"2" <<std::endl;
		std::cout << "Out: " << std::endl;
		char buffer[1024]={0};
		freopen(buffer,"r",stdin);
		
		std::cout << buffer <<endl;
	//	return -1;
	}
	Py_Finalize();
	
	return 0;
														
}

