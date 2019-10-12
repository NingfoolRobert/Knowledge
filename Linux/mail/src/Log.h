#pragma once

#include "CICCLog.h"


using namespace CICCTools;

#define __LOGINIT__  CLog::GetInstance().Enable()


#define LogInfo(format, ...) CLog::GetInstance().Info(format, ##__VA_ARGS__)

#define LogWarn(format, ...) CLog::GetInstance().Warn(format,##__VA_ARGS__)

#define LogError(format, ...) CLog::GetInstance().Error(format, ##__VA_ARGS__)

#define LogTest(format, ...) CLog::GetInstance().Test(format, ##__VA_ARGS__)





