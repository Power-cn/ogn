#include "Shared.h"

#if (defined(WIN32) || defined(WIN64))
#include <process.h>
#include <Windows.h>
#else
#include <pthread.h>
#endif


#include "Reference.h"
#include "SmartPointer.h"
#include "RTTI.h"
#include "Object.h"
#include "Event.h"

#include "Threader.h"

Threader::Threader(void):
m_processor(NULL),
m_thisObject(NULL),
m_callback(NULL),
m_hThread(NULL),
m_dwThreadID(0),
m_eStatus(eNone),
m_strName(""),
m_isActive(false)
{
}

Threader::~Threader(void)
{
	this->eixt();
	m_thisObject = NULL;
}

bool Threader::create(void* thisObject, ThreadCallBack callback, bool isRun /* = true */)
{
	m_thisObject = (Object*)thisObject;
	m_callback = callback;
	if (isRun)
		m_eStatus = eRuning;

#if (defined(WIN32) || defined(WIN64))
	//pThread->m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CThread::_ThreadProcess, pThread, isRun ? 0 : CREATE_SUSPENDED, &pThread->m_dwThreadID);
	// CreateThread(NULL, 0, pThrdProc, pParam, thrdFlag, pThreadId);
	m_hThread = (void*)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))Threader::_ThreadProcess, this, isRun ? 0 : CREATE_SUSPENDED, (unsigned int*)&m_dwThreadID);
#else
	pthread_create((pthread_t*)pThread->m_hThread, NULL, CThread::_ThreadProcess, pThread);
#endif // 

	m_isActive = true;
	return true;
}

bool Threader::create(ThreadProcessor* processor, bool isRun /*= true*/)
{
	m_processor = processor;
	if (isRun)
		m_eStatus = eRuning;

#if (defined(WIN32) || defined(WIN64))
	//pThread->m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CThread::_ThreadProcess, pThread, isRun ? 0 : CREATE_SUSPENDED, &pThread->m_dwThreadID);
	// CreateThread(NULL, 0, pThrdProc, pParam, thrdFlag, pThreadId);
	m_hThread = (void*)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))Threader::_ThreadProcess, this, isRun ? 0 : CREATE_SUSPENDED, (unsigned int*)&m_dwThreadID);
#else
	pthread_create((pthread_t*)pThread->m_hThread, NULL, CThread::_ThreadProcess, pThread);
#endif // 

	m_isActive = true;
	return true;
}

Threader* Threader::createThread(ThreadProcessor* processor, bool isRun /* = true */)
{
	Threader* pThread = new Threader;
	pThread->m_processor = processor;
	if (isRun)
		pThread->m_eStatus = eRuning;

#if (defined(WIN32) || defined(WIN64))
	//pThread->m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CThread::_ThreadProcess, pThread, isRun ? 0 : CREATE_SUSPENDED, &pThread->m_dwThreadID);
	// CreateThread(NULL, 0, pThrdProc, pParam, thrdFlag, pThreadId);
	pThread->m_hThread = (void*)_beginthreadex(NULL, 0, (unsigned int (__stdcall *)(void *))Threader::_ThreadProcess, pThread, isRun ? 0 : CREATE_SUSPENDED, (unsigned int*)&pThread->m_dwThreadID);
#else
	pthread_create((pthread_t*)pThread->m_hThread, NULL, CThread::_ThreadProcess, pThread);
#endif // 

	pThread->m_isActive = true;
	return pThread;
}


void Threader::eixt()
{
	if (m_eStatus == eSuspend)
	{
		this->resume();
	}
	m_isActive = false;
	
	if (m_hThread == NULL)
		return;

#if (defined(WIN32) || defined(WIN64))
	WaitForSingleObject(m_hThread, INFINITE);
	if (m_hThread)
		CloseHandle(m_hThread);
#else
	pthread_join((pthread_t)m_hThread,NULL);
	pthread_cancel((pthread_t)m_hThread);
#endif //

	m_hThread = NULL;
	m_dwThreadID = 0;
}

#ifdef WIN32
unsigned int Threader::_ThreadProcess(void* lp)
#else
void* Threader::_ThreadProcess(void* lp)
#endif // WIN32
{
	Threader* pThread = (Threader*)lp;
	//DWORD dwReturn = (pThread->m_thisObject->*(pThread->m_callback))(*pThread);
	DWORD dwReturn = pThread->m_processor->ThreadProcess(pThread);
	//DWORD dwReturn = pThread->m_callback(pThread->m_thisObject);
	pThread->m_eStatus = eEnd;
#ifdef WIN32
	return dwReturn;
#else
	return (void*)dwReturn;
#endif
}

void Threader::suspend()
{
	m_eStatus = eSuspend;
#ifdef WIN32
	::SuspendThread(m_hThread);
#endif // WIN32
}

void Threader::resume()
{
	m_eStatus = eRuning;
#ifdef WIN32
	::ResumeThread(m_hThread);
#endif // WIN32
}

void Threader::setName( const char* pszName )
{
	m_strName = pszName;
}

const char* Threader::getName()
{
	return m_strName.c_str();
}

Threader::Status Threader::getStatus()
{
	return m_eStatus;
}

void Threader::sleep(unsigned int dwMilliseconds)
{
#if (defined(WIN32) || defined(WIN64))
	::Sleep((DWORD)dwMilliseconds);
#else
	usleep(dwMilliseconds * 1000);
#endif
}

bool Threader::isActive()
{
	return m_isActive;
}

unsigned int Threader::getThreadID()
{
	return m_dwThreadID;
}