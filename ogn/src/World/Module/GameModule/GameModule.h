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
	GameModle* FindGameModule(uint32 insId);
	GameModle* FindPlrGameModule(uint32 userId);
	GameEntity* FindPlrGameEnt(uint32 userId);

	GameModle* AddGameModle(GameModle* aGameModle);
	GameModle* AddPlrGameModle(uint32 userId, GameModle* aGameModle);

	void DelGameModule(uint32 insId);
	void DelPlrGameModule(uint32 userId);
public:
	bool DoStartGame(Room* aRoom);
	bool DoStopGame();
protected:
	std::map<uint32, GameModle*>				mMapGameModle;
	std::map<uint32, GameModle*>				mMapPlrGameModle;
};
