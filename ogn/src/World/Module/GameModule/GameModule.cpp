#include "stdafx.h"

IMPLEMENT_CLASS(GameModule, Module)

GameModule::GameModule()
{

}

GameModule::~GameModule()
{

}

bool GameModule::Initialize()
{
	return true;
}

bool GameModule::Update(float time, float delay)
{
	return true;
}

bool GameModule::Destroy()
{
	return true;
}

bool GameModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool GameModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;
}

