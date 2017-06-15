#pragma once

class Player;
class Socket;
class Session
{
	friend class SessionManager;
public:
	Session(Socket* s);
	~Session();
public:
	void setPlayer(Player* plr) { mPlayer = plr; }
	Player* getPlayer() { return mPlayer; }
	void setSessionId(SessionID sId) { mSessionId = sId; }
	SessionID getSessionId() { return mSessionId; }
	void setSocket(Socket* s) { mSocket = s; }
	Socket* getSocket() { return mSocket; }
	uint32 getSocketId();

	void sendPacketToTarget(Packet& packet, Socket* target);
	void sendBufferToTarget(void* data, int32 count, Socket* target);

	void sendPacket(Packet& packet);
	void sendBuffer(void* data, int32 count);

	void sendPacketToWorld(Packet& packet);
	void sendBufferToWorld(void* data, int32 count);
protected:
	static		uint32 sId;
protected:
	SessionID	mSessionId;
	Player*		mPlayer;
	Socket*		mSocket;
};