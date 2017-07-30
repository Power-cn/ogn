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

Entity* luaPlrToEnt(Player* player)
{
	return player;
}

Entity* luaNpcToEnt(Npc* npc)
{
	return npc;
}

PropertyHelper* luaProperty()
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
