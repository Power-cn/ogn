#include "stdafx.h"

IMPLEMENT_CLASS(TeamModule, Module)
TeamModule::TeamModule()
{

}

TeamModule::~TeamModule()
{
	for (auto itr : mMapTeam)
		delete itr.second;
	mMapTeam.clear();
}

bool TeamModule::Update(float time, float delay)
{
	for (auto itr : mMapTeam)
	{
		Team* team = itr.second;
		team->Update();
		if (team->getPlayerCount() <= 0)
			destroyTeam(team);
	}

	while (mAddTeamList.size() > 0)
	{
		Team* team = mAddTeamList.front();
		mAddTeamList.pop_front();

		if (!addTeam(team))
			delete team;
	}
	mAddTeamList.clear();

	while (mDelTeamList.size() > 0)
	{
		uint32 teamId = mDelTeamList.front();
		mDelTeamList.pop_front();
		removeTeam(teamId);
	}
	mDelTeamList.clear();

	return true;
}

bool TeamModule::onEnterWorld(Player* player, Dictionary& dict)
{
	Team* tm = getPlayerTeam(player->getUserId());
	if (tm == NULL)
		return true;

	tm->onEnterWorld(player);
	return true;
}

bool TeamModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	Team* tm = getPlayerTeam(player->getUserId());
	if (tm == NULL)
		return true;

	tm->onLeaveWorld(player);
	return true;
}

Team* TeamModule::createTeam(Player* leader)
{
	Team* t = new Team(leader);
	mAddTeamList.push_back(t);
	return NULL;
}

void TeamModule::destroyTeam(Team* team)
{
	mDelTeamList.push_back(team->getId());
}

Team* TeamModule::addTeam(Team* team)
{
	if (getTeam(team->getId()))
		return NULL;
	mMapTeam.insert(std::make_pair(team->getId(), team));
	return team;
}

void TeamModule::removeTeam(uint32 teamId)
{
	auto itr = mMapTeam.find(teamId);
	if (itr != mMapTeam.end())
	{
		delete itr->second;
		mMapTeam.erase(itr);
	}
}

Team* TeamModule::getTeam(uint32 teamId)
{
	auto itr = mMapTeam.find(teamId);
	if (itr != mMapTeam.end())
		return itr->second;
	return NULL;
}

Team* TeamModule::getPlayerTeam(uint32 userid)
{
	auto itr = mMapPlayerTeam.find(userid);
	if (itr != mMapPlayerTeam.end())
		return itr->second;

	return NULL;
}

bool TeamModule::addPlayerTeam(uint32 userid, Team* team)
{
	auto itr = mMapPlayerTeam.find(userid);
	if (itr != mMapPlayerTeam.end())
		return false;

	mMapPlayerTeam.insert(std::make_pair(userid, team));
	return true;
}

void TeamModule::removePlayerTeam(uint32 userid)
{
	auto itr = mMapPlayerTeam.find(userid);
	if (itr != mMapPlayerTeam.end())
		mMapPlayerTeam.erase(itr);
}

bool TeamModule::doPlayerAddTeam(Player* player, Team* team)
{
	if (getPlayerTeam(player->getUserId()))
		return false;
	if (!team->addPlayer(player))
		return false;
	addPlayerTeam(player->getUserId(), team);
	team->onEnterTeam(player);
	return true;
}

bool TeamModule::doPlayerRemoveTeam(uint32 userId, Team* team)
{
	TeamEntity* teny = team->getPlayer(userId);
	if (teny && teny->player)
		team->onLeaveTeam(teny->player);

	if (!team->destoryPlayer(userId))
		return false;
	removePlayerTeam(userId);

	if (team->getPlayerCount() <= 0)
		destroyTeam(team);

	return true;
}

void TeamModule::sendPacketToAll(Packet& packet)
{
	for (auto itr : mMapTeam)
		itr.second->sendPacketToAll(packet);
}

void TeamModule::sendPacketToTeam(Packet& packet, Player* player)
{
	Team* tm = getPlayerTeam(player->getUserId());
	if (tm == NULL)
		return;
	tm->sendPacketToAll(packet);
}
