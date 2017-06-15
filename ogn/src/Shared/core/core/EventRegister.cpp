#include "Shared.hpp"
int EventRegister::s_id = 0;

EventRegister::EventRegister() :
id(++s_id)
{
}

EventRegister::~EventRegister()
{
}

bool EventRegister::equalListener(const char* name, EventCallback callback, Object* thisObject)
{
	if (this->event == name && this->callback == callback && this->thisObject == thisObject)
		return true;

	return false;
}
