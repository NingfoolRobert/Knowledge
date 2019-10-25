#include <stdio.h>
#include <stdlib.h>
#define __LOGINIT__  ;
#define LogTrace(format, ...) printf(format, ##__VA_ARGS__)

#define LogDebug(format, ...) printf(format, ##__VA_ARGS__)

#define LogInfo(format, ...)  printf(format,##__VA_ARGS__)

#define LogWarn(format, ...) printf(format, ##__VA_ARGS__)

#define LogError(format, ...) printf(format, ##__VA_ARGS__)

#define LogFatal(format, ...) printf(format, ##__VA_ARGS__)
