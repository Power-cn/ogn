#pragma once


class SocketHandler : public EventDispatcher
{
public:
	SocketHandler();
	int onConnect(SocketEvent& e);
	int onRecv(SocketEvent& e);
	
	int onExit(SocketEvent& e);
	int onException(SocketEvent& e);
public:
	void createRobot();
	void PushUser(cstring& user);
public:
	std::map<int32, Socket*> clients;
	std::set<SocketClient*>		mListSocketClient;
	std::queue<std::pair<std::string, std::string>> mUsers;
	int32	mIndex;
	bool	mCreate = false;
};
