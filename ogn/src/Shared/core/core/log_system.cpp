#ifdef WIN32
#include <Windows.h>
#endif // WIN32

#include "Shared.h"
#include "Reference.h"
#include "SmartPointer.h"
#include "RTTI.h"
#include "Object.h"
#include "Event.h"
#include "Threader.h"
#include "DateTime.h"

#include "log_outputer.h"
#include "log_system.h"
#include <stdio.h>
#include <stdarg.h>
#pragma warning(push)
#pragma warning(disable : 4996)

#define LOG_BUFFER_SIZE		1024 * 4 * 2

class LogThreadProcessor : public ThreadProcessor
{
public:
	LogThreadProcessor(LogSystem* log);
protected:
	unsigned int ThreadProcess(Threader* pThread);
private:
	LogSystem*				m_pLogSystem;
};

LogThreadProcessor::LogThreadProcessor( LogSystem* log )
{
	m_pLogSystem = log;
}

unsigned int LogThreadProcessor::ThreadProcess(Threader* pThread)
{
	while (pThread->isActive())
	{
		m_pLogSystem->processOutputer();
		Threader::sleep(1);
	}
	return 0;
}

LogSystem::LogSystem(void)
{
	m_log.clear();
	m_pThreadProcessor = new LogThreadProcessor(this);
	m_pThread = Threader::createThread(m_pThreadProcessor);
	m_buffer = new char[LOG_BUFFER_SIZE];
}

LogSystem::~LogSystem(void)
{
	m_pThread->eixt();

	SAFE_DELETE(m_pThread);
	SAFE_DELETE(m_pThreadProcessor);
	SAFE_DELETE_ARRAY(m_buffer);
	m_log.clear();
}

void LogSystem::append(LogOutputer* outputer)
{
	m_log.push_back(outputer);
}

void LogSystem::info(const char* formt, ...)
{
	va_list va;
	va_start(va, formt);

	m_mutex.lock();
	vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += m_buffer;
	st.level = logLevel_info;
	st.color = 0;
	m_contexts.push(st);
	m_mutex.unlock();

	va_end(va);
}

void LogSystem::warn(const char* formt, ...)
{
	va_list va;
	va_start(va, formt);
	m_mutex.lock();
	vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += m_buffer;
	st.level = logLevel_warn;
	st.color = 0;
	m_contexts.push(st);
	m_mutex.unlock();

	va_end(va);
}

void LogSystem::error(const char* formt, ...)
{
	va_list va;
	va_start(va, formt);
	m_mutex.lock();
	vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += m_buffer;
	st.level = logLevel_error;
	st.color = 0;
	m_contexts.push(st);
	m_mutex.unlock();

	va_end(va);
}

void LogSystem::processOutputer()
{
	m_mutex.lock();
	if (m_contexts.size() <= 0)
	{
		m_mutex.unlock();
		Threader::sleep(100);
	}
	else
	{
		stContent& st = m_contexts.front();
		m_mutex.unlock();

		LogOutputer* pLog = NULL;
		LogOutputerListItr itr;
		for (itr = m_log.begin();
			itr != m_log.end();
			++itr)
		{
			pLog = (*itr);
			pLog->Outputer(st.context.c_str(), st.color, st.level);
		}

		m_mutex.lock();
		m_contexts.pop();
		m_mutex.unlock();
	}
}

