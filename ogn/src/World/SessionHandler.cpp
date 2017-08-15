#include "stdafx.h"

SessionHandler::SessionHandler()
{
	RegWorldEvent(ID_NetSessionEnterNotify, &SessionHandler::onNetSessionEnterNotify, this);
	RegWorldEvent(ID_NetSessionLeaveNotify, &SessionHandler::onNetSessionLeaveNotify, this);
	RegWorldEvent(ID_NetLoginReq, &SessionHandler::onNetLoingReq, this);
	RegWorldEvent(ID_NetPingNotify, &SessionHandler::onNetPingNotify, this);
}

SessionHandler::~SessionHandler()
{

}

int SessionHandler::onNetSessionEnterNotify(Session* session, NetSessionEnterNotify* nfy)
{
	LOG_INFO("ssnId %0.16llx [%s] enter world", session->getSessionId(), nfy->host.c_str());
	session->setHost(nfy->host);
	session->sendPacketToDB(*nfy);
	return 0;
}

int SessionHandler::onNetSessionLeaveNotify(Session* session, NetSessionLeaveNotify* nfy)
{
	sApp.doSessionLeaveWorld(session);
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

	session->sendPacketToDB(*req);
	return 0;
}

int SessionHandler::onNetPingNotify(Session* session, NetPingNotify* nfy)
{
	session->sendPacketToWorld(*nfy);
	return 0;
}

