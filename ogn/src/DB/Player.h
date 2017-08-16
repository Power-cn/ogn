#pragma once


class Session;
class Player : public Entity
{
public:
	uint32		accId = 0;
	uint32		mUserId = 0;
	Session*	ssn;
	void sendPacket(Packet& packet);

	void setSession(Session* s) { ssn = s; }
	void setAccId(uint32 accId) { accId = accId; }
	void setUserId(uint32 userId) { mUserId = userId; }

	Session* getSession() { return ssn; }
	uint32 getAccId() { return accId; }
	uint32 getUserId() { return mUserId; }
protected:
	//DBAccount*					mAccount;
	//std::vector<DBUser*>		mLstUsers;
};