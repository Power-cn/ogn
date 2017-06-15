#pragma once


class Player;

class PlayerHandler : public Object
{
public:
	PlayerHandler();
protected:
	int32 onNetChatMsgNotify(Player* player, NetChatMsgNotify* nfy);
	int32 onNetGmMsg(Player* player, NetGmMsg* msg);
	int32 onNetEntityMoveNotify(Player* player, NetEntityMoveNotify* nfy);
	int32 onNetEntityMoveToNotify(Player* player, NetEntityMoveToNotify* nfy);
	int32 onNetOrganizeTeamReq(Player* player, NetOrganizeTeamReq* req);
	int32 onNetAgreeTeamReq(Player* player, NetAgreeTeamReq* req);
	int32 onNetTeamListReq(Player* player, NetTeamListReq* req);
};