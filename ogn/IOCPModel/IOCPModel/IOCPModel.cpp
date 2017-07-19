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


IOCPModel::IOCPModel()
{
	WSADATA data;
	WSAStartup(0x0202, &data);
	mIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	mWorkerThreadsCount = 16;
	for (int i = 0; i < mWorkerThreadsCount; ++i)
	{
		mWorkerThreads.push_back(new std::thread(&IOCPModel::WorkerThread, this));
	}

}

IOCPModel::~IOCPModel()
{

}

SocketListener* IOCPModel::listen(const std::string& host, short port)
{
	SOCKET socketId = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
	//SOCKET socketId = ::socket(AF_INET, SOCK_STREAM, 0);
	SocketListener* listener = new SocketListener;
	listener->socketId = socketId;
	CreateIoCompletionPort((HANDLE)socketId, mIOCP, (ULONG_PTR)listener, 0);

	SOCKADDR_IN& addr = listener->addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	int err = ::bind(socketId, (SOCKADDR*)&addr, sizeof(addr));
	err = ::listen(socketId, 5);
	int len = sizeof(addr);
	//SOCKET s = ::accept(socketId, (sockaddr*)&addr, &len);

	GUID guidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes = 0;
	WSAIoctl(socketId, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(guidAcceptEx), &listener->pfn, sizeof(listener->pfn),
		&dwBytes, NULL, NULL);

	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

	WSAIoctl(socketId, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockAddrs,
		sizeof(GuidGetAcceptExSockAddrs), &listener->pfnSocketAddrs, sizeof(listener->pfnSocketAddrs),
		&dwBytes, NULL, NULL);

	for (int i = 0; i < 1; i++)
	{
		PostAccept(listener);
	}

	AddListener(listener);
	return listener;
}

SocketClient* IOCPModel::connect(const std::string& host, short port)
{
	SOCKET socketId = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);

	SocketClient* client = new SocketClient;
	client->socketId = socketId;

	SOCKADDR_IN& addr = client->addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	addr.sin_port = htons(port);

	SOCKADDR_IN local;
	local.sin_family = AF_INET;
	local.sin_addr.S_un.S_addr = INADDR_ANY;
	local.sin_port = 0;
	if (SOCKET_ERROR == bind(socketId, (LPSOCKADDR)&local, sizeof(local)))
	{
		printf("绑定套接字失败!\r\n");
		delete client;
		return NULL;
	}

	GUID guidConnectEx = WSAID_CONNECTEX;
	DWORD dwBytes = 0;
	WSAIoctl(socketId, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidConnectEx, sizeof(guidConnectEx), &client->pfn, sizeof(client->pfn),
		&dwBytes, NULL, NULL);
	PostConnect(client);

	AddClient(client);

	return client;
}

bool IOCPModel::loop()
{
	mMutex.lock();
	int queueSize = mQueueEvent.size();
	mMutex.unlock();
	if (queueSize <= 0)
		return false;

	mMutex.lock();
	QueueResponse response = mQueueEvent.front();
	mQueueEvent.pop();
	mMutex.unlock();
	
	response.ioOverlapped->dwBytesTransferred = response.BytesTransferred;
	IO_OVERLAPPED* ioOverlapped = response.ioOverlapped;
	SocketAngent* angent = response.angent;
	switch (response.ioOverlapped->ioState)
	{
	case IOState_Accept:
		DoAccept(angent->socketId, ioOverlapped->socket);
		break;
	case IOState_Connect:
		DoConnect(angent->socketId, ioOverlapped->socket);
		break;
	case IOState_Recv:
		DoRead(ioOverlapped->socket);
		break;
	case IOState_Send:
		DoWrite(ioOverlapped->socket);
		break;
	}

	while (mQueueClose.size() > 0)
	{
		auto itr = mQueueClose.begin();
		
		DoExit(FindSocket(itr->first));
	}
	return true;
}

void IOCPModel::sendBuffer(Socket* target, void* dataBuffer, int dataCount)
{
	IO_OVERLAPPED& ioOverlapped = target->writeOverlapped;
	char* data = new char[dataCount];
	memcpy(data, dataBuffer, dataCount);
	target->sendQueue.push(std::make_pair(data, dataCount));
	if (!target->send)
		PostWrite(target);
}

