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

void GameGoldenFlower::DoCutCard()
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
	LuaEngine::Call(sScriptGame, "OnStart", GetInsId());
	return true;
}

bool GameGoldenFlower::OnClose()
{
	LuaEngine::Call(sScriptGame, "OnClose", GetInsId());
	return true;
}

bool GameGoldenFlower::OnEnter(GameEntity* aGameEnt)
{
	LuaEngine::Call(sScriptGame, "OnEnter", GetInsId(), aGameEnt->userId);
	return true;
}

bool GameGoldenFlower::OnLeave(GameEntity* aGameEnt)
{
	LuaEngine::Call(sScriptGame, "OnLeave", GetInsId(), aGameEnt->userId);
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
	info.pokers = cards;
	return true;
}

std::string GameEntity::ToString()
{
	char szBuff[256] = { 0 };
	sprintf_s(szBuff, 256, "[%d]", userId);
	uint32 strLen = (uint32)strlen(szBuff);
	for (uint32 i = 0; i < cards.size(); ++i)
	{
		sprintf_s(szBuff + strLen, 256 - strLen, "%d ", cards[i]);
		strLen = (uint32)strlen(szBuff);
	}
	return szBuff;
}

uint32 GameEntity::GetCard(uint32 idx)
{
	if (idx >= cards.size()) return 0;
	return cards[idx];
}

//luabind::object GameEntity::GetCards()
//{
//	LuaScript* luaScript = sLua.getScript(sScriptPlayer);
//	if (luaScript == NULL) return luabind::object();
//
//	luabind::object obj = luabind::newtable(luaScript->getLuaState());
//	for (uint32 i = 0; i < cards.size(); ++i) {
//		obj[i] = cards[i];
//	}
//	return obj;
//}

ThreeCardType GameEntity::GetThreeCardType()
{
	return ThreeCardType::TCT_None;
}
