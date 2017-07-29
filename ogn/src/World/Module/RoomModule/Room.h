#pragma once


enum RoomPlayerState
{
	RPS_None = 0,
	RPS_Observed,

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
	bool operator >> (RoomInfo& info);

	uint32 GetInsId() { return mId; }
	const std::string& GetName() { return mName; }
	uint32 GetMaxCount() { return mMaxCount; }

	RoomPlayer* DoEnter(Player* aPlr, bool isMaster = false);
	bool DoLeave(Player* aPlr);
	bool DoLeave(uint32 userId);
	RoomPlayer* FindPlayer(uint32 userId);
	void RemovePlayer(uint32 userId);
	RoomPlayer* AddPlayer(RoomPlayer* roomPlr);
	uint32 GetRoomPlayerCount() { return (uint32)mRoomPlayers.size(); }
	RoomPlayer* GetRoomPlayer(uint32 idx);
	bool IsFull();
	RoomPlayer* GetMaster() { return mMaster; }
	void SetMaster(RoomPlayer* master) { mMaster = master; }
	void SetPassword(const std::string& password) { mPassword = password; }
	const std::string& GetPassword() { return mPassword; }
protected:
	uint32									mId;
	RoomPlayer*								mMaster;
	std::string								mName;
	std::string								mPassword;
	uint32									mMaxCount;			// 房间最多人数
	std::vector<RoomPlayer*>				mRoomPlayers;
private:
};