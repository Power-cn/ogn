#pragma once

class Event;
class EventRegister;
class EventDispatcher;

class EventDispatcher : public Object
{
	DECLARE_CLASS(EventDispatcher)
public:
	EventDispatcher();
	~EventDispatcher();
	int32 addEventListener(const std::string& name, EventCallback callback, Object* thisObject, void* param = NULL);
	int32 removeEventListener(const std::string&, EventCallback callback, Object* thisObject);
	int32 dispatch(Event& event);

	int32 addEventListener(int32 id, EventCallbackProcess callback, Object* thisObject);
	int32 removeEventListener(int32 id, EventCallbackProcess callback, Object* thisObject);
	int32 dispatch(int32 id, void* lparam, void* wparam);

	int32 addEventListener(int32 id, EventDispatcherCallback callback, Object* thisObject);
	int32 dispatch(int32 id, Parameter& par);
protected:
	std::map<std::string, std::vector<EventRegister*>> mMapListener;
	std::map<int32, std::vector<std::pair<Object*, EventCallbackProcess>>> listener_;
	std::map<int32, std::vector<std::pair<Object*, EventDispatcherCallback>>> mMapEventDispatcher;
};

#define  RegisterEventListener(dis, name, cb, thisObject) dis->addEventListener(name, (EventCallback)callback, thisObject)
#define  RegisterEventListener_P1(dis, name, cb, thisObject, param) dis->addEventListener(name, (EventCallback)callback, thisObject, param)

#define RegisterEventProcess(dis, id, cb, thisObject) dis->addEventListener((int32)id, (EventCallbackProcess)cb, thisObject)
#define UnRegisterEventProcess(dis, id, cb, thisObject) dis->removeEventListener(id, (EventCallbackProcess)cb, thisObject)