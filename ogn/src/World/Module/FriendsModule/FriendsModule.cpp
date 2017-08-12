#include "stdafx.h"

IMPLEMENT_CLASS(FriendsModule, FriendsModule);

FriendsModule::FriendsModule()
{

}

FriendsModule::~FriendsModule()
{

}

Friends* FriendsModule::AddFriends(Friends* frds)
{
	if (GetFriends(frds->GetUserId())) return NULL;

	mMapFriends.insert(std::make_pair(frds->GetUserId(), frds));
	return frds;
}

Friend* FriendsModule::AddFriend(uint32 tarUserId, Player* tar)
{
	if (tarUserId == tar->getUserId()) return false;

	Friends* frds = GetFriends(tarUserId);
	if (frds == NULL)
	{
		frds = new Friends(tarUserId);
		if (AddFriends(frds) == NULL) {
			delete  frds;
			frds = NULL;
		}
	}
	if (frds == NULL) return NULL;
	return  frds->AddFriend(tar);
}

Friend* FriendsModule::AddFriend(uint32 tarUserId, Friend* frdTar)
{
	if (tarUserId == frdTar->mUserId) return false;

	Friends* frds = GetFriends(tarUserId);
	if (frds == NULL)
	{
		frds = new Friends(tarUserId);
		if (AddFriends(frds) == NULL) {
			delete  frds;
			frds = NULL;
		}
	}

	if (frds == NULL) return NULL;
	return  frds->AddFriend(frdTar);
}

Friend* FriendsModule::FindFriend(uint32 tarUserId, uint32 frdUserId)
{
	Friends* frds = GetFriends(tarUserId);
	if (frds == NULL) return NULL;
	return frds->FindFriend(frdUserId);
}

bool FriendsModule::DelFriend(uint32 tarUserId, uint32 frdUserId)
{
	Friends* frds = GetFriends(tarUserId);
	if (frds == NULL)
		return false;
	return frds->DelFriend(frdUserId);
}

bool FriendsModule::DelFriends(uint32 tarUserId)
{
	auto itr = mMapFriends.find(tarUserId);
	if (itr == mMapFriends.end()) return false;
	mMapFriends.erase(itr);
	return true;
}

Friends* FriendsModule::GetFriends(uint32 userId)
{
	auto itr = mMapFriends.find(userId);
	if (itr == mMapFriends.end()) return NULL;

	return itr->second;
}

PlayerRecord* FriendsModule::AddPlrRecord(PlayerRecord* aPlrRecord)
{
	if (FindPlrRecord(aPlrRecord->GetUserId())) return NULL;
	mMapPlrRecords[aPlrRecord->GetUserId()] = aPlrRecord;
	return aPlrRecord;
}

PlayerRecord* FriendsModule::FindPlrRecord(uint32 userId)
{
	auto itr = mMapPlrRecords.find(userId);
	if (itr == mMapPlrRecords.end()) return NULL;
	return itr->second;
}

bool FriendsModule::MutualBindFriend(Player* tar, Player* frd)
{
	if (FindFriend(tar->getUserId(), frd->getUserId()))
	{
		return false;
	}
	if (FindFriend(frd->getUserId(), tar->getUserId()))
	{
		return false;
	}

	if (tar->getUserId() == frd->getUserId()) return false;
	if (!AddFriend(tar->getUserId(), frd))
		return false;
	return AddFriend(frd->getUserId(), tar) != NULL;
}

bool FriendsModule::MutualDebindFriend(uint32 tarUserId, uint32 frdUserId)
{
	if (tarUserId == frdUserId) return false;
	DelFriend(tarUserId, frdUserId);
	DelFriend(frdUserId, tarUserId);
	return true;
}

void FriendsModule::ClearFriends()
{
	for (auto itr : mMapFriends)
	{
		delete itr.second;
	}
	mMapFriends.clear();
}

