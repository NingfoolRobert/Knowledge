
#pragma once 
#include <iostream>

extern char** __argv;
extern int	   __argc;
static void get_main_para_test(int argc, char* argv[], char* envp[])
{

	__argc = argc;
	__argv= argv;
}
#ifndef __INIT__
#define __INIT__ 
__attribute__((section(".init_array")))\
		void (*p_get_main_para_test_t)(int, char*[], char*[]) =&get_main_para_test;
#endif 

