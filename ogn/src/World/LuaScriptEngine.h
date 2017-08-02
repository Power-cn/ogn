#pragma once

//
class Object;
class Entity;
class Player;
class Npc;

class PropertyHelper;
class GameModle;
class GameGoldenFlower;

class WorldModule;
class RoomModule;
class GameModule;

Object* luaObject();
Entity* luaEntity();
Player* luaPlayer();

Entity* luaPlrToEnt(Player* player);
Entity* luaNpcToEnt(Npc* npc);
Player* luaFindPlrByUserId(uint32 userId);

GameModle* luaToGameModle(GameGoldenFlower* aGame);
GameGoldenFlower* luaGoldenFlower(uint32 insId);
GameGoldenFlower* luaPlrGoldenFlower(uint32 userId);

PropertyHelper* luaProperty();
WorldModule* luaWorld();
RoomModule* luaRoom();
GameModule* luaGame();
ConfigManager* luaCfg();

luabind::object GetTestTable();