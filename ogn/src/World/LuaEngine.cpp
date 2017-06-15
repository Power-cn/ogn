#include "stdafx.h"
#include "lua.hpp"
#include <luabind/luabind.hpp>


static Object* sCurrentObject = NULL;

Object* luaObject()
{
	return sCurrentObject;
}

Entity* luaEntity()
{
	return (Entity*)sCurrentObject;
}

Player* luaPlayer()
{
	return (Player*)sCurrentObject;
}

Entity* luaPlayerToEntity(Player* player)
{
	return player;
}

Entity* luaNpcToEntity(Npc* npc)
{
	return npc;
}

PropertyHelper* luaPropertyHelper()
{
	return &INSTANCE(PropertyHelper);
}

LuaScript::LuaScript(const std::string& path)
{
	int32 spos = path.find_last_of("/");
	spos++;
	int32 epos = path.find_last_of(".");
	mName = path.substr(spos, epos - spos);

	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);
	//luaL_openlib(mLuaState);
	//mLuaState = lua_open();
	//lua_cpcall(mLuaState, luaopen_base, 0);
	//lua_cpcall(mLuaState, luaopen_table, 0);
	//lua_cpcall(mLuaState, luaopen_io, 0);
	//lua_cpcall(mLuaState, luaopen_string, 0);
	//lua_cpcall(mLuaState, luaopen_math, 0);
	//lua_cpcall(mLuaState, luaopen_debug, 0);
	////脚本增加系统库支持;
	//lua_cpcall(mLuaState, luaopen_os, 0);


	//lua_error()

	luabind::open(mLuaState);

	bindScript();

	loadScript(path);
}

LuaScript::~LuaScript()
{
	if (mLuaState)
		lua_close(mLuaState);
	mLuaState = NULL;
}


bool LuaScript::loadScript(const std::string& path)
{
	if (luaL_dofile(mLuaState, path.c_str()))
	{
		LOG_ERROR(lua_tostring(mLuaState, -1));
		return false;
	}

	LOG_INFO("load script <%s> success", path.c_str());
	return true;
}

LuaEngine::LuaEngine()
{

}

LuaEngine::~LuaEngine()
{
	clearScript();
}

LuaScript* LuaEngine::loadScript(const std::string& path)
{
	LuaScript* luaScript = new LuaScript(path);
	mMapLuaScript.insert(std::make_pair(luaScript->getName(), luaScript));
	return luaScript;
}



LuaScript* LuaEngine::getScript(const std::string& name)
{
	auto itr = mMapLuaScript.find(name);
	if (itr != mMapLuaScript.end())
		return itr->second;

	return NULL;
}

void LuaEngine::clearScript()
{
	while (mMapLuaScript.size())
	{
		auto itr = mMapLuaScript.begin();
		delete itr->second;
		mMapLuaScript.erase(itr);
	}
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func, int32 p1)
{
	sCurrentObject = object;

	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func, int32 p1, int32 p2)
{
	sCurrentObject = object;

	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1, p2);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func, int32 p1, int32 p2, int32 p3)
{
	sCurrentObject = object;

	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1, p2, p3);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func, int32 p1, int32 p2, int32 p3, int32 p4)
{
	sCurrentObject = object;

	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1, p2, p3, p4);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func, const std::string& p1)
{
	sCurrentObject = object;
	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func, const std::string& p1, const std::string& p2)
{
	sCurrentObject = object;

	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1, p2);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func, const std::string& p1, const std::string& p2, const std::string& p3)
{
	sCurrentObject = object;

	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1, p2, p3);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func, const std::string& p1, const std::string& p2, const std::string& p3, const std::string& p4)
{
	sCurrentObject = object;

	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1, p2, p3, p4);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(const std::string& name, const std::string& func, const std::string& p1)
{
	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str(), p1);
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

uint32 LuaEngine::executeScript(Object* object, const std::string& name, const std::string& func)
{
	sCurrentObject = object;
	LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
	if (!luaScript) return 0;
	lua_State* luaState = luaScript->getLuaState();
	if (!luaState) return 0;

	try {
		int ret = luabind::call_function<int>(luaState, func.c_str());
		return ret;
	}
	catch (luabind::error& e)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), e.what());
		return 0;
	}
	catch (const char* msg)
	{
		LOG_ERROR("lua:%s func:%s error: %s", name.c_str(), func.c_str(), msg);
		return 0;
	}
	return 0;
}

int32 LuaEngine::GetInt32(const std::string& name, const std::string& field)
{
	int32 ret = 0;
	GetValue<int32>(name, field, ret);
	return ret;
}

bool LuaEngine::SetInt32(const std::string& name, const std::string& field, const int32 value)
{
	SetValue<int32>(name, field, value);
	return true;
}

float64 LuaEngine::GetFloat64(const std::string& name, const std::string& field)
{
	float64 ret = 0;
	GetValue<float64>(name, field, ret);
	return ret;
}

bool LuaEngine::SetFloat64(const std::string& name, const std::string& field, const float64 value)
{
	SetValue<float64>(name, field, value);
	return true;
}

std::string LuaEngine::GetString(const std::string& name, const std::string& field)
{
	std::string ret = "";
	GetValue<std::string>(name, field, ret);
	return ret;
}

bool LuaEngine::SetString(const std::string& name, const std::string& field, const std::string& value)
{
	SetValue<std::string>(name, field, value);
	return true;
}

