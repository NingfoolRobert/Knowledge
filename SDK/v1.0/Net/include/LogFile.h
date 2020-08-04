

#pragma once 

#include <stdio.h> 
#include <stdlib.h> 


#define LogFatal(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define LogError(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define LogWarn(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define LogInfo(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define LogDebug(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define LogTrace(fmt, ...)  printf(fmt, ##__VA_ARGS__)
