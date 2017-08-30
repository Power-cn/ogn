#include "Shared.hpp"

IMPLEMENT_CLASS(EventDispatcher, Object)
EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
	std::map<std::string, std::vector<EventRegister*>>::iterator itr;
	for (itr = mMapListener.begin(); itr != mMapListener.end(); ++itr)
	{
		std::vector<EventRegister*>& listeners = itr->second;
		std::vector<EventRegister*>::iterator l_itr;
		for (l_itr = listeners.begin(); l_itr != listeners.end(); ++l_itr)
		{
			delete (*l_itr);
		}
		listeners.clear();
	}
	mMapListener.clear();
	
}

int32 EventDispatcher::addEventListener(const std::string& name, EventCallback callback, Object* thisObject, void* param /* = NULL */)
{
	EventRegister* lis = new EventRegister();

	lis->event = name;
	lis->thisObject = thisObject;
	lis->callback = callback;
	lis->param = param;

	std::map<std::string, std::vector<EventRegister*>>::iterator itr = this->mMapListener.find(name);
	std::vector<EventRegister*>* listeners = NULL;
	if (itr != this->mMapListener.end())
	{
		listeners = &(itr->second);
		listeners->push_back(lis);
	}
	else
	{
		std::vector<EventRegister*> ls;
		ls.push_back(lis);
		this->mMapListener.insert(std::make_pair(name, ls));
	}

	return lis->id;
}

int32 EventDispatcher::addEventListener(int32 id, EventCallbackProcess callback, Object* thisObject)
{
	auto itr = listener_.find(id);
	std::vector<std::pair<Object*, EventCallbackProcess>>* listeners = NULL;
	if (itr != this->listener_.end())
	{
		listeners = &(itr->second);
		listeners->push_back(std::make_pair(thisObject, callback));
	}
	else
	{
		std::vector<std::pair<Object*, EventCallbackProcess>> lst;
		lst.push_back(std::make_pair(thisObject, callback));
		listener_.insert(std::make_pair(id, lst));
	}
	return 0;
}

int32 EventDispatcher::addEventListener(int32 id, EventDispatcherCallback callback, Object* thisObject)
{
	auto itr = mMapEventDispatcher.find(id);
	if (itr != mMapEventDispatcher.end())
	{
		std::vector<std::pair<Object*, EventDispatcherCallback>>& lst = itr->second;
		lst.push_back(std::make_pair(thisObject, callback));
	}
	else
	{
		std::vector<std::pair<Object *, EventDispatcherCallback>> lst;
		lst.push_back(std::make_pair(thisObject, callback));
		mMapEventDispatcher.insert(std::make_pair(id, lst));
	}
	return 0;
}

int32 EventDispatcher::dispatch(int32 id, Parameter& par)
{
	auto itr = mMapEventDispatcher.find(id);
	if (itr == mMapEventDispatcher.end())
		return 0;

	auto lst = itr->second;

	for (auto pa : lst) {
		if (pa.first == NULL || pa.second == NULL) continue;
		(pa.first->*(pa.second))(par);
	}
	return 0;
}

int32 EventDispatcher::removeEventListener(const std::string& name, EventCallback callback, Object* thisObject)
{
	auto itr = this->mMapListener.find(name);
	std::vector<EventRegister*>* listeners = NULL;
	if (itr != this->mMapListener.end())
	{
		listeners = &(itr->second);

		for (auto l_itr = listeners->begin();
			l_itr != listeners->end();
			++l_itr)
		{
			EventRegister* lis = (*l_itr);
			if (lis->equalListener(name.c_str(), callback, thisObject))
			{
				listeners->erase(l_itr);
				if (listeners->size() <= 0)
					this->mMapListener.erase(itr);
				delete lis;
				return 0;

			}
		}
	}
	return 0;

}

int32 EventDispatcher::removeEventListener(int32 id, EventCallbackProcess callback, Object* thisObject)
{
	auto itr = listener_.find(id);
	if (itr != listener_.end())
	{
		auto& itrListeners = itr->second;
		for (auto itrList = itrListeners.begin(); itrList != itrListeners.end(); ++itrList)
		{
			if (callback == itrList->second && thisObject == itrList->first)
			{
				itrListeners.erase(itrList);
				break;
			}
		}

		if (itrListeners.size() <= 0)
			listener_.erase(itr);
	}

	return 0;
}

int32 EventDispatcher::dispatch(Event& event)
{
	int32 result = 0;
	auto itr = this->mMapListener.find(event.event);
	std::vector<EventRegister*>* listeners = NULL;
	if (itr != this->mMapListener.end())
	{
		listeners = &(itr->second);

		for (auto l_itr = listeners->begin();
			l_itr != listeners->end();
			++l_itr)
		{
			EventRegister* lis = (*l_itr);
			event.targetDispatcher = this;
			event.targetRegister = lis;
			event.param = lis->param;
			try
			{
				result = 1;
				if (lis->thisObject == NULL || lis->callback == NULL)
					continue;
				(lis->thisObject->*(lis->callback))(event);
			}
			catch (std::string e)
			{
				e = "Registration callback and the definition of the function is not set!";
				throw(e);
			}
		}
	}
	return result;
}

int32 EventDispatcher::dispatch(int32 id, void* lparam, void* wparam)
{
	int32 result = 0;
	auto itr = listener_.find(id);
	std::vector<std::pair<Object*, EventCallbackProcess>>* listeners = NULL;
	if (itr != this->listener_.end())
	{
		listeners = &(itr->second);

		for (std::vector<std::pair<Object*, EventCallbackProcess>>::iterator l_itr = listeners->begin();
			l_itr != listeners->end();
			++l_itr)
		{
			std::pair<Object*, EventCallbackProcess>& lis = (*l_itr);
			if (lis.second == NULL || lis.first == NULL) continue;

			(lis.first->*(lis.second))(lparam, wparam);
			result = 1;
		}
	}
	return result;
}
