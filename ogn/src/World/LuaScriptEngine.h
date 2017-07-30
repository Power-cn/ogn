#pragma once

//
class Object;
class Entity;
class Player;
class Npc;
class PropertyHelper;
class WorldModule;
class RoomModule;

Object* luaObject();
Entity* luaEntity();
Player* luaPlayer();

Entity* luaPlrToEnt(Player* player);
Entity* luaNpcToEnt(Npc* npc);
PropertyHelper* luaProperty();
WorldModule* luaWorld();
RoomModule* luaRoom();