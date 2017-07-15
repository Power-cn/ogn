#include <WinSock2.h>
#include <mswsock.h>
#include <Windows.h>
#pragma comment(lib,"Ws2_32.lib")//Socket编程需用的动态链接库
#pragma comment(lib,"Kernel32.lib")//IOCP需要用到的动态链接库
#include <string>
#include <thread>
#include <map>
#include <queue>
#include <mutex>


#pragma warning(disable : 4996)
#pragma warning(disable : 4091)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1
#pragma warning(disable:4251)		// dll-interface bullshit


#include "IOCPModel.h"
void main()
{
	IOCPModel iocp;
	iocp.listen("127.0.0.1", 18989);
	while (true)
	{
		iocp.loop();
	}
}
