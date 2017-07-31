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
	std::string ToString();
	uint32 GetPokerCount() { return (uint32)poker.size(); }
	uint32 GetPoker(uint32 idx);
protected:
};

/*
* 
*/
class GameGoldenFlower : public GameModle
{
	friend class GameModule;
public:
	GameGoldenFlower();
	~GameGoldenFlower();
	bool operator >> (GameGoldenFlowerInfo& info);

	void DoShuffle();			// 洗
	void DoCutPoker();			// 切
	void SetBanker(uint32 userId) { mBankerUserId = userId; }
	void SetCurSpeakPlr(uint32 userId) { mCurSpeakUserId = userId; }
	void SetSpeakTime(uint32 speakTime) { mSpeakTime = speakTime; }

	uint8 DoDealPoker();		// 发

	uint32 GetNextSpeakPlr();
	uint32 GetBanker() { return mBankerUserId; }
	uint32 GetCurSpeak() { return mCurSpeakUserId; }
	uint32 GetSpeakTime() { return mSpeakTime; }

	virtual std::string ToString();
protected:
	virtual bool OnStart();
	virtual bool OnClose();
	virtual bool OnEnter(GameEntity* aGameEnt);
	virtual bool OnLeave(GameEntity* aGameEnt);
protected:
	uint8							mPoker[MAX_POKER_COUNT];
	std::queue<uint8>				mCurPoker;				// 没有使用
	std::queue<uint8>				mOpenPoker;				// 已经使用
	uint32							mBankerUserId;			// 庄家
	uint32							mCurSpeakUserId;		// 当前说话的人
	uint32							mSpeakTime;				// 说话剩余时间秒
};