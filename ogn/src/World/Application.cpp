#include "stdafx.h"

Application::Application()
{
	mTime = 0.0;
	mDelay = 0.0;
	mFPSTimer = 0.0;
	mFPS = 0;

}
Application::~Application()
{
	Destroy();
}

int32 callback(int32 ctrlType)
{
	if (ctrlType == 2)
	{
		CmdEvent e;
		INSTANCE(Application).onClose(e);
	}
	return 0;
}

bool Application::Initialize()
{
	std::string str = "11212#aaaa#111";
	std::vector<std::string> dest;
	Shared::split(str, dest, "#");

	std::function<void(int, int)> f1;

	int32 i = 0;

	//INSTANCE(RedisProxy).sendCommand("auth root123456");
	//INSTANCE(RedisProxy).sendCommand("lpush %s %s", "uid", "1");
	//INSTANCE(RedisProxy).sendCommand("lpush %s %s", "uid", "2");
	//INSTANCE(RedisProxy).sendCommand("lpush %s %s", "uid", "4");
	//INSTANCE(RedisProxy).sendCommand("lpush %s %s", "uid", "5");
	//INSTANCE(RedisProxy).sendCommand("lpush %s %s", "uid", "6");
	//INSTANCE(RedisProxy).sendCommand("lpush %s %s", "uid", "7");
	//INSTANCE(RedisProxy).sendCommand("lpush %s %s", "uid", "8");
	//INSTANCE(RedisProxy).sendCommand("lpush %s %s", "uid", "9");
	//
	//INSTANCE(RedisProxy).set("name_1", "123123123123");
	//INSTANCE(RedisProxy).set("level_1", "10");

	//INSTANCE(RedisProxy).sendCommand("append player name");
	//INSTANCE(RedisProxy).sendCommand("append player name1");
	//INSTANCE(RedisProxy).sendCommand("save");

	Shared::srand(DateTime::Now());

	INSTANCE(CmdDispatcher);
	INSTANCE(CmdDispatcher).addEventListener("ref", (EventCallback)&Application::onRefresh, this);
	INSTANCE(CmdDispatcher).addEventListener("close", (EventCallback)&Application::onClose, this);

	INSTANCE(ConfigManager);
	INSTANCE(SessionManager);
	INSTANCE(LuaEngine);

	INSTANCE(ConfigManager).reloadConfig();
	INSTANCE(LuaEngine).reloadScript();

	int32 maxplayer = 0;
	maxplayer = LuaEngine::GetInt32("global", "maxplayer");

	maxplayer = 100;
	LuaEngine::SetInt32("global", "maxplayer", maxplayer);

	maxplayer = LuaEngine::GetInt32("global", "maxplayer");

	LuaEngine::SetInt32("global", "EC_WORLD", EnumChannel::EC_WORLD);
	LuaEngine::SetInt32("global", "EC_MAP", EnumChannel::EC_MAP);
	LuaEngine::SetInt32("global", "EC_VIEW", EnumChannel::EC_VIEW);
	LuaEngine::SetInt32("global", "EC_TEAM", EnumChannel::EC_TEAM);
	LuaEngine::SetInt32("global", "EC_TARGET", EnumChannel::EC_TARGET);


	LuaEngine::executeScript("team", "onEnterTeam", "sdfsdfsdfdsf");
	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("World");
	worldServer = INSTANCE(Network).listen(cf.Port);
	IF_FALSE(!worldServer)
		return false;

	ServerConfig& cfg = sCfgMgr.getConfig("Redis");
	IF_FALSE(!sRedisProxy.AsyncConnect(cfg.Host, cfg.Port))
		return false;
	sRedisProxy.addEventListener(RedisEvent::CONNECT, (EventCallback)&Application::RedisConnect, this);




	INSTANCE(SessionHandler);
	INSTANCE(PlayerHandler);

	LOG_DEBUG(LogSystem::csl_color_green, "World listen Port:%d success", cf.Port);

	worldServer->addEventListener(SocketEvent::ACCEPT, (EventCallback)&Application::onWorldAccept, this);
	worldServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onWorldRecv, this);
	worldServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onWorldExit, this);

	addModule(new WorldModule);
	addModule(new MapModule);
	addModule(new TeamModule);
	addModule(new WarModule);
	for (auto itr : mMapModule)
		itr.second->Initialize();

	mTime = DateTime::GetNowAppUS();
	mTotalTime = 0.0;

	Shared::setConsoleCtrlHandler(callback, 1);

	addSessionMessage(ID_NetSessionLeaveNotify);
	addSessionMessage(ID_NetSessionEnterNotify);
	addSessionMessage(ID_NetLoginReq);
	addSessionMessage(ID_NetPingNotify);
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
		sprintf_s(szBuffer, 256, "FPS:%d Player:%d", mFPS, GetModule(WorldModule)->getPlayerCount());
		Shared::setConsoleTitle(szBuffer);
		//LOG_INFO("FPS:%d", mFPS);
		mFPS = 0;
		mFPSTimer = 0.0;
	}

	sRedisProxy.loop();
	INSTANCE(Network).update(0.f, 0.f);
	INSTANCE(CmdDispatcher).update((float32)mTime, (float32)mDelay);
	for (auto itr : mMapModule)
		itr.second->Update((float32)mTime, (float32)mDelay);

	if (!dbServer)
	{
		ServerConfig& cf = INSTANCE(ConfigManager).getConfig("DB");
		dbServer = INSTANCE(Network).connect(cf.Host.c_str(), cf.Port);

		if (dbServer)
		{
			LOG_DEBUG(LogSystem::csl_color_green, "connect DB Port:%d ...", cf.Port);
			dbServer->addEventListener(SocketEvent::CONNECT, (EventCallback)&Application::onDBConnect, this);
			dbServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onDBRecv, this);
			dbServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onDBExit, this);
			dbServer->addEventListener(SocketEvent::EXCEPTION, (EventCallback)&Application::onDBException, this);
		}
		else
		{
			LOG_DEBUG(LogSystem::csl_color_green, "connect DB Port:%d fail...", cf.Port);
		}
	}
	return true;
}

