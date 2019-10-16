
#include <time.h>
#include "MappingHandle.h"

Mapping::Mapping(MappingType type)
{
	m_type = type;
	switch (type)
	{
		case ShortMapping:
		{
			m_map = new ShortMappingBuffer();
			break;
		}
		case IntMapping:
		{
			m_map = new IntMappingBuffer();
			break;
		}
		case StringMapping:
		{
			m_map = new StringMappingBuffer();
			m_type = StringMapping;
			break;
		}
		default:
		{
			m_map = new StringMappingBuffer();
			m_type = StringMapping;
			break;
		}

	}
	
}
Mapping::~Mapping()
{
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_bufferAtom.Lock();
	switch (m_type)
	{
		case ShortMapping:
		{
			ShortMappingBuffer *tmpMap = (ShortMappingBuffer *)m_map;
			ShortMappingBuffer::iterator itr = tmpMap->begin();
			for (; itr != tmpMap->end(); itr++)
			{
				delete itr->second;
			}
			tmpMap->clear();
			delete m_map;
			m_map = NULL;
			break;
		}
		case IntMapping:
		{
			IntMappingBuffer *tmpMap = (IntMappingBuffer *)m_map;
			IntMappingBuffer::iterator itr = tmpMap->begin();
			for (; itr != tmpMap->end(); itr++)
			{
				delete itr->second;
			}
			tmpMap->clear();
			delete m_map;
			m_map = NULL;
			break;
		}
		case StringMapping:
		{
			StringMappingBuffer *tmpMap = (StringMappingBuffer *)m_map;
			StringMappingBuffer::iterator itr = tmpMap->begin();
			for (; itr != tmpMap->end(); itr++)
			{
				delete itr->second;
			}
			tmpMap->clear();
			delete m_map;
			m_map = NULL;
			break;
		}

		default:
			break;
	}
	//m_bufferAtom.UnLock();
}

bool Mapping::Insert(unsigned short key, void* value)
{
	if (ShortMapping != m_type) return false;
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	ShortMappingBuffer *tmpMap = (ShortMappingBuffer *)m_map;
	ShortMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		//m_lockMap.unlock();
		return false;
	}
	else
	{
		tmpMap->insert(make_pair(key, value));
		//m_lockMap.unlock();
		return true;
	}	
}
void *Mapping::GetValue(unsigned short key)
{
	if (ShortMapping != m_type) return false;
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	ShortMappingBuffer *tmpMap = (ShortMappingBuffer *)m_map;
	ShortMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		//m_lockMap.unlock();
		return itr->second;
	}
	//m_lockMap.unlock();
	return NULL;
}

bool Mapping::Insert(unsigned int key, void* value)
{
	if (IntMapping != m_type) return false;
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	IntMappingBuffer *tmpMap = (IntMappingBuffer *)m_map;
	IntMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		//m_lockMap.unlock();
		return false;
	}
	else
	{
		tmpMap->insert(make_pair(key, value));
		//m_lockMap.unlock();
		return true;
	}
}
void *Mapping::GetValue(unsigned int key)
{
	if (IntMapping != m_type) return false;
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	IntMappingBuffer *tmpMap = (IntMappingBuffer *)m_map;
	IntMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		//m_lockMap.unlock();
		return itr->second;
	}
	//m_lockMap.unlock();
	return NULL;
}

bool Mapping::InsertNoLock(string key, void* value)
{
	if (StringMapping != m_type) return false;
	//CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	StringMappingBuffer *tmpMap = (StringMappingBuffer *)m_map;
	StringMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		return false;
	}
	else
	{
		tmpMap->insert(make_pair(key, value));
		return true;
	}
}

bool Mapping::Insert(string key, void* value)
{
	if (StringMapping != m_type) return false;
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	StringMappingBuffer *tmpMap = (StringMappingBuffer *)m_map;
	StringMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		//m_lockMap.unlock();
		return false;
	}
	else
	{
		tmpMap->insert(make_pair(key, value));
		//m_lockMap.unlock();
		return true;
	}
}

//return the original value in the map
void* Mapping::InsertOverwrite(string key, void* value)
{
	if (StringMapping != m_type);
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	StringMappingBuffer *tmpMap = (StringMappingBuffer *)m_map;
	StringMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{

		void *pTemp = itr->second;
		//tmpMap->erase(itr);
		//tmpMap->insert(make_pair(key, value));

		itr->second = value;

		return pTemp;
	}
	else
	{
		tmpMap->insert(make_pair(key, value));
		//m_lockMap.unlock();
		return NULL;
	}
}


void *Mapping::GetValue(string key)
{
	if (StringMapping != m_type) return NULL;
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	StringMappingBuffer *tmpMap = (StringMappingBuffer *)m_map;
	StringMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		//m_lockMap.unlock();
		return itr->second;
	}
	//m_lockMap.unlock();
	return NULL;
}

void *Mapping::Delete(string key)
{
	if (StringMapping != m_type);
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	StringMappingBuffer *tmpMap = (StringMappingBuffer *)m_map;
	StringMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		void * value = itr->second;
		tmpMap->erase(itr);
		return value;
	}
	return NULL;
	//m_lockMap.unlock();
}

void *Mapping::Delete(unsigned int key)
{
	if (StringMapping != m_type);
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	//m_lockMap.lock();
	IntMappingBuffer *tmpMap = (IntMappingBuffer *)m_map;
	IntMappingBuffer::iterator itr = tmpMap->find(key);
	if (itr != tmpMap->end())
	{
		void * value = itr->second;
		tmpMap->erase(itr);
		return value;
	}
	return NULL;
	//m_lockMap.unlock();
}

void * Mapping::GetNextValue(int index)
{
	//++index;
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	if (StringMapping == m_type)
	{
		StringMappingBuffer *tmpMap = (StringMappingBuffer *)m_map;
		StringMappingBuffer::iterator itr = tmpMap->begin();
		if (index > tmpMap->size()) return NULL;
		int count = 1;
		for (; itr != tmpMap->end(); itr++)
		{
			if (index == count) return itr->second;
			count++;
		}
		return NULL;
	}
	if (IntMapping == m_type)
	{
		IntMappingBuffer *tmpMap = (IntMappingBuffer *)m_map;
		IntMappingBuffer::iterator itr = tmpMap->begin();
		if (index > tmpMap->size()) return NULL;
		int count = 1;
		for (; itr != tmpMap->end(); itr++)
		{
			if (index == count) return itr->second;
			count++;
		}
		return NULL;
	}
	return NULL;
}

int Mapping::GetMapSize()
{
	CICCTools::AtomLockRegion temp(&m_bufferAtom);
	if (StringMapping == m_type)
	{
		StringMappingBuffer *tmpMap = (StringMappingBuffer *)m_map;		
		return tmpMap->size();	
	}
	if (IntMapping == m_type)
	{
		IntMappingBuffer *tmpMap = (IntMappingBuffer *)m_map;
		return tmpMap->size();
	}
	return 0;
}
