#pragma once

class GameHandler : public Object
{
public:
	GameHandler();
protected:
	int32 onNetRoomChipInReq(Player* aPlr, NetRoomChipInReq* req);
private:
};