void IOCPModel::PostAccept(SocketListener* listener)
{
	Socket* socket = new Socket;
	socket->angent = listener;

	socket->socketId = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	LPFN_ACCEPTEX pfn = listener->pfn;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	printf("s:%p io:%p %d\n", socket, &socket->readOverlapped, socket->socketId);
	memset(&ioOverlapped.overlapped, 0, sizeof(OVERLAPPED));
	ioOverlapped.socket = socket;
	ioOverlapped.ioState = IOState_Accept;
	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	ioOverlapped.wBuffer.len = ioOverlapped.dataBufferCount;

	DWORD dwBytes = 0;
	DWORD dwLocalAddressLength = sizeof(SOCKADDR_IN) + 16;
	DWORD dwRemoteAddressLength = sizeof(SOCKADDR_IN) + 16;
	DWORD dwReceiveDataLength = ioOverlapped.dataBufferCount - dwLocalAddressLength - dwRemoteAddressLength;

	BOOL ret = pfn(
		listener->socketId,
		socket->socketId,
		ioOverlapped.dataBuffer,
		0,
		dwLocalAddressLength, 
		dwRemoteAddressLength,
		&dwBytes,
		&(ioOverlapped.overlapped));

	if (!ret && WSA_IO_PENDING != WSAGetLastError())
	{
		//cerr << "lpfnAcceptEx failed with error code: " << WSAGetLastError() << endl;
		int err = WSAGetLastError();
		delete socket;
		return;
	}

	AddSocket(socket);
}

void IOCPModel::PostConnect(SocketClient* client)
{
	Socket* socket = new Socket;
	socket->angent = client;
	socket->socketId = client->socketId;
	CreateIoCompletionPort((HANDLE)socket->socketId, mIOCP, (ULONG_PTR)client, 0);

	LPFN_CONNECTEX pfn = client->pfn;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	memset(&ioOverlapped.overlapped, 0, sizeof(OVERLAPPED));
	ioOverlapped.socket = socket;
	ioOverlapped.ioState = IOState_Connect;

	int nLen = sizeof(SOCKADDR_IN);
	char lpSendBuffer[256] = "connect";
	DWORD dwSendDataLength = strlen(lpSendBuffer) + 1;
	DWORD dwBytesSent = 0;
	// 重点
	BOOL bResult = pfn(
		client->socketId,
		(sockaddr*)&client->addr,				// [in] 对方地址
		sizeof(client->addr),									// [in] 对方地址长度
		NULL,									// [in] 连接后要发送的内容，这里不用
		0,										// [in] 发送内容的字节数 ，这里不用
		&dwBytesSent,							// [out] 发送了多少个字节，这里不用
		&ioOverlapped.overlapped);				// [in]
	DWORD dwError = WSAGetLastError();
	if (ERROR_IO_PENDING != dwError)
	{
		int err = WSAGetLastError();
		delete socket;
		return;
	}
	//DWORD dwFlag = 0, dwTrans = 0;

	//if (!WSAGetOverlappedResult(client->socketId, &ioOverlapped.overlapped, &dwTrans, TRUE, &dwFlag))
	//{
	//	printf("等待异步结果失败\r\n");
	//	return;
	//}
	AddSocket(socket);
}

void IOCPModel::PostRead(Socket* socket)
{
	printf(__FUNCTION__);
	printf("\n");
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	memset(&ioOverlapped.overlapped, 0, sizeof(OVERLAPPED));
	ioOverlapped.ioState = IOState_Recv;
	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	ioOverlapped.wBuffer.len = ioOverlapped.dataBufferCount;
	DWORD dwBufferCount = 1, dwRecvBytes = 0, Flags = 0;
	int result = WSARecv(socket->socketId, &ioOverlapped.wBuffer, dwBufferCount, &dwRecvBytes, &Flags, &ioOverlapped.overlapped, NULL);
	DWORD dwError = WSAGetLastError();
	//if (ERROR_IO_PENDING != dwError)
	//{
	//	int err = WSAGetLastError();
	//	delete socket;
	//	return;
	//}
}

void IOCPModel::PostWrite(Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->writeOverlapped;
	ioOverlapped.socket = socket;
	socket->send = false;

	std::queue<std::pair<char*, int>>& sendQueue = socket->sendQueue;

	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	if (sendQueue.size() <= 0) return;
	DWORD dwBufferCount = 1, dwRecvBytes = 0, Flags = 0;
	char* wBuffer = ioOverlapped.dataBuffer;
	int sendSize = 0;
	while (sendQueue.size() > 0)
	{
		std::pair<char*, int> packet = sendQueue.front();
		if (sendSize + packet.second > ioOverlapped.dataBufferCount)
			break;

		memcpy(wBuffer + sendSize, packet.first, packet.second);
		sendSize += packet.second;
		sendQueue.pop();
		delete[] packet.first;
	}
	socket->send = true;

	memset(&ioOverlapped.overlapped, 0, sizeof(OVERLAPPED));
	ioOverlapped.wBuffer.len = sendSize;
	ioOverlapped.ioState = IOState_Send;
	int result = WSASend(socket->socketId, &ioOverlapped.wBuffer, dwBufferCount, &dwRecvBytes, Flags, &ioOverlapped.overlapped, NULL);
	DWORD dwError = WSAGetLastError();
	//if (ERROR_IO_PENDING != dwError)
	//{
	//	int err = WSAGetLastError();
	//	delete socket;
	//	return;
	//}
}

