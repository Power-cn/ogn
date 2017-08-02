#include "stdafx.h"

#define REGISTER_EVENT(msg, cb, obj) INSTANCE(Application).getDBServer()->addEventListener(msg, (EventCallbackProcess)cb, obj)
SessionHandler::SessionHandler()
{
	REGISTER_EVENT(ID_NetSessionEnterNotify, &SessionHandler::onNetSessionEnterNotify, this);
	REGISTER_EVENT(ID_NetSessionLeaveNotify, &SessionHandler::onNetSessionLeaveNotify, this);
	REGISTER_EVENT(ID_NetLoginReq, &SessionHandler::onNetLoginReq, this);
	REGISTER_EVENT(ID_NetQueryRoleReq, &SessionHandler::onNetQueryRoleReq, this);
	REGISTER_EVENT(ID_NetQueryRoleRes, &SessionHandler::onNetQueryRoleRes, this);
}

SessionHandler::~SessionHandler()
{

}

int SessionHandler::onNetSessionEnterNotify(Session* session, NetSessionEnterNotify* nfy)
{
	LOG_INFO("ssnId %0.16llx enter world", session->getSessionId());

	return 0;
}

int SessionHandler::onNetSessionLeaveNotify(Session* ssn, NetSessionLeaveNotify* nfy)
{
	LOG_INFO("ssnId %0.16llx leave world", ssn->getSessionId());

	INSTANCE(SessionManager).removeSessionsBySocket(ssn->getSocketId(), ssn);
	return 0;
}

int SessionHandler::onNetLoginReq(Session* session, NetLoginReq* req)
{
	DBAccount account;
	account.user = req->user;
	uint32 queryCount = 0;
	NetLoginRes res;
    const int8* err = INSTANCE(Application).getDBConnector()->doQuery(account, &account, queryCount, "user", "");
	do 
	{
		if (err)
		{
			res.result = 1;
			LOG_ERROR(err);
			session->sendPacketToWorld(res);
			return 0;
		}
		if (queryCount == 0)
		{
			account.user = req->user;
			account.createTime = DateTime::Now();
			err = INSTANCE(Application).getDBConnector()->doInsert(account, "", "user");
			if (err) {
				res.result = 1;
				LOG_ERROR(err);
				session->sendPacketToWorld(res);
				return 0;
			}
		}
	} while (false);

	res.result = 0;
	res.accountInfo.id = account.id;
	res.accountInfo.user = account.user;
	res.accountInfo.password.write(account.password.datas(), account.password.wpos());
	res.accountInfo.name = account.name;
	res.accountInfo.email = account.email;
	res.accountInfo.phone = account.phone;
	session->sendPacketToWorld(res);

	LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] login success", account.user.c_str());
	return 0;
}

int SessionHandler::onNetQueryRoleReq(Session* session, NetQueryRoleReq* req)
{
	DBUser role;
	role.accountId = req->accountId;
	uint32 queryCount = 0;
	int32 maxCount = 1;
	NetQueryRoleRes res;
	uint32 t0 = (uint32)DateTime::GetNowAppUS();
	const int8* err = INSTANCE(Application).getDBConnector()->doQuery(role, &role, queryCount, "accountId", "", maxCount);
	do
	{
		if (err)
		{
			LOG_ERROR(err);
			session->sendPacketToWorld(res);
			return 0;
		}
		res.accountId = req->accountId;

		if (queryCount > 0)
		{
			DBRoleInfo info;
			info.id = role.id;
			info.accountId = role.id;
			info.name = role.name;
			info.property.write(role.property.datas(), role.property.wpos());
			res.roleInfos.push_back(info);
			uint32 t1 = (uint32)DateTime::GetNowAppUS() - t0;
			LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] query role[%s] time:[%d]", req->user.c_str(), role.name.c_str(), t1);
		}
		else
		{
			role.accountId = req->accountId;
			role.name = req->user;
			role.createTime = DateTime::Now();
			char* err = INSTANCE(Application).getDBConnector()->doInsert(role, "", "name");
			if (err)
			{
				LOG_ERROR(err);
				session->sendPacketToWorld(res);
				return 0;
			}

			DBRoleInfo info;
			info.id = role.id;
			info.accountId = role.id;
			info.name = role.name;
			info.property.write(role.property.datas(), role.property.wpos());
			res.roleInfos.push_back(info);

			uint32 t1 = (uint32)DateTime::GetNowAppUS() - t0;

			LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] insert role[%s] time:[%d]", req->user.c_str(), role.name.c_str(), t1);
		}
	} while (false);

	session->sendPacketToWorld(res);
	return 0;
}

int SessionHandler::onNetQueryRoleRes(Session* session, NetQueryRoleRes* res)
{
	if (res->roleInfos.size() > 0)
	{
		DBRoleInfo& info = res->roleInfos[0];
		DBUser dbRole;
		dbRole.id = info.id;
		dbRole.property.write(info.property.datas(), info.property.wpos());
		uint32 updateRows = 0;
		const int8* err = INSTANCE(Application).getDBConnector()->doUpdate(dbRole, "id", updateRows, "property");
		if (!err) return 0;
		LOG_ERROR(err);
	}
	return 0;
}
