#pragma once

class RoomModule : public Module
{
	DECLARE_CLASS(RoomModule)
public:
	RoomModule();
	virtual ~RoomModule();

public:
	Room* Create(Player* aPlr);
	void Remove(uint32 roomId);
	void Remove(Room* aRoom);
	bool EnterRoom(uint32 roomId, Player* aPlr);
	bool EnterRoom(Room* aRoom, Player* aPlr);
	bool LeaveRoom(uint32 roomId, uint32 userId);
	bool LeaveRoom(Room* aRoom, uint32 userId);
	bool ChangeMaster(Room* aRoom, uint32 oldUserId, uint32 newUserId);

	Room* FindPlayerRoom(uint32 userId);
	Room* AddPlayerRoom(uint32 userId, Room* rm);
	void RemovePlayerRoom(uint32 userId);

	Room* FindRoom(uint32 roomId);
	Room* AddRoom(Room* aRoom);
	void RemoveRoom(uint32 roomId);
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
protected:
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
protected:
	void ClearRoom();
protected:
	std::umap<uint32, Room*>	mMapRoo;
	std::map<uint32, Room*>		mMapRoom;
	std::map<uint32, Room*>		mMapPlayerRoom;
};
