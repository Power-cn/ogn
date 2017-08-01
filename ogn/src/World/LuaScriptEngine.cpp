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

Player* luaFindPlrByUserId(uint32 userId)
{
	return sWorld.FindPlrByUserId(userId);
}

GameModle* luaToGameModle(GameGoldenFlower* aGame)
{
	return (GameModle*)aGame;
}

GameGoldenFlower* luaGoldenFlower(uint32 insId)
{
	return (GameGoldenFlower*)sGame.FindGameModle(insId);
}

GameGoldenFlower* luaPlrGoldenFlower(uint32 userId)
{
	return (GameGoldenFlower*)sGame.FindPlrGameModle(userId);
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

GameModule* luaGame()
{
	return &sGame;
}
