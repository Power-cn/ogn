#include "Shared.hpp"
#include <event.h>

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

static struct event_base* s_base_event = NULL;

Network::Network()
{
	INSTANCE(WinNet);
	s_base_event = event_base_new();
}

Network::~Network()
{

}

void Network::update(float32 time, float32 dealy)
{
	event_base_loop(s_base_event, EVLOOP_NONBLOCK);

	if (m_queue_close_socket_.size() > 0)
	{
		std::pair<SocketAngent*, int>& close_s = m_queue_close_socket_.front();
		if (close_s.first != NULL)
			OnExit(getSocket(close_s.second));

		m_queue_close_socket_.pop();
	}
}

SocketServer* Network::listen(int16 port, const char* host /* = 0 */)
{
	struct sockaddr_in addr;
	uint32 socketId = (uint32)socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	int32 _error = 0;
	_error = ::bind(socketId, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (_error == SOCKET_ERROR)
		return NULL;

	_error = ::listen(socketId, 20);
	if (_error == SOCKET_ERROR)
		return NULL;

	SocketServer* socketServer = newSocketServer();
	socketServer->setSocketId(socketId);
	struct event* event_ = socketServer->event_;
	socketServer->network = this;
	this->addAcceptor(socketServer, socketId);
	event_set(event_, socketId, EV_READ | EV_PERSIST, on_accept, socketServer);
	event_base_set(s_base_event, event_);
	event_add(event_, NULL);

	return socketServer;
}

SocketClient* Network::connect(const std::string& host, int16 port)
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	Shared::inet_addr(AF_INET, host.c_str(), &addr.sin_addr.s_addr);
	addr.sin_port = htons(port);
	SocketClient* socketClient = newSocketClient();
	socketClient->network = this;
	struct bufferevent* event_ = bufferevent_socket_new(s_base_event, -1, BEV_OPT_CLOSE_ON_FREE);
	socketClient->buffer = event_;

	bufferevent_setcb(event_, NULL, NULL, on_connect, socketClient);
	bufferevent_enable(event_, EV_PERSIST);
	if (bufferevent_socket_connect(event_, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		SAFE_DELETE(socketClient);
		return NULL;
	}
	return socketClient;
}

void Network::addSocket(Socket* sock)
{
	if (getSocket(sock->getSocketId()) == NULL)
		m_map_socket_.insert(std::make_pair(sock->getSocketId(), sock));
}

void Network::removeSocket(Socket* sock)
{
	MapSocket::iterator itr;
	itr = m_map_socket_.find(sock->getSocketId());
	if (itr != m_map_socket_.end())
	{
		m_map_socket_.erase(itr);
		delete sock;
	}
}


Socket* Network::getSocket(int32 socketId)
{
	MapSocket::iterator itr;
	itr = m_map_socket_.find(socketId);
	if (itr != m_map_socket_.end())
		return itr->second;

	return NULL;
}

void Network::addClient(SocketClient* client)
{
	if (getClient(client->getSocketId()) == NULL)
		m_map_connector_.insert(std::make_pair(client->getSocketId(), client));
}

void Network::removeClient(SocketClient* client)
{
	MapConnector::iterator itr;
	itr = m_map_connector_.find(client->getSocketId());
	if (itr != m_map_connector_.end())
	{
		m_map_connector_.erase(itr);
		delete client;
	}
}

SocketClient* Network::getClient(int32 socketId)
{
	MapConnector::iterator itr;
	itr = m_map_connector_.find(socketId);
	if (itr != m_map_connector_.end())
		return itr->second;

	return NULL;
}

Socket* Network::newSocket()
{
	return new Socket;
}

SocketClient* Network::newSocketClient()
{
	return new SocketClient;
}

SocketServer* Network::newSocketServer()
{
	return new SocketServer;
}

void Network::addAcceptor(SocketServer* acceptor, int32 sock)
{
	if (getAcceptor(sock) == NULL)
		m_map_acceptor_.insert(std::make_pair(sock, acceptor));
}

void Network::removeAcceptor(SocketServer* acceptor, int32 sock)
{
	{
		MapAcceptor::iterator itr;
		itr = m_map_acceptor_.find(sock);
		if (itr != m_map_acceptor_.end())
		{
			acceptor->removeSocket(sock);
			m_map_acceptor_.erase(itr);
		}
	}
}

SocketServer* Network::getAcceptor(int32 socketId)
{
	MapAcceptor::iterator itr;
	itr = m_map_acceptor_.find(socketId);
	if (itr != m_map_acceptor_.end())
		return itr->second;

	return NULL;
}

void Network::on_accept(int32 socketId, int16 _event, void* _arg)
{
	struct sockaddr_in addr;
	int _addr_size = 0;
	_addr_size = sizeof(addr);

	SocketServer* server = (SocketServer*)_arg;

	Network* network = server->network;

	Socket* socket = network->newSocket();
	socket->network = network;
	int32 acceptSocketId = (int32)::accept(socketId, (struct sockaddr*)&addr, &_addr_size);
	socket->setSocketId(acceptSocketId);

	event_set(socket->readEvent, acceptSocketId, EV_READ, Network::on_read, _arg);
	event_base_set(s_base_event, socket->readEvent);
	
	event_set(socket->writeEvent, acceptSocketId, EV_WRITE, Network::on_write, _arg);
	event_base_set(s_base_event, socket->writeEvent);

	socket->setHost(Shared::inet_ntoa(&addr.sin_addr.s_addr).c_str());
	socket->setPort(ntohs(addr.sin_port));

	network->addAcceptor(server, socket->getSocketId());
	network->OnAccept(server, socket);

	event_add(socket->readEvent, NULL);
}

void Network::on_connect(struct bufferevent* buffer, int16 _event, void* _arg)
{
	SocketClient* connector = (SocketClient*)_arg;
	Network* network = (Network*)connector->network;

	//int err = GetLastError();
	if (_event & BEV_EVENT_CONNECTED)
	{
		Socket* socket = network->newSocket();
		socket->network = network;
		int32 socketId = (int32)bufferevent_getfd(buffer);
		socket->setSocketId(socketId);
		connector->setSocketId(socketId);
		connector->setSocket(socket);

		sockaddr_in addr;
		int32 addr_len = sizeof(addr);
		getsockname(socketId, (sockaddr*)&addr, &addr_len);
		socket->setHost(Shared::inet_ntoa(&addr.sin_addr.s_addr).c_str());
		socket->setPort(ntohs(addr.sin_port));

		event_set(socket->readEvent, socketId, EV_READ, Network::on_read, _arg);
		event_base_set(s_base_event, socket->readEvent);

		event_set(socket->writeEvent, socketId, EV_WRITE, Network::on_write, _arg);
		event_base_set(s_base_event, socket->writeEvent);

		network->addClient(connector);
		network->OnConnect(connector, socket);

		event_add(socket->readEvent, NULL);
		return;
	}

	SocketEvent se;
	se.event = SocketEvent::EXCEPTION;
	connector->dispatch(se);
	SAFE_DELETE(connector);
}

void Network::on_write(int32 socketId, int16 _event, void* _arg)
{
	SocketAngent* angent = (SocketAngent*)_arg;
	Network* network = (Network*)angent->network;

	Socket* socket = network->getSocket(socketId);
	if (socket == NULL)
		return;

	if (socket->queueSend.size() <= 0)
	{
		socket->startSend = false;
		return;
	}

	int32 sendSize = 0;
	while (true)
	{
		if (socket->queueSend.size() <= 0)
			break;

		StreamBuffer packet = socket->queueSend.front();
		if (sendSize + packet.length > PACKET_MAX_LENGTH)
			break;

		memcpy(sPacketBuffer_0 + sendSize, packet.buffer, packet.length);
		sendSize += packet.length;
		socket->queueSend.pop();
		SAFE_FREE(packet.buffer);
	}

	int32 size = ::send(socketId, (const int8*)sPacketBuffer_0, sendSize, 0);
	if (size <= 0)
	{
		socket->startSend = false;
		return;
	}

	SocketEvent event;
	event.event = SocketEvent::SEND;
	event.socket = socket;
	event.data = sPacketBuffer_0;
	event.count = sendSize;

	angent->dispatch(event);
	socket->dispatch(event);

	if (socket->queueSend.size() <= 0)
	{
		socket->startSend = false;
		return;
	}
	socket->startSend = true;
	event_add(socket->writeEvent, NULL);
}

void Network::on_read(int32 socketId, int16 _event, void* _arg)
{
	SocketAngent* angent = (SocketAngent*)_arg;
	Network* network = angent->network;

	Socket* socket = network->getSocket(socketId);
	if (socket == NULL)
		return;

	CircularBuffer* readBuffer = socket->readStream;
	if (readBuffer->get_space_length() <= 0)
	{
		network->addCloseSocket(angent, socket->getSocketId());
		// 读取数据的环形缓冲不已满;
		LOG_ERROR("readBuffer full");
		return;
	}

	int32 len = ::recv(socketId, sPacketBuffer_0, PACKET_MAX_LENGTH, 0);
	if (len <= 0)
	{
		network->addCloseSocket(angent, socket->getSocketId());
		// 正常下线;
		return;
	}
	if (readBuffer->get_space_length() < (uint32)len)
	{
		network->addCloseSocket(angent, socket->getSocketId());
		LOG_ERROR("readBuffer data not enough");
		return;
	}
	readBuffer->Push(sPacketBuffer_0, len);
	socket->packetCount = 0;
	while (true)
	{
		bool recv_ = false;
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
						network->addCloseSocket(angent, socket->getSocketId());
						return;
					}

					socket->packetCount++;

					//int8* output = sPacketBuffer_1;
					//int32 output_count = PACKET_MAX_LENGTH;

					//if (!socket->unCompression((int8*)data, len, output, output_count))
					//{
					//	network->addCloseSocket(angent, socket->getSocketId());
					//	return;
					//}
					//RecvPacket recvPacket;
					//recvPacket.buffer.length = len;
					//recvPacket.buffer.buffer = new int8[recvPacket.buffer.length];
					//memcpy(recvPacket.buffer.buffer, data, len);
					//recvPacket.socket = socket;
					//recvPacket.angent = angent;
					//network->mQueueRecvPacket.push(recvPacket);

					SocketEvent event;
					event.event = SocketEvent::RECV;
					event.socket = socket;
					//event.data = output;
					//event.count = output_count;

					event.data = (void*)data;
					event.count = len;

					angent->dispatch(event);
					socket->dispatch(event);
					recv_ = true;
				}
				else
				{
					network->addCloseSocket(angent, socket->getSocketId());
					return;
				}
			}
		}

		if (recv_ == false)
			break;
	}

	event_add(socket->readEvent, NULL);
}

