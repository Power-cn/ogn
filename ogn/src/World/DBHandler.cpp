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
	RegDBEvent(ID_NetCreateRoleRes, &DBHandler::onNetCreateRoleRes, this);
	RegDBEvent(ID_NetSelectRoleRes, &DBHandler::onNetSelectRoleRes, this);
	RegDBEvent(ID_NetQueryRoleRes, &DBHandler::onNetQueryRoleRes, this);
}

int DBHandler::onNetNetLoginRes(Session* session, NetLoginRes* res)
{
	uint32 accId = res->accInfo.id;

	if (res->result != NResultSuccess)
	{
		session->sendPacketToWorld(*res);
		return 0;
	}

	Player* player = sWorld.FindPlrByAccId(accId);
	do 
	{
		if (player) {
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
		player->setAccId(res->accInfo.id);
		player->setUser(res->accInfo.user);
		sWorld.addPlayerByAccId(player);
	} while (false);
	res->guid = player->getGuid();
	player->sendPacket(*res);
	return 0;
}

int DBHandler::onNetCreateRoleRes(Session* session, NetCreateRoleRes* res)
{
	if (res->result == NResultFail)
	{
		session->sendPacketToWorld(*res);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	/// 角色创建成功 ///

	char szBuffer[256] = { 0 };
	sprintf_s(szBuffer, 256, "hmset %s %d %s", sUserIdToName, res->roleInfo.id, res->roleInfo.name.c_str());
	sRedisProxy.sendCmd(szBuffer, NULL, NULL);

	sprintf_s(szBuffer, 256, "hmset %s %s %d", sNameToUserId, res->roleInfo.name.c_str(), res->roleInfo.id);
	sRedisProxy.sendCmd(szBuffer, NULL, NULL);

	session->sendPacketToWorld(*res);
	return 0;
}

int DBHandler::onNetSelectRoleRes(Session* session, NetSelectRoleRes* res)
{
	if (res->result == NResultFail) {
		session->sendPacketToWorld(*res);
		return 0;
	}

	Player* aPlr = sWorld.FindPlrByAccId(res->accId);
	if (aPlr == NULL) {
		res->result = NResultFail;
		session->sendPacketToWorld(*res);
		return 0;
	}

	session->sendPacketToWorld(*res);

	aPlr->SetOfflineTimer(DateTime::Now());

	DBRoleInfo& info = res->roleInfo;
	aPlr->setUserId(info.id);
	aPlr->setName(info.name);
	aPlr->SetOnlineTimer(DateTime::Now());
	sWorld.addPlayerToUserId(aPlr);
	sWorld.addPlayerByName(aPlr);

	Dictionary dict;
	if (info.property.wpos() <= 0)
		aPlr->DoCreateCharacter(dict, info);
	else
		info.property >> dict;

	sApp.onLoad(aPlr, dict);
	sApp.onEnterWorld(aPlr, dict);
	return 0;
}

int DBHandler::onNetQueryRoleRes(Session* session, NetQueryRoleRes* res)
{
	Player* aPlr = session->getPlayer();
	if (aPlr == NULL) {
		return 0;
	}

	session->sendPacketToWorld(*res);
	return 0;
}
