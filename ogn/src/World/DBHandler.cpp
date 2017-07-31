#include "stdafx.h"

DBHandler::DBHandler()
{

}

DBHandler::~DBHandler()
{

}

void DBHandler::doRegister()
{
	RegDBEvent(ID_NetLoginRes, &DBHandler::onNetNetLoginRes, this);
	RegDBEvent(ID_NetQueryRoleRes, &DBHandler::onNetQueryRoleRes, this);
}

int DBHandler::onNetNetLoginRes(Session* session, NetLoginRes* res)
{
	if (res->result != 0)
	{
		session->sendPacketToWorld(*res);
		return 0;
	}

	Player* player = GetModule(WorldModule)->getPlayer(res->accountInfo.id);
	do 
	{
		if (player)
		{
			Session* oldSession = player->getSession();
			if (oldSession)
			{
				player->sendRespnoseMsg(MC_LoginTheNumberInTheRemote);
				Dictionary dict;
				sApp.onLeaveWorld(player, dict);

				NetLoginRes res;
				res.result = 3;
				oldSession->sendPacketToWorld(res);
				player->unbindSession();

				sApp.doSessionLeaveWorld(oldSession);
			}
			player->SetOnline(true);
			player->bindSession(session);
			break;
		}

		player = new Player;
		player->SetOnline(true);
		player->bindSession(session);
		player->setAccId(res->accountInfo.id);
		player->setUser(res->accountInfo.user);
		player->setName(res->accountInfo.user);
		sWorld.addPlayer(player);

	} while (false);

	NetQueryRoleReq req;
	req.accountId = player->getAccId();
	req.user = player->getUser();
	req.roleCount = 1;
	session->sendPacketToTarget(req, sApp.getDBServer() ? sApp.getDBServer()->getSocket() : NULL);
	res->guid = player->getGuid();
	player->sendPacket(*res);
	return 0;
}

int DBHandler::onNetQueryRoleRes(Session* session, NetQueryRoleRes* res)
{
	Player* player = GetModule(WorldModule)->getPlayer(res->accountId);
	if (!player) return 0;

	if (res->roleInfos.size() == 1)
	{
		player->SetOfflineTimer(DateTime::Now());

		DBRoleInfo& info = res->roleInfos[0];
		player->setUserId(info.id);
		player->setName(info.name);
		player->SetOnlineTimer(DateTime::Now());
		sWorld.addPlayerToUserId(player);

		char szBuffer[256] = { 0 };
		sprintf_s(szBuffer, 256, "hmset %s %s %d", sNameToUserId, player->getName(), player->getUserId());
		sRedisProxy.sendCmd(szBuffer, NULL, NULL);

		Dictionary dict;
		if (info.property.getWPostion() <= 0)
			player->DoCreateCharacter(dict, info);
		else
			info.property >> dict;

		sApp.onLoad(player, dict);
		sApp.onEnterWorld(player, dict);
		return 0;
	}
	LOG_ERROR("create character fail");
	return 0;
}
