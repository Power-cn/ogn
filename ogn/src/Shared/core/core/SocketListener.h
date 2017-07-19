#pragma once

class Socket;
class Packet;
class SocketListener : public SocketAngent
{
public:
	SocketListener();
	~SocketListener();

	Socket* addSocket(Socket* sock);
	void delSocket(int32 socketId);
	Socket* getSocket(int32 socketId);
	void clearSocket();
	void sendPacket(Socket* socket, Packet& packet);
	void sendPacket(int32 socketId, Packet& packet);
	void sendBuffer(Socket* socket, void* data, int32 count);
	void sendBuffer(int32 socketId, void* data, int32 count);
	void sendPacketToAll(Packet& packet);
	void sendBufferToAll(void* data, int32 count);
public:
	LPFN_ACCEPTEX					mPfn;
	LPFN_GETACCEPTEXSOCKADDRS		mPfnSocketAddrs;
protected:
	std::map<int32, Socket*>		mSockets;
private:
};