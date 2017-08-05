#include "stdafx.h"

GameHandler::GameHandler()
{
	RegWorldEvent(ID_NetGameChipInReq, &GameHandler::onNetGameChipInReq, this);
}

int32 GameHandler::onNetGameChipInReq(Player* aPlr, NetGameChipInReq* req)
{
	sGame.DoChipInReq(aPlr, req->chiptype, req->gold);
	return 0;
}