bool Application::Destroy()
{
	for (auto itr : mMapModule)
		itr.second->Destroy();

	return true;
}

Module* Application::getModule(const std::string& name)
{
	auto itr = mMapModule.find(name);
	if (itr != mMapModule.end())
		return itr->second;
	return NULL;
}

Module* Application::addModule(Module* module)
{
	auto itr = mMapModule.find(module->getClassName());
	if (itr != mMapModule.end())
		return NULL;

	mMapModule.insert(std::make_pair(module->getClassName(), module));
	return module;
}

void Application::removeModule(const std::string& name, bool free /* = true */)
{
	auto itr = mMapModule.find(name);
	if (itr != mMapModule.end())
	{
		if (free)
			delete itr->second;
		mMapModule.erase(name);
	}
}

bool Application::onEnterWorld(Player* player, Dictionary& dict)
{
	for (auto itr : mMapModule)
		itr.second->onEnterWorld(player, dict);
	return true;
}

bool Application::onLeaveWorld(Player* player, Dictionary& dict)
{
	for (auto itr : mMapModule)
		itr.second->onLeaveWorld(player, dict);
	return true;
}

bool Application::onLoad(Player* player, Dictionary& bytes)
{
	player->onLoad(bytes);
	for (auto itr : mMapModule)
		itr.second->onLoad(player, bytes);

	for (auto itr : mMapModule)
		itr.second->onLoadEnd(player, bytes);
	return true;
}

bool Application::onSave(Player* player, Dictionary& bytes)
{
	for (auto itr : mMapModule)
		itr.second->onSave(player, bytes);
	player->onSave(bytes);

	for (auto itr : mMapModule)
		itr.second->onSaveEnd(player, bytes);
	return true;
}

void Application::sendPacketToDB(Packet& packet, Session* session)
{
	if (dbServer == NULL || dbServer->getSocket() == NULL)
		return;
	session->sendPacketToTarget(packet, dbServer->getSocket());
}

void Application::doSessionLeaveWorld(Session* session)
{
	LOG_INFO("sessionId %0.16llx leave world", session->getSessionId());
	Player* plr = session->getPlayer();
	if (plr)
	{
		plr->SetOfflineTimer(DateTime::getNow());
		Dictionary dict;
		doPlayerSave(plr, dict);

		onLeaveWorld(plr, dict);
		GetModule(WorldModule)->removePlayer(plr->getAccountId());
	}

	NetSessionLeaveNotify nfy;
	
	sendPacketToDB(nfy, session);
	session->sendPacketToWorld(nfy);

	INSTANCE(SessionManager).removeSessionsBySocket(session->getSocketId(), session);
	INSTANCE(SessionManager).removeSession(session->getSessionId());
}

void Application::doPlayerSave(Player* plr, Dictionary& bytes)
{
	Session* session = plr->getSession();
	if (session == NULL)
		return;
	onSave(plr, bytes);
	BinaryStream stream;
	stream.setResize(true);
	stream << bytes;

	NetQueryRoleRes res;
	DBRoleInfo info;
	info.accountId = plr->getAccountId();
	info.id = plr->getUserId();
	info.property.WriteBytes(stream.getPtr(), stream.getWPostion());
	res.accountId = plr->getAccountId();
	res.roleInfos.push_back(info);
	sendPacketToDB(res, session);
}

