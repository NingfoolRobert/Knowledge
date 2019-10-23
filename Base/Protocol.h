#pragma once

#include <stdio.h>


typedef struct stHeader
{
	unsigned int	dwLength;
	unsigned short	wOrigin;
	unsigned short	wParam;
}HEADER,*PHEADER;


#define HEADSIZE  sizeof(HEADER)
