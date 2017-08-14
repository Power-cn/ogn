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
	INSTANCE(CmdDispatcher).addEventListener("reflua", (EventCallback)&Application::onRefreshLua, this);
	INSTANCE(CmdDispatcher).addEventListener("close", (EventCallback)&Application::onClose, this);

	INSTANCE(CmdDispatcher).addEventListener("addfrd", (EventCallback)&Application::onAddFrd, this);

	INSTANCE(CmdDispatcher).addEventListener("test", (EventCallback)&Application::onTest, this);


	INSTANCE(ConfigManager);
	INSTANCE(SessionManager);
	INSTANCE(LuaEngine);

	INSTANCE(ConfigManager).reloadConfig();
	INSTANCE(LuaEngine).reloadScript();

	ServerConfig& cf = INSTANCE(ConfigManager).getConfig("World");
	worldServer = INSTANCE(Network).listen(cf.Port);
	IF_FALSE(!worldServer) {
		LOG_ERROR("World listen Port:%d fial", cf.Port);
		return false;
	}

	sRedisProxy.addEventListener(RedisEvent::CONNECT, (EventCallback)&Application::RedisConnect, this);

	INSTANCE(SessionHandler);
	INSTANCE(PlayerHandler);
	INSTANCE(WarHandler);
	INSTANCE(TeamHandler);
	INSTANCE(RoomHandler);

	LOG_DEBUG(LogSystem::csl_color_green, "World listen Port:%d success", cf.Port);

	worldServer->addEventListener(SocketEvent::ACCEPT, (EventCallback)&Application::onWorldAccept, this);
	worldServer->addEventListener(SocketEvent::RECV, (EventCallback)&Application::onWorldRecv, this);
	worldServer->addEventListener(SocketEvent::EXIT, (EventCallback)&Application::onWorldExit, this);

	addModule(new WorldModule);
	addModule(new MapModule);
	addModule(new TeamModule);
	addModule(new WarModule);
	addModule(new FriendsModule);
	addModule(new RoomModule);
	addModule(new GameModule);
	for (auto itr : mMapModule)
		itr.second->Initialize();

	mTime = DateTime::GetNowAppUS();
	mTotalTime = 0.0;

	Shared::setConsoleCtrlHandler(callback, 1);

	addSessionMessage(ID_NetSessionLeaveNotify);
	addSessionMessage(ID_NetSessionEnterNotify);
	addSessionMessage(ID_NetLoginReq);
	addSessionMessage(ID_NetPingNotify);


	OnInitialize();


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
		sprintf_s(szBuffer, 256, "World FPS:%d Player:%d", mFPS, sWorld.GetPlrCount());
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
	player->OnEnter();
	for (auto itr : mMapModule)
		itr.second->onEnterWorld(player, dict);
	return true;
}

bool Application::onLeaveWorld(Player* player, Dictionary& dict)
{
	for (auto itr : mMapModule)
		itr.second->onLeaveWorld(player, dict);
	player->OnLeave();
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
	player->onSaveBegin(bytes);
	for (auto itr : mMapModule)
		itr.second->onSave(player, bytes);
	player->onSave(bytes);
	player->onSaveEnd(bytes);
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
	LOG_INFO("ssnId %0.16llx leave world", session->getSessionId());
	Player* plr = session->getPlayer();
	if (plr) {
		doPlayerLeaveWorld(plr);
	}

	NetSessionLeaveNotify nfy;
	
	sendPacketToDB(nfy, session);
	session->sendPacketToWorld(nfy);

	sSsnMgr.removeSessionsBySocket(session->getSocketId(), session);
	sSsnMgr.removeSession(session->getSessionId());
}

void Application::doPlayerLeaveWorld(Player* aPlr)
{
	aPlr->SetOnline(false);
	aPlr->SetOfflineTimer(DateTime::Now());
	Dictionary dict;
	doPlayerSave(aPlr, dict);
	onLeaveWorld(aPlr, dict);
	aPlr->unbindSession();
}

