#include "stdafx.h"

class LUATest
{
public:
	int32 a;
	int32 b;

	void TestFunc(std::string a)
	{
		LOG_ERROR(a.c_str());
	}
};

void LuaScript::bindScript()
{
	luabind::module(mLuaState)
		[
			luabind::def("luaObject", &luaObject),
			luabind::def("luaEntity", &luaEntity),
			luabind::def("luaPlayer", &luaPlayer),
			luabind::def("luaPlayerToEntity", &luaPlayerToEntity),
			luabind::def("luaNpcToEntity", &luaNpcToEntity),
			luabind::def("luaPropertyHelper", &luaPropertyHelper),
			luabind::def("luaWorld", &luaWorld),
			luabind::def("luaRoom", &luaRoom)
			//luabind::def("TestFunc", &TestFunc)
		];

	luabind::module(mLuaState)
		[
			luabind::class_<BinaryStream>("BinaryStream")
			,
			luabind::class_<Guid>("Guid")
			,
			luabind::class_<Object>("Object")
			.def("Serialize", &Object::operator<<)
			.def("Deserialize", &Object::operator>>)
			,
			luabind::class_<Entity>("Entity")
			.def("getGuid", &Player::getGuid)
			,
			luabind::class_<Player>("Player")
			.def("getGuid", &Player::getGuid)
			.def("getUserId", &Player::getUserId)
			.def("getName", &Player::getName)
			.def("onCreate", &Player::onCreate)
			,

			luabind::class_<Npc>("Npc"),
			luabind::class_<PropertyHelper>("PropertyHelper")
			.def("setMaxHp", &PropertyHelper::setMaxHp)
			,
			luabind::class_<WorldModule>("WorldModule")
			.def("sendPacketToAll", &WorldModule::sendPacketToAll)
			.def("sendPacketToTarget", &WorldModule::sendPacketToTarget)
			.def("sendPacketToMsg", &WorldModule::sendPacketToMsg)
			.def("getEntityByName", &WorldModule::getEntityByName)
			.def("getPlayerByName", &WorldModule::getPlayerByName)
			.def("getPlayerToUserId", &WorldModule::getPlayerToUserId)
			,

			luabind::class_<RoomPlayer>("RoomPlayer")
			.def_readonly("mPlayer", &RoomPlayer::mPlayer)
			.def_readonly("mUserId", &RoomPlayer::mUserId)
			.def_readonly("mInsId", &RoomPlayer::mInsId)
			.def_readonly("mName", &RoomPlayer::mName)
			.def_readonly("mState", &RoomPlayer::mState)
			,

			luabind::class_<Room>("Room")
			.def("sendPacketToAll", &Room::sendPacketToAll)
			.def("RemovePlayer", &Room::RemovePlayer)
			.def("SetMaster", &Room::SetMaster)
			.def("SetPassword", &Room::SetPassword)
			.def("SetName", &Room::SetName)

			.def("Serialize", &Room::operator >>)
			.def("DoLeave",(bool(Player::*)(Player*)) &Room::DoLeave)
			.def("DoLeave", (bool(Player::*)(uint32)) &Room::DoLeave)
			.def("IsFull", &Room::IsFull)

			.def("GetInsId", &Room::GetInsId)
			.def("GetMaxCount", &Room::GetMaxCount)
			.def("GetRoomPlayerCount", &Room::GetRoomPlayerCount)

			.def("DoEnter", &Room::DoEnter)
			.def("FindPlayer", &Room::FindPlayer)
			.def("AddPlayer", &Room::AddPlayer)
			.def("GetRoomPlayer", &Room::GetRoomPlayer)
			.def("GetMaster", &Room::GetMaster)
			,

			luabind::class_<RoomModule>("RoomModule")
			.def("DoCreateRoom", &RoomModule::DoCreateRoom)
			.def("DoEnterRoom", &RoomModule::DoEnterRoom)
			.def("DoLeaveRoom", &RoomModule::DoLeaveRoom)
			.def("DoChangeRoomMaster", &RoomModule::DoChangeRoomMaster)
			.def("DoRoomList", &RoomModule::DoRoomList)
			,

			luabind::class_<LUATest>("LUATest")
			.def_readonly("a", &LUATest::a)
			.def("TestFunc", &LUATest::TestFunc)
		];
}
//template < class ValueWrapper>
//inline int obj_size(ValueWrapper const& value)
//{
//	lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(value);
//	value_wrapper_traits<ValueWrapper>::unwrap(interpreter, value);
//	detail::stack_pop pop(interpreter, 1);
//	return lua_objlen(interpreter, -1);
//}

void LuaEngine::reloadScript()
{
	clearScript();

	loadScript("../config/cfg/script/global.lua");
	loadScript("../config/cfg/script/player.lua");
	loadScript("../config/cfg/script/gm.lua");
	loadScript("../config/cfg/script/team.lua");

	//LuaScript* luaScript = INSTANCE(LuaEngine).getScript("global");
	//lua_State* luaState = luaScript->getLuaState();
	//luabind::object luaTable = luabind::globals(luaState)["t"];
	////std::string str = luabind::object_cast<std::string>(luaTable[1]);
	//uint32 size = luabind::tablecount(luaTable);
	//std::string desc = luabind::object_cast<std::string>(luabind::gettable(luaTable, "name"));

	LOG_DEBUG(LogSystem::csl_color_green, "lua script load finish");
}
