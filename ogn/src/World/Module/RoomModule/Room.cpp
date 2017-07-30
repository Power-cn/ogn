#include "stdafx.h"

Room::Room()
{
	static uint32 sRoomId = 0;
	mId = ++sRoomId;
	mMaxCount = 8;
	mMaster = NULL;
}

Room::~Room()
{

}

void Room::sendPacketToAll(Packet& packet)
{
	for (RoomPlayer* aRoomPlayer : mRoomPlayers)
	{
		if (aRoomPlayer->mPlayer == NULL) continue;
		aRoomPlayer->mPlayer->sendPacket(packet);
	}
}

bool Room::operator >> (RoomInfo& info)
{
	info.name = GetName();
	info.roomId = GetInsId();
	info.maxCount = GetMaxCount();

	for (RoomPlayer* aRoomPlayer: mRoomPlayers){
		RoomPlayerInfo aPlrInfo;
		(*aRoomPlayer) >> aPlrInfo;
		info.roomPlayerInfos.push_back(aPlrInfo);
	}
	return true;
}

RoomPlayer* Room::DoEnter(Player* aPlr, bool isMaster /* = false */)
{
	if (IsFull())
	{
		return NULL;
	}

	if (FindPlayer(aPlr->getUserId()))
	{
		return NULL;
	}

	RoomPlayer* roomPlr = new RoomPlayer;
	roomPlr->mUserId = aPlr->getUserId();
	roomPlr->mName = aPlr->getName();
	roomPlr->mPlayer = aPlr;
	if (isMaster)
		SetMaster(roomPlr);

	return AddPlayer(roomPlr);
}

bool Room::DoLeave(Player* aPlr)
{
	RoomPlayer* roomPlr = FindPlayer(aPlr->getUserId());
	if (roomPlr == NULL) return false;

	RemovePlayer(aPlr->getUserId());
	return true;
}

bool Room::DoLeave(uint32 userId)
{
	RemovePlayer(userId);
	return true;
}

RoomPlayer* Room::FindPlayer(uint32 userId)
{
	for (auto itr = mRoomPlayers.begin();
		itr != mRoomPlayers.end();
		++itr)
	{
		if ((*itr)->mUserId == userId)
		{
			return (*itr);
		}
	}
	return NULL;
}

void Room::RemovePlayer(uint32 userId)
{
	for (auto itr = mRoomPlayers.begin();
		 itr != mRoomPlayers.end();
		 ++itr)
	{
		if ((*itr)->mUserId == userId)
		{
			delete *itr;
			mRoomPlayers.erase(itr);
			break;
		}
	}
}

RoomPlayer* Room::AddPlayer(RoomPlayer* roomPlr)
{
	mRoomPlayers.push_back(roomPlr);
	return roomPlr;
}

RoomPlayer* Room::GetRoomPlayer(uint32 idx)
{
	if (idx > GetRoomPlayerCount()) return NULL;
	return mRoomPlayers[idx];
}

bool Room::IsFull()
{
	if (GetRoomPlayerCount() >= mMaxCount)
		return true;
	return false;
}

bool Room::IsCanStart()
{
	for (RoomPlayer* aRoomPlayer:mRoomPlayers) {

		if (aRoomPlayer->GetState() == RPS_Ready) continue;
		if (aRoomPlayer->GetState() == RPS_None) {

			return false;
		}
	}
	return true;
}

bool Room::DoAllStart()
{
	for (RoomPlayer* aRoomPlayer : mRoomPlayers) {
		if (aRoomPlayer->GetState() == RPS_Ready) {
			aRoomPlayer->SetState(RPS_Game);
		}
	}
	return true;
}

bool RoomPlayer::operator<<(RoomPlayerInfo& info)
{
	return true;
}

bool RoomPlayer::operator >> (RoomPlayerInfo& info)
{
	info.insId = mInsId;
	info.userId = mUserId;
	info.name = mName;
	info.state = mState;
	return true;
}
