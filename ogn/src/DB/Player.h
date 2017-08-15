#pragma once


class Session;
class Player : public Entity
{
public:
	uint32		accId = 0;
	uint32		mUserId = 0;
	Session*	session;
	void sendPacket(Packet& packet);

	void setSession(Session* s) { session = s; }
	void setAccId(uint32 accId) { accId = accId; }
	void setUserId(uint32 userId) { mUserId = userId; }

	Session* getSession() { return session; }
	uint32 getAccId() { return accId; }
	uint32 getUserId() { return mUserId; }
protected:
	//DBAccount*					mAccount;
	//std::vector<DBUser*>		mLstUsers;
};