#include "stdafx.h"

IMPLEMENT_CLASS(RoomModule, Module)
RoomModule::RoomModule()
{

}

RoomModule::~RoomModule()
{

}

void RoomModule::sendPacketToTeam(Packet& packet, Player* player)
{
	Room* aRoom = FindPlayerRoom(player->getUserId());
	if (aRoom == NULL) return;
	aRoom->sendPacketToAll(packet);
}

Room* RoomModule::Create(Player* aPlr)
{
	if (FindPlayerRoom(aPlr->getUserId())) return NULL;
	Room* aRoom = new Room();
	AddRoom(aRoom);
	OnCreate(aRoom, aPlr->getUserId());
	return aRoom;
}

void RoomModule::Remove(uint32 roomId)
{
	Room* aRoom = FindRoom(roomId);
	if (aRoom == NULL) return;
	Remove(aRoom);
}

void RoomModule::Remove(Room* aRoom)
{
	for (uint32 i = 0; i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* aRoomPlayer = aRoom->GetRoomPlayer(i);
		if (aRoomPlayer == NULL) continue;

		Player* aPlr = aRoomPlayer->mPlayer;
		if (aPlr == NULL) continue;

		NetLeaveRoomRes res;
		res.roomId = aRoom->GetInsId();
		res.userId = aPlr->getUserId();
		aRoom->sendPacketToAll(res);
		aPlr->sendPacket(res);
	}

	RemoveRoom(aRoom->GetInsId());
}

bool RoomModule::EnterRoom(uint32 roomId, Player* aPlr)
{
	Room* aRoom = FindRoom(roomId);
	if (aRoom == NULL) return false;
	return EnterRoom(aRoom, aPlr);
}

bool RoomModule::EnterRoom(Room* aRoom, Player* aPlr)
{
	if (FindPlayerRoom(aPlr->getUserId()))
		return false;
	if (aRoom->FindPlayer(aPlr->getUserId()))
		return false;
	AddPlayerRoom(aPlr->getUserId(), aRoom);
	RoomPlayer* curPlayer = aRoom->DoEnter(aPlr, true);
	if (curPlayer == NULL)
		return false;

	NetEnterRoomRes res;
	(*aRoom) >> res.roomInfo;
	aPlr->sendPacket(res);

	NetEnterRoomNotify nfy;
	*curPlayer >> nfy.roomPlayerInfo;
	for (uint32 i = 0; i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* roomPlayer = aRoom->GetRoomPlayer(i);
		if (roomPlayer == NULL) continue;

		if (roomPlayer == curPlayer) continue;

		Player* aSndPlr = sWorld.getPlayerToUserId(roomPlayer->mUserId);
		if (aSndPlr == NULL) continue;
		aSndPlr->sendPacket(nfy);
	}

	OnEnter(aRoom, aPlr->getUserId());
	return true;
}

bool RoomModule::LeaveRoom(uint32 roomId, uint32 userId)
{
	Room* aRoom = FindPlayerRoom(userId);
	if (aRoom == NULL) return false;

	if (aRoom->GetInsId() != roomId) return false;
	LeaveRoom(aRoom, userId);

	return true;
}

bool RoomModule::LeaveRoom(Room* aRoom, uint32 userId)
{
	if (aRoom->FindPlayer(userId) == NULL)
	{
		// 不在房间
		return false;
	}

	NetLeaveRoomRes res;
	res.roomId = aRoom->GetInsId();
	res.userId = userId;
	aRoom->sendPacketToAll(res);

	OnLeave(aRoom, userId);

	aRoom->DoLeave(userId);
	RemovePlayerRoom(userId);
	if (aRoom->GetRoomPlayerCount() <= 0)
		RemoveRoom(aRoom->GetInsId());
	return true;
}

bool RoomModule::ChangeMaster(Room* aRoom, uint32 oldUserId, uint32 newUserId)
{
	RoomPlayer* oldRoomPlayer = aRoom->GetRoomPlayer(oldUserId);
	RoomPlayer* newRoomPlayer = aRoom->GetRoomPlayer(newUserId);
	if (oldRoomPlayer == NULL)
	{
		return false;
	}

	if (newRoomPlayer == NULL)
	{
		return false;
	}

	aRoom->SetMaster(newRoomPlayer);
	NetChangeRoomMasterRes res;
	res.roomId = aRoom->GetInsId();
	res.masterUserId = newRoomPlayer->mUserId;
	aRoom->sendPacketToAll(res);
	OnChangeMaster(aRoom, oldUserId, newUserId);
	return true;
}

