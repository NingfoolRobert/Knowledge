#pragma once
#include <string>
#ifdef WIN32
#include <uuids.h>
#include <objbase.h>
#else
#include "uuid.h"

#endif


namespace CICCTools
{
using namespace std;
#ifdef WIN32
#else
typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
#endif
inline string GetUUID()
{
#ifdef WIN32
	GUID guid;
	if (S_OK == CoCreateGuid(&guid))
	{
		char buf[64] = { 0 };
		_snprintf_s(
			buf,
			sizeof(buf),
			"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
		return std::string(buf);
	}
	else
	{
		return "UUID_FAILED";
	}
#else
	GUID uuid;
	uuid_generate((unsigned char*)&uuid);
	char buf[64] = { 0 };
	snprintf(
		buf,
		sizeof(buf),
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		uuid.Data1, uuid.Data2, uuid.Data3,
		uuid.Data4[0], uuid.Data4[1],
		uuid.Data4[2], uuid.Data4[3],
		uuid.Data4[4], uuid.Data4[5],
		uuid.Data4[6], uuid.Data4[7]);
	return std::string(buf);

#endif // WIN32


}


};