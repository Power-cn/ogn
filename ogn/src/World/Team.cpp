#include "stdafx.h"
#include "Application.h"

uint32 Team::sId = 0;
Team::Team(Player* leader):
mInstanceId(++sId),
mLeader(leader)
{

}

Team::~Team()
{

}

void Team::Update()
{
	std::queue<uint32> delQueue;
	for (TeamEntity* tEnt: mTeamEntityList)
	{
		if (tEnt->isdel)
			delQueue.push(tEnt->userId);
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
	for (auto itr : mTeamEntityList)
	{
		if (itr->player)
			itr->player->sendBuffer(in.getPtr(), in.getWPostion());
	}
}

bool Team::addPlayer(Player* player)
{
	if (getPlayer(player->getUserId()))
		return false;

	TeamEntity* eny = new TeamEntity;
	eny->player = player;
	eny->name = player->getName();
	eny->userId = player->getUserId();
	eny->isdel = false;
	mTeamEntityList.push_back(eny);
	return true;
}

bool Team::destoryPlayer(uint32 userId)
{
	TeamEntity* tEnt = getPlayer(userId);
	if (tEnt == NULL) return false;
	tEnt->isdel = true;
	return true;
}

bool Team::removePlayer(uint32 userId)
{
	for (std::vector<TeamEntity *>::iterator itr = mTeamEntityList.begin();
		 itr != mTeamEntityList.end();
		 ++itr)
	{
		if ((*itr)->userId == userId)
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
		if (tEnt->isdel) continue;
		if (tEnt->userId == userId) return tEnt;
	}
	return NULL;
}

void Team::onEnterWorld(Player* player)
{
	TeamEntity* teny = getPlayer(player->getUserId());
	if (teny == NULL)
		return;
	teny->player = player;
}

void Team::onLeaveWorld(Player* player)
{
	TeamEntity* teny = getPlayer(player->getUserId());
	if (teny == NULL)
		return;
	teny->player = NULL;

	if (mLeader == player)
	{
		TeamEntity* nteny = ChooseLeader();
		if (nteny)
			ChangeLeader(nteny->player);
	}
}

bool Team::onEnterTeam(Player* player)
{
	return true;
}

bool Team::onLeaveTeam(Player* player)
{
	return true;
}

bool Team::onChangeLeader(const std::string& oldLeader, const std::string& newLeader)
{
	return true;
}

uint32 Team::getPlayerCount()
{
	uint32 count = 0;
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->isdel) continue;
		count++;
	}
	return count;
}

TeamEntity* Team::getPlayerEntity(uint32 idx)
{
	uint32 count = 0;
	for (TeamEntity* tEnt : mTeamEntityList)
	{
		if (tEnt->isdel) continue;
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

	if (mLeader == newLeader)
		return false;
	
	Player* lastLader = mLeader;
	mLeader = newLeader;
	onChangeLeader(lastLader->getName(), mLeader->getName());
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

	for (auto itr : mTeamEntityList)
	{
		if (itr->player && mLeader != itr->player && !itr->isdel)
			return itr;
	}
	return NULL;
}

