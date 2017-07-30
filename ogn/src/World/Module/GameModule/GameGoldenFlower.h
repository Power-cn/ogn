#pragma once

#define MAX_UNIT_POKER_COUNT 3
#define MAX_POKER_COUNT 54

class GameEntity
{
public:
	GameEntity();
	~GameEntity();
public:
	uint32					userId;
	uint8					maxCount = MAX_UNIT_POKER_COUNT;
	std::vector<uint8>		poker;
protected:
};

/*
* 
*/
class GameGoldenFlower : public GameModle
{
public:
	GameGoldenFlower();
	~GameGoldenFlower();
protected:
	uint8				mPoker[MAX_POKER_COUNT];
};