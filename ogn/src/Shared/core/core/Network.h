#pragma once

class Socket;
class Packet;
class SocketAngent;
class SocketServer;
class SocketClient;

struct RecvPacket
{
	StreamBuffer buffer;
	Socket* socket;
	SocketAngent* angent;
};

class Network
{
	friend class Socket;
public:
	Network();
	~Network();
	void update(float32 time, float32 dealy);
public:
	SocketServer* listen(int16 port, const char* host = 0);
	SocketClient* connect(const std::string& host, int16 port);
	void addCloseSocket(SocketAngent* angent, int32 socketId);
	Socket* getSocket(int32 socketId);
	SocketServer* getAcceptor(int32 socketId);
	SocketClient* getClient(int32 socketId);

protected:
	virtual Socket* newSocket();
	virtual SocketClient* newSocketClient();
	virtual SocketServer* newSocketServer();
protected:
	typedef std::map<int32, Socket*>					MapSocket;
	typedef std::map<int32, SocketClient*>			MapConnector;
	typedef std::map<int32, SocketServer*>			MapAcceptor;
	typedef std::map<SocketServer*, std::set<int32>>   MapAcceptorHandlerSocket;
protected:
	void postSend(Socket* socket, Packet& packet);
	void postSend(Socket* socket, const void* data, int32 count);
	void postSendEncode(Socket* socket, const void* data, int32 count);

	bool compression(char* input, int inCount, char* output, int& outCount);
	bool unCompression(char* input, int inCount, char* output, int& outCount);
protected:
	virtual void OnAccept(SocketServer* acceptor, Socket* socket);
	virtual void OnConnect(SocketClient* connector , Socket* socket);
	virtual void OnExit(Socket* socket);
protected:
	void addSocket(Socket* sock);
	void removeSocket(Socket* sock);
	void addClient(SocketClient* client);
	void removeClient(SocketClient* client);
	void addAcceptor(SocketServer* acceptor, int32 sock);
	void removeAcceptor(SocketServer* acceptor, int32 sock);
protected:
	static void on_accept(int32 socketId, int16 _event, void* _arg);
	static void on_write(int32 socketId, int16 _event, void* _arg);
	static void on_read(int32 socketId, int16 _event, void* _arg);
	static void on_connect(struct bufferevent* buffer, int16 _event, void* _arg);
protected:
	MapSocket										m_map_socket_;
	MapConnector									m_map_connector_;
	MapAcceptor										m_map_acceptor_;
	std::queue<RecvPacket>							mQueueRecvPacket;
	std::queue<std::pair<SocketAngent*, int32>>		m_queue_close_socket_;
protected:
	static							int8 sPacketBuffer_0[PACKET_MAX_LENGTH];
	static							int8 sPacketBuffer_1[PACKET_MAX_LENGTH];
};
