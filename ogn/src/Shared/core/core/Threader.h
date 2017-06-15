#pragma once

class ThreadProcessor;

class Threader;

class ThreadProcessor : public Object
{
	friend class Threader;
public:
	ThreadProcessor(void) {}
	virtual ~ThreadProcessor(void) {}
protected:
	virtual unsigned int ThreadProcess(Threader* pThread) { return 0; }
};



//typedef uint32(ThreadCallBack) (Object& therader);

class Threader
{
public:
	typedef uint32 (Object::*ThreadCallBack) (Threader& therader);
	Threader(void);
	virtual ~Threader(void);
	bool create(void* thisObject, ThreadCallBack callback, bool isRun = true);
	bool create(ThreadProcessor* processor, bool isRun = true);
public:
	enum Status
	{
		eNone,
		eRuning,
		eSuspend,
		eEnd,
	};
public:
	void eixt();
	void suspend();
	void resume();
	void setName(const char* name);
	const char* getName();
	Status getStatus();
	bool isActive();
	unsigned int getThreadID();
public:
	static Threader* createThread(ThreadProcessor* processor, bool isRun = true);
	static void sleep(unsigned int dwMilliseconds);
private:
#ifdef WIN32
	static unsigned int __stdcall _ThreadProcess(void* lp);
#else
	static void* _ThreadProcess(void* lp);
#endif // WIN32
public:
	ThreadProcessor*		m_processor;
	Object*					m_thisObject;
	ThreadCallBack			m_callback;
	void*					m_hThread;
	unsigned int			m_dwThreadID;
	Status					m_eStatus;
	std::string				m_strName;
	bool					m_isActive;
private:

};
