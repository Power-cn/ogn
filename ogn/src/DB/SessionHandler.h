#pragma once

class Session;
class SessionHandler : public Object
{
public:
	SessionHandler();
	~SessionHandler();
protected:

	int onNetSessionEnterNotify(Session* session, NetSessionEnterNotify* nfy);
	int onNetSessionLeaveNotify(Session* session, NetSessionLeaveNotify* nfy);
	int onNetPlayerSaveNotify(Session* session, NetPlayerSaveNotify* req);
	int onNetLoginReq(Session* session, NetLoginReq* req);
	int onNetCreateRoleReq(Session* session, NetCreateRoleReq* req);
	int onNetSelectRoleReq(Session* session, NetSelectRoleReq* req);
	int onNetQueryRoleReq(Session* session, NetQueryRoleReq* req);
private:
	int DoLogin(Session* session, NetLoginReq* req, NetLoginRes& res);
	int DoQueryRole(Session* session, NetLoginReq* req, NetLoginRes& res);
	int DoCreateRole(Session* session, NetCreateRoleReq* req, NetCreateRoleRes& res);
	int DoSelectRole(Session* session, NetSelectRoleReq* req, NetSelectRoleRes& res);
};