#pragma once

class FriendsModule : public Module
{
	DECLARE_CLASS(FriendsModule)
public:
	FriendsModule();
	virtual ~FriendsModule();
public:
	Friends* AddFriends(Friends* frds);
	Friend* AddFriend(uint32 tarUserId, Player* tar);
	Friend* AddFriend(uint32 tarUserId, Friend* frdTar);
	bool DelFriend(uint32 tarUserId, uint32 frdUserId);
	Friends* GetFriends(uint32 userId);
	bool MutualBindFriend(Player* tar, Player* frd);
	bool MutualDebindFriend(uint32 tarUserId, uint32 frdUserId);
	void ClearFriends();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);

	virtual bool onLoad(Player* player, Dictionary& bytes);
	virtual bool onSave(Player* player, Dictionary& bytes);

public:
	std::map<uint32, Friends*>				mMapFriends;
};
