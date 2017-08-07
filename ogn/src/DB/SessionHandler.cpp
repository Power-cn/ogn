#include "stdafx.h"

#define REGISTER_EVENT(msg, cb, obj) INSTANCE(Application).getDBServer()->addEventListener(msg, (EventCallbackProcess)cb, obj)
SessionHandler::SessionHandler()
{
	REGISTER_EVENT(ID_NetSessionEnterNotify, &SessionHandler::onNetSessionEnterNotify, this);
	REGISTER_EVENT(ID_NetSessionLeaveNotify, &SessionHandler::onNetSessionLeaveNotify, this);
	REGISTER_EVENT(ID_NetPlayerSaveNotify, &SessionHandler::onNetPlayerSaveNotify, this);

	REGISTER_EVENT(ID_NetLoginReq, &SessionHandler::onNetLoginReq, this);
	REGISTER_EVENT(ID_NetCreateRoleReq, &SessionHandler::onNetCreateRoleReq, this);
	REGISTER_EVENT(ID_NetSelectRoleReq, &SessionHandler::onNetSelectRoleReq, this);
	REGISTER_EVENT(ID_NetQueryRoleReq, &SessionHandler::onNetQueryRoleReq, this);
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

int SessionHandler::onNetPlayerSaveNotify(Session* session, NetPlayerSaveNotify* nfy)
{
	for (uint32 i = 0; i < nfy->roleInfos.size() && i < sMaxRoleCount; ++i)
	{
		DBRoleInfo& info = nfy->roleInfos[i];
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

int SessionHandler::onNetLoginReq(Session* session, NetLoginReq* req)
{
	NetLoginRes res;
	if (DoLogin(session, req, res) == 0)
	{
		res.result = NResultFail;
		session->sendPacketToWorld(res);
		return 0;
	}
	if (DoQueryRole(session, req, res) == 0)
	{
		res.result = NResultFail;
		session->sendPacketToWorld(res);
		return 0;
	}
	res.result = NResultSuccess;
	session->sendPacketToWorld(res);
	LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] login success", req->user.c_str());
	return 0;
}


int SessionHandler::onNetCreateRoleReq(Session* session, NetCreateRoleReq* req)
{
	NetCreateRoleRes res;
	if (DoCreateRole(session, req, res) == 0)
	{
		session->sendPacketToWorld(res);
		return 0;
	}
	session->sendPacketToWorld(res);
	LOG_DEBUG(LogSystem::csl_color_red_blue, "accId[%d] insert role[%s]", req->accId, res.roleInfo.name.c_str());
	return 1;
}

int SessionHandler::onNetSelectRoleReq(Session* session, NetSelectRoleReq* req)
{
	NetSelectRoleRes res;
	if (DoSelectRole(session, req, res) == 0)
	{
		session->sendPacketToWorld(res);
		return 0;
	}

	session->sendPacketToWorld(res);
	LOG_DEBUG(LogSystem::csl_color_red_blue, "accId[%d] userId[%d] selete role[%s]", res.roleInfo.accountId, res.roleInfo.id, res.roleInfo.name.c_str());
	return 1;
}

int SessionHandler::onNetQueryRoleReq(Session* session, NetQueryRoleReq* req)
{
	NetQueryRoleRes res;
	DBUser role;
	role.accountId = req->accountId;
	uint32 queryCount = 0;
	const int32 maxCount = sMaxRoleCount;
	DBUser retRoles[maxCount];
	std::vector<DBRecord*> result_records;
	result_records->push_back((DBRecord*)&retRoles[0]);
	DBRecord* result_records[] = { (DBRecord*)&retRoles [0],(DBRecord*)&retRoles[1],(DBRecord*)&retRoles[2] };
	const int8* err = INSTANCE(Application).getDBConnector()->doQuery(role, result_records, queryCount, "accountId", "", maxCount);
	do {
		if (err)
		{
			LOG_ERROR(err);
			session->sendPacketToWorld(res);
			return 0;
		}

		if (queryCount > 0)
		{
			for (uint32 i = 0; i < queryCount; ++i)
			{
				DBRoleInfo info;
				info.id = role.id;
				info.accountId = role.id;
				info.name = role.name;
				DBUser& dbUser = retRoles[i];
				info.property.write(dbUser.property.datas(), dbUser.property.wpos());
				res.roleInfos.push_back(info);
			}
			LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] query role count[%d]", req->user.c_str(), queryCount);
		}
	} while (false);
	session->sendPacketToWorld(res);
	return 1;
}

int SessionHandler::DoLogin(Session* session, NetLoginReq* req, NetLoginRes& res)
{
	DBAccount account;
	account.user = req->user;
	uint32 queryCount = 0;
	const int8* err = INSTANCE(Application).getDBConnector()->doQuery(account, { (DBRecord*)&account }, queryCount, "user", "");
	do
	{
		if (err)
		{
			LOG_ERROR(err);
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
				return 0;
			}
		}
	} while (false);

	res.accInfo.id = account.id;
	res.accInfo.user = account.user;
	res.accInfo.password.write(account.password.datas(), account.password.wpos());
	res.accInfo.name = account.name;
	res.accInfo.email = account.email;
	res.accInfo.phone = account.phone;
	return 1;
}

