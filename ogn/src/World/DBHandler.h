#pragma once


class Session;
class DBHandler : public Object
{
public:
	DBHandler();
	~DBHandler();
	void doRegister();
protected:
	int onNetNetLoginRes(Session* session, NetLoginRes* res);
	int onNetQueryRoleRes(Session* session, NetQueryRoleRes* res);
private:
};