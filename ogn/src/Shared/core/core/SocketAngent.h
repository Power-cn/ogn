#pragma once

class SocketAngent :public EventDispatcher
{
	friend class Network;
public:
	SocketAngent();
	~SocketAngent();
	void setSocketId(int32 socketId) { mSocketId = socketId; }
	void setSocket(Socket* s) { mSocket = s; }
	Socket* getSocket() { return mSocket; }
private:
	uint32 getSocketId() { return mSocketId; }
	Network* network;
protected:
	uint32				mSocketId;
	Socket*				mSocket;
};