Room* RoomModule::FindPlayerRoom(uint32 userId)
{
	auto itr = mMapPlayerRoom.find(userId);
	if (itr != mMapPlayerRoom.end())
		return itr->second;
	return NULL;
}

Room* RoomModule::AddPlayerRoom(uint32 userId, Room* aRoom)
{
	if (FindPlayerRoom(userId)) return NULL;
	mMapPlayerRoom[userId] = aRoom;
	return aRoom;
}

void RoomModule::RemovePlayerRoom(uint32 userId)
{
	auto itr = mMapPlayerRoom.find(userId);
	if (itr != mMapPlayerRoom.end())
		mMapPlayerRoom.erase(itr);
}

Room* RoomModule::FindRoom(uint32 roomId)
{
	auto itr = mMapRoom.find(roomId);
	if (itr != mMapRoom.end())
		return itr->second;
	return NULL;
}

Room* RoomModule::AddRoom(Room* aRoom)
{
	if (FindRoom(aRoom->GetInsId())) return NULL;
	mMapRoom[aRoom->GetInsId()] = aRoom;
	return aRoom;
}

void RoomModule::RemoveRoom(uint32 roomId)
{
	auto itr = mMapRoom.find(roomId);
	if (itr != mMapRoom.end()) {
		delete itr->second;
		mMapRoom.erase(itr);
	}
}

void RoomModule::DoCreateRoom(Player* aPlr)
{
	Room* aRoom = sRoom.FindPlayerRoom(aPlr->getUserId());
	if (aRoom)
	{
		//
		return;
	}

	aRoom = sRoom.Create(aPlr);
	sRoom.EnterRoom(aRoom, aPlr);
}

void RoomModule::DoEnterRoom(Player* aPlr, uint32 roomId)
{
	Room* aRoom = FindRoom(roomId);
	if (aRoom == NULL)
	{
		// 房间不存在
		return;
	}

	aRoom = sRoom.FindPlayerRoom(aPlr->getUserId());
	if (aRoom)
	{
		// 已经有房间
		return;
	}

	sRoom.EnterRoom(aRoom, aPlr);
}

void RoomModule::DoLeaveRoom(Player* aPlr, uint32 roomId)
{
	Room* aRoom = sRoom.FindRoom(roomId);
	if (aRoom == NULL)
	{
		// 房间不存在
		return;
	}

	sRoom.LeaveRoom(aRoom, aPlr->getUserId());
}

void RoomModule::DoChangeRoomMaster(Player* aPlr, uint32 roomId, uint32 newUserId)
{
	Room* aRoom = sRoom.FindRoom(roomId);
	if (aRoom == NULL)
	{
		// 房间不存在
		return ;
	}
	sRoom.ChangeMaster(aRoom, aPlr->getUserId(), newUserId);
}

void RoomModule::DoRoomList(Player* aPlr, uint32 start, uint32 count)
{
	NetRoomListRes res;

	std::udmap<uint32, Room*>& mapRooms = sRoom.GetMapRoom();
	uint32 idx = 0;
	for (auto& itr : mapRooms)
	{
		if (idx >= start)
		{
			Room* aRoom = itr.second;
			if (aRoom == NULL) {
				idx++;
				continue;
			}

			RoomInfo info;
			*aRoom >> info;
			res.roomInfoInfos.push_back(info);
			if ((uint32)res.roomInfoInfos.size() >= count)
				break;
		}
		idx++;
	}

	aPlr->sendPacket(res);
}

void RoomModule::OnCreate(Room* aRoom, uint32 userId)
{
	LuaEngine::executeScript("room", "OnCreate", aRoom->GetInsId(), userId);
}

void RoomModule::OnEnter(Room* aRoom, uint32 userId)
{
	LuaEngine::executeScript("room", "OnEnter", aRoom->GetInsId(), userId);
}

void RoomModule::OnLeave(Room* aRoom, uint32 userId)
{
	LuaEngine::executeScript("room", "OnLeave", aRoom->GetInsId(), userId);
}

void RoomModule::OnChangeMaster(Room* aRoom, uint32 oldUserId, uint32 newUserId)
{
	LuaEngine::executeScript("room", "OnChangeMaster", aRoom->GetInsId(), oldUserId, newUserId);
}

bool RoomModule::Initialize()
{
	return true;
}

bool RoomModule::Update(float time, float delay)
{
	return true;
}

bool RoomModule::Destroy()
{
	ClearRoom();
	return true;
}

bool RoomModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool RoomModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

void RoomModule::ClearRoom()
{
	for (auto& itr : mMapRoom)
		delete itr.second;
	mMapRoom.clear();
}