void FriendsModule::DelPlrRecord(uint32 userId)
{
	auto itr = mMapPlrRecords.find(userId);
	if (itr == mMapPlrRecords.end()) return;
	delete itr->second;
	mMapPlrRecords.erase(itr);
}

bool FriendsModule::Initialize()
{
	return true;
}

bool FriendsModule::Update(float time, float delay)
{
	return true;
}

bool FriendsModule::Destroy()
{
	return true;
}

bool FriendsModule::onEnterWorld(Player* player, Dictionary& dict)
{
	if (FindPlrRecord(player->getUserId()) == NULL)
	{
		PlayerRecord* aPlrRcd = new PlayerRecord;
		aPlrRcd->mUserId = player->getUserId();
		aPlrRcd->mPlayer = player;
		AddPlrRecord(aPlrRcd);
	}

	Friends* frds = GetFriends(player->getUserId());
	if (frds == NULL)
	{
		return true;
	}

	for (Friend* frd : frds->GetFriends())
	{
		PlayerRecord* aPlrRcd = FindPlrRecord(frd->mUserId);
		if (aPlrRcd == NULL) {
			aPlrRcd = new PlayerRecord;
			aPlrRcd->mUserId = frd->mUserId;
			AddPlrRecord(aPlrRcd);
		}

		Player* aPlr = sWorld.FindPlrByUserId(frd->mUserId);
		if (aPlr) {
			aPlrRcd->mPlayer = aPlr;
		}
		else {
			char szBuffer[64] = { 0 };
			sprintf_s(szBuffer, 64, "hget %s %d", sUser, frd->mUserId);
			std::vector<std::string> parstr;
			parstr.push_back(Shared::int32tostr(player->getUserId()));
			parstr.push_back(Shared::int32tostr(frd->mUserId));
			sRedisProxy.sendCmd(szBuffer, (EventCallback)&FriendsModule::onRedisFindPlr, this, parstr);
		}
	}
	return true;
}

bool FriendsModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	DelFriends(player->getUserId());
	return true;
}

bool FriendsModule::onLoad(Player* player, Dictionary& bytes)
{

	Json::Value& root = player->GetJson();
	Json::Value arrayVal = root["friends"];

	for (uint32 i = 0 ; i < arrayVal.size(); ++i)
	{
		Friend* frd = new Friend;
		(*frd) << arrayVal[i];
		if (!AddFriend(player->getUserId(), frd))
			delete frd;
	}
	
	return true;
}

bool FriendsModule::onSave(Player* player, Dictionary& bytes)
{
	Friends* frds = GetFriends(player->getUserId());
	if (frds == NULL) return false;

	Json::Value& root = player->GetJson();

	Json::Value arrayVal;
	for (Friend* frd : frds->GetFriends())
	{
		Json::Value frdJson;
		(*frd) >> frdJson;
		arrayVal.append(frdJson);
	}
	root["friends"] = arrayVal;
	return true;
}

int32 FriendsModule::onRedisFindPlr(RedisEvent& e)
{
	if (e.backstr.size() <= 0) return 0;
	Json::Reader jsonReader;
	Json::Value root;
	if (!jsonReader.parse(e.backstr[0].c_str(), root))
		return 0;
	Json::Value userJson = root["user"];

	uint32 tarUserId = Shared::strtoint32(e.parstr[0]);
	uint32 frdUserId = Shared::strtoint32(e.parstr[1]);

	PlayerRecord* aPlrRcd = FindPlrRecord(frdUserId);
	if (aPlrRcd) {
		aPlrRcd->mName = userJson["name"].asString();
	}
	return 0;
}

uint32 PlayerRecord::GetUserId()
{
	if (mPlayer) {
		return mPlayer->getUserId();
	}
	return mUserId;
}

const std::string& PlayerRecord::GetName()
{
	if (mPlayer == NULL) return mName;
	return mPlayer->GetNameStr();
}

bool PlayerRecord::GetOnline()
{
	if (mPlayer == NULL) return false;
	return mPlayer->GetOnline();
}
