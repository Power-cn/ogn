#include "Shared.hpp"

int8 Network::sPacketBuffer_0[PACKET_MAX_LENGTH] = { 0 };
int8 Network::sPacketBuffer_1[PACKET_MAX_LENGTH] = { 0 };

class WinNet
{
public:
	WinNet()
	{
#ifdef WIN32
		WSADATA data;
		WSAStartup(0x0202, &data);
#endif
	}
	~WinNet()
	{
#ifdef WIN32
		WSACleanup();
#endif // WIN32
	}
protected:
	bool mStart;
};

Network::Network()
{
	INSTANCE(WinNet);
	mIOCPModel = new IOCPModel(this);
}

Network::~Network()
{
	SAFE_DELETE(mIOCPModel);
}

void Network::update(float32 time, float32 dealy)
{
	mIOCPModel->loop();
}

SocketListener* Network::listen(int16 port, const std::string host /* = "" */)
{
	return mIOCPModel->listen(host, port);
}

SocketClient* Network::connect(const std::string& host, int16 port)
{
	return mIOCPModel->connect(host, port);
}

void Network::addCloseSocket(SocketAngent* angent, int32 socketId)
{
	mIOCPModel->PushQueueClose(angent, socketId);
}

Socket* Network::newSocket()
{
	return new Socket;
}

SocketClient* Network::newSocketClient()
{
	return new SocketClient;
}

SocketListener* Network::newSocketListener()
{
	return new SocketListener;
}

void Network::postSend(Socket* socket, Packet& packet)
{
	mIOCPModel->postSend(socket, packet);
}

void Network::postSend(Socket* socket, void* dataBuffer, int dataCount)
{
	mIOCPModel->postSend(socket, dataBuffer, dataCount);
}

void Network::postSendEncode(Socket* socket, void* dataBuffer, int dataCount)
{
	mIOCPModel->postSendEncode(socket, dataBuffer, dataCount);
}



SocketListener* Network::AddListener(SocketListener* listener)
{
	mMapListener.insert(std::make_pair(listener->getSocketId(), listener));
	return listener;
}

SocketListener* Network::FindListener(uint32 socketId)
{
	auto itr = mMapListener.find(socketId);
	if (itr != mMapListener.end()) return itr->second;

	return NULL;
}

void Network::DelListener(uint32 socketId)
{
	auto itr = mMapListener.find(socketId);
	if (itr != mMapListener.end())
	{
		delete itr->second;
		mMapListener.erase(itr);
	}
}

SocketClient* Network::AddClient(SocketClient* client)
{
	mMapClient[client->getSocketId()] = client;
	return client;
}

SocketClient* Network::FindClient(uint32 socketId)
{
	auto itr = mMapClient.find(socketId);
	if (itr != mMapClient.end()) return itr->second;
	return NULL;
}

void Network::DelClient(uint32 socketId)
{
	auto itr = mMapClient.find(socketId);
	if (itr != mMapClient.end())
	{
		delete itr->second;
		mMapClient.erase(itr);
	}
}

Socket* Network::AddSocket(Socket* socket)
{
	mMapSocket[socket->getSocketId()] = socket;
	return socket;
}

Socket* Network::FindSocket(uint32 socketId)
{
	auto itr = mMapSocket.find(socketId);
	if (itr != mMapSocket.end()) return itr->second;
	return NULL;
}

void Network::DelSocket(uint32 socketId)
{
	auto itr = mMapSocket.find(socketId);
	if (itr != mMapSocket.end())
	{
		delete itr->second;
		mMapSocket.erase(itr);
	}
}

void Network::OnAccept(SocketListener* listener, Socket* socket)
{
	SocketEvent event;
	event.event = SocketEvent::ACCEPT;
	event.socket = socket;
	listener->addSocket(socket);
	socket->dispatch(event);
	listener->dispatch(event);
}

void Network::OnConnect(SocketClient* client, Socket* socket)
{
	SocketEvent event;
	event.event = SocketEvent::CONNECT;
	event.socket = socket;

	socket->dispatch(event);
	client->dispatch(event);
}

