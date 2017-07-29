#include "stdafx.h"

RoomHandler::RoomHandler()
{
	RegWorldEvent(ID_NetCreateRoomReq, &RoomHandler::onNetChangeRoomMasterReq, this);
	RegWorldEvent(ID_NetEnterRoomReq, &RoomHandler::onNetEnterRoomReq, this);
	RegWorldEvent(ID_NetLeaveRoomReq, &RoomHandler::onNetLeaveRoomReq, this);
	RegWorldEvent(ID_NetChangeRoomMasterReq, &RoomHandler::onNetChangeRoomMasterReq, this);
}

int32 RoomHandler::onNetCreateRoomReq(Player* aPlr, NetCreateRoomReq* req)
{
	Room* aRoom = sRoom.FindPlayerRoom(aPlr->getUserId());
	if (aRoom)
	{
		//
		return 0;
	}

	aRoom = sRoom.Create(aPlr);
	sRoom.EnterRoom(aRoom, aPlr);
	return 0;
}

int32 RoomHandler::onNetEnterRoomReq(Player* aPlr, NetEnterRoomReq* req)
{
	Room* aRoom = sRoom.FindRoom(req->roomId);
	if (aRoom == NULL)
	{
		 // 房间不存在
		return 0;
	}

	aRoom = sRoom.FindPlayerRoom(aPlr->getUserId());
	if (aRoom)
	{
		// 已经有房间
		return 0;
	}

	sRoom.EnterRoom(aRoom, aPlr);
	return 0;
}

int32 RoomHandler::onNetLeaveRoomReq(Player* aPlr, NetLeaveRoomReq* req)
{
	Room* aRoom = sRoom.FindRoom(req->roomId);
	if (aRoom == NULL)
	{
		// 房间不存在
		return 0;
	}

	sRoom.LeaveRoom(aRoom, aPlr->getUserId());
	return 0;
}

int32 RoomHandler::onNetChangeRoomMasterReq(Player* aPlr, NetChangeRoomMasterReq* req)
{
	Room* aRoom = sRoom.FindRoom(req->roomId);
	if (aRoom == NULL)
	{
		// 房间不存在
		return 0;
	}
	sRoom.ChangeMaster(aRoom, aPlr->getUserId(), req->userId);
	return 0;
}
