#include "stdafx.h"

PlayerHandler::PlayerHandler()
{
	RegWorldEvent(ID_NetFirst, &PlayerHandler::onNetFirst, this);
	RegWorldEvent(ID_NetChatMsgNotify, &PlayerHandler::onNetChatMsgNotify, this);
	RegWorldEvent(ID_NetGmMsg, &PlayerHandler::onNetGmMsg, this);
	RegWorldEvent(ID_NetEntityMoveToNotify, &PlayerHandler::onNetEntityMoveToNotify, this);
	RegWorldEvent(ID_NetEntityMoveNotify, &PlayerHandler::onNetEntityMoveNotify, this);
}

int32 PlayerHandler::onNetFirst(Player* player, NetFirst* nfy)
{
	return 0;
}

int32 PlayerHandler::onNetChangeNameReq(Player* player, NetChangeNameReq* req)
{
	char szBuffer[256] = { 0 };
	sprintf_s(szBuffer, 256, "hget %s %s", sNameToUserId, req->newName.c_str());
	std::vector<std::string> parstr;
	parstr.push_back(Shared::int32tostr(player->getUserId()));
	parstr.push_back(req->newName);
	sRedisProxy.sendCmd(szBuffer, (EventCallback)&PlayerHandler::OnRedisFindName, this);
	return 0;
}

int32 PlayerHandler::onNetChatMsgNotify(Player* player, NetChatMsgNotify* nfy)
{
	nfy->from = player->getName();
	uint32 tarUserId = 0;

	if (nfy->to != "")
	{
		Player* tar = sWorld.FindPlrByName(nfy->to);
		if (tar)
			tarUserId = tar->getUserId();
	}
	sWorld.sendPacketToTarget((EnumChannel)nfy->channelType, *nfy, player, tarUserId);
	return 0;
}

int32 PlayerHandler::onNetGmMsg(Player* aPlr, NetGmMsg* msg)
{
	std::string p1 = msg->gmParams.size() >= 1 ? msg->gmParams[0] : "";
	std::string p2 = msg->gmParams.size() >= 2 ? msg->gmParams[1] : "";
	std::string p3 = msg->gmParams.size() >= 3 ? msg->gmParams[2] : "";
	LuaEngine::executeScript(aPlr, "gm", "GmCmd", msg->name, p1, p2, p3);

	LOG_DEBUG(LogSystem::csl_color_green, "[%s] GmCmd:%s %s %s %s", aPlr->getName(),msg->name.c_str(), p1.c_str(), p2.c_str(), p3.c_str());
	return 0;
}

int32 PlayerHandler::onNetEntityMoveNotify(Player* player, NetEntityMoveNotify* nfy)
{
	if (player->getCellX() == nfy->x && player->getCellY() == nfy->y)
		return 0;

	nfy->guid = player->getInstanceId();
	player->setCellX(nfy->x);
	player->setCellY(nfy->y);
	player->setCellTarPos(player->getCellPos());
	player->getMap()->checkMapEntityView(player);
	player->sendPacketToView(*nfy);
	return 0;
}

int32 PlayerHandler::onNetEntityMoveToNotify(Player* player, NetEntityMoveToNotify* nfy)
{
	if (player->getCellTarPos().x == nfy->x && player->getCellTarPos().y == nfy->y)
		return 0;
	nfy->guid = player->getInstanceId();
	player->setCellTarX(nfy->x);
	player->setCellTarY(nfy->y);
	player->sendPacketToView(*nfy);
	player->MoveTo();
	return 0;
}

int32 PlayerHandler::OnRedisFindName(RedisEvent& e)
{
	std::string sUserId = e.parstr.size() >= 1 ? e.parstr[0] : "";
	std::string sName = e.parstr.size() >= 2 ? e.parstr[1] : "";
	uint32 tarUserId = Shared::strtoint32(sUserId);
	std::string newName = sName;
	Player* aPlr = sWorld.FindPlrByUserId(tarUserId);
	if (aPlr == NULL) return;

	NetChangeNameRes res;
	Entity* ent = sWorld.FindEntByName(newName);
	if (ent)
	{
		res.result = NResultFail;
		aPlr->sendPacket(res);
		return 0;
	}

	sWorld.ChangeName(aPlr, newName);
	char szBuffer[256] = { 0 };
	sprintf_s(szBuffer, 256, "hmset %s %s %d", sNameToUserId, aPlr->getName(), aPlr->getUserId());
	sRedisProxy.sendCmd(szBuffer, NULL, NULL);

	res.result = NResultSuccess;
	res.newName = newName;
	aPlr->sendPacket(res);

	return 0;
}
