#pragma once

//
class Object;
class Entity;
class Player;
class Npc;
class PropertyHelper;
class WorldModule;
class RoomModule;
class GameModle;
class GameGoldenFlower;

Object* luaObject();
Entity* luaEntity();
Player* luaPlayer();

Entity* luaPlrToEnt(Player* player);
Entity* luaNpcToEnt(Npc* npc);
GameGoldenFlower* luaGoldenFlower(uint32 insId);
GameGoldenFlower* luaPlrGoldenFlower(uint32 userId);
PropertyHelper* luaProperty();
WorldModule* luaWorld();
RoomModule* luaRoom();