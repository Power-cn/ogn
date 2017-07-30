#include "stdafx.h"

IMPLEMENT_CLASS(WorldModule, Module);
WorldModule::WorldModule()
{

}

WorldModule::~WorldModule()
{
	while (mMapEntity.size() > 0)
	{
		auto itr = mMapEntity.begin();
		removeEntity(itr->second->getInstanceId());
	}
}

bool WorldModule::Update(float time, float delay)
{
	float64 s0 = DateTime::GetNowAppUS();

	for (auto itr : mMapEntity)
		itr.second->Update(time, delay);

	float64 s1 = DateTime::GetNowAppUS() - s0;
	if (s1 >= 0.02f)
	{
		LOG_ERROR("Update delay: %0.3f", s1);
	}
	return true;
}

bool WorldModule::onEnterWorld(Player* player, Dictionary& dict)
{
	LOG_DEBUG(LogSystem::csl_color_blue, "player [%s] enter world", player->getName());
	return true;
}

bool WorldModule::onLeaveWorld(Player* player, Dictionary& dict)
{
	LOG_DEBUG(LogSystem::csl_color_blue, "player [%s] leave world", player->getName());
	return true;
}

Entity* WorldModule::getEntityByName(const std::string& name)
{
	auto itr = mMapNameEntity.find(name);
	if (itr != mMapNameEntity.end())
		return itr->second;

	return NULL;
}

Entity* WorldModule::getEntity(Guid guid)
{	
	auto itr = mMapEntity.find(guid);
	if (itr != mMapEntity.end())
		return itr->second;

	return NULL;
}

Entity* WorldModule::addEntity(Entity* ent)
{
	auto itr = mMapEntity.find(ent->getInstanceId());
	if (itr != mMapEntity.end())
		return NULL;

	mMapEntity.insert(std::make_pair(ent->getInstanceId(), ent));
	mMapNameEntity.insert(std::make_pair(ent->getName(), ent));
	return ent;
}

void WorldModule::removeEntity(Guid guid)
{
	auto itr = mMapEntity.find(guid);
	if (itr == mMapEntity.end())
		return;
	removeEntity(itr->second);
}

void WorldModule::removeEntity(Entity* ent)
{
	mMapEntity.erase(ent->getGuid());
	mMapNameEntity.erase(ent->getName());
	delete ent;
}

Player* WorldModule::getPlayerByName(const std::string& name)
{
	Entity* entity = getEntityByName(name);
	if (entity == NULL)
		return NULL;

	if (entity->getEntityType() == ET_Player)
		return (Player*)entity;
	return NULL;
}

Player* WorldModule::getPlayerByGuid(Guid guid)
{
	Entity* entity = getEntity(guid);
	if (entity == NULL)
		return NULL;

	if (entity->getEntityType() == ET_Player)
		return (Player*)entity;
	return NULL;
}

Player* WorldModule::getPlayer(uint32 accountId)
{
	auto itr = mMapPlayer.find(accountId);
	if (itr != mMapPlayer.end())
		return itr->second;

	return NULL;
}

Player* WorldModule::addPlayer(Player* plr)
{
	addEntity(plr);
	auto itr = mMapPlayer.find(plr->getAccId());
	if (itr != mMapPlayer.end())
		return NULL;

	mMapPlayer.insert(std::make_pair(plr->getAccId(), plr));
	return plr;
}

void WorldModule::removePlayer(uint32 accountId)
{
	auto itr = mMapPlayer.find(accountId);
	if (itr == mMapPlayer.end())
		return;

	Player* plr = itr->second;
	if (plr == NULL)
		return;
	removePlayer(plr);
}

void WorldModule::removePlayer(Player* plr)
{
	removePlayerToUserId(plr->getUserId());
	mMapPlayer.erase(plr->getAccId());
	removeEntity(plr->getGuid());
}

Player* WorldModule::addPlayerToUserId(Player* plr)
{
	auto itr = mMapUserIdPlayer.find(plr->getUserId());
	if (itr != mMapUserIdPlayer.end())
		return NULL;

	mMapUserIdPlayer.insert(std::make_pair(plr->getUserId(), plr));
	return plr;
}

void WorldModule::removePlayerToUserId(uint32 userId)
{
	auto itr = mMapUserIdPlayer.find(userId);
	if (itr != mMapUserIdPlayer.end())
		mMapUserIdPlayer.erase(itr);
}

Player* WorldModule::getPlayerToUserId(uint32 userId)
{
	auto itr = mMapUserIdPlayer.find(userId);
	if (itr != mMapUserIdPlayer.end())
		return itr->second;
	return NULL;		
}

Npc* WorldModule::addNpc(Npc* npc)
{
	addEntity(npc);
	auto itr = mMapPlayer.find(npc->getNpcId());
	if (itr != mMapPlayer.end())
		return NULL;

	mMapNpc.insert(std::make_pair(npc->getNpcId(), npc));
	return npc;
}

Npc* WorldModule::getNpc(uint32 npcId)
{
	auto itr = mMapNpc.find(npcId);
	if (itr != mMapNpc.end())
		return itr->second;
	return NULL;
}

Npc* WorldModule::getNpcByName(const std::string& name)
{
	Entity* entity = getEntityByName(name);
	if (entity == NULL)
		return NULL;

	if (entity->getEntityType() == ET_Npc)
		return (Npc*)entity;
	return NULL;
}

Npc* WorldModule::getNpcByGuid(Guid guid)
{
	Entity* entity = getEntity(guid);
	if (entity == NULL)
		return NULL;

	if (entity->getEntityType() == ET_Npc)
		return (Npc*)entity;
	return NULL;
}

void WorldModule::removeNpc(uint32 npcId)
{
	auto itr = mMapNpc.find(npcId);
	if (itr == mMapNpc.end())
		return;

	Npc* npc = itr->second;
	if (npc == NULL)
		return;
	removeNpc(npc);
}

void WorldModule::removeNpc(Npc* npc)
{
	mMapNpc.erase(npc->getNpcId());
	removeEntity(npc->getGuid());
}

void WorldModule::sendPacketToAll(Packet& packet)
{
	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	for (auto itr : mMapPlayer)
		itr.second->sendBuffer(in.getPtr(), in.getWPostion());
}

void WorldModule::sendPacketToTarget(EnumChannel ec, Packet& packet, Player* self, uint32 tarUserId /* = 0 */)
{
	switch (ec)
	{
	case EC_WORLD:
		self->sendPacketToWorld(packet);
		break;
	case EC_MAP:
		self->sendPacketToMap(packet);
		break;
	case EC_VIEW:
		self->sendPacketToView(packet);
		break;
	case EC_TEAM:
		self->sendPacketToTeam(packet);
		break;
	case EC_ROOM:
		self->sendPacketToRoom(packet);
		break;
	case EC_TARGET:
		if (tarUserId == 0)
			self->sendPacket(packet);
		else
		{
			Player* tarPlr = sWorld.getPlayerToUserId(tarUserId);
			if (tarPlr)
				tarPlr->sendPacket(packet);
		}
		break;
	}
}

void WorldModule::sendPacketToMsg(EnumChannel ec, const std::string& msg, Player* self)
{
	NetChatMsgNotify nfy;
	nfy.chatMsg = msg;
	nfy.channelType = ec;
	nfy.from = self->getName();
	sendPacketToTarget((EnumChannel)ec, nfy, self);
}