void Network::OnRecv(Socket* socket)
{
	CircularBuffer* readBuffer = socket->readStream;
	SocketAngent* angent = socket->angent;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;

	if (readBuffer->get_space_length() <= 0)
	{
		addCloseSocket(angent, socket->getSocketId());
		// ��ȡ���ݵĻ��λ��岻����;
		LOG_ERROR("readBuffer full");
		return;
	}

	if (readBuffer->get_space_length() < (uint32)ioOverlapped.dwBytesTransferred)
	{
		addCloseSocket(angent, socket->getSocketId());
		LOG_ERROR("readBuffer data not enough");
		return;
	}
	readBuffer->Push(ioOverlapped.dataBuffer, ioOverlapped.dwBytesTransferred);
	//socket->packetCount = 0;
	while (true)
	{
		bool recvPacket = false;
		int32 packetCount = 0;
		if (readBuffer->Read(&packetCount, sizeof(packetCount), sizeof(packetCount)))
		{
			packetCount = Shared::ntohl(packetCount);
			if (readBuffer->get_data_length() >= (uint32)packetCount)
			{
				int8* packetBuffer = sPacketBuffer_0;
				if (readBuffer->Pop(packetBuffer, PACKET_MAX_LENGTH, packetCount))
				{
					const int8* data = packetBuffer + sizeof(int32);
					int32 len = packetCount - sizeof(int32);

					if (len > PACKET_MAX_LENGTH)
					{
						addCloseSocket(socket->angent, socket->getSocketId());
						return;
					}

					//socket->packetCount++;
					SocketEvent event;
					event.event = SocketEvent::RECV;
					event.socket = socket;
					event.data = (void*)data;
					event.count = len;

					angent->dispatch(event);
					socket->dispatch(event);
					recvPacket = true;
				}
				else
				{
					addCloseSocket(angent, socket->getSocketId());
					return;
				}
			}
		}

		if (!recvPacket)break;
	}
}

void Network::OnSend(Socket* socket)
{
	SocketAngent* angent = socket->angent;
	IO_OVERLAPPED& ioOverlapped = socket->readOverlapped;

	SocketEvent event;
	event.event = SocketEvent::SEND;
	event.socket = socket;
	event.data = ioOverlapped.dataBuffer;
	event.count = ioOverlapped.dwBytesTransferred;

	angent->dispatch(event);
	socket->dispatch(event);
}

void Network::OnExit(Socket* socket)
{
	int32 socketId = socket->getSocketId();

	SocketAngent* angent = socket->angent;

	SocketClient* client = FindClient(angent->getSocketId());
	SocketListener* listener = FindListener(angent->getSocketId());

	SocketEvent event;
	event.event = SocketEvent::EXIT;
	event.socket = socket;

	if (client)
		client->dispatch(event);

	if (listener)
		listener->dispatch(event);

	socket->dispatch(event);

	DelSocket(socketId);

	if (client)
		DelClient(socketId);

	if (listener)
		listener->delSocket(socketId);
}

void Network::OnException(Socket* socket)
{
	uint32 socketId = socket->getSocketId();
	SocketAngent* angent = socket->angent;
	SocketEvent se;
	se.event = SocketEvent::EXCEPTION;
	socket->dispatch(se);
	angent->dispatch(se);
	DelSocket(socketId);
	DelClient(socketId);
}

bool Network::unCompression(char* input, int inCount, char* output, int& outCount)
{
	char* bytes = NULL;
	Compression::zlib_UnCompression((char*)input, inCount, bytes, outCount);
	if (outCount > PACKET_MAX_LENGTH || outCount <= 0)
	{
		LOG_ERROR("error: %s", __FUNCTION__);
		delete bytes;
		return false;
	}
	memcpy_s(output, PACKET_MAX_LENGTH, bytes, outCount);
	delete bytes;
	return true;
}

bool Network::compression(char* input, int inCount, char* output, int& outCount)
{
	char* bytes = NULL;
	Compression::zlib_Compression((char*)input, inCount, bytes, outCount);
	if (outCount > PACKET_MAX_LENGTH || outCount <= 0)
	{
		LOG_ERROR("error: %s", __FUNCTION__);
		delete bytes;
		return false;
	}
	memcpy_s(output, PACKET_MAX_LENGTH, bytes, outCount);
	delete bytes;
	return true;
}