void Application::doPlayerSave(Player* plr, Dictionary& bytes)
{
	Session* session = plr->getSession();
	if (session == NULL)
		return;
	onSave(plr, bytes);
	BinaryStream stream(1024);
	stream << bytes;

	NetPlayerSaveNotify nfy;
	DBRoleInfo info;
	info.accountId = plr->getAccId();
	info.id = plr->getUserId();
	info.property.write(stream.datas(), stream.wpos());
	nfy.accountId = plr->getAccId();
	nfy.roleInfos.push_back(info);
	sendPacketToDB(nfy, session);
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
	int32 rpos = out.rpos();
	CHECK_RETURN(out >> msgId, 0);
	out.rpos(rpos);
	Session* session = sSsnMgr.getSession(sessionId);
	do 
	{

	if (session == NULL && msgId == ID_NetSessionEnterNotify)
	{
		session = sSsnMgr.createSession(e.socket, sessionId);
		if (session == NULL)
			return 0;

		sSsnMgr.addSessionsBySocket(e.socket->getSocketId(), session);
	}

	if (session == NULL)
		break;

	Packet* pack = sPacketMgr.Alloc(msgId);
	if (pack == NULL) break;

	if ((out >> (*pack)) == false)
	{
		LOG_ERROR("msg:%s packet errer", sPacketMgr.GetName(msgId).c_str());
		sPacketMgr.Free(pack);
		break;
	}

	if (checkSessionMessage(msgId))
	{
		if (worldServer->dispatch(pack->getMsgId(), session, pack) == 0)
			LOG_WARN("[%s] not register func", sPacketMgr.GetName(msgId).c_str());
	}
	else
	{
		Player* plr = session->getPlayer();
		if (plr) {
			if (worldServer->dispatch(pack->getMsgId(), plr, pack) == 0)
				LOG_WARN("[%s] not register func", sPacketMgr.GetName(msgId).c_str());
		}
		else {
			LOG_WARN("[%s] no login not pro packet", sPacketMgr.GetName(msgId).c_str());
		}
	}
	sPacketMgr.Free(pack);
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
	int32 rpos = out.rpos();
	CHECK_RETURN(out >> msgId, 0);
	out.rpos(rpos);
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
	std::map<uint32, Player*> mapPlayer = GetModule(WorldModule)->getMapPlayer();
	for (auto& itr : mapPlayer)
	{
		Player* plr = itr.second;
		Session* session = plr->getSession();

		NetSessionLeaveNotify nfy;
		if (session == NULL) continue;
		session->sendPacketToWorld(nfy);
		//doSessionLeaveWorld(plr->getSession());
	}
	return 0;
}

int32 Application::onAddFrd(CmdEvent& e)
{
	Player* tar = sWorld.FindPlrByName(e.cmdExecute->params[0]);
	Player* frd = sWorld.FindPlrByName(e.cmdExecute->params[1]);
	if (tar && frd)
	{
		sFriends.MutualBindFriend(tar, frd);
	}

	return 0;
}

int32 Application::onTest(CmdEvent& e)
{
	int ret1 = LuaEngine::Call(sScriptCard, "testfun", "√∑ª®A,√∑ª®K,√∑ª®Q", "∫ÏÃ“A,∫ÏÃ“K,∫ÏÃ“Q");
	//int ret2 = LuaEngine::Call(sScriptCard, "CompareStringCard", "√∑ª®A,√∑ª®K,√∑ª®Q", "∫ÏÃ“A,∫ÏÃ“K,∫ÏÃ“Q");
	//int ret2 = LuaEngine::Call(sScriptCard, "CompareStringCard", "√∑ª®2,√∑ª®K,√∑ª®Q", "∫ÏÃ“A,∫ÏÃ“K,∫ÏÃ“Q");
	//int ret2 = LuaEngine::Call(sScriptCard, "CompareStringCard", "√∑ª®A,√∑ª®K,√∑ª®Q", "∫ÏÃ“A,∫ÏÃ“K,∫ÏÃ“Q");
	//int ret2 = LuaEngine::Call(sScriptCard, "CompareStringCard", "√∑ª®A,√∑ª®K,√∑ª®Q", "∫ÏÃ“A,∫ÏÃ“K,∫ÏÃ“Q");
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
	ServerConfig& cfg = sCfgMgr.getConfig("Redis");

	LOG_DEBUG(LogSystem::csl_color_green, "redis auth success  %s %d", cfg.Host.c_str(), cfg.Port);
	//sRedisProxy.sendCmd("zrevrank test w2", (EventCallback)&Application::RedisCallback1, this);
	//sRedisProxy.sendCmd("zrange test 0 10 withscores", (EventCallback)&Application::RedisCallback1, this);
	//sRedisProxy.sendCmd("ZCARD test", (EventCallback)&Application::RedisCallback1, this);

	//int ret = LuaEngine::Call(NULL, sScriptPlayer, "OnEnter");
	
	//int testaaa = 10;
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

int32 Application::onRefreshLua(CmdEvent& e)
{
	INSTANCE(LuaEngine).reloadScript();
	return 0;
}

void Application::OnInitialize()
{
	ServerConfig& cfg = sCfgMgr.getConfig("Redis");
	IF_FALSE(!sRedisProxy.AsyncConnect(cfg.Host, cfg.Port))
		return ;
	LuaEngine::SetInt32("global", "EC_WORLD", EnumChannel::EC_WORLD);
	LuaEngine::SetInt32("global", "EC_MAP", EnumChannel::EC_MAP);
	LuaEngine::SetInt32("global", "EC_VIEW", EnumChannel::EC_VIEW);
	LuaEngine::SetInt32("global", "EC_TEAM", EnumChannel::EC_TEAM);
	LuaEngine::SetInt32("global", "EC_ROOM", EnumChannel::EC_ROOM);
	LuaEngine::SetInt32("global", "EC_TARGET", EnumChannel::EC_TARGET);
	int32 testint = 0;
	testint = LuaEngine::GetInt32("global", "EC_WORLD");
	testint = LuaEngine::GetInt32("global", "EC_MAP");
	testint = LuaEngine::GetInt32("global", "EC_VIEW");
	testint = LuaEngine::GetInt32("global", "EC_TEAM");
	testint = LuaEngine::GetInt32("global", "EC_ROOM");
	testint = LuaEngine::GetInt32("global", "EC_TARGET");
}
