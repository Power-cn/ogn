#pragma once


enum IOCP_State
{
	IOState_None,
	IOState_Accept,
	IOState_Connect,
	IOState_Recv,
	IOState_Send,
};

#define DATA_BUFFER_COUNT 2048
class SocketAngent;
class Socket;

struct IO_OVERLAPPED
{
	OVERLAPPED							overlapped = { 0 };
	WSABUF								wBuffer = { 0 };
	IOCP_State							ioState = IOState_None;
	SOCKET								targetSocket = 0;
	SOCKET								clientSocket = 0;
	char								dataBuffer[DATA_BUFFER_COUNT] = { 0 };
	int									dataBufferCount = DATA_BUFFER_COUNT;
	DWORD								dwBytesTransferred = 0;
	Socket*								socket = NULL;
};

class SocketAngent
{
public:
	SOCKET				socketId = 0;
	SOCKADDR_IN			addr = { 0 };
};

class SocketClient : public SocketAngent
{
public:
	LPFN_CONNECTEX		pfn = NULL;
	Socket*				mSocket = NULL;
};

class SocketListener : public SocketAngent
{
public:
	LPFN_ACCEPTEX					pfn = NULL;
	LPFN_GETACCEPTEXSOCKADDRS		pfnSocketAddrs = NULL;
	std::map<SOCKET, Socket*>		mSockets;
};

class Socket
{
public:
	Socket()
	{
		printf("this: %p\n", this);
	}
	SOCKET								socketId = 0;
	IO_OVERLAPPED						readOverlapped;
	IO_OVERLAPPED						writeOverlapped;
	SocketAngent*						angent = NULL;
	std::queue<std::pair<char*, int>>	sendQueue;
	bool								send = false;
};

struct QueueResponse
{
	IO_OVERLAPPED* ioOverlapped = NULL;
	SocketAngent* angent = NULL;
	DWORD result = 0;
	DWORD BytesTransferred = 0;
};

class IOCPModel
{
public:
	IOCPModel();
	~IOCPModel();
	SocketListener* listen(const std::string& host, short port);
	SocketClient* connect(const std::string& host, short port);
	bool loop();
	void sendBuffer(Socket* target, void* dataBuffer, int dataCount);
protected:
	void PostAccept(SocketListener* listener);
	void PostConnect(SocketClient* client);
	void PostRead(Socket* socket);
	void PostWrite(Socket* socket);
protected:
	void DoAccept (SOCKET socketId, Socket* socket);
	void DoConnect(SOCKET socketId, Socket* socket);
	void DoRead(Socket* socket);
	void DoWrite(Socket* socket);
	void DoExit(Socket* socket);

	SocketListener* AddListener(SocketListener* listener);
	SocketListener* FindListener(SOCKET socketId);
	void DelListener(SOCKET socketId);

	SocketClient* AddClient(SocketClient* client);
	SocketClient* FindClient(SOCKET socketId);
	void DelClient(SOCKET socketId);

	Socket* AddSocket(Socket* socket);
	Socket* FindSocket(SOCKET socketId);
	void DelSocket(SOCKET socketId);

	void PushQueueResponse(QueueResponse& response);
	void PushQueueClose(SocketAngent* angent, SOCKET socketId);
protected:
	void WorkerThread();
protected:
	std::map<SOCKET, SocketListener*>		mMapListener;
	std::map<SOCKET, SocketClient*>			mMapClient;
	std::map<SOCKET, Socket*>				mMapSocket;
	HANDLE									mIOCP;
	std::mutex								mMutex;
	std::queue<QueueResponse>				mQueueEvent;
	std::map<SOCKET, SocketAngent*>			mQueueClose;
	std::vector<std::thread*>				mWorkerThreads;
	int										mWorkerThreadsCount;

};

