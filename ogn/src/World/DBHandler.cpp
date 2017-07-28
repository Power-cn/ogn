#include "stdafx.h"

DBHandler::DBHandler()
{

}

DBHandler::~DBHandler()
{

}

void DBHandler::doRegister()
{
	if (GetDBServer() == NULL)
	{
		LOG_ERROR("GetDBServer() == NULL");
		return;
	}
	RegisterEventProcess(GetDBServer(), ID_NetLoginRes, &DBHandler::onNetNetLoginRes, this);
	RegisterEventProcess(GetDBServer(), ID_NetQueryRoleRes, &DBHandler::onNetQueryRoleRes, this);
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
				INSTANCE(Application).onLeaveWorld(player, dict);

				NetLoginRes res;
				res.result = 3;
				oldSession->sendPacketToWorld(res);
				player->unbindSession();

				INSTANCE(Application).doSessionLeaveWorld(oldSession);
			}
			player->bindSession(session);
			break;
		}

		player = new Player;
		player->setAccountId(res->accountInfo.id);
		player->setUser(res->accountInfo.user);
		player->setName(res->accountInfo.user);
		player->bindSession(session);
		GetModule(WorldModule)->addPlayer(player);

	} while (false);

	NetQueryRoleReq req;
	req.accountId = player->getAccountId();
	req.user = player->getUser();
	req.roleCount = 1;
	session->sendPacketToTarget(req, INSTANCE(Application).getDBServer()->getSocket());
	res->instanceId = player->getInstanceId();
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
		sprintf_s(szBuffer, 256, "hmset %s %s %d", sNameToUserId, player->getName().c_str(), player->getUserId());
		sRedisProxy.sendCmd(szBuffer, NULL, NULL);

		Dictionary dict;
		if (info.property.getWPostion() <= 0)
			player->DoCreateCharacter(dict, info);
		else
			info.property >> dict;

		INSTANCE(Application).onLoad(player, dict);
		INSTANCE(Application).onEnterWorld(player, dict);
		return 0;
	}
	LOG_ERROR("create character fail");
	return 0;
}
