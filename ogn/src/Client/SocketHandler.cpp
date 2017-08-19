#include "stdafx.h"

SocketHandler::SocketHandler()
{
	mIndex = 0;

	//std::map<int32, RobotJson>& mapRobotJson = INSTANCE(ConfigManager).getMapRobotJson();
	//for (auto itr : mapRobotJson)
	//{
	//	RobotJson& robotJson = itr.second;
	//	mUsers.push(std::make_pair(robotJson.User, robotJson.Password));
	//}
	//for (int i = 0; i < 1; ++i)
	//{
	//	createRobot();
	//}
	//createRobot();
}

int SocketHandler::onConnect(SocketEvent& e)
{
	Robot* robot = new Robot(e.socket);
	INSTANCE(RobotManager).add(e.socket, robot);
	std::pair<std::string, std::string>& pa = mUsers.front();
	robot->doLogin(pa.first, pa.second);
	mUsers.pop();
	if (!INSTANCE(RobotManager).mCurRobot) {
		INSTANCE(RobotManager).mCurRobot = robot;
		mCreate = false;
	}
	return 0;
}

int SocketHandler::onRecv(SocketEvent& e)
{
	//AES aes(sKey);
	//aes.InvCipher(e.data, e.count);
	Shared::XOR((char*)e.data, e.count, sKey);

	BinaryStream out(e.data, e.count);
	int32 msgId = 0;
	int32 rpos = out.wpos();
	CHECK_RETURN(out >> msgId, 0);
	out.rpos(rpos);
	Packet* pack = INSTANCE(PacketManager).Alloc(msgId);
	if (pack == NULL) return 0;

	if ((out >> (*pack)) == false)
	{
		LOG_ERROR("pack->deSerialize(out)");
		INSTANCE(PacketManager).Free(pack);
		return 0;
	}
	//this->dispatchEvent(pack->getMsgId(), e.socket, pack);

	Robot* robot = INSTANCE(RobotManager).getRobat(e.socket);
	if (robot)
		INSTANCE(RobotManager).dispatch(pack->getMsgId(), robot, pack);

	INSTANCE(PacketManager).Free(pack);
	return 0;
}

int SocketHandler::onExit(SocketEvent& e)
{
	INSTANCE(RobotManager).removeRobat(e.socket);
	clients.erase(e.socket->getSocketId());
	INSTANCE(RobotManager).mCurRobot = NULL;
	LOG_ERROR("onExit");
	return 0;
}

int SocketHandler::onException(SocketEvent& e)
{
	LOG_ERROR(__FUNCTION__);
	delete e.targetDispatcher;

	createRobot();
	return 0;
}

void SocketHandler::createRobot()
{
	if (mUsers.size() <= 0)
		return;
	mCreate = true;
	std::pair<std::string, std::string>& pa = mUsers.front();
	//LOG_INFO("createRobot: %s", pa.first.c_str());

	int32 idx = rand() % 4;
	char szBuf[32] = { 0 };
	sprintf_s(szBuf, 32, "Gate");
	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig(szBuf);

	SocketClient* client = INSTANCE(Network).connect(cfg.Host.c_str(), cfg.Port);
	mListSocketClient.push_back(client);
	mIndex++;

	client->addEventListener(SocketEvent::CONNECT, (EventCallback)&SocketHandler::onConnect, this);
	client->addEventListener(SocketEvent::RECV, (EventCallback)&SocketHandler::onRecv, this);
	client->addEventListener(SocketEvent::EXIT, (EventCallback)&SocketHandler::onExit, this);
	client->addEventListener(SocketEvent::EXCEPTION, (EventCallback)&SocketHandler::onException, this);
}

void SocketHandler::PushUser(cstring& user)
{
	mUsers.push(std::make_pair(user, ""));
}
