#include "stdafx.h"

PlayerHandler::PlayerHandler()
{
	if (GetWorldServer() == NULL)
	{
		LOG_ERROR("GetWorldServer() == NULL");
		return;
	}

	INSTANCE(WarHandler);
	
	RegisterEventProcess(GetWorldServer(), ID_NetChatMsgNotify, &PlayerHandler::onNetChatMsgNotify, this);
	RegisterEventProcess(GetWorldServer(), ID_NetGmMsg, &PlayerHandler::onNetGmMsg, this);
	RegisterEventProcess(GetWorldServer(), ID_NetEntityMoveToNotify, &PlayerHandler::onNetEntityMoveToNotify, this);
	RegisterEventProcess(GetWorldServer(), ID_NetEntityMoveNotify, &PlayerHandler::onNetEntityMoveNotify, this);
	RegisterEventProcess(GetWorldServer(), ID_NetOrganizeTeamReq, &PlayerHandler::onNetOrganizeTeamReq, this);
	RegisterEventProcess(GetWorldServer(), ID_NetAgreeTeamReq, &PlayerHandler::onNetAgreeTeamReq, this);
	RegisterEventProcess(GetWorldServer(), ID_NetTeamListReq, &PlayerHandler::onNetTeamListReq, this);
}

int32 PlayerHandler::onNetChatMsgNotify(Player* player, NetChatMsgNotify* nfy)
{
	nfy->from = player->getName();
	sWorld.sendPacketToTarget((EnumChannel)nfy->channelType, *nfy, player, nfy->to);
	return 0;
}

int32 PlayerHandler::onNetGmMsg(Player* player, NetGmMsg* msg)
{
	std::string p1 = msg->gmParams.size() >= 1 ? msg->gmParams[0] : "";
	std::string p2 = msg->gmParams.size() >= 2 ? msg->gmParams[1] : "";
	std::string p3 = msg->gmParams.size() >= 3 ? msg->gmParams[2] : "";
	LuaEngine::executeScript(player, "gm", "GmCmd", msg->name, p1, p2, p3);
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

int32 PlayerHandler::onNetOrganizeTeamReq(Player* player, NetOrganizeTeamReq* req)
{
	Player* target = GetModule(WorldModule)->getPlayerByName(req->name);
	if (!target)
		return 0;

	Team* tm0 = GetModule(TeamModule)->getPlayerTeam(player->getUserId());
	Team* tm1 = GetModule(TeamModule)->getPlayerTeam(target->getUserId());
	if (tm0 && tm1)
	{
		// 已经有队伍 ;
		return 0;
	}

	if (tm0 && !tm1)
	{
		Player* leader = tm0->getLeader();
		if (leader == NULL)
			return 0;

		NetOrganizeTeamRes res;
		res.name = leader->getName();
		res.isJoin = 0;
		target->sendPacket(res);
		return 0;
	}

	if (!tm0 && tm1)
	{
		Player* leader = tm1->getLeader();
		if (leader == NULL)
			return 0;

		NetOrganizeTeamRes res;
		res.name = player->getName();
		res.isJoin = 1;
		leader->sendPacket(res);

		return 0;
	}

	if (!tm0 && !tm1)
	{
		Team* tm = GetModule(TeamModule)->createTeam(player);
		if (tm == NULL)
			return 0;

		NetOrganizeTeamRes res;
		res.name = player->getName();
		res.isJoin = 0;
		target->sendPacket(res);
		return 0;
	}
	return 1;
}

int32 PlayerHandler::onNetAgreeTeamReq(Player* player, NetAgreeTeamReq* req)
{
	Player* target = GetModule(WorldModule)->getPlayerByName(req->name);
	if (!target)
		return 0;

	Team* tm0 = GetModule(TeamModule)->getPlayerTeam(player->getUserId());
	Team* tm1 = GetModule(TeamModule)->getPlayerTeam(target->getUserId());

	if (tm0 && tm1)
		return 0;

	// 邀请;
	if (req->isJoin == 0)
	{
		if (tm0)
		{
			// 已经有队伍了;
			player->sendRespnoseMsg(MC_InviterHaveTeam);
			return 0;
		}

		if (!tm1)
		{
			// 队伍不存在;
			player->sendRespnoseMsg(MC_JoinerTeamNotExist);
			return 0;
		}

		if (tm1->CanAddTeam(player) == 0)
		{
			tm1->addPlayer(player);
			return 0;
		}
	}
	else
	{
		// 加入;
		if (tm1)
		{
			// 已经有队伍了;
			player->sendRespnoseMsg(MC_HaveTeam);
			return 0;
		}
		if (!tm0)
		{
			// 已经没有队伍了;
			player->sendRespnoseMsg(MC_TeamNotExist);
			return 0;
		}

		if (tm0->getLeader() != player)
		{
			// 不是队长不能同意加入;
			player->sendRespnoseMsg(MC_AgreeAddTeamNotLeader);
			return 0;
		}
		if (tm0->CanAddTeam(target) == 0)
		{
			tm0->addPlayer(target);
			return 0;
		}
	}
	player->sendRespnoseMsg(MC_TeamError);
	return 0;
}

int32 PlayerHandler::onNetTeamListReq(Player* player, NetTeamListReq* req)
{
	return 1;
}