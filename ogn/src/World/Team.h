#pragma once

#include "Shared.h"

class Player;
class Packet;

class TeamEntity
{
public:
	Player*			player;
	std::string		name;
	uint32			userId;
	bool			isdel;
};

class Team
{
public:
	Team(Player* leader);
	~Team();
	void Update();
	uint32 getId() { return mInstanceId; }
	void sendPacketToAll(Packet& packet);
	Player* getLeader() { return mLeader; }
	bool addPlayer(Player* player);
	bool destoryPlayer(uint32 userId);
	bool removePlayer(uint32 userId);
	TeamEntity* getPlayer(uint32 userId);
	bool ChangeLeader(Player* newLeader);

	int32 CanAddTeam(Player* tar);

	void onEnterWorld(Player* player);
	void onLeaveWorld(Player* player);

	bool onEnterTeam(Player* player);
	bool onLeaveTeam(Player* player);
	bool onChangeLeader(const std::string& oldLeader, const std::string& newLeader);
	uint32 getPlayerCount();
	TeamEntity* getPlayerEntity(uint32 idx);
protected:
	TeamEntity*	ChooseLeader();
protected:
	static uint32 sId;
protected:
	uint32						mInstanceId;
	Player*						mLeader;
	std::vector<TeamEntity*>	mTeamEntityList;
};