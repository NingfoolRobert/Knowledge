#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <iostream>


bool GetAllFileName(const char* pszPathName, std::vector<std::string>& listFileName, bool bRcv = false);

