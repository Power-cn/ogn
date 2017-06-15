#pragma once

class Packet;
class CircularBuffer;
struct event;
class Socket : public EventDispatcher
{
public:
	friend class Network;
	Socket(void);
	~Socket(void);
	int32 getSocketId() { return mSocketId; }

	void sendPacket(Packet& packet);
	void sendPacket(void* data, int32 count);
	void sendBuffer(void* data, int32 count);

	const char* getIP() { return mIP.c_str(); }
	short getPort() { return mPort; }
private:
	void setSocketId(int32 socketId) { mSocketId = socketId; }
	void setHost(const std::string& host) { mIP = host; }
	void setPort(short port) { mPort = port; }
public:
	Network*						network;
	event*							readEvent;
	event*							writeEvent;
	CircularBuffer*					readStream;
	std::queue<StreamBuffer>		queueSend;
	bool							startSend;
	uint32							packetCount;
protected:
	int32							mSocketId;
	std::string						mIP;
	int16							mPort;
};

