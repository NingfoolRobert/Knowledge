#pragma once

#include <algorithm>

#include "GlobalVar.h"


bool GlobalBegin(const char* pszSimpleName);

bool GlobalEnd();

bool GetWorkPath(char* pszWorkPath);
