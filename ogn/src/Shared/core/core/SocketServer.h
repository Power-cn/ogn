#pragma once

struct event;
class Socket;
class Packet;
class SocketServer : public SocketAngent
{
	friend class Network;
public:
	SocketServer();
	~SocketServer();

	Socket* addSocket(Socket* sock);
	void removeSocket(int32 socketId);
	Socket* getSocket(int32 socketId);
	void clearSocket();
	void sendPacket(Socket* socket, Packet& packet);
	void sendPacket(int32 socketId, Packet& packet);
	void sendBuffer(Socket* socket, void* data, int32 count);
	void sendBuffer(int32 socketId, void* data, int32 count);
	void sendPacketToAll(Packet& packet);
	void sendBufferToAll(void* data, int32 count);
public:
	struct event*		event_;
protected:
	std::map<int32, Socket*> _clients;
private:
};