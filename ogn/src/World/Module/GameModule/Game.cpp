#include "stdafx.h"

uint32 GameModle::sId = 0;

GameModle::GameModle():
mInsId(++sId)
{
	mScript = "game";
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
