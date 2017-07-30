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

int32 PlayerHandler::onNetChatMsgNotify(Player* player, NetChatMsgNotify* nfy)
{
	nfy->from = player->getName();
	uint32 tarUserId = 0;

	if (nfy->to != "")
	{
		Player* tar = sWorld.getPlayerByName(nfy->to);
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
