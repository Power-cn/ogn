#include "stdafx.h"

Application::Application():
gateServer(NULL),
worldServer(NULL)
{
	mTime = 0.0;
	mDelay = 0.0;
	mFPSTimer = 0.0;
	mFPS = 0;
}
Application::~Application()
{

}

bool Application::Initialize()
{
	INSTANCE(ConfigManager);
	INSTANCE(SessionManager);
	INSTANCE(ConfigManager).reloadConfig();

	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("Gate");
	gateServer = INSTANCE(Network).listen(cf.Port);
	if (!gateServer) {
		LOG_ERROR("Gate listen Port:%d fial", cf.Port);
		return false;
	}

	LOG_DEBUG(LogSystem::csl_color_green, "Gate listen Port:%d success", cf.Port);

	gateServer->addEventListener(SocketEvent::ACCEPT, (EventCallback)&Application::onGateAccept, this);
	gateServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onGateRecv, this);
	gateServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onGateExit, this);


	return true;
}

bool Application::Update()
{
	mDelay = DateTime::GetNowAppUS() - mTime;
	mTotalTime += mDelay;
	mFPSTimer += mDelay;
	mTime = DateTime::GetNowAppUS();
	mFPS++;
	if (mFPSTimer >= 1.0)
	{
		static char szBuffer[256] = { 0 };
		sprintf_s(szBuffer, 256, "Gate:FPS:%d Player:%d", mFPS, gateServer->getSocketCount());
		Shared::setConsoleTitle(szBuffer);
		//LOG_INFO("FPS:%d", mFPS);
		mFPS = 0;
		mFPSTimer = 0.0;
	}

	INSTANCE(Network).update(0.f, 0.f);
	if (!worldServer)
	{
		ServerConfig& cf = INSTANCE(ConfigManager).getConfig("World");
		worldServer = INSTANCE(Network).connect(cf.Host.c_str(), cf.Port);

		if (worldServer)
		{
			LOG_DEBUG(LogSystem::csl_color_green, "connect World Port:%d ...", cf.Port);
			worldServer->addEventListener(SocketEvent::CONNECT, (EventCallback)&Application::onWorldConnect, this);
			worldServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onWorldRecv, this);
			worldServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onWorldExit, this);
			worldServer->addEventListener(SocketEvent::EXCEPTION, (EventCallback)&Application::onWorldException, this);
		}
		else
		{
			LOG_DEBUG(LogSystem::csl_color_green, "connect World Port:%d fail...", cf.Port);
		}
	}


	return true;
}

bool Application::Destroy()
{
	return true;
}

bool Application::sendPacketToWorld(Packet& packet, Session* tar)
{
	if (!worldServer)
		return false;
	if (tar)
		tar->sendPacketToTarget(packet, worldServer->getSocket());
	return true;
}

bool Application::sendBufferToWorld(int8* data, int32 count, Session* tar)
{
	if (!worldServer)
		return false;
	if (tar)
		tar->sendBufferToTarget(data, count, worldServer->getSocket());
	return true;
}

int Application::onGateAccept(SocketEvent& e)
{
	if (worldServer == NULL) {
		INSTANCE(Network).closesocket(e.socket->getSocketId());
		return 0;
	}

	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("Gate");

	uint64 sessionId = ((uint64)cf.Port << 32) | ++Session::sId;
	Session* session = INSTANCE(SessionManager).createSession(e.socket, sessionId);
	if (!session)
		return 0;
	session = INSTANCE(SessionManager).addSessionBySocket(e.socket->getSocketId(), session);
	if (!session)
		return 0;

	NetSessionEnterNotify nfy;
	nfy.host = e.socket->getIP();
	LOG_INFO("sessionId %0.16llx [%s] accept", session->getSessionId(), e.socket->getIP());
	sendPacketToWorld(nfy, session);
	return 0;
}

int Application::onGateRecv(SocketEvent& e)
{
	if (worldServer == NULL) {
		INSTANCE(Network).closesocket(e.socket->getSocketId());
		return 0;
	}

	Session* session = INSTANCE(SessionManager).getSessionBySocket(e.socket->getSocketId());
	if (session == NULL)
	{
		INSTANCE(Network).closesocket(e.socket->getSocketId());
		return 0;
	}

#ifdef _DEBUG
	BinaryStream out(e.data, e.count);
	int32 msgId = 0;
	int32 rpos = out.getWPostion();
	CHECK_RETURN(out >> msgId, 0);
	out.setRPostion(rpos);
	DEBUG_DEBUG(LogSystem::csl_color_green_blue, "session:%0.16llx c to s %s size:%d", session->getSessionId(), INSTANCE(PacketManager).GetName(msgId).c_str(), e.count);
#endif // _DEBUG

	/*
	*********** 解密 ***********
	*/
	sendBufferToWorld((int8*)e.data, e.count, session);
	/*
	*********** 解密 ***********
	*/
	return 0;
}

int Application::onGateExit(SocketEvent& e)
{
	Session* session = INSTANCE(SessionManager).getSessionBySocket(e.socket->getSocketId());
	if (!session)
		return 0;

	LOG_INFO("sessionId %0.16llx leave", session->getSessionId());

	NetSessionLeaveNotify nfy;
	sendPacketToWorld(nfy, session);

	INSTANCE(SessionManager).removeSessionBySocket(e.socket->getSocketId());
	INSTANCE(SessionManager).removeSession(session->getSessionId());
	return 0;
}

int Application::onWorldConnect(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_green, "world connect success");
	return 0;
}

int Application::onWorldRecv(SocketEvent& e)
{
	BinaryStream out(e.data, e.count);
	uint64 sessionId = 0;
	int32 packetCount = 0;
	int32 msgId = 0;
	CHECK_RETURN(out >> sessionId, 0);
	CHECK_RETURN(out >> packetCount, 0);
	int32 rpos = out.getRPostion();
	CHECK_RETURN(out >> msgId, 0);
	out.setRPostion(rpos);

	Session* session = INSTANCE(SessionManager).getSession(sessionId);
	do
	{
		if (session == NULL)
			break;

#ifdef _DEBUG
		DEBUG_DEBUG(LogSystem::csl_color_yellow, "session:%0.16llx s to c %s size:%d", session->getSessionId(), INSTANCE(PacketManager).GetName(msgId).c_str(), packetCount);
#endif // DEBUG

		if (msgId == ID_NetSessionLeaveNotify)
			break;

		/*
		*********** 加密 ***********
		*/
		session->sendBuffer((int8*)e.data + rpos, packetCount);
		/*
		*********** 加密 ***********
		*/
		return 0;

	} while (false);

	if (!session)
		return 0;


	NetSessionLeaveNotify nfy;
	sendPacketToWorld(nfy, session);
	INSTANCE(Network).closesocket(session->getSocketId());
	return 0;
}

int Application::onWorldExit(SocketEvent& e)
{
	worldServer = NULL;
	std::map<uint64, Session*>  mapSession = INSTANCE(SessionManager).getMapSession();
	for (auto& itr : mapSession)
	{
		Session* session = itr.second;
		INSTANCE(Network).closesocket(session->getSocketId());
	}

	LOG_DEBUG(LogSystem::csl_color_red, "world exit");
	return 0;
}

int Application::onWorldException(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_red, "world not exist");
	worldServer = NULL;
	return 0;
}
