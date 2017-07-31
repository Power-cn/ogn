#include "stdafx.h"

GameGoldenFlower::GameGoldenFlower()
{
	mBankerUserId = 0;
	mCurSpeakUserId = 0;
	mSpeakTime = 0;
}

GameGoldenFlower::~GameGoldenFlower()
{

}

bool GameGoldenFlower::operator >> (GameGoldenFlowerInfo& info)
{
	info.insId = GetInsId();
	info.roomId = GetRoomId();
	info.bankerUserId = GetBanker();
	info.curSpeakUserId = GetCurSpeak();
	info.speakTime = GetSpeakTime();
	for (GameEntity* aGameEnt : mLstGameEntity)
	{
		GameEntityInfo entInfo;
		*aGameEnt >> entInfo;
		info.gameEntInfos.push_back(entInfo);
	}
	return true;
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
	int32 idx = GetPlrInx(mCurSpeakUserId);
	if (idx >= 0 && idx < GetGameEntCount())
	{
		idx++;
		if (idx >= GetGameEntCount())
			idx = 0;
		GameEntity* aGameEnt =  GetGameEnt(idx);
		if (aGameEnt)
			return aGameEnt->userId;
	}
	return 0;
}

std::string GameGoldenFlower::ToString()
{
	std::string str;
	char szBuffer[256] = { 0 };
	sprintf_s(szBuffer, 256, "ID:%d rID:%d Banker:%d sID:%d sTime:%ds\n", GetInsId(), GetRoomId(), GetBanker(), GetCurSpeak(), GetSpeakTime());
	str += szBuffer;
	for (uint32 i = 0; i < mLstGameEntity.size(); ++i)
	{
		str += mLstGameEntity[i]->ToString();
		str += "\n";
	}

	return str;
}

bool GameGoldenFlower::OnStart()
{
	LuaEngine::executeScript(sScriptGame, "OnStart", GetInsId());
	return true;
}

bool GameGoldenFlower::OnClose()
{
	LuaEngine::executeScript(sScriptGame, "OnClose", GetInsId());
	return true;
}

bool GameGoldenFlower::OnEnter(GameEntity* aGameEnt)
{
	LuaEngine::executeScript(sScriptGame, "OnEnter", GetInsId(), aGameEnt->userId);
	return true;
}

bool GameGoldenFlower::OnLeave(GameEntity* aGameEnt)
{
	LuaEngine::executeScript(sScriptGame, "OnLeave", GetInsId(), aGameEnt->userId);
	return true;
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
