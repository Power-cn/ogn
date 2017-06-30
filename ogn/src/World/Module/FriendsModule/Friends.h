#pragma once

class Friend
{
public:
	bool operator >> (Json::Value& json);
	bool operator << (Json::Value& json);
public:
	std::string		mName;
	uint32			mUserId;
	uint32			mCharId;
	uint8			mState;
};

class Friends
{
public:
	Friends(uint32 tarUserId);
	~Friends();
	void SetUserId(uint32 userId) { mUserId = userId; }
	uint32 GetUserId() { return mUserId; }
	Friend* AddFriend(Player* tar);
	Friend* AddFriend(Friend* frd);
	bool DelFriend(uint32 userId);
	Friend* FindFriend(uint32 userId);
	void ClearFriends();
	std::list<Friend*> GetFriends() { return mFriends; }
protected:
	uint32						mUserId;
	std::list<Friend*>			mFriends;
};