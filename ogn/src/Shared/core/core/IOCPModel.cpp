#include "Shared.hpp"

IOCPModel::IOCPModel(Network* network)
{
	mNetwork = network;
	mIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	mWorkerThreadsCount = sysInfo.dwNumberOfProcessors * 2 + 1;
	for (uint32 i = 0; i < mWorkerThreadsCount; ++i)
	{
		mWorkerThreads.push_back(new std::thread(&IOCPModel::WorkerThread, this));
	}
}

IOCPModel::~IOCPModel()
{
	PostQueuedCompletionStatus(mIOCP, 0, 0, NULL);
}

SocketListener* IOCPModel::listen(const std::string& host, short port)
{
	uint32 socketId = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
	SocketListener* listener = mNetwork->newSocketListener();
	listener->setSocketId(socketId);
	CreateIoCompletionPort((HANDLE)socketId, mIOCP, (ULONG_PTR)listener, 0);

	SOCKADDR_IN& addr = listener->getSockaddr();
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	::bind(socketId, (SOCKADDR*)&addr, sizeof(addr));
	::listen(socketId, 5);

	GUID guidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes = 0;
	WSAIoctl(socketId, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(guidAcceptEx), &listener->mPfn, sizeof(listener->mPfn),
		&dwBytes, NULL, NULL);

	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

	WSAIoctl(socketId, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockAddrs,
		sizeof(GuidGetAcceptExSockAddrs), &listener->mPfnSocketAddrs, sizeof(listener->mPfnSocketAddrs),
		&dwBytes, NULL, NULL);

	for (int32 i = 0; i < 10; i++)
	{
		PostAccept(listener);
	}
	mNetwork->AddListener(listener);
	return listener;
}

SocketClient* IOCPModel::connect(const std::string& host, short port)
{
	uint32 socketId = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);

	SocketClient* client = mNetwork->newSocketClient();
	client->setSocketId(socketId);

	SOCKADDR_IN& addr = client->getSockaddr();
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
		&guidConnectEx, sizeof(guidConnectEx), &client->mPfn, sizeof(client->mPfn),
		&dwBytes, NULL, NULL);
	PostConnect(client);

	mNetwork->AddClient(client);

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
	response.ioOverlapped->dwResult = response.result;
	IO_OVERLAPPED* ioOverlapped = response.ioOverlapped;
	SocketAngent* angent = response.angent;
	switch (response.ioOverlapped->ioState)
	{
	case IOState_Accept:
		DoAccept(angent->getSocketId(), ioOverlapped->socket);
		break;
	case IOState_Connect:
		DoConnect(angent->getSocketId(), ioOverlapped->socket);
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
		
		DoExit(mNetwork->FindSocket(itr->first));
	}
	return true;
}

void IOCPModel::postSend(Socket* socket, Packet& packet)
{
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	postSend(socket, in.getPtr(), in.getWPostion());
}

void IOCPModel::postSend(Socket* socket, void* dataBuffer, int dataCount)
{
	IO_OVERLAPPED& ioOverlapped = socket->writeOverlapped;

	StreamBuffer packet;
	packet.length = dataCount + sizeof(int);
	packet.buffer = new int8[packet.length];

	BinaryStream in(packet.buffer, packet.length);
	in << packet.length;
	in.WriteBytes(dataBuffer, dataCount);

	socket->sendQueue.push(packet);
	if (!socket->startSend)
		PostWrite(socket);
}

void IOCPModel::postSendEncode(Socket* socket, void* dataBuffer, int dataCount)
{

}

