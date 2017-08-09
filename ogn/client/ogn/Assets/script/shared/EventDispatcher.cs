using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


public class EventTarget
{
    public string name = "";
   	public EventDispatcher targetDispatcher = null;
	public EventRegister targetRegister = null;
}

public class EventRegister
{
    public static int s_id = 0;
    public int id = 0;
    public string name = "";
    public EventDispatcher.EventCallBack callback = null;
    public Object param = null;

    public EventRegister()
    {
        this.id = ++s_id;
    }
    public bool equalListener(string name, EventDispatcher.EventCallBack callback)
    {
        if (this.name == name && this.callback == callback)
		    return true;

	    return false;
    }
}
public class EventDispatcher
{
    public delegate int EventCallBack(EventTarget e);
    public delegate int EventCallBackProcess(Object lprama, Object wprama);
    protected Dictionary<string, List<EventRegister>> listener = new Dictionary<string, List<EventRegister>>();
    protected Dictionary<int, List<EventCallBackProcess>> listener_ = new Dictionary<int, List<EventCallBackProcess>>();

    public EventDispatcher()
    {

    }

    public int addEventListener(string name, EventCallBack callback, Object param = null)
    {
        EventRegister lis = new EventRegister();
        lis.name = name;
	    lis.callback = callback;
	    lis.param = param;
        List<EventRegister> listeners = null;
        if (listener.ContainsKey(name))
        {
            listeners = listener[name];
        }
        else
        {
            listeners = new List<EventRegister>();
            listener[name] = listeners;
        }

        listeners.Add(lis);

        return lis.id;
    }

    public int addEventListener(int id, EventCallBackProcess callback)
    {
        EventCallBackProcess c = callback as EventCallBackProcess;
        List<EventCallBackProcess> listeners = null;
        if (listener_.ContainsKey(id))
        {
            listeners = listener_[id];
        }
        else
        {
            listeners = new List<EventCallBackProcess>();
            listener_[id] = listeners;
        }

        listeners.Add(c);

        return 0;
    }

    public int removeEventListener(string name, EventCallBack callback)
    {
        List<EventRegister> listeners = null;
        if (!listener.ContainsKey(name))
            return 0;
           
        listeners = listener[name];
        for (int i = 0; i < listeners.Count; ++i )
        {
            if (listeners[i].equalListener(name, callback))
            {
                listeners.RemoveAt(i);
                return 0;
            }
        }
        return 0;
    }
	public int dispatchEvent(EventTarget e)
    {
        List<EventRegister> listeners = null;
        if (!listener.ContainsKey(e.name))
            return 0;
           
        listeners = listener[e.name];
        for (int i = 0; i < listeners.Count; ++i)
        {
            e.targetDispatcher = this;
            e.targetRegister = listeners[i];
            listeners[i].callback.Invoke(e);
            //listeners[i].thisObject.(listeners[i].callback)(e);
        }
        return 0;
    }

    public int dispatchEvent(int id, Object lparam, Object wparam)
    {
        List<EventCallBackProcess> listeners = null;
        if (!listener_.ContainsKey(id))
            return 0;

        listeners = listener_[id];
        for (int i = 0; i < listeners.Count; ++i)
        {
            listeners[i].Invoke(lparam, wparam);
        }
        return 0;
    }

}
