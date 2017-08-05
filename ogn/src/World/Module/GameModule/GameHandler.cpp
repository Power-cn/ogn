#include "stdafx.h"

GameHandler::GameHandler()
{
	RegWorldEvent(ID_NetRoomChipInReq, &GameHandler::onNetRoomChipInReq, this);
}

int32 GameHandler::onNetRoomChipInReq(Player* aPlr, NetRoomChipInReq* req)
{
	sGame.DoChipInReq(aPlr, req->chiptype, req->gold);
	return 0;
}

