#pragma once

class Packet;
class CircularBuffer;
class SocketAngent;
class Socket;

enum SocketState
{
	IOState_None,
	IOState_Accept,
	IOState_Connect,
	IOState_Recv,
	IOState_Send,
};

struct IO_OVERLAPPED
{
	OVERLAPPED							overlapped = { 0 };
	WSABUF								wBuffer = { 0 };
	SocketState							ioState = IOState_None;
	char								dataBuffer[PACKET_MAX_LENGTH] = { 0 };
	int32								dataBufferCount = PACKET_MAX_LENGTH;
	uint32								dwBytesTransferred = 0;
	uint32								dwResult = 0;
	Socket*								socket = NULL;
};

class Socket : public EventDispatcher
{
public:
	friend class Network;
	friend class IOCPModel;
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
	Network*						network = NULL;
	SocketAngent*					angent = NULL;
public:
	CircularBuffer*					readStream;
	IO_OVERLAPPED					readOverlapped;
	IO_OVERLAPPED					writeOverlapped;
	std::queue<StreamBuffer>		sendQueue;
	bool							startSend = false;
	uint32							postCount = 0;
protected:
	int32							mSocketId;
	std::string						mIP;
	int16							mPort;
};

