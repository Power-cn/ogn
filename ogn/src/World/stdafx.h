#pragma once
#include "Shared.hpp"
#include "ConfigManager.h"
#include "Property.h"
#include "Entity.h"
#include "Player.h"
#include "Npc.h"

#include "Session.h"
#include "SessionManager.h"


#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "libevent_core.lib")
#pragma comment(lib, "libevent_extras.lib")

#ifdef _DEBUG

#pragma comment(lib, "Shared_d.lib")
#pragma comment(lib, "lua_d.lib")
#pragma comment(lib, "luabind_d.lib")
#pragma comment(lib, "hiredis_d.lib")

#else

#pragma comment(lib, "Shared.lib")
#pragma comment(lib, "lua.lib")
#pragma comment(lib, "luabind.lib")
#pragma comment(lib, "hiredis.lib")

#endif // DEBUG

#endif

#include "MsgCode.h"
#include "LuaEngine.h"
#include "Channel.h"
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

#include "protocols/protocols.h"
#include "Utils.h"
#include "DBHandler.h"
#include "PlayerHandler.h"
#include "SessionHandler.h"
#include "./Handler/WarHandler.h"
#include "RedisProxy.h"
#include "json/json.h"

#include "Application.h"

#define sApp INSTANCE(Application)
#define sPacketMgr INSTANCE(PacketManager)
#define sCfgMgr INSTANCE(ConfigManager)
#define sSessionMgr INSTANCE(SessionManager)
#define sRedisProxy INSTANCE(RedisProxy)

#define sWorld (*GetModule(WorldModule))
