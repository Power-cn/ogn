#include "stdafx.h"

FriendsHandler::FriendsHandler()
{
	RegWorldEvent(ID_NetAddFriendReq, &FriendsHandler::onNetAddFriendReq, this);
}

int32 FriendsHandler::onNetAddFriendReq(Player* aPlr, NetAddFriendReq* req)
{
	return 0;
}