void IOCPModel::DoAccept(SOCKET socketId, Socket* socket)
{
	SocketListener* listener = FindListener(socketId);
	if (listener == NULL)  return;
	CreateIoCompletionPort((HANDLE)socket->socketId, mIOCP, (ULONG_PTR)listener, 0);

	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;

	SOCKADDR_IN* remote = NULL;
	SOCKADDR_IN* local = NULL;

	int remoteLen = sizeof(SOCKADDR_IN);
	int localLen = sizeof(SOCKADDR_IN);
	DWORD dwLocalAddressLength = sizeof(SOCKADDR_IN) + 16;
	DWORD dwRemoteAddressLength = sizeof(SOCKADDR_IN) + 16;
	DWORD dwReceiveDataLength = ioOverlapped.dataBufferCount - dwLocalAddressLength - dwRemoteAddressLength;

	listener->pfnSocketAddrs(
		ioOverlapped.dataBuffer,
		0,
		dwLocalAddressLength,
		dwRemoteAddressLength,
		(LPSOCKADDR*)&local, &localLen, 
		(LPSOCKADDR*)&remote, &remoteLen);

	printf("listener %d  accept %d accept ok!\n", socketId, socket->socketId);
	PostRead(socket);

	PostAccept(listener);
}

void IOCPModel::DoConnect(SOCKET socketId, Socket* socket)
{
	printf("%d connect ok!\n", socket->socketId);
	PostRead(socket);
	sendBuffer(socket, "send", 5);
}

void IOCPModel::DoRead(Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	if (ioOverlapped.dwBytesTransferred <= 0)
	{
		PushQueueClose(socket->angent, socket->socketId);
		return;
	}
	printf("%s\n", ioOverlapped.dataBuffer);
	PostRead(socket);
	sendBuffer(socket, "send", 5);
}

void IOCPModel::DoWrite(Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->writeOverlapped;
	if (ioOverlapped.dwBytesTransferred <= 0)
	{
		PushQueueClose(socket->angent, socket->socketId);
		return;
	}
	PostWrite(socket);
}

void IOCPModel::DoExit(Socket* socket)
{
	if (socket == NULL) return;

	closesocket(socket->socketId);
}

SocketListener* IOCPModel::AddListener(SocketListener* listener)
{
	mMapListener.insert(std::make_pair(listener->socketId, listener));
	return listener;
}

SocketListener* IOCPModel::FindListener(SOCKET socketId)
{
	auto itr = mMapListener.find(socketId);
	if (itr != mMapListener.end()) return itr->second;

	return NULL;
}

void IOCPModel::DelListener(SOCKET socketId)
{
	auto itr = mMapListener.find(socketId);
	if (itr != mMapListener.end())
	{
		delete itr->second;
		mMapListener.erase(itr);
	}
}

SocketClient* IOCPModel::AddClient(SocketClient* client)
{
	mMapClient[client->socketId] = client;
	return client;
}

SocketClient* IOCPModel::FindClient(SOCKET socketId)
{
	auto itr = mMapClient.find(socketId);
	if (itr != mMapClient.end()) return itr->second;
	return NULL;
}

void IOCPModel::DelClient(SOCKET socketId)
{
	auto itr = mMapClient.find(socketId);
	if (itr != mMapClient.end())
	{
		delete itr->second;
		mMapClient.erase(itr);
	}
}

Socket* IOCPModel::AddSocket(Socket* socket)
{
	mMapSocket[socket->socketId] = socket;
	return socket;
}

Socket* IOCPModel::FindSocket(SOCKET socketId)
{
	auto itr = mMapSocket.find(socketId);
	if (itr != mMapSocket.end()) return itr->second;
	return NULL;
}

void IOCPModel::DelSocket(SOCKET socketId)
{
	auto itr = mMapSocket.find(socketId);
	if (itr != mMapSocket.end())
	{
		delete itr->second;
		mMapSocket.erase(itr);
	}
}

void IOCPModel::PushQueueResponse(QueueResponse& response)
{
	mMutex.lock();
	mQueueEvent.push(response);
	mMutex.unlock();
}

void IOCPModel::PushQueueClose(SocketAngent* angent, SOCKET socketId)
{
	auto itr = mQueueClose.find(socketId);
	if (itr != mQueueClose.end()) return;

	mQueueClose.insert(std::make_pair(socketId, angent));
}

void IOCPModel::WorkerThread()
{
	DWORD CompleteBytes = 0;

	DWORD dwKey = 0;

	while (true)
	{
		OVERLAPPED* overlapped = NULL;
		IO_OVERLAPPED* ioOverlapped = NULL;
		BOOL bRet = false;
		DWORD dwBytesTransferred = 0;
		Socket* socketptr = NULL;
		SocketAngent* angent = NULL;
		bRet = GetQueuedCompletionStatus(mIOCP, &dwBytesTransferred, (PULONG_PTR)&angent, &overlapped, INFINITE);
		ioOverlapped = (IO_OVERLAPPED*)CONTAINING_RECORD(overlapped, IO_OVERLAPPED, overlapped);
		QueueResponse response;
		response.ioOverlapped = ioOverlapped;
		response.angent = angent;
		response.result = bRet;
		response.BytesTransferred = dwBytesTransferred;
		PushQueueResponse(response);
	}
}
