#pragma once

class CmdExecute;
class CmdEvent : public Event
{
public:
	const char* CMDEVENT;
public:
	CmdExecute* cmdExecute;
};

class CmdExecute
{
public:
	std::string cmd;
	std::vector<std::string> params;
};

class CmdDispatcher : public EventDispatcher
{
	friend class CmdThreadProcessor;
public:
	CmdDispatcher();
	~CmdDispatcher();
	void update(float32 time, float32 delay);
protected:
	Threader*											m_pThread;
	ThreadProcessor*									m_pThreadProcessor;
	Mutex												m_mutex;
	std::queue<CmdExecute*>								mQueueCmd;
};