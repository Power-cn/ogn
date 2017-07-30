#pragma once


enum RoomPlayerState
{
	RPS_None = 0,
	RPS_Ready,				// 准备
	RPS_Game,				// 游戏中
	RPS_Observed,			// 观战中
};

class RoomPlayer
{
public:
	Player*			mPlayer = NULL;
	uint32			mUserId = 0;
	Guid			mInsId = 0;
	std::string		mName;
	uint8			mState = RPS_None;
public:
	bool operator << (RoomPlayerInfo& info);
	bool operator >> (RoomPlayerInfo& info);
};

class Room
{
public:
	Room();
	~Room();
	void sendPacketToAll(Packet& packet);
	void RemovePlayer(uint32 userId);
	void SetMaster(RoomPlayer* master) { mMaster = master; }
	void SetPassword(const std::string& password) { mPassword = password; }
	void SetName(const std::string& name) { mName = name; }

	bool operator >> (RoomInfo& info);
	bool DoLeave(Player* aPlr);
	bool DoLeave(uint32 userId);
	bool IsFull();

	uint32 GetInsId() { return mId; }
	uint32 GetMaxCount() { return mMaxCount; }
	uint32 GetRoomPlayerCount() { return (uint32)mRoomPlayers.size(); }

	RoomPlayer* DoEnter(Player* aPlr, bool isMaster = false);
	RoomPlayer* FindPlayer(uint32 userId);
	RoomPlayer* AddPlayer(RoomPlayer* roomPlr);
	RoomPlayer* GetRoomPlayer(uint32 idx);
	RoomPlayer* GetMaster() { return mMaster; }
	
	const std::string& GetPassword() { return mPassword; }
	const std::string& GetName() { return mName; }
protected:
	uint32									mId;
	RoomPlayer*								mMaster;
	std::string								mName;
	std::string								mPassword;
	uint32									mMaxCount;			// 房间最多人数
	std::vector<RoomPlayer*>				mRoomPlayers;
private:
};