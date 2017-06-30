#include "stdafx.h"


bool Friend::operator >> (Json::Value& json)
{
	json["mName"] = mName;
	json["mUserId"] = mUserId;
	json["mCharId"] = mCharId;
	json["mState"] = mState;
	return true;
}

bool Friend::operator << (Json::Value& json)
{
	mName = json["mName"].asString();
	mUserId = json["mUserId"].asUInt();
	mCharId = json["mCharId"].asUInt();
	mState = json["mState"].asUInt();
	return true;
}

Friends::Friends(uint32 tarUserId):
mUserId(tarUserId)
{

}

Friends::~Friends()
{
	ClearFriends();
}

Friend* Friends::AddFriend(Player* tar)
{
	if (FindFriend(tar->getUserId()))
		return NULL;
	Friend* frd = new Friend;
	frd->mName = tar->getName();
	frd->mUserId = tar->getUserId();
	frd->mCharId = tar->getCharId();
	mFriends.push_back(frd);
	return frd;
}

Friend* Friends::AddFriend(Friend* frd)
{
	if (FindFriend(frd->mUserId)) {
		delete frd;
		return NULL;
	}
	mFriends.push_back(frd);
	return frd;
}

bool Friends::DelFriend(uint32 userId)
{
	for (Friend* frd : mFriends)
	{
		if (frd->mUserId == userId)
		{
			delete frd;
			mFriends.remove(frd);
			return true;
		}
	}
	return false;
}

Friend* Friends::FindFriend(uint32 userId)
{
	for (Friend* frd : mFriends)
	{
		if (userId == frd->mUserId)
			return frd;
	}
	return NULL;
}

void Friends::ClearFriends()
{
	for (Friend* frd : mFriends)
		delete frd;
	mFriends.clear();
}
