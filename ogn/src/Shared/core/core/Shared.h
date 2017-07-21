#pragma once

#if (defined(WIN32) || defined(WIN64))

// #define dynamic_lib

#ifndef dynamic_lib
#define api_lib
#else
#ifndef lib_import
#define api_lib __declspec(dllexport)
#else
#define api_lib __declspec(dllimport)
#endif
#endif

#else

#define api_lib

#endif // WIN32

#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <memory>
#include <time.h>
#include <thread>
#include <mutex>
#include <functional>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <conio.h>
#include <regex>
#include <unordered_map> 
#include <io.h>   
#include <fcntl.h>
#include <direct.h>

//#pragma comment(lib, "libevent/libevent.lib")
//#pragma comment(lib, "libevent/libevent_core.lib")
//#pragma comment(lib, "libevent/libevent_extras.lib")

#if (defined(WIN32) || defined(WIN64))
#pragma warning(disable : 4996)
#pragma warning(disable : 4091)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1
#pragma warning(disable:4251)
#define hash_stdext std

// include

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>  
#include <Dbghelp.h>
#include <WS2tcpip.h>
#include <nb30.h>
#include <consoleapi.h>
#include <corecrt_io.h>
#include <process.h>

// lib
#pragma comment(lib, "DbgHelp")
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Kernel32.lib") //IOCP需要用到的动态链接库

typedef	char							int8;
typedef	short							int16;
typedef	int								int32;
typedef	__int64							int64;
typedef	unsigned char					uint8;
typedef	unsigned short					uint16;
typedef	unsigned int					uint32;
typedef	unsigned __int64				uint64;
typedef float							float32;
typedef double							float64;

#else
#include <ext/hash_map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<netdb.h>
#include <pthread.h>

#define hash_stdext __gnu_cxx

typedef int8_t							int8;
typedef int16_t							int16;
typedef int32_t							int32;
typedef int64_t							int64;
typedef uint8_t							uint8;
typedef uint16_t						uint16;
typedef uint32_t						uint32;
typedef uint64_t						uint64;
typedef float							float32;
typedef double							float64;

#endif

typedef void*			voidptr;
typedef void*			varptr;

#ifndef MAX_PATH
#define MAX_PATH          260
#endif // !1


#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif

#ifndef SAFE_SOCKET
#define SAFE_SOCKET(x)	if (x != INVALID_SOCKET && x != 0) { closesocket(x); x = 0; }
#endif



#define BIT(x)		(1<<x)


#define Guid64
#ifdef Guid64
typedef uint64					Guid;
#else 
typedef uint32					Guid;
#endif // Guid64

#define Session64

#ifdef Session64
typedef uint64					SessionID;
#else
typedef uint32					SessionID;
#endif // Session64


// 计算类成员的内存偏移值;
#ifndef offsetof
#define offsetof(classname, member) (size_t)&reinterpret_cast<const volatile char&>((((classname *)0)->member))
#endif // !offsetof

#define  CHECK(v) {if (!(v)) return false;}
#define  CHECK_RETURN(v, ret) {if (!(v)) return ret;}
#define  CHECK_BREAK(v) {if (!(v)) break;}

#define BINARY_LENGTH 4096
#define PACKET_MAX_LENGTH (4096 * 2)

namespace Shared
{
	// 本机是否高位先存;
	bool bitlocal();
	bool bitswap(const int8* src, int32 s_len, int8* dest, int32 d_len);
	
	//////////////////////////////////////////////////////////////////////////
	int16 htons(int16 s);
	int16 ntohs(int16 s);
	int32 htonl(int32 l);
	int32 ntohl(int32 l);
	int64 htonll(int64 ll);
	int64 ntohll(int64 ll);
	float32 htonf(float32 f);
	float32 ntohf(float32 f);
	float64 htond(float64 d);
	float64 ntohd(float64 d);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	void split(const std::string& str, std::vector<std::string>& dest, const std::string& separator);
	std::string& replace(std::string& str, const std::string& oldstr, const std::string& newstr);
	std::string& replace_all(std::string& str, const std::string& oldstr, const std::string& newstr);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	std::string int32tostr(int32 val);
	std::string int64tostr(int64 val);
	int32 strtoint32(const std::string& val);
	int64 strtoint64(const std::string& val);
	//////////////////////////////////////////////////////////////////////////

	std::string hosttoaddr(const std::string& host);
	std::string localname();
	std::string localmac();

	std::string GBKToUTF8(const std::string& str);
	std::string UTF8ToGBK(const std::string& str);

	std::string encode(const std::string &uri);
	std::string decode(const std::string &uri);

	int32 inet_addr(const std::string& host);
	int32 inet_addr(int32 family, const std::string& host, void* addr);
	std::string inet_ntoa(void* addr);
	void setConsoleTitle(const std::string& title);
	int32 setConsoleCtrlHandler(int32 (*cb)(int32 type), int32 add);

	void SetDirectory(const std::string& dir);
	std::string GetDirectory();
	void GetDirectoryFiles(const std::string& path, std::vector<std::string>& files, const std::string slash = "/");
	void Clipboard(const std::string& str);
	int32 GetFileLength(FILE* fp);
	std::string GetCodeMessage(int32 error_code);

	void srand(uint32 seed);
	void Sleep(uint32 milliseconds);
};

//
//void* operator new(size_t size);
//void* operator new[](size_t size);
