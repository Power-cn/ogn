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
	if (getPlayerTeam(leader->getUserId())) return NULL;
	Team* t = new Team();
	addTeam(t);
	t->OnCreate(leader->getUserId());
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

bool TeamModule::doPlayerAddTeam(Player* player, Team* team, bool isLeader /* = false */)
{
	if (getPlayerTeam(player->getUserId()))
		return false;
	if (!team->addPlayer(player, isLeader))
		return false;
	addPlayerTeam(player->getUserId(), team);
	team->OnEnter(player->getUserId());
	return true;
}

bool TeamModule::doPlayerRemoveTeam(uint32 userId, Team* team)
{
	TeamEntity* teny = team->getPlayer(userId);
	if (teny && teny->getPlayer())
		team->OnLeave(teny->getPlayer()->getUserId());

	if (!team->destoryPlayer(userId))
		return false;
	removePlayerTeam(userId);

	if (team->getPlayerCount() <= 0)
		destroyTeam(team);

	return true;
}

void TeamModule::DoCreateTeamReq(Player* aPlr)
{
	Team* aTeam = createTeam(aPlr);
	if (aTeam == NULL) return;
	doPlayerAddTeam(aPlr, aTeam);
}

void TeamModule::DoOrganizeTeamReq(Player* aPlr, cstring& tarName)
{
	Player* target = sWorld.FindPlrByName(tarName);
	if (!target)
		return ;

	Team* tm0 = getPlayerTeam(aPlr->getUserId());
	Team* tm1 = getPlayerTeam(target->getUserId());
	if (tm0 && tm1)
	{
		// 已经有队伍 ;
		return ;
	}

	if (tm0 && !tm1)
	{
		Player* leader = tm0->getLeader();
		if (leader == NULL)
			return ;

		NetOrganizeTeamRes res;
		res.name = leader->getName();
		res.isJoin = 0;
		target->sendPacket(res);
		return ;
	}

	if (!tm0 && tm1)
	{
		Player* leader = tm1->getLeader();
		if (leader == NULL)
			return ;

		NetOrganizeTeamRes res;
		res.name = aPlr->getName();
		res.isJoin = 1;
		leader->sendPacket(res);

		return ;
	}

	if (!tm0 && !tm1)
	{
		Team* tm = createTeam(aPlr);
		if (tm == NULL)
			return ;
		doPlayerAddTeam(aPlr, tm);
		doPlayerAddTeam(target, tm);

		NetOrganizeTeamRes res;
		res.name = aPlr->getName();
		res.isJoin = 0;
		target->sendPacket(res);
		return ;
	}
}

void TeamModule::DoAgreeTeamReq(Player* aPlr, cstring& tarName, uint8 isJoin)
{
	Player* target = sWorld.FindPlrByName(tarName);
	if (!target)
		return;

	Team* tm0 = getPlayerTeam(aPlr->getUserId());
	Team* tm1 = getPlayerTeam(target->getUserId());

	if (tm0 && tm1)
		return;

	// 邀请;
	if (isJoin == 0)
	{
		if (tm0)
		{
			// 已经有队伍了;
			aPlr->sendRespnoseMsg(MC_InviterHaveTeam);
			return;
		}

		if (!tm1)
		{
			// 队伍不存在;
			aPlr->sendRespnoseMsg(MC_JoinerTeamNotExist);
			return;
		}

		if (tm1->CanAddTeam(aPlr) == 0)
		{
			doPlayerAddTeam(aPlr, tm1);
			return;
		}
	}
	else
	{
		// 加入;
		if (tm1)
		{
			// 已经有队伍了;
			aPlr->sendRespnoseMsg(MC_HaveTeam);
			return;
		}
		if (!tm0)
		{
			// 已经没有队伍了;
			aPlr->sendRespnoseMsg(MC_TeamNotExist);
			return;
		}

		if (tm0->getLeader() != aPlr)
		{
			// 不是队长不能同意加入;
			aPlr->sendRespnoseMsg(MC_AgreeAddTeamNotLeader);
			return;
		}
		if (tm0->CanAddTeam(target) == 0)
		{
			doPlayerAddTeam(target, tm0);
			return;
		}
	}
	aPlr->sendRespnoseMsg(MC_TeamError);
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
