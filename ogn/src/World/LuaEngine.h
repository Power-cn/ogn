#pragma once

//
struct lua_State;
class Object;
class Player;

namespace luabind
{
	namespace detail {
		template<class T>
		struct delete_s;
		template<class T>
		struct destruct_only_s;
	}
};

class LuaScript
{
public:
	LuaScript(const std::string& path);
	~LuaScript();
	bool loadScript(const std::string& path);
	void bindScript();
	const std::string& getName() { return mName; }
	lua_State* getLuaState() { return mLuaState; }
protected:
	lua_State* mLuaState;
	std::string mPath;
	std::string mName;
};

class LuaEngine
{
public:
	LuaEngine();
	~LuaEngine();
	void reloadScript();
protected:
	LuaScript* loadScript(const std::string& path);
	LuaScript* getScript(const std::string& name);
	void clearScript();
public:
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func);
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func, int32 p1);
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func, int32 p1, int32 p2);
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func, int32 p1, int32 p2, int32 p3);
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func, int32 p1, int32 p2, int32 p3, int32 p4);
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func, const std::string& p1);
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func, const std::string& p1, const std::string& p2);
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func, const std::string& p1, const std::string& p2, const std::string& p3);
	static uint32 executeScript(Object* object, const std::string& name, const std::string& func, const std::string& p1, const std::string& p2, const std::string& p3, const std::string& p4);
	static uint32 executeScript(const std::string& name, const std::string& func, const std::string& p1);

	static int32 GetInt32(const std::string& name, const std::string& field);
	static bool SetInt32(const std::string& name, const std::string& field, const int32 value);
	static float64 GetFloat64(const std::string& name, const std::string& field);
	static bool SetFloat64(const std::string& name, const std::string& field, const float64 value);
	static std::string GetString(const std::string& name, const std::string& field);
	static bool SetString(const std::string& name, const std::string& field, const std::string& value);


	template<class T>
	static bool GetValue(const std::string& name, const std::string& field, T& ret)
	{
		LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
		if (!luaScript)	return false;

		lua_State* luaState = luaScript->getLuaState();
		if (!luaState) return false;

		try {
			ret = luabind::object_cast<T>(luabind::globals(luaState)[field.c_str()]);
			return true;
		}
		catch (luabind::error& e)
		{
			LOG_ERROR("lua:%s field:%s error: %s", name.c_str(), field.c_str(), e.what());
			return false;
		}
		catch (const char* msg)
		{
			LOG_ERROR("lua:%s field:%s error: %s", name.c_str(), field.c_str(), msg);
			return false;
		}
		return false;
	}

	template<class T>
	static bool SetValue(const std::string& name, const std::string& field, const T& ret) {

		LuaScript* luaScript = INSTANCE(LuaEngine).getScript(name);
		if (!luaScript)	return false;

		lua_State* luaState = luaScript->getLuaState();
		if (!luaState) return false;

		try {
			luabind::globals(luaState)[field.c_str()] = ret;
			return true;
		}
		catch (luabind::error& e)
		{
			LOG_ERROR("lua:%s field:%s error: %s", name.c_str(), field.c_str(), e.what());
			return false;
		}
		catch (const char* msg)
		{
			LOG_ERROR("lua:%s field:%s error: %s", name.c_str(), field.c_str(), msg);
			return false;
		}
		return false;
	}
protected:

	std::map<std::string, LuaScript*>		mMapLuaScript;
private:
};

Object* luaObject();
Entity* luaEntity();
Player* luaPlayer();

Entity* luaPlayerToEntity(Player* player);
Entity* luaNpcToEntity(Npc* npc);
PropertyHelper* luaPropertyHelper();

