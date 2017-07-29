#pragma once
#include "Shared.hpp"
//////////////////////////////////////////////////////////////////////////
#include <json/json.h>
#include <lua.hpp>
#include <luabind/luabind.hpp>
//////////////////////////////////////////////////////////////////////////

#include "protocols/protocols.h"
#include "Channel.h"
#include "ConfigManager.h"
#include "Property.h"
#include "Entity.h"
#include "Player.h"
#include "Npc.h"

#include "Session.h"
#include "SessionManager.h"


#ifndef LINUX


#ifdef WIN64

#ifdef _DEBUG


#pragma comment(lib, "hiredis64_d.lib")
#pragma comment(lib, "Shared64_d.lib")
#pragma comment(lib, "lua64_d.lib")
#pragma comment(lib, "luabind64_d.lib")


#else

#pragma comment(lib, "hiredis64.lib")
#pragma comment(lib, "Shared64.lib")
#pragma comment(lib, "lua64.lib")
#pragma comment(lib, "luabind64.lib")

#endif // DEBUG

#else

#ifdef _DEBUG

#pragma comment(lib, "hiredis_d.lib")
#pragma comment(lib, "Shared_d.lib")
#pragma comment(lib, "lua_d.lib")
#pragma comment(lib, "luabind_d.lib")
#pragma comment(lib, "python/python36_d.lib")
#else

#pragma comment(lib, "hiredis.lib")
#pragma comment(lib, "Shared.lib")
#pragma comment(lib, "lua.lib")
#pragma comment(lib, "luabind.lib")
#pragma comment(lib, "python/python36.lib")
#endif // DEBUG

#endif

#endif // !LINUX



#include "MsgCode.h"
#include "RedisDef.h"
#include "LuaEngine.h"
#include "LuaScriptEngine.h"

#include "Map.h"
#include "Team.h"
#include "Item.h"
#include "Backpack.h"

#include "Module.h"
#include "WorldModule.h"
#include "MapModule.h"
#include "TeamModule.h"
#include "ItemModule.h"
#include "BackpackModule.h"

#include "BackpackModule.h"
#include "ItemModule.h"
#include "./Module/WarModule/Skill.h"
#include "./Module/WarModule/EntityStatus.h"
#include "./Module/WarModule/War.h"
#include "./Module/WarModule/WarModule.h"
#include "./Module/FriendsModule/Friends.h"
#include "./Module/FriendsModule/FriendsModule.h"

#include "./Module/RoomModule/RoomHandler.h"
#include "./Module/RoomModule/Room.h"
#include "./Module/RoomModule/RoomModule.h"

#include "Utils.h"
#include "DBHandler.h"
#include "PlayerHandler.h"
#include "SessionHandler.h"
#include "./Handler/WarHandler.h"
#include "RedisProxy.h"

#include "Application.h"


#define sApp INSTANCE(Application)
#define sPacketMgr INSTANCE(PacketManager)
#define sCfgMgr INSTANCE(ConfigManager)
#define sSsnMgr INSTANCE(SessionManager)
#define sRedisProxy INSTANCE(RedisProxy)

#define sWorld (*GetModule(WorldModule))
#define sTeam (*GetModule(TeamModule))
#define sFriends (*GetModule(FriendsModule))
#define sRoom (*GetModule(RoomModule))

#define RegDBEvent(id, cb, thisObject) RegisterEventProcess(GetDBServer(), id, cb, thisObject)
#define RegWorldEvent(id, cb, thisObject) RegisterEventProcess((GetWorldServer()), id, cb, thisObject)