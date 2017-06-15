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
	int onNetLoingReq(Session* session, NetLoginReq* req);
	int onNetPingNotify(Session* session, NetPingNotify* nfy);
private:
};