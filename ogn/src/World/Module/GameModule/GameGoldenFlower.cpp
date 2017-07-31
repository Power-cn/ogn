#include "stdafx.h"

GameGoldenFlower::GameGoldenFlower():
mRoomId(0)
{

}

GameGoldenFlower::~GameGoldenFlower()
{
	for (GameEntity* aGameEnt:mLstGameEntity)
	{
		delete aGameEnt;
	}
	mLstGameEntity.clear();
}

bool GameGoldenFlower::operator >> (GameGoldenFlowerInfo& info)
{
	info.insId = GetInsId();
	info.roomId = GetRoomId();
	for (GameEntity* aGameEnt : mLstGameEntity)
	{
		GameEntityInfo entInfo;
		*aGameEnt >> entInfo;
		info.gameEntInfos.push_back(entInfo);
	}
	return true;
}

GameEntity* GameGoldenFlower::AddGameEnt(GameEntity* aGameEnt)
{
	mLstGameEntity.push_back(aGameEnt);
	return aGameEnt;
}

GameEntity* GameGoldenFlower::GetGameEnt(uint32 idx)
{
	if (idx < mLstGameEntity.size())
		return mLstGameEntity[idx];
	return NULL;
}

GameEntity* GameGoldenFlower::FindGameEnt(uint32 userId)
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

void GameGoldenFlower::DelGameEnt(uint32 userId)
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

void GameGoldenFlower::DoShuffle()
{
	for (uint32 i = 0; i < MAX_POKER_COUNT; ++i) {
		mPoker[i] = i + 1;
	}
	uint32 curSize = MAX_POKER_COUNT;
	while (curSize > 0)
	{
		uint32 idx = rand() % curSize;
		mCurPoker.push(mPoker[idx]);
		curSize--;
		mPoker[idx] = mPoker[curSize];
	}
}

uint8 GameGoldenFlower::DoDealPoker()
{
	uint8 pkr = mCurPoker.front();
	mCurPoker.pop();
	return pkr;
}

void GameGoldenFlower::DoCutPoker()
{

}

uint32 GameGoldenFlower::GetNextSpeakPlr()
{
	return 0;
}

int32 GameGoldenFlower::GetPlrInx(uint32 userId)
{
	for (uint32 i = 0; i < mLstGameEntity.size(); ++i)
	{
		if (mLstGameEntity[i]->userId == userId)
			return i;
	}
	return -1;
}

std::string GameGoldenFlower::ToString()
{
	std::string str;

	for (uint32 i = 0; i < mLstGameEntity.size(); ++i)
	{
		str += mLstGameEntity[i]->ToString();
		str += "\n";
	}

	return str;
}

GameEntity::GameEntity()
{

}

GameEntity::~GameEntity()
{

}

bool GameEntity::operator >> (GameEntityInfo& info)
{
	info.userId = userId;
	info.pokers = poker;
	return true;
}

std::string GameEntity::ToString()
{
	char szBuff[256] = { 0 };
	sprintf_s(szBuff, 256, "[%d]", userId);
	uint32 strLen = (uint32)strlen(szBuff);
	for (uint32 i = 0; i < poker.size(); ++i)
	{
		sprintf_s(szBuff + strLen, 256 - strLen, "%d ", poker[i]);
		strLen = (uint32)strlen(szBuff);
	}
	return szBuff;
}

uint32 GameEntity::GetPoker(uint32 idx)
{
	if (idx >= poker.size()) return 0;
	return poker[idx];
}
