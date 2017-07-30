#pragma once

class GameModle
{
public:
	GameModle();
	~GameModle();
public:
	uint32 GetInsId() { return mInsId; }
protected:
	static uint32 sId;
	uint32 mInsId;
private:
};