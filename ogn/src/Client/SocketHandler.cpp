#include "stdafx.h"

SocketHandler::SocketHandler()
{
	mIndex = 0;

	std::map<int32, RobotJson>& mapRobotJson = INSTANCE(ConfigManager).getMapRobotJson();
	for (auto itr : mapRobotJson)
	{
		RobotJson& robotJson = itr.second;
		mUsers.push(std::make_pair(robotJson.User, robotJson.Password));
	}

	createRobot();
}

int SocketHandler::onConnect(SocketEvent& e)
{
	Robot* robot = new Robot(e.socket);
	INSTANCE(RobotManager).add(e.socket, robot);
	std::pair<std::string, std::string>& pa = mUsers.front();
	robot->doLogin(pa.first, pa.second);
	mUsers.pop();

	return 0;
}

int SocketHandler::onRecv(SocketEvent& e)
{
	BinaryStream out(e.data, e.count);
	int32 msgId = 0;
	int32 rpos = out.getWPostion();
	CHECK_RETURN(out >> msgId, 0);
	out.setRPostion(rpos);
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
	LOG_ERROR("onExit");
	return 0;
}

int SocketHandler::onException(SocketEvent& e)
{
	return 0;
}

void SocketHandler::createRobot()
{
	if (mUsers.size() <= 0)
		return;


	std::pair<std::string, std::string>& pa = mUsers.front();
	LOG_INFO("createRobot: %s", pa.first.c_str());

	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig("OGN");

	SocketClient* client = INSTANCE(Network).connect(cfg.Host.c_str(), cfg.Port);
	mListSocketClient.push_back(client);
	mIndex++;

	client->addEventListener(SocketEvent::CONNECT, (EventCallback)&SocketHandler::onConnect, this);
	client->addEventListener(SocketEvent::RECV, (EventCallback)&SocketHandler::onRecv, this);
	client->addEventListener(SocketEvent::EXIT, (EventCallback)&SocketHandler::onExit, this);
	client->addEventListener(SocketEvent::EXCEPTION, (EventCallback)&SocketHandler::onException, this);
}