void IOCPModel::PostAccept(SocketListener* listener)
{
	Socket* socket = mNetwork->newSocket();
	socket->angent = listener;
	socket->network = mNetwork;
	uint32 socketId = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	socket->setSocketId(socketId);

	//setsockopt(socketId,
	//	SOL_SOCKET,
	//	SO_UPDATE_ACCEPT_CONTEXT,
	//	(char *)&sListenSocket,
	//	sizeof(sListenSocket));


	LPFN_ACCEPTEX pfn = listener->mPfn;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	memset(&ioOverlapped.overlapped, 0, sizeof(OVERLAPPED));
	ioOverlapped.socket = socket;
	ioOverlapped.ioState = IOState_Accept;
	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	ioOverlapped.wBuffer.len = ioOverlapped.dataBufferCount;

	DWORD dwBytes = 0;
	DWORD dwLocalAddressLength = sizeof(sockaddr_in) + 16;
	DWORD dwRemoteAddressLength = sizeof(sockaddr_in) + 16;
	DWORD dwReceiveDataLength = ioOverlapped.dataBufferCount - dwLocalAddressLength - dwRemoteAddressLength;

	BOOL ret = pfn(
		listener->getSocketId(),
		socket->getSocketId() ,
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

	mNetwork->AddSocket(socket);
}

void IOCPModel::PostConnect(SocketClient* client)
{
	Socket* socket = mNetwork->newSocket();
	socket->angent = client;
	socket->network = mNetwork;
	socket->setSocketId(client->getSocketId());
	client->setSocket(socket);

	CreateIoCompletionPort((HANDLE)socket->getSocketId(), mIOCP, (ULONG_PTR)client, 0);

	LPFN_CONNECTEX pfn = client->mPfn;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	memset(&ioOverlapped.overlapped, 0, sizeof(OVERLAPPED));
	ioOverlapped.socket = socket;
	ioOverlapped.ioState = IOState_Connect;

	sockaddr_in& addr = client->getSockaddr();
	int nLen = sizeof(SOCKADDR_IN);
	DWORD dwBytesSent = 0;
	// 重点
	BOOL bResult = pfn(
		client->getSocketId(),
		(sockaddr*)&addr,				// [in] 对方地址
		sizeof(addr),									// [in] 对方地址长度
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

	std::string host = Shared::inet_ntoa(&addr.sin_addr.s_addr);
	socket->setHost(host);


	//DWORD dwFlag = 0, dwTrans = 0;

	//if (!WSAGetOverlappedResult(client->socketId, &ioOverlapped.overlapped, &dwTrans, TRUE, &dwFlag))
	//{
	//	printf("等待异步结果失败\r\n");
	//	return;
	//}
	mNetwork->AddSocket(socket);
}

void IOCPModel::PostRead(Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	memset(&ioOverlapped.overlapped, 0, sizeof(OVERLAPPED));
	ioOverlapped.ioState = IOState_Recv;
	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	ioOverlapped.wBuffer.len = ioOverlapped.dataBufferCount;
	DWORD dwBufferCount = 1, dwRecvBytes = 0, Flags = 0;
	int result = WSARecv(socket->getSocketId(), &ioOverlapped.wBuffer, dwBufferCount, &dwRecvBytes, &Flags, &ioOverlapped.overlapped, NULL);
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
	socket->startSend = false;

	std::queue<StreamBuffer>& sendQueue = socket->sendQueue;

	ioOverlapped.wBuffer.buf = ioOverlapped.dataBuffer;
	if (sendQueue.size() <= 0) return;
	DWORD dwBufferCount = 1, dwRecvBytes = 0, Flags = 0;
	char* wBuffer = ioOverlapped.dataBuffer;
	int sendSize = 0;
	while (sendQueue.size() > 0)
	{
		StreamBuffer& packet = sendQueue.front();
		if (sendSize + packet.length > ioOverlapped.dataBufferCount)
			break;

		memcpy(wBuffer + sendSize, packet.buffer, packet.length);
		sendSize += packet.length;
		sendQueue.pop();
		delete[] packet.buffer;
	}
	socket->startSend = true;

	memset(&ioOverlapped.overlapped, 0, sizeof(OVERLAPPED));
	ioOverlapped.wBuffer.len = sendSize;
	ioOverlapped.ioState = IOState_Send;
	int result = WSASend(socket->getSocketId(), &ioOverlapped.wBuffer, dwBufferCount, &dwRecvBytes, Flags, &ioOverlapped.overlapped, NULL);
	DWORD dwError = WSAGetLastError();
	//if (ERROR_IO_PENDING != dwError)
	//{
	//	int err = WSAGetLastError();
	//	delete socket;
	//	return;
	//}
}

void IOCPModel::DoAccept(uint32 socketId, Socket* socket)
{
	SocketListener* listener = mNetwork->FindListener(socketId);
	if (listener == NULL)  return;
	CreateIoCompletionPort((HANDLE)socket->getSocketId(), mIOCP, (ULONG_PTR)listener, 0);

	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;

	SOCKADDR_IN* remote = NULL;
	SOCKADDR_IN* local = NULL;

	int32 remoteLen = sizeof(SOCKADDR_IN);
	int32 localLen = sizeof(SOCKADDR_IN);
	uint32 dwLocalAddressLength = sizeof(sockaddr_in) + 16;
	uint32 dwRemoteAddressLength = sizeof(sockaddr_in) + 16;
	uint32 dwReceiveDataLength = ioOverlapped.dataBufferCount - dwLocalAddressLength - dwRemoteAddressLength;
	sockaddr_in addr;
	int32 addrlen = sizeof(addr);
	getsockname(socket->getSocketId(), (sockaddr*)&addr, &addrlen);
	listener->mPfnSocketAddrs(
		ioOverlapped.dataBuffer,
		0,
		dwLocalAddressLength,
		dwRemoteAddressLength,
		(LPSOCKADDR*)&local, &localLen,
		(LPSOCKADDR*)&remote, &remoteLen);

	std::string host = Shared::inet_ntoa(&remote->sin_addr.s_addr);
	socket->setHost(host);

	mNetwork->OnAccept(listener, socket);
	PostRead(socket);

	PostAccept(listener);
}

void IOCPModel::DoConnect(uint32 socketId, Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	SocketAngent* angent = socket->angent;

	if (ioOverlapped.dwResult == 0 && ioOverlapped.dwBytesTransferred == 0)
	{
		SocketEvent se;
		se.event = SocketEvent::EXCEPTION;
		socket->dispatch(se);
		angent->dispatch(se);
		mNetwork->DelSocket(socketId);
		mNetwork->DelClient(socketId);
		return;
	}
	mNetwork->OnConnect((SocketClient*)angent, socket);
	PostRead(socket);
}

void IOCPModel::DoRead(Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;
	if (ioOverlapped.dwBytesTransferred <= 0)
	{
		PushQueueClose(socket->angent, socket->getSocketId());
		return;
	}

	mNetwork->OnRecv(socket);
	PostRead(socket);
}

void IOCPModel::DoWrite(Socket* socket)
{
	IO_OVERLAPPED& ioOverlapped = socket->writeOverlapped;
	if (ioOverlapped.dwBytesTransferred <= 0)
	{
		PushQueueClose(socket->angent, socket->getSocketId());
		return;
	}
	mNetwork->OnSend(socket);
	PostWrite(socket);
}

void IOCPModel::DoExit(Socket* socket)
{
	if (socket == NULL) return;
	mNetwork->OnExit(socket);
}


void IOCPModel::PushQueueResponse(QueueResponse& response)
{
	mMutex.lock();
	mQueueEvent.push(response);
	mMutex.unlock();
}

void IOCPModel::PushQueueClose(SocketAngent* angent, uint32 socketId)
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
