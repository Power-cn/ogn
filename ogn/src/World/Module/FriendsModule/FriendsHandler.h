#pragma once

class FriendsHandler : public Object
{
public:
	FriendsHandler();

protected:
	int32 onNetAddFriendReq(Player* aPlr, NetAddFriendReq* req);
private:
};