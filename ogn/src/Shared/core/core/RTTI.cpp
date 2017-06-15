#include "Shared.hpp"

typedef Object* (*CreateObjectFunction)();

std::map<std::string, RTTI*>* msClassDict = NULL;


RTTI::RTTI(const char* className, int objectSize, void* pfnCreateObject, RTTI* baseClass, RTTI* nextClass):
mClassName(className),
mObjectSize(objectSize),
mPfnCreateObject((CreateObjectFunction)pfnCreateObject),
mBaseClass(baseClass),
mNextClass(nextClass)
{
	static std::map<std::string, RTTI*> mClassDict;
	if (msClassDict == NULL)
		msClassDict = &mClassDict;

	msClassDict->insert(std::make_pair(className, this));
}

RTTI::~RTTI(void)
{
	std::map<std::string, RTTI*>::iterator itr = msClassDict->find(this->mClassName);
	if (itr != msClassDict->end())
		msClassDict->erase(itr);
}

Object* RTTI::createObject()
{
	if (mPfnCreateObject == NULL)
		return NULL;

	return mPfnCreateObject();
}

Object* RTTI::createObject( const char* className )
{
	RTTI* rtti = getRTTI(className);
	if (rtti == NULL)
		return NULL;
	
	return rtti->mPfnCreateObject();
}

RTTI* RTTI::getRTTI( const char* className )
{
	RTTI* rtti = NULL;
	if (msClassDict == NULL)
		return rtti;

	std::map<std::string, RTTI*>::iterator itr = msClassDict->find(className);
	if (itr != msClassDict->end())
		rtti = itr->second;
	return rtti;
}

Object* RTTI::dynamicCast(Object* object, const char* className )
{
	RTTI* rtti = object->getThisClass();
	while (rtti)
	{
		if (rtti->mClassName == className)
			return object;
		rtti = rtti->mBaseClass;
	}

	return NULL;
}
