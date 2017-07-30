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
	GameModle* AddGameModle(GameModle* aGameModle);
	GameModle* FindGameModule(uint32 insId);
	void DelGameModule(uint32 insId);

	GameModle* AddPlrGameModle(uint32 userId, GameModle* aGameModle);
	GameModle* FindPlrGameModule(uint32 userId);
	void DelPlrGameModule(uint32 insId);

public:
	bool DoStartGame(Room* aRoom);
protected:
	std::map<uint32, GameModle*>				mMapGameModle;
	std::map<uint32, GameModle*>				mMapPlrGameModle;
};
