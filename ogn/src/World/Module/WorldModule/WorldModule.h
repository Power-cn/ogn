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
	void sendPacketToTarget(EnumChannel ec, Packet& packet, Player* self, uint32 tarUserId = 0);
	void sendPacketToMsg(EnumChannel ec, const std::string& msg, Player* self);
public:
	void removeEntity(Entity* ent);
	void removeEntity(Guid guid);
	void removePlayer(uint32 accId);
	void removePlayer(Player* plr);
	void removePlayerToUserId(uint32 userId);
	void removeNpc(uint32 npcId);
	void removeNpc(Npc* npc);

	bool ChangeName(Entity* ent, cstring& sname);
	Entity* addEntity(Entity* ent);

	Entity* FindEntByName(cstring& name);
	Entity* FindEntByGuid(Guid guid);
	
	Player* addPlayer(Player* plr);
	Player* addPlayerToUserId(Player* plr);

	Player* FindPlrByName(cstring& name);
	Player* FindPlrByGuid(Guid guid);
	Player* FindPlrByAccId(uint32 accId);
	Player* FindPlrByUserId(uint32 userId);

	Npc* addNpc(Npc* npc);
	
	Npc* FindNpcByCfgId(uint32 cfgId);
	Npc* FindNpcByName(cstring& name);
	Npc* FindNpcByGuid(Guid guid);

	void DestroyEnt(Guid guid);
	void DestroyEnt(Entity* ent);

	uint32 GetPlrCount() { return (uint32)mMapPlayer.size(); }

	std::map<uint32, Player*>& getMapPlayer() { return mMapPlayer; }
protected:
	std::map<Guid, Entity*>						mMapEntity;
	std::map<std::string, Entity*>					mMapNameEntity;
	std::map<uint32, Player*>						mMapPlayer;
	std::map<uint32, Player*>						mMapUserIdPlayer;
	std::map<uint32, Npc*>							mMapNpc;

};