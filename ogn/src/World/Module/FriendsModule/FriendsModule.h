#pragma once

class PlayerRecord
{
public:
	uint32 GetUserId();
	const std::string& GetName();
	bool GetOnline();
public:
	uint32 mUserId = 0;
	std::string mName;
	Player* mPlayer = NULL;
};


class FriendsModule : public Module
{
	DECLARE_CLASS(FriendsModule)
public:
	FriendsModule();
	virtual ~FriendsModule();
public:
	Friend* AddFriend(uint32 tarUserId, Player* tar);
	Friend* AddFriend(uint32 tarUserId, Friend* frdTar);
	Friend* FindFriend(uint32 tarUserId, uint32 frdUserId);

	Friends* AddFriends(Friends* frds);
	Friends* GetFriends(uint32 userId);

	PlayerRecord* AddPlrRecord(PlayerRecord* aPlrRecord);
	PlayerRecord* FindPlrRecord(uint32 userId);

	std::map<uint32, PlayerRecord*>& GetMapPlayer() { return mMapPlrRecords; }

	bool DelFriend(uint32 tarUserId, uint32 frdUserId);
	bool DelFriends(uint32 tarUserId);
	bool MutualBindFriend(Player* tar, Player* frd);
	bool MutualDebindFriend(uint32 tarUserId, uint32 frdUserId);

	void ClearFriends();
	void DelPlrRecord(uint32 userId);

	void DoFriendsList(Player* aPlr);

protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);

	virtual bool onLoad(Player* player, Dictionary& bytes);
	virtual bool onSave(Player* player, Dictionary& bytes);
protected:
	void ClearPlayerRecord(); 
protected:
	int32 onRedisAuth(Event& e);
	int32 onRedisAllPlr(RedisEvent& e);
	int32 onRedisFindPlr(RedisEvent& e);
public:
	std::map<uint32, Friends*>					mMapFriends;
	std::map<uint32, PlayerRecord*>				mMapPlrRecords;
};
