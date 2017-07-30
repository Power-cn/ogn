#include "stdafx.h"
#include "Application.h"

uint32 Team::sId = 0;
Team::Team():
mInstanceId(++sId),
mLeader(NULL)
{
	mScript = "team";
}

Team::~Team()
{

}

void Team::Update()
{
	std::queue<uint32> delQueue;
	for (TeamEntity* tEnt: mTeamEntityList)
	{
		if (tEnt->getDel())
			delQueue.push(tEnt->getUserId());
	}

	while (delQueue.size() > 0)
	{
		removePlayer(delQueue.front());
		delQueue.pop();
	}
}

void Team::sendPacketToAll(Packet& packet)
{
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->getPlayer() && !tEnt->getDel())
			tEnt->getPlayer()->sendBuffer(in.getPtr(), in.getWPostion());
	}
}

bool Team::addPlayer(Player* player, bool isLeader /* = false */)
{
	if (getPlayer(player->getUserId()))
		return false;

	TeamEntity* eny = new TeamEntity;
	eny->setPlayer(player);
	eny->setName(player->getName());
	eny->setUserId(player->getUserId());
	eny->setDel(false);
	if (isLeader) {
		mLeader = eny;
	}
	mTeamEntityList.push_back(eny);
	return true;
}

bool Team::destoryPlayer(uint32 userId)
{
	TeamEntity* tEnt = getPlayer(userId);
	if (tEnt == NULL) return false;
	tEnt->setDel(true);
	return true;
}

bool Team::removePlayer(uint32 userId)
{
	for (std::vector<TeamEntity *>::iterator itr = mTeamEntityList.begin();
		 itr != mTeamEntityList.end();
		 ++itr)
	{
		if ((*itr)->getUserId() == userId)
		{
			delete (*itr);
			mTeamEntityList.erase(itr);
			return true;
		}
	}

	return false;
}

TeamEntity* Team::getPlayer(uint32 userId)
{
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (!tEnt->getDel()) continue;
		if (tEnt->getUserId() == userId) return tEnt;
	}
	return NULL;
}

void Team::onEnterWorld(Player* player)
{
	TeamEntity* teny = getPlayer(player->getUserId());
	if (teny == NULL)
		return;
	teny->setPlayer(player);
}

void Team::onLeaveWorld(Player* player)
{
	TeamEntity* teny = getPlayer(player->getUserId());
	if (teny == NULL)
		return;
	teny->setPlayer(NULL);

	if (getLeader() == player)
	{
		TeamEntity* nteny = ChooseLeader();
		if (nteny == NULL) return;
		ChangeLeader(nteny->getPlayer());
	}
}


uint32 Team::getPlayerCount()
{
	uint32 count = 0;
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->getDel()) continue;
		count++;
	}
	return count;
}

TeamEntity* Team::getPlayerEntity(uint32 idx)
{
	uint32 count = 0;
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->getDel()) continue;
		if (count == idx)
			return tEnt;
		count++;
	}
	return NULL;
}

bool Team::ChangeLeader(Player* newLeader)
{
	TeamEntity* teny = getPlayer(newLeader->getUserId());
	if (teny == NULL)
		return false;

	if (getLeader() == newLeader)
		return false;
	
	Player* lastLader = getLeader();
	mLeader = teny;
	OnChangeLeader(lastLader->getUserId(), newLeader->getUserId());
	return true;
}

int32 Team::CanAddTeam(Player* tar)
{
	return 0;
}

TeamEntity* Team::ChooseLeader()
{
	if (mLeader == NULL)
		return NULL;

	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->getDel()) continue;
		if (tEnt->getPlayer() && mLeader != tEnt)
			return tEnt;
	}
	return NULL;
}

bool Team::OnCreate(uint32 userId)
{
	LuaEngine::executeScript(GetScript(), "OnCreate", getId(), userId);
	return true;
}

bool Team::OnEnter(uint32 userId)
{
	LuaEngine::executeScript(GetScript(), "OnEnter", getId(), userId);
	return true;
}

bool Team::OnLeave(uint32 userId)
{
	LuaEngine::executeScript(GetScript(), "OnLeave", getId(), userId);
	return true;
}

bool Team::OnChangeLeader(uint32 oldUserId, uint32 newUserId)
{
	LuaEngine::executeScript(GetScript(), "OnChangeLeader", getId(), oldUserId, newUserId);
	return true;
}