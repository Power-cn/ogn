#pragma once

class GameHandler : public Object
{
public:
	GameHandler();
protected:
	int32 onNetGameChipInReq(Player* aPlr, NetGameChipInReq* req);
private:
};

