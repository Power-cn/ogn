#include "stdafx.h"

SessionHandler::SessionHandler()
{
	if (GetWorldServer() == NULL)
	{
		LOG_ERROR("GetWorldServer() == NULL");
		return;
	}

	RegisterEventProcess(GetWorldServer(), ID_NetSessionEnterNotify, &SessionHandler::onNetSessionEnterNotify, this);
	RegisterEventProcess(GetWorldServer(), ID_NetSessionLeaveNotify, &SessionHandler::onNetSessionLeaveNotify, this);
	RegisterEventProcess(GetWorldServer(), ID_NetLoginReq, &SessionHandler::onNetLoingReq, this);
	RegisterEventProcess(GetWorldServer(), ID_NetPingNotify, &SessionHandler::onNetPingNotify, this);
}

SessionHandler::~SessionHandler()
{

}

int SessionHandler::onNetSessionEnterNotify(Session* session, NetSessionEnterNotify* nfy)
{
	LOG_INFO("sessionId %0.16llx enter world", session->getSessionId());
	session->sendPacketToTarget(*nfy, INSTANCE(Application).getDBServer()->getSocket());
	return 0;
}

int SessionHandler::onNetSessionLeaveNotify(Session* session, NetSessionLeaveNotify* nfy)
{
	INSTANCE(Application).doSessionLeaveWorld(session);
	return 0;
}

int SessionHandler::onNetLoingReq(Session* session, NetLoginReq* req)
{
	Player* plr = session->getPlayer();
	if (plr)
	{
		plr->sendRespnoseMsg(MC_LoginYouHaveLanded);
		return 0;
	}

	INSTANCE(Application).sendPacketToDB(*req, session);
	return 0;
}

int SessionHandler::onNetPingNotify(Session* session, NetPingNotify* nfy)
{
	session->sendPacketToWorld(*nfy);
	return 0;
}

