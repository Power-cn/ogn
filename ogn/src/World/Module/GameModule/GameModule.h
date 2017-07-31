#pragma once

class GameModule : public Module
{
	DECLARE_CLASS(GameModule)
public:
	GameModule();
	virtual ~GameModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
public:
	GameModle* FindGameModle(uint32 insId);
	GameModle* FindPlrGameModle(uint32 userId);
	GameModle* FindRoomGameModle(uint32 roomId);
	GameEntity* FindPlrGameEnt(uint32 userId);

	GameModle* AddGameModle(GameModle* aGameModle);
	GameModle* AddPlrGameModle(uint32 userId, GameModle* aGameModle);
	GameModle* AddRoomGameModle(uint32 roomId, GameModle* aGameModle);

	void DelGameModle(uint32 insId);
	void DelPlrGameModle(uint32 userId);
	void DelRoomGameModle(uint32 roomId);
public:
	bool DoStartGame(Room* aRoom);
	bool DoCloseGame(uint32 roomId);
protected:
	std::map<uint32, GameModle*>				mMapGameModle;
	std::map<uint32, GameModle*>				mMapRoomGameModle;
	std::map<uint32, GameModle*>				mMapPlrGameModle;
};