void Network::addCloseSocket(SocketAngent* angent, int32 socketId)
{
	//getSocket()
	m_queue_close_socket_.push(std::make_pair(angent, socketId));
}

void Network::postSend(Socket* socket, Packet& packet)
{
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	postSend(socket, in.getPtr(), in.getWPostion());
}

void Network::postSend(Socket* socket, const void* data, int32 count)
{
	StreamBuffer packet;
	packet.length = count + sizeof(int);
	packet.buffer = (int8*)malloc(packet.length);

	BinaryStream in(packet.buffer, packet.length);
	in << packet.length;
	in.WriteBytes(data, count);

	socket->queueSend.push(packet);
	if (socket->startSend)
		return;

	socket->startSend = true;
	event_add(socket->writeEvent, NULL);
}

void Network::postSendEncode(Socket* socket, const void* data, int32 count)
{
	if (count > PACKET_MAX_LENGTH)
	{
		LOG_ERROR("postSendEncode data too long");
		return;
	}
	int32 outputCount = PACKET_MAX_LENGTH;
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	if (!compression((char*)data, count, sPacketBuffer, outputCount))
		return;

	postSend(socket, (void*)sPacketBuffer, outputCount);
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


void Network::OnAccept(SocketServer* acceptor, Socket* socket)
{
	acceptor->addSocket(socket);
	addSocket(socket);

	SocketEvent event;
	event.event = SocketEvent::ACCEPT;
	event.socket = socket;
	socket->dispatch(event);
	acceptor->dispatch(event);
}

void Network::OnConnect(SocketClient* connector , Socket* socket)
{
	addSocket(socket);

	SocketEvent event;
	event.event = SocketEvent::CONNECT;
	event.socket = socket;

	socket->dispatch(event);
	connector->dispatch(event);
}

void Network::OnExit(Socket* socket)
{
	if (socket == NULL)
		return;

	int32 socketId = socket->getSocketId();

	SocketClient* connctor = getClient(socketId);
	if (connctor)
		connctor->setSocket(NULL);
	SocketServer* accept = getAcceptor(socketId);

	SocketEvent event;
	event.event = SocketEvent::EXIT;
	event.socket = socket;

	if (connctor)
		connctor->dispatch(event);

	if (accept)
		accept->dispatch(event);

	socket->dispatch(event);

	removeSocket(socket);

	if (connctor)
		removeClient(connctor);

	if (accept)
		removeAcceptor(accept, socketId);
}
