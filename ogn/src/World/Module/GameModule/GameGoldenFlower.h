#pragma once

#define MAX_UNIT_POKER_COUNT 3
#define MAX_POKER_COUNT 54

class GameEntity
{
public:
	GameEntity();
	~GameEntity();
	bool operator >> (GameEntityInfo& info);
public:
	uint32					userId = 0;
	uint8					maxCount = MAX_UNIT_POKER_COUNT;
	std::vector<uint8>		poker;
protected:
};

/*
* 
*/
class GameGoldenFlower : public GameModle
{
public:
	GameGoldenFlower();
	~GameGoldenFlower();
	bool operator >> (GameGoldenFlowerInfo& info);

	GameEntity* AddGameEnt(GameEntity* aGameEnt);
	GameEntity* GetGameEnt(uint32 idx);
	GameEntity* FindGameEnt(uint32 userId);
	void DelGameEnt(uint32 userId);
	void DoShuffle();
	uint8 DoDealPoker();
	void DoCutPoker();
	void SetRoomId(uint32 roomId) { mRoomId = roomId; }
	uint32 GetRoomId() { return mRoomId; }

	uint32 GetGameEntCount() { return (uint32)mLstGameEntity.size(); }
protected:
	uint32							mRoomId;
	uint8							mPoker[MAX_POKER_COUNT];
	std::queue<uint8>				mCurPoker;				// 没有使用
	std::queue<uint8>				mOpenPoker;				// 已经使用
	std::vector<GameEntity*>		mLstGameEntity;
};