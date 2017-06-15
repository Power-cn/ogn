#pragma once

class SocketServer;
class SocketClient;

class SocketEvent;
class Session;
class Packet;

class Application : public Object
{
public:
	Application();
	~Application();

	bool Initialize();
	bool Update();
	bool Destroy();

public:
	bool sendPacketToWorld(Packet& packet, Session* tar);
	bool sendBufferToWorld(int8* data, int32 count, Session* tar);
protected:
	int onGateAccept(SocketEvent& e);
	int onGateRecv(SocketEvent& e);
	int onGateExit(SocketEvent& e);

	int onWorldConnect(SocketEvent& e);
	int onWorldRecv(SocketEvent& e);
	int onWorldExit(SocketEvent& e);
	int onWorldException(SocketEvent& e);
protected:
	SocketServer* gateServer;
	SocketClient* worldServer;
private:
};