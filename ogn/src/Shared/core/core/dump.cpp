#include "dump.h"
#include "Shared.hpp"
#ifdef WIN32
#include <windows.h>
#include <Dbghelp.h>
#pragma comment(lib, "DbgHelp")
#endif

char global_dump_file_name[MAX_PATH] = "dump_file.dmp";

LONG WINAPI CustomUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	std::string dateStr;
	DateTime::getNow(dateStr);
	sprintf_s(global_dump_file_name, MAX_PATH, "%s.dmp", dateStr.c_str());
	HANDLE lhDumpFile = CreateFileA(global_dump_file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);

	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
	loExceptionInfo.ExceptionPointers = ExceptionInfo;
	loExceptionInfo.ThreadId = GetCurrentThreadId();
	loExceptionInfo.ClientPointers = TRUE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);

	CloseHandle(lhDumpFile);

	return EXCEPTION_EXECUTE_HANDLER;
}

Dump::Dump(const char *name)
{
#ifdef WIN32
	//strcpy_s(global_dump_file_name, MAX_PATH,name);
	SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);
#endif
}

Dump::~Dump()
{
#ifdef WIN32
	SetUnhandledExceptionFilter(NULL);
#endif
}