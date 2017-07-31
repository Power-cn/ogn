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
		return NULL;

	mMapPlrGameModle[userId] = aGameModle;
	return aGameModle;
}

GameModle* GameModule::FindPlrGameModule(uint32 userId)
{
	auto itr = mMapPlrGameModle.find(userId);
	if (itr != mMapPlrGameModle.end())
		return itr->second;
	return NULL;
}

void GameModule::DelPlrGameModule(uint32 userId)
{
	auto itr = mMapPlrGameModle.find(userId);
	if (itr != mMapPlrGameModle.end())
		mMapPlrGameModle.erase(itr);
}

GameEntity* GameModule::FindPlrGameEnt(uint32 userId)
{
	GameModle* aGameModule = FindPlrGameModule(userId);
	if (aGameModule == NULL)
		return NULL;
	return aGameModule->FindGameEnt(userId);
}

bool GameModule::DoStartGame(Room* aRoom)
{
	GameGoldenFlower* aGame = new GameGoldenFlower;
	aRoom->SetGameInsId(aGame->GetInsId());

	for (uint32 i = 0; i < aRoom->GetRoomPlayerCount(); ++i)
	{
		RoomPlayer* aRoomPlayer = aRoom->GetRoomPlayer(i);
		if (aRoomPlayer == NULL) continue;
		if (aRoomPlayer->GetState() != RPS_Game) continue;
		
		GameEntity* aGameEnt = new GameEntity;
		aGameEnt->userId = aRoomPlayer->mUserId;
		aGame->AddGameEnt(aGameEnt);
		AddPlrGameModle(aRoomPlayer->mUserId, aGame);
	}
	AddGameModle(aGame);

	for (uint8 k = 0; k < MAX_UNIT_POKER_COUNT; ++k)
	{
		for (uint32 i = 0; i < aGame->GetGameEntCount(); ++i)
		{
			GameEntity* aGameEnt = aGame->GetGameEnt(i);
			aGameEnt->poker.push_back(aGame->DoDealPoker());
		}
	}

	aGame->OnStart();
	for (uint32 i = 0; i < aGame->GetGameEntCount(); ++i)
	{
		GameEntity* aGameEnt = aGame->GetGameEnt(i);
		if (aGameEnt)
		{
			aGame->OnEnter(aGameEnt);
		}
	}
	NetGameStartNotify nfy;
	*aGame >> nfy.info;
	aRoom->sendPacketToAll(nfy);
	return true;
}

bool GameModule::DoStopGame()
{
	return true;
}

