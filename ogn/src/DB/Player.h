#pragma once


class Session;
class Player : public Entity
{
public:
	uint32		playerId;
	Session*	session;

	void setSession(Session* s) { session = s; }
	Session* getSession() { return session; }
	void setPlayerId(uint32 plrId) { playerId = plrId; }
	uint32 getPlayerId() { return playerId; }
protected:
private:
};