#ifndef _GENERICFACTORY_H_
#define _GENERICFACTORY_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>

#define REGISTER_CLASS(BASE_CLASS, DERIVED_CLASS)\
	RegisterInpFactory<BASE_CLASS, DERIVED_CLASS> Register##DERIVED_CLASS(#DERIVED_CLASS)

#define CREATE_CLASS(BASE_CLASS,DERIVED_CLASS)\
	GenericFactory<BASE_CLASS>::instance().Create(#DERIVED_CLASS)

using  std::string;
using  std::map;

template <class ManufacturedType, typename ClassIDKey=std::string>
class GenericFactory
{
    typedef ManufacturedType* (*BaseCreateFn)();

    typedef std::map<ClassIDKey, BaseCreateFn> FnRegistry;

    FnRegistry registry;

    GenericFactory()    {}

    GenericFactory(const GenericFactory&)  {}  // 没有实现

    GenericFactory &operator=(const GenericFactory&) {} // 没有实现

public:
    static GenericFactory& instance()
    {
		static GenericFactory<ManufacturedType,ClassIDKey> bf;
		return bf;
    }

    void RegCreateFn(const ClassIDKey &classname, BaseCreateFn fn)
    {
        registry[classname] = fn;
    }

    ManufacturedType* Create(const ClassIDKey &classname) const
    {
		ManufacturedType* theObject(0);
		//FnRegistry::const_iterator regEntry = registry.find(classname);
		auto regEntry = registry.find(classname);
		if (regEntry != registry.end())
		{
			theObject = regEntry->second();
		}
		return theObject;
    }
};

template <class AncestorType, class ManufacturedType, typename ClassIDKey=std::string>
class RegisterInpFactory
{
public:
    static AncestorType* CreateInstance()
    {
        return (new ManufacturedType);
    }
    RegisterInpFactory(const ClassIDKey &id)
    {
        GenericFactory<AncestorType,ClassIDKey>::instance().RegCreateFn(id, CreateInstance);
    }
};

#endif
