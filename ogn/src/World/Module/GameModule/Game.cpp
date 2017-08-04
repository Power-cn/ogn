#include "stdafx.h"

uint32 GameModle::sId = 0;

GameModle::GameModle():
mInsId(++sId),
mRoomId(0)
{
}

GameModle::~GameModle()
{
	for (GameEntity* aGameEnt : mLstGameEntity)
	{
		delete aGameEnt;
	}
	mLstGameEntity.clear();
}

GameEntity* GameModle::AddGameEnt(GameEntity* aGameEnt)
{
	mLstGameEntity.push_back(aGameEnt);
	return aGameEnt;
}

GameEntity* GameModle::GetGameEnt(uint32 idx)
{
	if (idx < mLstGameEntity.size())
		return mLstGameEntity[idx];
	return NULL;
}

GameEntity* GameModle::FindGameEnt(uint32 userId)
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

void GameModle::DelGameEnt(uint32 userId)
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

uint32 GameModle::GetPlrCard(uint32 userId, uint32 idx)
{
	GameEntity* aGameEnt = FindGameEnt(userId);
	if (aGameEnt == NULL) return 0;
	return aGameEnt->GetCard(idx);
}

std::string GameModle::ToString()
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

int32 GameModle::GetPlrInx(uint32 userId)
{
	for (uint32 i = 0; i < mLstGameEntity.size(); ++i)
	{
		if (mLstGameEntity[i]->userId == userId)
			return i;
	}
	return -1;
}