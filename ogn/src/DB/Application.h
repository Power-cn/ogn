#pragma once

class SocketServer;
class SocketEvent;
class DBConnector;

class Application : public Object
{
public:
	Application();
	~Application();

	bool Initialize();
	bool Update();
	bool Destroy();

	SocketServer* getDBServer() { return dbServer; }
	DBConnector* getDBConnector() { return dbConnector; }
protected:
	int32 onDBAccept(SocketEvent& e);
	int32 onDBRecv(SocketEvent& e);
	int32 onDBExit(SocketEvent& e);
protected:
	SocketServer* dbServer;
	DBConnector* dbConnector;
private:
};