int SessionHandler::DoQueryRole(Session* session, NetLoginReq* req, NetLoginRes& res)
{
	DBUser role;
	role.accountId = res.accInfo.id;
	uint32 queryCount = 0;
	const int32 maxCount = sMaxRoleCount;
	DBUser retRoles[maxCount];

	int len = sizeof(DBUser);
	int len1 = sizeof(DBRecord);
	//uint32 t0 = (uint32)DateTime::GetNowAppUS();
	const int8* err = INSTANCE(Application).getDBConnector()->doQuery(role, &retRoles[0], queryCount, "accountId", "", maxCount);
	do {
		if (err)
		{
			LOG_ERROR(err);
			return 0;
		}

		if (queryCount > 0)
		{
			for (uint32 i = 0; i < queryCount; ++i)
			{
				DBRoleInfo info;
				DBUser& dbUser = retRoles[i];
				info.id = dbUser.id;
				info.accountId = dbUser.accountId;
				info.name = dbUser.name;
				info.property.write(dbUser.property.datas(), dbUser.property.wpos());
				res.roleInfos.push_back(info);
			}
			LOG_DEBUG(LogSystem::csl_color_red_blue, "[%s] query role count[%d]", req->user.c_str(), queryCount);
		}
	} while (false);
	return 1;
}

int SessionHandler::DoCreateRole(Session* session, NetCreateRoleReq* req, NetCreateRoleRes& res)
{
	res.accId = req->accId;
	{
		DBUser queryRole;
		queryRole.accountId = req->accId;
		uint32 queryCount = 0;
		const int32 maxCount = sMaxRoleCount;
		DBUser retRoles[maxCount];
		char* err = INSTANCE(Application).getDBConnector()->doQuery(queryRole, retRoles, queryCount, "accountId", "", maxCount);
		if (queryCount >= sMaxRoleCount)
		{
			res.result = NResultFail;
			return 0;
		}
	}

	{
		DBUser role;
		role.accountId = req->accId;
		role.name = req->name;
		role.createTime = DateTime::Now();
		char* err = INSTANCE(Application).getDBConnector()->doInsert(role, "", "name");
		if (err)
		{
			LOG_ERROR(err);
			res.result = NResultFail;

			return 0;
		}
		res.result = NResultSuccess;
		DBRoleInfo& info = res.roleInfo;
		info.id = role.id;
		info.accountId = role.id;
		info.name = role.name;
		info.property.write(role.property.datas(), role.property.wpos());
	}
	return 1;
}

int SessionHandler::DoSelectRole(Session* session, NetSelectRoleReq* req, NetSelectRoleRes& res)
{
	res.accId = req->accId;
	DBUser role;
	role.id = req->userId;
	uint32 queryCount = 0;
	const int32 maxCount = 1;
	const int8* err = INSTANCE(Application).getDBConnector()->doQuery(role, &role, queryCount, "id", "", maxCount);
	do {
		if (err)
		{
			LOG_ERROR(err);
			res.result = NResultFail;
			return 0;
		}

		if (queryCount > 0)
		{
			DBRoleInfo& info = res.roleInfo;
			info.id = role.id;
			info.accountId = role.accountId;
			info.name = role.name;
			info.property.write(role.property.datas(), role.property.wpos());
			res.result = 0;
		}
	} while (false);
	res.result = NResultSuccess;
	return 1;
}
