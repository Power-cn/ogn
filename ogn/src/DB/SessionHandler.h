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
	int onNetLoginReq(Session* session, NetLoginReq* req); 
	int onNetQueryRoleReq(Session* session, NetQueryRoleReq* req);
	int onNetQueryRoleRes(Session* session, NetQueryRoleRes* res);
private:
};