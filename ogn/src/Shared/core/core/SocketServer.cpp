#include "Shared.hpp"
#include <event.h>


SocketServer::SocketServer()
{
	event_ = (struct event*)malloc(sizeof(event));
}

SocketServer::~SocketServer()
{
	clearSocket();

	event_free(event_);
	event_ = NULL;
}

Socket* SocketServer::addSocket(Socket* sock)
{
	if (getSocket(sock->getSocketId()))
		return NULL;

	_clients.insert(std::make_pair(sock->getSocketId(), sock));
	return sock;
}

void SocketServer::removeSocket(int32 socketId)
{
	std::map<int32, Socket*>::iterator itr = _clients.find(socketId);
	if (itr != _clients.end())
		_clients.erase(itr);
}

Socket* SocketServer::getSocket(int32 socketId)
{
	std::map<int32, Socket*>::iterator itr = _clients.find(socketId);
	if (itr != _clients.end())
		return itr->second;

	return NULL;
}

void SocketServer::clearSocket()
{
	_clients.clear();
}

void SocketServer::sendPacket(Socket* socket, Packet& packet)
{
	if (socket == NULL)
		return;

	socket->sendPacket(packet);
}

void SocketServer::sendPacket(int32 socketId, Packet& packet)
{
	Socket* socket = getSocket(socketId);
	if (socket == NULL)
		return;

	socket->sendPacket(packet);
}

void SocketServer::sendBuffer(Socket* socket, void* data, int32 count)
{
	if (socket == NULL)
		return;

	socket->sendBuffer(data, count);
}

void SocketServer::sendBuffer(int32 socketId, void* data, int32 count)
{
	Socket* socket = getSocket(socketId);
	if (socket == NULL)
		return;

	socket->sendBuffer(data, count);
}

void SocketServer::sendPacketToAll(Packet& packet)
{
	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	in << packet;
	sendBufferToAll(in.getPtr(), in.getWPostion());
}

void SocketServer::sendBufferToAll(void* data, int32 count)
{
	for (auto itr: _clients)
	{
		itr.second->sendBuffer(data, count);
	}
}
