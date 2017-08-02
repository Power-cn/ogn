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

GameModle* GameModule::FindGameModle(uint32 insId)
{
	auto itr = mMapGameModle.find(insId);
	if (itr != mMapGameModle.end())
		return itr->second;
	return NULL;
}

void GameModule::DelGameModle(uint32 insId)
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

GameModle* GameModule::AddRoomGameModle(uint32 roomId, GameModle* aGameModle)
{
	auto itr = mMapRoomGameModle.find(roomId);
	if (itr != mMapRoomGameModle.end())
		return NULL;

	mMapRoomGameModle[roomId] = aGameModle;
	return aGameModle;
}

GameModle* GameModule::FindPlrGameModle(uint32 userId)
{
	auto itr = mMapPlrGameModle.find(userId);
	if (itr != mMapPlrGameModle.end())
		return itr->second;
	return NULL;
}

void GameModule::DelPlrGameModle(uint32 userId)
{
	auto itr = mMapPlrGameModle.find(userId);
	if (itr != mMapPlrGameModle.end())
		mMapPlrGameModle.erase(itr);
}

void GameModule::DelRoomGameModle(uint32 roomId)
{
	auto itr = mMapRoomGameModle.find(roomId);
	if (itr != mMapRoomGameModle.end())
		mMapRoomGameModle.erase(itr);
}

GameModle* GameModule::FindRoomGameModle(uint32 roomId)
{
	auto itr = mMapRoomGameModle.find(roomId);
	if (itr != mMapRoomGameModle.end())
		return itr->second;
	return NULL;
}

GameEntity* GameModule::FindPlrGameEnt(uint32 userId)
{
	GameModle* aGameModule = FindPlrGameModle(userId);
	if (aGameModule == NULL)
		return NULL;
	return aGameModule->FindGameEnt(userId);
}

bool GameModule::DoStartGame(Room* aRoom)
{
	if (FindRoomGameModle(aRoom->GetInsId()))
	{
		// 当前房间在游戏中
		return false;
	}

	GameGoldenFlower* aGame = new GameGoldenFlower;
	AddGameModle(aGame);

	aGame->DoShuffle();
	aGame->SetRoomId(aRoom->GetInsId());
	aRoom->SetGameInsId(aGame->GetInsId());
	aRoom->DoAllStart();

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

	for (uint8 k = 0; k < MAX_UNIT_POKER_COUNT; ++k)
	{
		for (uint32 i = 0; i < aGame->GetGameEntCount(); ++i)
		{
			GameEntity* aGameEnt = aGame->GetGameEnt(i);
			aGameEnt->cards.push_back(aGame->DoDealPoker());
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

bool GameModule::DoCloseGame(uint32 roomId)
{
	GameModle* aGameModle = FindRoomGameModle(roomId);
	if (aGameModle == NULL)
	{
		LOG_ERROR("%d 房间没有开始游戏", roomId);
		return false;
	}
	std::queue<uint32> delQueue;
	for (uint32 i = 0; i < aGameModle->GetGameEntCount(); ++i)
	{
		GameEntity* aGameEnt = aGameModle->GetGameEnt(i);
		if (aGameEnt)
		{
			aGameModle->OnLeave(aGameEnt);
			delQueue.push(aGameEnt->userId);
		}
	}

	aGameModle->OnClose();

	while (delQueue.size())
	{
		uint32 userId = delQueue.front();
		delQueue.pop();

		DelPlrGameModle(userId);
		aGameModle->DelGameEnt(userId);
	}
	DelRoomGameModle(roomId);
	DelGameModle(aGameModle->GetInsId());
	return true;
}

