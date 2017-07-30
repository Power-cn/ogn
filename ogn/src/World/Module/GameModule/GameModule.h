#pragma once

class GameModule : public Module
{
	DECLARE_CLASS(GameModule)
public:
	GameModule();
	virtual ~GameModule();
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
private:
};
