#include "stdafx.h"

Application::Application():
dbServer(NULL)
{

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
	if (!dbServer)
		return false;

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
		LOG_DEBUG(LogSystem::csl_color_green, "Database connect %s %d success", cfMysql.DbName.c_str(), cfMysql.Port);
	else
	{
		LOG_ERROR("Database connect fail...");
		return false;
	}

	//DBAccount account;
	//int8* err = dbConnector->doInsert(account, "user");
	//if (err)
	//	LOG_ERROR(err);
	//uint32 queryCount = 0;
	//dbConnector->doQuery(account, &account, &queryCount, "user", "");
	return true;
}

bool Application::Update()
{
	INSTANCE(Network).update(0.f, 0.f);
	INSTANCE(DBProxy).update(0.f, 0.f);
	return true;
}

bool Application::Destroy()
{
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
	int32 rpos = out.getRPostion();
	CHECK_RETURN(out >> msgId, 0);
	out.setRPostion(rpos);

	Session* session = INSTANCE(SessionManager).getSession(sessionId);
	do 
	{
		if (session == NULL && msgId == ID_NetSessionEnterNotify)
		{
			session = INSTANCE(SessionManager).createSession(e.socket, sessionId);
			if (session == NULL)
				return 0;

			INSTANCE(SessionManager).addSessionsBySocket(e.socket->getSocketId(), session);
		}

		if (session == NULL)
			break;

		Packet* pack = INSTANCE(PacketManager).Alloc(msgId);
		if (pack == NULL) break;
			
		if (out >> (*pack) == false)
		{
			LOG_ERROR("pack->deSerialize(out)");
			INSTANCE(PacketManager).Free(pack);
			break;
		}

		dbServer->dispatch(pack->getMsgId(), session, pack);
		INSTANCE(PacketManager).Free(pack);
		return 0;

	} while (false);

	if (session == NULL)
		return 0;
	NetSessionLeaveNotify nfy;
	session->sendPacketToWorld(nfy);
	LOG_DEBUG(LogSystem::csl_color_red, "sessionId %0.16llx packet error leave world", session->getSessionId());

	INSTANCE(SessionManager).removeSessionsBySocket(e.socket->getSocketId(), session);
	INSTANCE(SessionManager).removeSession(session->getSessionId());
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
			INSTANCE(SessionManager).removeSession(session->getSessionId());
		}
	}

	return 0;
}
