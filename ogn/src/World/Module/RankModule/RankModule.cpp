#include "stdafx.h"

IMPLEMENT_CLASS(RankModule, Module)

RankModule::RankModule()
{

}

RankModule::~RankModule()
{

}

bool RankModule::Initialize()
{
	return true;
}

bool RankModule::Update(float time, float delay)
{
	return true;
}

bool RankModule::Destroy()
{
	return true;
}

bool RankModule::onEnterWorld(Player* player, Dictionary& dict)
{
	return true;
}

bool RankModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	return true;

}

bool RankModule::onLoad(Player* player, Dictionary& bytes)
{
	return true;
}

bool RankModule::onSave(Player* player, Dictionary& bytes)
{
	return true;
}

bool RankModule::onLoadEnd(Player* player, Dictionary& bytes)
{
	return true;
}

bool RankModule::onSaveEnd(Player* player, Dictionary& bytes)
{
	return true;
}

bool RankModule::RankAdd(uint8 rankType, uint32 userId, uint64 value)
{
	char szBuffer[256] = {};
	sprintf_s(szBuffer, 256, "zadd sGoldRank %llu %d", value, userId);
	sRedisProxy.sendCmd(szBuffer, NULL, NULL);
	return true;
}

bool RankModule::RankFind(uint8 rankType, uint32 start, uint32 count)
{
	char szBuffer[256] = {};
	sprintf_s(szBuffer, 256, "ZREVRANGE sGoldRank %d %d WITHSCORES", start, start + count);
	sRedisProxy.sendCmd(szBuffer, (EventCallback)&RankModule::onRankFind, this);
	return true;
}

int32 RankModule::onRankFind(RedisEvent& e)
{
	return 0;
}
