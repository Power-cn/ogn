#pragma once

class Entity;
class Player;
class Packet;
class Team;

class TeamModule : public Module
{
	DECLARE_CLASS(TeamModule)
public:
	TeamModule();
	~TeamModule();
	virtual bool Initialize() { return true; }
	virtual bool Update(float time, float delay);
	virtual bool Destroy() { return true; }
protected:
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	Team* createTeam(Player* leader);
	void destroyTeam(Team* team);
	Team* addTeam(Team* team);
	void removeTeam(uint32 teamId);
	Team* getTeam(uint32 teamId);
	Team* getPlayerTeam(uint32 userid);
	bool addPlayerTeam(uint32 userid, Team* team);
	void removePlayerTeam(uint32 userid);

	bool doPlayerAddTeam(Player* player, Team* team, bool isLeader = false);
	bool doPlayerRemoveTeam(uint32 userId, Team* team);
public:
	void DoCreateTeamReq(Player* aPlr);
	void DoOrganizeTeamReq(Player* aPlr, cstring& tarName);
	void DoAgreeTeamReq(Player* aPlr, cstring& tarName, uint8 isJoin);
public:
	void sendPacketToAll(Packet& packet);
	void sendPacketToTeam(Packet& packet, Player* player);
protected:
	std::map<uint32, Team*>								mMapTeam;
	std::map<uint32, Team*>								mMapPlayerTeam;
	std::list<uint32>									mDelTeamList;
};