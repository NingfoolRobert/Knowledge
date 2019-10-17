#pragma once

#include "CICCLog.h"


//using namespace CICCTools;

#define __LOGINIT__  CICCTools::CLog::GetInstance().Enable()


#define LogInfo(format, ...)  CICCTools::CLog::GetInstance().Info(format, ##__VA_ARGS__)

#define LogWarn(format, ...)  CICCTools::CLog::GetInstance().Warn(format, ##__VA_ARGS__)

#define LogError(format, ...) CICCTools::CLog::GetInstance().Error(format, ##__VA_ARGS__)

#define LogTest(format, ...)  CICCTools::CLog::GetInstance().Test(format, ##__VA_ARGS__)





