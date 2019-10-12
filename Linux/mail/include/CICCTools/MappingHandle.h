#pragma once

#include <map>
#ifdef WIN32
#include <hash_map>
#else
#include <ext/hash_map> 
using namespace __gnu_cxx;

namespace __gnu_cxx
{
	template<> struct hash<std::string>
	{
		size_t operator()(const std::string& s) const
		{
			return __stl_hash_string(s.c_str());
		}
	};
}

#endif

#include <string>
#include<mutex>

#include "CiccAtomLock.h"

using namespace std;

enum MappingType{
	ShortMapping = 1,
	IntMapping,
	StringMapping,
	ENUM_COUNTS
};


typedef hash_map<unsigned short, void *> ShortMappingBuffer;
typedef hash_map<unsigned int, void *> IntMappingBuffer;
typedef hash_map<string, void *> StringMappingBuffer;
//��װhash_map
class Mapping
{
public:
	Mapping(MappingType type);
	~Mapping();


	bool Insert(unsigned short key, void* value);
	void * GetValue(unsigned short key);
	bool Insert(unsigned int key, void* value);
	void * GetValue(unsigned int key);
	bool InsertNoLock(string key, void* value);
	bool Insert(string key, void* value);
	void * InsertOverwrite(string key, void* value);
	void * GetValue(string key);
	void * Delete(string key);
	void * Delete(unsigned int key);
	void * GetNextValue(int index);
	int GetMapSize();
private:
	MappingType m_type;
	void *m_map;
	CICCTools::AtomLock m_bufferAtom;
	//mutex m_lockMap;
};

