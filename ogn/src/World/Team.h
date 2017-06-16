#pragma once

#include "Shared.h"

class Player;
class Packet;

class TeamEntity
{
public:
	void setPlayer(Player* plr) { mPlayer = plr; }
	Player* getPlayer() { return mPlayer; }
	void setName(const std::string& name) { mName = name; }
	const std::string& getName() { return mName; }
	uint32 getUserId() { return mUserId; }
	void setUserId(uint32 rId) { mUserId = rId; }
	bool getDel() { return mIsdel; }
	void setDel(bool del) { mIsdel = del; }
protected:
	Player*			mPlayer;
	std::string		mName;
	uint32			mUserId;
	bool			mIsdel;
};

class Team
{
	friend class TeamModule;
public:
	Team(Player* leader);
	~Team();
	void Update();
	uint32 getId() { return mInstanceId; }
	void sendPacketToAll(Packet& packet);
	Player* getLeader() { return mLeader ? mLeader->getPlayer() : NULL; }
	TeamEntity* getLeaderEntity() { return mLeader; }
	bool addPlayer(Player* player);
	bool destoryPlayer(uint32 userId);
	bool removePlayer(uint32 userId);
	TeamEntity* getPlayer(uint32 userId);
	bool ChangeLeader(Player* newLeader);

	int32 CanAddTeam(Player* tar);

	void onEnterWorld(Player* player);
	void onLeaveWorld(Player* player);

	uint32 getPlayerCount();
	TeamEntity* getPlayerEntity(uint32 idx);
protected:
	TeamEntity*	ChooseLeader();
protected:
	bool onEnterTeam(Player* player);
	bool onLeaveTeam(Player* player);
	bool onChangeLeader(Player* oldLeader, Player* newLeader);
protected:
	static uint32 sId;
protected:
	uint32						mInstanceId;
	TeamEntity*					mLeader;
	std::vector<TeamEntity*>	mTeamEntityList;
};