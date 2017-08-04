#include "stdafx.h"

void LuaScript::bindScript()
{
	luabind::module(mLuaState)
		[
			luabind::def("luaObject", &luaObject),
			luabind::def("luaEntity", &luaEntity),
			luabind::def("luaPlayer", &luaPlayer),
			luabind::def("luaPlrToEnt", &luaPlrToEnt),
			luabind::def("luaNpcToEnt", &luaNpcToEnt),
			luabind::def("luaFindPlrByUserId", &luaFindPlrByUserId),
			luabind::def("luaToGameModle", &luaToGameModle),
			luabind::def("luaGoldenFlower", &luaGoldenFlower),
			luabind::def("luaPlrGoldenFlower", &luaPlrGoldenFlower),
			luabind::def("luaProperty", &luaProperty),
			luabind::def("luaWorld", &luaWorld),
			luabind::def("luaRoom", &luaRoom),
			luabind::def("luaGame", &luaGame),
			luabind::def("GetTestTable", &GetTestTable),
			//luabind::def("GetTestTable", &GetTestTable, mLuaState),
			luabind::def("luaCfg", &luaCfg)
			//luabind::def("TestFunc", &TestFunc)
		];
	luabind::module(mLuaState)
		[
			luabind::class_<EnumChannel>("A") 
			.enum_("EnumChannel")
			[
				luabind::value("EC_NONE", EC_NONE),
				luabind::value("EC_WORLD", EC_WORLD),
				luabind::value("EC_SYSTEM", EC_SYSTEM),
				luabind::value("EC_MAP", EC_MAP),
				luabind::value("EC_VIEW", EC_VIEW),
				luabind::value("EC_TEAM", EC_TEAM),
				luabind::value("EC_ROOM", EC_ROOM),
				luabind::value("EC_TARGET", EC_TARGET)
			]
			.enum_("")
				[
					luabind::value("EC_NONE", EC_NONE),
					luabind::value("EC_WORLD", EC_WORLD),
					luabind::value("EC_SYSTEM", EC_SYSTEM),
					luabind::value("EC_MAP", EC_MAP),
					luabind::value("EC_VIEW", EC_VIEW),
					luabind::value("EC_TEAM", EC_TEAM),
					luabind::value("EC_ROOM", EC_ROOM),
					luabind::value("EC_TARGET", EC_TARGET)
				]
		];

	luabind::module(mLuaState)
		[

		luabind::class_<BinaryStream>("BinaryStream")
		.def(luabind::constructor<>())
		//.scope[
		//	luabind::def("Instance", &BinaryStream::Instance)
		//]
		,
		luabind::class_<Guid>("Guid")
		,
		luabind::class_<Object>("Object")
		.def(luabind::constructor<>())
		.def("Serialize", &Object::operator<<)
		.def("Deserialize", &Object::operator >> )
		,
		luabind::class_<Entity, Object>("Entity")
		.def(luabind::constructor<>())
		.def("getGuid", &Player::getGuid)
		.def("sendPacketToMsg", (void (Player::*)(const std::string&))&Player::sendPacketToMsg)
		.def("sendPacketToMsg", (void (Player::*)(EnumChannel, const std::string&))&Player::sendPacketToMsg)
		,
		luabind::class_<Player, Entity>("Player")
		.def("getUserId", &Player::getUserId)
		.def("getName", &Player::getName)
		,

		luabind::class_<Npc, Entity>("Npc")
		,
		luabind::class_<PropertyHelper>("PropertyHelper")
		.def("setMaxHp", &PropertyHelper::setMaxHp)
		,

		luabind::class_<Module>("Module")
		,

		luabind::class_<WorldModule, Module>("WorldModule")
		.def("sendPacketToAll", &WorldModule::sendPacketToAll)
		.def("sendPacketToTarget", &WorldModule::sendPacketToTarget)
		.def("sendPacketToMsg", &WorldModule::sendPacketToMsg)

		.def("FindEntByName", &WorldModule::FindEntByName)
		.def("FindEntByGuid", &WorldModule::FindEntByGuid)

		.def("FindPlrByName", &WorldModule::FindPlrByName)
		.def("FindPlrByGuid", &WorldModule::FindPlrByGuid)
		.def("FindPlrByAccId", &WorldModule::FindPlrByAccId)
		.def("FindPlrByUserId", &WorldModule::FindPlrByUserId)

		.def("GetPlrCount", &WorldModule::GetPlrCount)
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

		.def("Serialize", &Room::operator >> )
		.def("DoLeave", (bool(Player::*)(Player*)) &Room::DoLeave)
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
		.def("GetPassword", &Room::GetPassword)
		.def("GetName", &Room::GetName)
		,

		luabind::class_<RoomModule, Module>("RoomModule")
		.def("DoCreateRoom", &RoomModule::DoCreateRoom)
		.def("DoEnterRoom", &RoomModule::DoEnterRoom)
		.def("DoLeaveRoom", (void (RoomModule::*)(Player*, uint32))&RoomModule::DoLeaveRoom)
		.def("DoLeaveRoom", (void (RoomModule::*)(uint32, uint32))&RoomModule::DoLeaveRoom)
		.def("DoChangeRoomMaster", &RoomModule::DoChangeRoomMaster)
		.def("DoRoomList", &RoomModule::DoRoomList)
		.def("DoRoomReady", &RoomModule::DoRoomReady)
		.def("DoRoomStartGame", &RoomModule::DoRoomStartGame)
		.def("DoRoomClose", &RoomModule::DoRoomClose)
		.def("DoAutoMatch", &RoomModule::DoAutoMatch)
		,

		luabind::class_<GameEntity>("GameEntity")
		.def_readonly("userId", &GameEntity::userId)
		.def("ToString", &GameEntity::ToString)
		.def("GetCardCount", &GameEntity::GetCardCount)
		.def("GetCard", &GameEntity::GetCard)
		.def("GetCards", &GameEntity::GetCards)
		,

		luabind::class_<GameModle>("GameModle")
		.def("GetInsId", &GameModle::GetInsId)
		.def("GetGameEnt", &GameModle::GetGameEnt)
		.def("FindGameEnt", &GameModle::FindGameEnt)
		.def("GetPlrInx", &GameModle::GetPlrInx)
		.def("ToString", &GameModle::ToString)
		.def("GetGameEntCount", &GameModle::GetGameEntCount)
		.def("GetRoomId", &GameModle::GetRoomId)
		,
	
		luabind::class_<GameGoldenFlower, GameModle>("GameGoldenFlower")
		//.def("GetInsId", &GameGoldenFlower::GetInsId)
		//.def("GetGameEnt", &GameGoldenFlower::GetGameEnt)
		//.def("FindGameEnt", &GameGoldenFlower::FindGameEnt)
		//.def("GetPlrInx", &GameGoldenFlower::GetPlrInx)
		//.def("ToString", &GameGoldenFlower::ToString)
		//.def("GetGameEntCount", &GameGoldenFlower::GetGameEntCount)
		,

		luabind::class_<GameModule, Module>("GameModule")
		.def("FindGameModle", &GameModule::FindGameModle)
		.def("FindPlrGameModle", &GameModule::FindPlrGameModle)
		.def("FindPlrGameEnt", &GameModule::FindPlrGameEnt)
		.def("DoStartGame", &GameModule::DoStartGame)
		,
	
		luabind::class_<CardJson>("CardJson")
		.def_readonly("ID", &CardJson::ID)
		.def_readonly("Number", &CardJson::Number)
		.def_readonly("Color", &CardJson::Color)
		.def_readonly("Name", &CardJson::Name)
		,

		luabind::class_<ConfigManager>("ConfigManager")
		.def("getCardJson", &ConfigManager::getCardJson)
		//.def("GetGameEnt", &GameGoldenFlower::GetGameEnt)
		//.def("FindGameEnt", &GameGoldenFlower::FindGameEnt)
		//.def("GetPlrInx", &GameGoldenFlower::GetPlrInx)
		//.def("ToString", &GameGoldenFlower::ToString)
		//.def("GetGameEntCount", &GameGoldenFlower::GetGameEntCount)
			
		
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

	//std::vector<std::string> files;
	//Shared::GetDirectoryFiles("../config/cfg/script", files);
	//for (std::string& path : files)
	//{
	//	loadScript(path);
	//}
	loadScript("../config/cfg/script/global.lua");
	loadScript("../config/cfg/script/player.lua");
	loadScript("../config/cfg/script/gm.lua");
	loadScript("../config/cfg/script/team.lua");
	loadScript("../config/cfg/script/room.lua");
	loadScript("../config/cfg/script/game.lua");

	//LuaScript* luaScript = INSTANCE(LuaEngine).getScript("global");
	//lua_State* luaState = luaScript->getLuaState();
	//luabind::object luaTable = luabind::globals(luaState)["t"];
	////std::string str = luabind::object_cast<std::string>(luaTable[1]);
	//uint32 size = luabind::tablecount(luaTable);
	//std::string desc = luabind::object_cast<std::string>(luabind::gettable(luaTable, "name"));

	LOG_DEBUG(LogSystem::csl_color_green, "lua script load finish");
}
