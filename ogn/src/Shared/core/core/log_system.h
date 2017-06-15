#pragma once

#include "Threader.h"
#include "Mutex.h"

#include <vector>
#include <queue>
#include <string>

class LogOutputer;
class LogSystem
{
public:
	enum
	{
		csl_color_white,		// ��;
		csl_color_b_w_f_g,		// �׵�����;	
		csl_color_b_w_f_r,		// �׵�����;
		csl_color_b_w_f_b,		// �׵�����;
		csl_color_red,			// ����;
		csl_color_green,		// ����;
		csl_color_blue,			// ����;
		csl_color_yellow,		// ����;
		csl_color_green_blue,	// ����;
		csl_color_red_blue,		// �� ����;
	};
	friend class LogThreadProcessor;
public:
	LogSystem(void);
	virtual ~LogSystem(void);
public:
	void append(LogOutputer* outputer);
	void info(const char* formt, ...);
	void warn(const char* formt, ...);
	void error(const char* formt, ...);
	void debug(int color, const char* formt, ...);
	void error_line(const char* formt, const char* file, const int line, const char* func, ...);
	void debug_line(int color, const char* formt, const char* file, const int line, const char* func, ...);
	void outHex(const void* data, int len);
public:
	static bool assertFail(const char* pszExpression, const char* pszFile, const char* pszFunction, const int iLine);
	static bool outMessagebox(const char* formt, const char* pszFile, const char* pszFunction, const int iLine, ...);
private:
	void processOutputer();
private:
	typedef std::vector<LogOutputer*>					LogOutputerList;
	typedef LogOutputerList::iterator					LogOutputerListItr;
private:
	struct stContent
	{
		int				level;
		std::string		context;
		int				color;	// 0 1 2 3;
	};
	LogOutputerList										m_log;
	Threader*											m_pThread;
	ThreadProcessor*									m_pThreadProcessor;
	Mutex												m_mutex;
	typedef std::queue<stContent>						contextQueue;
	contextQueue										m_contexts;
	char*												m_buffer;
};

#ifdef _DEBUG

#define ASSERTFAIL(e)						LogSystem::assertFail(#e, __FILE__, __FUNCTION__, __LINE__)
#define IF_TRUE(e)							if (e ? 1: (ASSERTFAIL(e), 0))
#define IF_FALSE(e)							if (e ? (ASSERTFAIL(e), 1): 0)
#define DEBUG_MESSAGE_BOX(formt, ...)		LogSystem::outMessagebox(formt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define DEBUG_ASSERT(e)						(e ? 1: (ASSERTFAIL(e), 0))

#else

#define ASSERTFAIL(e)
#define IF_TRUE(e)							if (e)
#define IF_FALSE(e)							if (e)
#define DEBUG_MESSAGE_BOX(formt, ...)
#define DEBUG_ASSERT(e)

#endif // _DEBUG