int Application::onWorldAccept(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_green, "gate accept success");
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

		if ((out >> (*pack)) == false)
		{
			LOG_ERROR("packet errer");
			INSTANCE(PacketManager).Free(pack);
			break;
		}

		if (checkSessionMessage(msgId))
		{
			if (worldServer->dispatch(pack->getMsgId(), session, pack) == 0)
				LOG_WARN("[%s] not register func", INSTANCE(PacketManager).GetName(msgId).c_str());
		}
		else
		{
			Player* plr = session->getPlayer();
			if (plr) {
				if (worldServer->dispatch(pack->getMsgId(), plr, pack) == 0)
					LOG_WARN("[%s] not register func", INSTANCE(PacketManager).GetName(msgId).c_str());
			}
			else {
				LOG_WARN("[%s] not register func", INSTANCE(PacketManager).GetName(msgId).c_str());
			}
		}
		INSTANCE(PacketManager).Free(pack);
		return 0;
	} while (false);

	if (session == NULL)
		return 0;

	doSessionLeaveWorld(session);
	return 0;
}

int Application::onWorldExit(SocketEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_red, "gate exit");

	std::set<Session*>* sset = INSTANCE(SessionManager).getSessionsBySocket(e.socket->getSocketId());
	if (!sset)
		return 0;

	for (auto itr : (*sset))
		INSTANCE(SessionManager).removeSession(itr->getSessionId());

	sset->clear();
	return 0;
}

int Application::onDBConnect(SocketEvent & e)
{
	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("DB");
	LOG_DEBUG(LogSystem::csl_color_green, "connect DB Port:%d success", cf.Port);
	INSTANCE(DBHandler).doRegister();
	return 0;
}

int Application::onDBRecv(SocketEvent & e)
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
	if (!session) return 0;

	Player* player = session->getPlayer();

	if (!player && msgId != ID_NetLoginRes) return 0;

	do
	{
		Packet* pack = INSTANCE(PacketManager).Alloc(msgId);
		if (pack == NULL) break;

		if ((out >> (*pack)) == false)
		{
			LOG_ERROR("pack->deSerialize(out)");
			INSTANCE(PacketManager).Free(pack);
			break;
		}

		if (dbServer->dispatch(pack->getMsgId(), session, pack) == 0)
			LOG_WARN("[%d] not register func");

		INSTANCE(PacketManager).Free(pack);
		return 0;
	} while (false);

	if (session == NULL)
		return 0;

	doSessionLeaveWorld(session);
	return 0;
}

int Application::onDBExit(SocketEvent & e)
{
	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("DB");
	LOG_DEBUG(LogSystem::csl_color_red, "DB Port:%d exit", cf.Port);
	dbServer = NULL;
	return 0;
}

int Application::onDBException(SocketEvent & e)
{
	dbServer = NULL;
	LOG_ERROR(__FUNCTION__);
	return 0;
}

bool Application::checkSessionMessage(uint32 msgId)
{
	auto itr = mSetSessionMessage.find(msgId);
	if (itr != mSetSessionMessage.end())
		return true;
	return false;
}

void Application::addSessionMessage(uint32 msgId)
{
	mSetSessionMessage.insert(msgId);
}

int32 Application::onClose(CmdEvent& e)
{

	std::map<uint32, Player*>& mapPlayer = GetModule(WorldModule)->getMapPlayer();
	while (mapPlayer.size() > 0)
	{
		auto itr = mapPlayer.begin();
		if (itr == mapPlayer.end())
			continue;

		Player* plr = itr->second;
		doSessionLeaveWorld(plr->getSession());
	}

	return 0;
}

int32 Application::RedisConnect(RedisEvent& e)
{
	ServerConfig& cfg = sCfgMgr.getConfig("Redis");

	char szBuffer[64] = { 0 };
	sprintf_s(szBuffer, 64, "auth %s", cfg.Password.c_str());
	sRedisProxy.sendCmd(szBuffer, (EventCallback)&Application::RedisAuth, this);

	return 0;
}

int32 Application::RedisAuth(RedisEvent& e)
{
	LOG_DEBUG(LogSystem::csl_color_green, "redis auth success");
	sRedisProxy.sendCmd("zrevrank test w2", (EventCallback)&Application::RedisCallback1, this);
	sRedisProxy.sendCmd("zrange test 0 10 withscores", (EventCallback)&Application::RedisCallback1, this);
	sRedisProxy.sendCmd("ZCARD test", (EventCallback)&Application::RedisCallback1, this);
	return 0;
}

int32 Application::RedisCallback1(RedisEvent& e)
{
	return 0;
}

int32 Application::onRefresh(CmdEvent& e)
{
	INSTANCE(ConfigManager).reloadConfig();
	INSTANCE(LuaEngine).reloadScript();
	return 0;
}
