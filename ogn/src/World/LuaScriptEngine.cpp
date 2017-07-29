#include "stdafx.h"


Object* luaObject()
{
	return LuaEngine::sCurrentObject;
}

Entity* luaEntity()
{
	return (Entity*)LuaEngine::sCurrentObject;
}

Player* luaPlayer()
{
	return (Player*)LuaEngine::sCurrentObject;
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

WorldModule* luaWorld()
{
	return &sWorld;
}

RoomModule* luaRoom()
{
	return &sRoom;
}
