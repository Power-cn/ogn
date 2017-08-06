#include "stdafx.h"

uint32 GameComponent::sId = 0;

GameComponent::GameComponent():
mInsId(++sId),
mRoomId(0)
{
}

GameComponent::~GameComponent()
{
	for (GameEntity* aGameEnt : mLstGameEntity)
	{
		delete aGameEnt;
	}
	mLstGameEntity.clear();
}

GameEntity* GameComponent::AddGameEnt(GameEntity* aGameEnt)
{
	mLstGameEntity.push_back(aGameEnt);
	return aGameEnt;
}

GameEntity* GameComponent::GetGameEnt(uint32 idx)
{
	if (idx < mLstGameEntity.size())
		return mLstGameEntity[idx];
	return NULL;
}

GameEntity* GameComponent::FindGameEnt(uint32 userId)
{
	for (GameEntity* aGameEnt : mLstGameEntity)
	{
		if (aGameEnt->userId == userId)
		{
			return aGameEnt;
		}
	}
	return NULL;
}

void GameComponent::DelGameEnt(uint32 userId)
{
	for (auto itr = mLstGameEntity.begin();
		itr != mLstGameEntity.end();
		++itr)
	{
		if ((*itr)->userId == userId)
		{
			delete (*itr);
			mLstGameEntity.erase(itr);
			return;
		}
	}
}

uint32 GameComponent::GetPlrCard(uint32 userId, uint32 idx)
{
	GameEntity* aGameEnt = FindGameEnt(userId);
	if (aGameEnt == NULL) return 0;
	return aGameEnt->GetCard(idx);
}

std::string GameComponent::ToString()
{
	std::string str;
	char szBuffer[256] = { 0 };
	for (uint32 i = 0; i < mLstGameEntity.size(); ++i)
	{
		str += mLstGameEntity[i]->ToString();
		str += "\n";
	}
	return str;
}

int32 GameComponent::GetPlrInx(uint32 userId)
{
	for (uint32 i = 0; i < mLstGameEntity.size(); ++i)
	{
		if (mLstGameEntity[i]->userId == userId)
			return i;
	}
	return -1;
}