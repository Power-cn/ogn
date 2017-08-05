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
	std::vector<uint8>		cards;
	std::string ToString();
	uint32 GetCardCount() { return (uint32)cards.size(); }
	uint32 GetCard(uint32 idx);
	uint32 getGold() { return mGold; }

	//luabind::object GetCards();
	ThreeCardType GetThreeCardType();

	void addGold(uint32 gold) { mGold += gold; }
protected:
	uint32					mGold;
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
	void DoCutCard();			// 切
	void SetBanker(uint32 userId) { mBankerUserId = userId; }
	void SetCurSpeakPlr(uint32 userId) { mCurSpeakUserId = userId; }
	void SetSpeakTime(uint32 speakTime) { mSpeakTime = speakTime; }
	void SetGameLv(uint8 gamelv) { mGameLv = gamelv; }
	void SetCurMaxGold(uint32 maxGold) { mCurMaxGold = maxGold; }
	void SetRound(uint32 round) { mRound = round; }
	void SetCurGold(uint32 gold) { mCurGold = gold; }
	void AddCurGold(uint32 gold) { mCurGold += gold; }
	uint8 DoDealPoker();		// 发

	uint32 GetNextSpeakPlr();
	uint32 GetBanker() { return mBankerUserId; }
	uint32 GetCurSpeak() { return mCurSpeakUserId; }
	uint32 GetSpeakTime() { return mSpeakTime; }
	uint32 GetCurGold() { return mCurGold; }
	uint32 GetCurMaxGold() { return mCurMaxGold; }
	uint32 GetRound() { return mRound; }
	uint8 GetGameLv() { return mGameLv; }

	uint32 CheckUserGold(uint32 gold);
	virtual std::string ToString();

	bool DoChipin(uint32 gold, GameEntity* aGameEnt);
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
	uint32							mCurGold;
	uint8							mGameLv;
	uint32							mCurMaxGold;			// 当前最大
	uint32							mRound;					// 回合数
};