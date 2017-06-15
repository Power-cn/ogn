#pragma once

class Object;

class RTTI
{
public:
	RTTI(const char* className, int objectSize, void* pfnCreateObject, RTTI* baseClass, RTTI* nextClass);
	~RTTI(void);
	Object* createObject();
	static Object* createObject(const char* className);
	static RTTI* getRTTI(const char* className);
	static Object* dynamicCast(Object* object, const char* className);
public:
	std::string mClassName;
	int mObjectSize;
	Object* (*mPfnCreateObject)();
	RTTI* mBaseClass;
	RTTI* mNextClass;
};

#define DYNAMIC_CAST(object, class_name) RTTI::dynamicCast(object, #class_name)

#define DECLARE_CLASS(class_name) \
public: \
	static RTTI class##class_name; \
	static Object* createObject(); \
	virtual RTTI* getThisClass(); \
	virtual RTTI* getBaseClass(); \
	virtual const char* getClassName();


#define IMPLEMENT_CLASS(class_name, base_class_name) \
	RTTI class_name::class##class_name(#class_name, sizeof(class class_name), class_name::createObject, &base_class_name::class##base_class_name, NULL); \
	Object* class_name::createObject() { return new class_name; } \
	RTTI* class_name::getThisClass() { return &class##class_name; } \
	RTTI* class_name::getBaseClass() { return class##class_name.mBaseClass; } \
	const char* class_name::getClassName() {	return #class_name; }

