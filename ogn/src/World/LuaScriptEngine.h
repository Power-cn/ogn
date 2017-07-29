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

Entity* luaPlayerToEntity(Player* player);
Entity* luaNpcToEntity(Npc* npc);
PropertyHelper* luaPropertyHelper();
WorldModule* luaWorld();
RoomModule* luaRoom();