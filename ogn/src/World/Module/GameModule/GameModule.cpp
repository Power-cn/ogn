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

GameModle* GameModule::AddGameModle(GameModle* aGameModle)
{
	auto itr = mMapGameModle.find(aGameModle->GetInsId());
	if (itr != mMapGameModle.end())
		return NULL;
	mMapGameModle[aGameModle->GetInsId()] = aGameModle;
	return aGameModle;
}

GameModle* GameModule::FindGameModule(uint32 insId)
{
	auto itr = mMapGameModle.find(insId);
	if (itr != mMapGameModle.end())
		return itr->second;
	return NULL;
}

void GameModule::DelGameModule(uint32 insId)
{
	auto itr = mMapGameModle.find(insId);
	if (itr != mMapGameModle.end()){
		delete itr->second;
		mMapGameModle.erase(itr);
	}
}

GameModle* GameModule::AddPlrGameModle(uint32 userId, GameModle* aGameModle)
{
	auto itr = mMapPlrGameModle.find(userId);
	if (itr != mMapPlrGameModle.end())
	{

	}
	return NULL;
}

GameModle* GameModule::FindPlrGameModule(uint32 userId)
{
	return NULL;
}

void GameModule::DelPlrGameModule(uint32 insId)
{

}

bool GameModule::DoStartGame(Room* aRoom)
{
	GameGoldenFlower* aGame = new GameGoldenFlower;
	for (uint32 i = 0; i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* aRoomPlayer = aRoom->GetRoomPlayer(i);
		if (aRoomPlayer == NULL) continue;

	}
	return true;
}

