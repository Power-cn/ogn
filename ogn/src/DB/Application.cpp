#include "stdafx.h"

Application::Application():
dbServer(NULL)
{
	mRun = true;
}
Application::~Application()
{

}


bool StartRedisServer()
{
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE; //TRUE表示显示创建的进程的窗口  
	char szBuffer[MAX_PATH] = { 0 };
	sprintf_s(szBuffer, MAX_PATH, "%s %s", "redis-64.3.0.503\\redis-server.exe", "redis-64.3.0.503\\redis.windows.conf");
	if (!CreateProcessA(NULL, szBuffer, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		int error = GetLastError();
		LOG_ERROR("redis start error id %d", error);
		return false;
	}

	return true;
}

bool Application::Initialize()
{
	StartRedisServer();
	INSTANCE(ConfigManager);
	INSTANCE(SessionManager);
	INSTANCE(DBProxy);
	INSTANCE(ConfigManager).reloadConfig();

	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("DB");
	dbServer = INSTANCE(Network).listen(cf.Port);
	if (!dbServer) {
		LOG_ERROR("DB listen Port:%d fial", cf.Port);
		return false;
	}

	LOG_DEBUG(LogSystem::csl_color_green, "DB listen Port:%d success", cf.Port);

	dbServer->addEventListener(SocketEvent::ACCEPT, (EventCallback)&Application::onDBAccept, this);
	dbServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onDBRecv, this);
	dbServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onDBExit, this);

	INSTANCE(SessionHandler);

	ServerConfig& cfMysql = INSTANCE(ConfigManager).getConfig("Mysql");
	dbConnector = INSTANCE(DBProxy).connect(
		cfMysql.Host.c_str(),
		cfMysql.User.c_str(),
		cfMysql.Password.c_str(),
		cfMysql.DbName.c_str(),
		cfMysql.Port);

	if (dbConnector)
		LOG_DEBUG(LogSystem::csl_color_green, "Database connect %s %s %d success", cfMysql.DbName.c_str(), cfMysql.Host.c_str(), cfMysql.Port);
	else
	{
		LOG_ERROR("Database connect fail...");
		return false;
	}

	sRedisProxy.addEventListener(RedisEvent::CONNECT, (EventCallback)&Application::RedisConnect, this);
	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig("Redis");
	IF_FALSE(!sRedisProxy.AsyncConnect(cfg.Host, cfg.Port))
		return false;
	return true;
}

bool Application::Update()
{
	INSTANCE(Network).update(0.f, 0.f);
	INSTANCE(DBProxy).update(0.f, 0.f);
	sRedisProxy.loop();
	return true;
}

bool Application::Destroy()
{
	sRedisProxy.Destroy();
	return true;
}

int32 Application::onDBAccept(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_green, "world server accept success");
	return 0;
}

int32 Application::onDBRecv(SocketEvent& e)
{
	BinaryStream out(e.data, e.count);
	uint64 sessionId = 0;
	int32 packetCount = 0;
	int32 msgId = 0;
	CHECK_RETURN(out >> sessionId, 0);
	CHECK_RETURN(out >> packetCount, 0);
	int32 rpos = out.rpos();
	CHECK_RETURN(out >> msgId, 0);
	out.rpos(rpos);

	Session* session = INSTANCE(SessionManager).getSession(sessionId);
	do 
	{
		if (session == NULL && msgId == ID_NetSessionEnterNotify)
		{
			session = INSTANCE(SessionManager).newSession(e.socket, sessionId);
			INSTANCE(SessionManager).addSessionsBySocket(e.socket->getSocketId(), session);
		}

		if (session == NULL)
			break;

		Packet* pack = sPacketMgr.Alloc(msgId);
		if (pack == NULL) break;
			
		if (out >> (*pack) == false)
		{
			LOG_ERROR("pack->deSerialize(out)");
			sPacketMgr.Free(pack);
			break;
		}
		if (msgId == ID_NetSessionEnterNotify || msgId == ID_NetSessionLeaveNotify || msgId == ID_NetLoginReq)
			dbServer->dispatch(pack->getMsgId(), session, pack);
		else if (session->getPlayer())
			dbServer->dispatch(pack->getMsgId(), session->getPlayer(), pack);
		else {
			sPacketMgr.Free(pack);
			break;
		}
		sPacketMgr.Free(pack);
		return 0;

	} while (false);

	if (session == NULL)
		return 0;
	NetSessionLeaveNotify nfy;
	session->sendPacketToWorld(nfy);
	LOG_DEBUG(LogSystem::csl_color_red, "ssnId %0.16llx packet error leave world", session->getSessionId());

	INSTANCE(SessionManager).removeSessionsBySocket(e.socket->getSocketId(), session);
	return 0;
}

int32 Application::onDBExit(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_red, "world server exit db");
	
	std::set<Session*>* setSession = INSTANCE(SessionManager).getSessionsBySocket(e.socket->getSocketId());
	if (setSession)
	{
		std::set<Session*> copySetSession = (*setSession);
		for (auto session : copySetSession)
		{
			INSTANCE(SessionManager).removeSessionsBySocket(e.socket->getSocketId(), session);
		}
	}
	return 0;
}

int32 Application::RedisConnect(RedisEvent& e)
{
	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig("Redis");
	char szBuffer[64] = { 0 };
	sprintf_s(szBuffer, 64, "auth %s", cfg.Password.c_str());
	sRedisProxy.sendCmd(szBuffer, (EventCallback)&Application::RedisAuth, this);
	return 0;
}

int32 Application::RedisAuth(RedisEvent& e)
{
	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig("Redis");
	LOG_DEBUG(LogSystem::csl_color_green, "redis auth success  %s %d", cfg.Host.c_str(), cfg.Port);
	return 0;
}