void LogSystem::outHex(const void* data, int len)
{
	std::string str;
	char s[10] = {0};
	int startPos = 0;
	void* temp = (void*)data;
	str = "hex:\n";
	for (int i = 0; i < len; ++i)
	{
		sprintf_s(s, sizeof(s), "%02x", ((unsigned char*)data)[i]);
		str += s;
	}
	//for (int i = 0; i < len; ++i)
	//{
	//	if (i % 16 == 0 && i > 0)
	//	{
	//		str += "		";
	//		for (int j = 0; j < i - startPos; ++j)
	//		{
	//			if (((unsigned char*)temp)[j] == 0)
	//			{
	//				str += '.';
	//			}
	//			else
	//			{
	//				str += ((unsigned char*)temp)[j];
	//			}
	//		}
	//		str += "\r\n";
	//		startPos = i;
	//		temp = (unsigned char*)data + startPos;
	//	}
	//	else if (i % 4 == 0 && i > 0)
	//	{
	//		str += ' ';
	//	}

	//	sprintf_s(s, sizeof(s), "%02x", ((unsigned char*)data)[i]);
	//	str += s;

	//	if (i == len - 1)
	//	{
	//		if (i - startPos < 16)
	//		{
	//			// ʣ�µ�;
	//			int pos = 16 - (i - startPos + 1);
	//			for (int j = 0; j < pos; ++j)
	//			{
	//				str += "  ";
	//			}

	//			for (int j = 0; j < pos / 4; ++j)
	//			{
	//				str += ' ';
	//			}
	//			str += "		";

	//			for (int j = 0; j <= i - startPos; ++j)
	//			{
	//				unsigned char uc = ((unsigned char*)temp)[j];
	//				if (uc == 0 || uc == '\r' || uc == '\n')
	//				{
	//					str += '.';
	//				}
	//				else
	//				{
	//					str += uc;
	//				}
	//			}
	//		}
	//	}
	//}
	m_mutex.lock();
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += str.c_str();
	st.level = logLevel_hex;
	m_contexts.push(st);
	m_mutex.unlock();
}

bool LogSystem::assertFail( const char* pszExpression, const char* pszFile, const char* pszFunction, const int iLine )
{
	char szTempBuffer[1024] = {0};
	sprintf_s(szTempBuffer, sizeof(szTempBuffer), "Expression: %s\r\nFile: %s\r\nFunc: %s\r\nLine: %d\r\n", pszExpression, pszFile, pszFunction, iLine);
#ifdef WIN32
	int iButtonResult = ::MessageBoxA(
		NULL, 
		szTempBuffer, 
		"assert", 
		MB_ABORTRETRYIGNORE);

	switch(iButtonResult)
	{
	case IDABORT:
		exit(EXIT_FAILURE);
	default:
	case IDRETRY:
		__debugbreak();
		break;
	case IDIGNORE:
		break;
	}
#else
	printf(szTempBuffer);
#endif // WIN32

	return false;
}

bool LogSystem::outMessagebox(const char* formt, const char* pszFile, const char* pszFunction, const int iLine, ...)
{
	va_list va;
	va_start(va, formt);
	char buffer[2048] = {0};
	vsnprintf(buffer, 2048, formt, va);
	va_end(va);

	return LogSystem::assertFail(buffer, pszFile, pszFunction, iLine);
}

void LogSystem::debug( int color, const char* formt, ... )
{
	va_list va;
	va_start(va, formt);
	m_mutex.lock();
	vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += m_buffer;
	st.level = logLevel_debug;
	st.color = color;
	m_contexts.push(st);
	m_mutex.unlock();

	va_end(va);
}

void LogSystem::error_line( const char* formt, const char* file, const int line, const char* func, ... )
{
	va_list va;
	va_start(va, formt);
	m_mutex.lock();
	int buf_len = vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);

	buf_len = sprintf_s(m_buffer, LOG_BUFFER_SIZE - buf_len, "file:%s line:%d func:%s e:%s", file, line, func, m_buffer);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += m_buffer;
	st.level = logLevel_error;
	st.color = 0;
	m_contexts.push(st);
	m_mutex.unlock();

	va_end(va);
}

void LogSystem::debug_line( int color, const char* formt, const char* file, const int line, const char* func, ... )
{
	va_list va;
	va_start(va, formt);
	m_mutex.lock();
	int buf_len = vsnprintf(m_buffer, LOG_BUFFER_SIZE, formt, va);
	
	buf_len = sprintf_s(m_buffer, LOG_BUFFER_SIZE - buf_len, "file:%s line:%d func:%s e:%s", file, line, func, m_buffer);
	stContent st;

	std::string now_time = "";
	DateTime::Now(now_time);
	st.context = "[";
	st.context += now_time;
	st.context += "]";

	st.context += m_buffer;
	st.level = logLevel_debug;
	st.color = color;
	m_contexts.push(st);
	m_mutex.unlock();

	va_end(va);
}

#pragma warning(pop)