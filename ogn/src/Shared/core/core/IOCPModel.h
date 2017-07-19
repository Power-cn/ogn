#pragma once


class SocketAngent;
class Socket;

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
	friend class Network;
	IOCPModel(Network* network);
	~IOCPModel();
	SocketListener* listen(const std::string& host, short port);
	SocketClient* connect(const std::string& host, short port);
	bool loop();
protected:
	void postSend(Socket* socket, Packet& packet);
	void postSend(Socket* socket, void* dataBuffer, int dataCount);
	void postSendEncode(Socket* socket, void* dataBuffer, int dataCount);
protected:
	void PostAccept(SocketListener* listener);
	void PostConnect(SocketClient* client);
	void PostRead(Socket* socket);
	void PostWrite(Socket* socket);
protected:
	void DoAccept (uint32 socketId, Socket* socket);
	void DoConnect(uint32 socketId, Socket* socket);
	void DoRead(Socket* socket);
	void DoWrite(Socket* socket);
	void DoExit(Socket* socket);

	void PushQueueResponse(QueueResponse& response);
	void PushQueueClose(SocketAngent* angent, uint32 socketId);
protected:
	void WorkerThread();
protected:

	HANDLE									mIOCP;
	std::mutex								mMutex;
	std::queue<QueueResponse>				mQueueEvent;
	std::map<uint32, SocketAngent*>			mQueueClose;
	std::vector<std::thread*>				mWorkerThreads;
	uint32									mWorkerThreadsCount;
	Network*								mNetwork;

};

