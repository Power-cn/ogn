#pragma once

class Session;

class Player : public Entity
{
protected:
	uint32				accountId;
	uint32				userId;
	std::string			user;
	std::string			password;
	Session*			session;
	std::string			mJson;
public:
	Player();
	~Player();
	virtual bool Update(float time, float delay);
	void setSession(Session* s) { session = s; }
	Session* getSession() { return session; }
	void setAccountId(uint32 plrId) { accountId = plrId; }
	uint32 getAccountId() { return accountId; }
	uint32 getUserId() { return userId; }
	void setUserId(uint32 rId) { userId = rId; }
	void bindSession(Session* session);
	void unbindSession();
	void setUser(const std::string& u) { user = u; }
	const std::string& getUser() { return user; }
public:
	void onCreate();
public:
	virtual void sendPacket(Packet& packet);
	virtual void sendBuffer(void* data, int32 count);
	virtual void sendPacketToView(Packet& packet);
	virtual void sendPacketToMap(Packet& packet);
	virtual void sendPacketToWorld(Packet& packet);
	virtual void sendPacketToTeam(Packet& packet);
	virtual void sendPacketToTarget(Packet& packet, Entity* tar);
	virtual void sendRespnoseMsg(int32 msgId, std::vector<std::string>* msgParams = NULL);
public:
	bool onLoad(Dictionary& dict);
	bool onSave(Dictionary& dict);
protected:
	bool onLoadJson(Dictionary& dict);
	bool onSavejson(Dictionary& dict);
	bool onLoadJson(Json::Value& root);
	bool onSavejson(Json::Value& root);
protected:
	virtual bool onEnterMap();
	virtual bool onLeaveMap();

	// tar enter this;
	virtual bool onEnterView(Entity* tar);
	virtual bool onLeaveView(Entity* tar);

	virtual bool onEnterPlayerView(Player* plr);
	virtual bool onLeavePlayerView(Player* plr);
public:
	uint32 GetOnlineTimer() { return mOnlineTimer; }
	void SetOnlineTimer(uint32 t) { mOnlineTimer = t; }
	uint32 GetOfflineTimer() { return mOfflineTimer; }
	void SetOfflineTimer(uint32 t) { mOfflineTimer = t; }
protected:
	uint32	mOnlineTimer;
	uint32	mOfflineTimer;
};