#pragma once

class GameEntity;
class GameModle
{
	friend class GameModule;
public:
	GameModle();
	virtual ~GameModle();
public:
	GameEntity* AddGameEnt(GameEntity* aGameEnt);
	GameEntity* GetGameEnt(uint32 idx);
	GameEntity* FindGameEnt(uint32 userId);
	void DelGameEnt(uint32 userId);
	void SetRoomId(uint32 roomId) { mRoomId = roomId; }

	int32 GetPlrInx(uint32 userId);

	uint32 GetInsId() { return mInsId; }
	uint32 GetGameEntCount() { return (uint32)mLstGameEntity.size(); }
	uint32 GetRoomId() { return mRoomId; }
	uint32 GetPlrCard(uint32 userId, uint32 idx);

	virtual std::string ToString();
protected:
	virtual bool OnStart() { return true; }
	virtual bool OnClose() { return true; }
	virtual bool OnEnter(GameEntity* aGameEnt) { return true; }
	virtual bool OnLeave(GameEntity* aGameEnt) { return true; }
protected:
	static uint32 sId;
	uint32							mInsId;
	uint32							mRoomId;
	std::vector<GameEntity*>		mLstGameEntity;
private:
};