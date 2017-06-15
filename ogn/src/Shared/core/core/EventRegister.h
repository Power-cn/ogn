#pragma once

class Object;
class Event;

typedef int (Object::*EventCallback) (Event& e);

class EventRegister
{
public:
	static int s_id;
public:
	int id;
	std::string event;
	Object* thisObject;
	EventCallback callback;
	void* param;
public:
	EventRegister();
	~EventRegister();

	bool equalListener(const char* name, EventCallback callback, Object* thisObject);
};

