#pragma once


class Session;
class Player : public Entity
{
public:
	uint32		accId;
	Session*	session;

	void setSession(Session* s) { session = s; }
	Session* getSession() { return session; }
	void setAccId(uint32 plrId) { accId = plrId; }
	uint32 getAccId() { return accId; }
protected:
	//DBAccount*					mAccount;
	//std::vector<DBUser*>		mLstUsers;
};