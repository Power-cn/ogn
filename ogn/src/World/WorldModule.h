#pragma once

class WorldModule : public Module
{
	DECLARE_CLASS(WorldModule)
public:
	WorldModule();
	~WorldModule();
protected:
	virtual bool Initialize() { return true; }
	virtual bool Update(float time, float delay);
	virtual bool Destroy() { return true; }
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	void sendPacketToAll(Packet& packet);
	void sendPacketToTarget(EnumChannel ec, Packet& packet, Player* self, const std::string& tar = "");
	void sendPacketToMsg(EnumChannel ec, const std::string& msg, Player* self);
public:
	Entity* getEntityByName(const std::string& name);
	Entity* getEntity(Guid guid);
	Entity* addEntity(Entity* ent);
	void removeEntity(Entity* ent);
	void removeEntity(Guid guid);
	Player* getPlayerByName(const std::string& name);
	Player* getPlayerByGuid(Guid guid);
	Player* getPlayer(uint32 accountId);
	Player* addPlayer(Player* plr);
	void removePlayer(uint32 accountId);
	void removePlayer(Player* plr);
	Player* addPlayerToUserId(Player* plr);
	void removePlayerToUserId(uint32 userId);
	Player* getPlayerToUserId(uint32 userId);

	Npc* addNpc(Npc* npc);
	Npc* getNpc(uint32 npcId);
	Npc* getNpcByName(const std::string& name);
	Npc* getNpcByGuid(Guid guid);
	void removeNpc(uint32 npcId);
	void removeNpc(Npc* npc);
	int32 getPlayerCount() { return mMapPlayer.size(); }
	std::map<uint32, Player*>& getMapPlayer() { return mMapPlayer; }
protected:
	std::map<Guid, Entity*>						mMapEntity;
	std::map<std::string, Entity*>					mMapNameEntity;
	std::map<uint32, Player*>						mMapPlayer;
	std::map<uint32, Player*>						mMapUserIdPlayer;
	std::map<uint32, Npc*>							mMapNpc;

};