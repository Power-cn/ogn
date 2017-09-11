#pragma once

class Event;
class BinaryStream;
class Object;

class Parameter
{
public:
	Parameter() {}
	virtual ~Parameter() {}
};

typedef int (Object::*EventCallback) (Event& e);
typedef int (Object::*EventCallbackProcess) (void* lparam, void* wparam);
typedef int (Object::*EventDispatcherCallback) (Parameter& par);

#define CastEventCallback(func) (EventCallback)(func)
#define CastEventCallbackProcess(func) (EventCallbackProcess)(func)

SPointer(Object);
class Object : public Reference
{
public:
	Object(void);
	~Object(void);
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);
public:
	static RTTI classObject;
	static Object* createObject();
	virtual RTTI* getThisClass();
public:
	virtual Object* createInstance();
	virtual RTTI* getBaseClass();
	virtual const char* getClassName();
};

