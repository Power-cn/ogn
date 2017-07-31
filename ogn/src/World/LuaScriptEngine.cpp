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

GameGoldenFlower* luaGoldenFlower(uint32 insId)
{
	return (GameGoldenFlower*)sGame.FindGameModule(insId);
}

GameGoldenFlower* luaPlrGoldenFlower(uint32 userId)
{
	return (GameGoldenFlower*)sGame.FindPlrGameModule(userId);
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
