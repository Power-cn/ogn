#pragma once

class GameEntity;
class GameModle
{
	friend class GameModule;
public:
	GameModle();
	~GameModle();
public:
	GameEntity* AddGameEnt(GameEntity* aGameEnt);
	GameEntity* GetGameEnt(uint32 idx);
	GameEntity* FindGameEnt(uint32 userId);
	void DelGameEnt(uint32 userId);
	uint32 GetInsId() { return mInsId; }
	uint32 GetGameEntCount() { return (uint32)mLstGameEntity.size(); }

	cstring& GetScript() { return mScript; }
	virtual std::string ToString();
protected:
	virtual bool OnStart() { return true; }
	virtual bool OnClose() { return true; }
	virtual bool OnEnter(GameEntity* aGameEnt) { return true; }
	virtual bool OnLeave(GameEntity* aGameEnt) { return true; }
protected:
	static uint32 sId;
	uint32 mInsId;
	std::string mScript;
	std::vector<GameEntity*>		mLstGameEntity;
private:
};