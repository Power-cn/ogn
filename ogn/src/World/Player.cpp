#include "stdafx.h"

Player::Player() :
Entity(ET_Player),
accountId(0),
mJson()
{
	mOnlineTimer = 0;
	mOfflineTimer = 0;
}

Player::~Player()
{

}

bool Player::Update(float time, float delay)
{
	Entity::Update(time, delay);
	return true;
}

void Player::sendPacket(Packet& packet)
{
	if (!session)
		return;
	session->sendPacketToWorld(packet);
}

void Player::sendBuffer(void* data, int32 count)
{
	if (!session)
		return;
	session->sendBufferToWorld(data, count);
}

void Player::sendPacketToView(Packet& packet)
{
	if (!getMap())
		return;

	EntitySet* setEntity = getMap()->getEntityView(getInstanceId());
	if (!setEntity)
		return;

	static char sPacketBuffer[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(sPacketBuffer, PACKET_MAX_LENGTH);
	in << packet;
	for (auto itr : *setEntity)
		itr->sendBuffer(in.getPtr(), in.getWPostion());
}

void Player::sendPacketToMap(Packet& packet)
{
	if (mMap == NULL)
		return;
	mMap->sendPacketToAll(packet);
}

void Player::sendPacketToWorld(Packet& packet)
{
	if (GetModule(WorldModule) == NULL)
		return;

	GetModule(WorldModule)->sendPacketToAll(packet);
}

void Player::sendPacketToTeam(Packet& packet)
{
	GetModule(TeamModule)->sendPacketToTeam(packet, this);
}

void Player::sendPacketToTarget(Packet& packet, Entity* tar)
{
	tar->sendPacket(packet);
}

void Player::sendRespnoseMsg(int32 msgId, std::vector<std::string>* msgParams /* = NULL */)
{
	NetResponseMsgNotify nfy;
	nfy.msgId = msgId;
	if (msgParams)
		nfy.msgParams = *msgParams;

	LOG_INFO("%s: %s", getName().c_str(), INSTANCE(ConfigManager).getMsg(msgId).c_str());

	sendPacket(nfy);
}

void Player::bindSession(Session* session)
{
	session->setPlayer(this);
	setSession(session);
}

void Player::unbindSession()
{
	if (session)
		session->setPlayer(NULL);
	setSession(NULL);
}

Json::Value& Player::GetJson()
{
	return mJson;
}

void Player::onCreate()
{
	LuaEngine::executeScript(this, "player", "onCreate");
}

bool Player::onSaveBegin(Dictionary& dict)
{
	Json::Value& root = GetJson();
	root.clear();
	return true;
}

bool Player::onSave(Dictionary& dict)
{
	onSavejson(dict);
	dict.Add(ep_name, getName());
	dict.Add(ep_CharId, getCharId());
	dict.Add(ep_mapId, getMapId());
	dict.Add(ep_speed, getSpeed());
	dict.Add(ep_posX, getCellX());
	dict.Add(ep_posY, getCellY());
	dict.Add(ep_dirPos, getDirPosition());

	dict.Add(ep_Level, INSTANCE(PropertyHelper).getLevel(this));
	dict.Add(ep_Hp, INSTANCE(PropertyHelper).getHp(this));
	dict.Add(ep_Mp, INSTANCE(PropertyHelper).getMp(this));

	LOG_DEBUG(LogSystem::csl_color_green, "onSave[%s]", getName().c_str());
	for (auto itr : dict.MapVariant())
	{
		const Variant& var = itr.first;
		const Variant& second = itr.second;

		if (var.type() == Variant::TypeString)
			LOG_INFO("type:[%s] value:[%s]", var.valueString(), second.toString().c_str());
		else
			LOG_INFO("type:[%s] value:[%s]", Property::GetPropertyName(var.valueInt32()).c_str(), second.toString().c_str());
	}
	LOG_DEBUG(LogSystem::csl_color_green, "onSave");
	return true;
}

bool Player::onSaveEnd(Dictionary& dict)
{
	return true;
}

bool Player::onLoad(Dictionary& dict)
{
	onLoadJson(dict);

	if (dict.ContainsKey(ep_name))
		setName(dict[ep_name].valueString());

	if (dict.ContainsKey(ep_CharId))
		setCharId(dict[ep_CharId].valueUint32());

	//if (dict.ContainsKey(ep_mapId))
	//{
	//	int32 mapId = dict[ep_mapId].valueUint32();
	//	Map* m = GetModule(MapModule)->getMapByMapId(mapId);
	//	if (m)
	//		setMap(m);
	//}
	if (dict.ContainsKey(ep_speed))
		setSpeed(dict[ep_speed].valueFloat32());

	if (dict.ContainsKey(ep_posX))
	{
		setCellX(dict[ep_posX].valueInt32());
		setCellTarX(getCellX());
	}

	if (dict.ContainsKey(ep_posY))
	{
		setCellY(dict[ep_posY].valueInt32());
		setCellTarY(getCellY());
	}

	if (dict.ContainsKey(ep_dirPos))
		setDirPosition(dict[ep_dirPos].valueInt8());

	if (dict.ContainsKey(ep_Level))
		INSTANCE(PropertyHelper).setLevel(this, dict[ep_Level].valueUint8());

	if (dict.ContainsKey(ep_Hp))
		INSTANCE(PropertyHelper).setHp(this, dict[ep_Hp].valueUint32());

	if (dict.ContainsKey(ep_Mp))
		INSTANCE(PropertyHelper).setMp(this, dict[ep_Mp].valueUint32());

	INSTANCE(PropertyHelper).setMaxHp(this, INSTANCE(PropertyHelper).CalculateMaxHp(this));
	INSTANCE(PropertyHelper).setMaxMp(this, INSTANCE(PropertyHelper).CalculateMaxMp(this));
	INSTANCE(PropertyHelper).setAttack(this, INSTANCE(PropertyHelper).CalculateAttack(this));
	INSTANCE(PropertyHelper).setDefense(this, INSTANCE(PropertyHelper).CalculateDefense(this));

	LOG_DEBUG(LogSystem::csl_color_green, "onLoad[%s]", getName().c_str());
	for (auto itr : dict.MapVariant())
	{
		const Variant& var = itr.first;
		const Variant& second = itr.second;

		if (var.type() == Variant::TypeString)
			LOG_INFO("type:[%s] value:[%s]", var.valueString(), second.toString().c_str());
		else
			LOG_INFO("type:[%s] value:[%s]", Property::GetPropertyName(var.valueInt32()).c_str(), second.toString().c_str());
	}
	LOG_DEBUG(LogSystem::csl_color_green, "onLoad");

	return true;
}

bool Player::onLoadJson(Dictionary& dict)
{
	std::string jsonstr;
	if (dict.ContainsKey("json"))
		jsonstr = dict["json"].valueString();
	Json::Reader jsonReader;
	Json::Value& root = GetJson();
	root.clear();

	if (!jsonReader.parse(jsonstr, root))
		return false;
	onLoadJson(root);
	return true;
}

bool Player::onLoadJson(Json::Value& root)
{
	Json::Value userJson = root["user"]; 

	std::string teststr = userJson["test"].asString();
	uint32 offline_time = userJson["offline"].asUInt();
	SetOfflineTimer(offline_time);

	return true;
}

bool Player::onSavejson(Dictionary& dict)
{
	Json::Value& root = GetJson();
	onSavejson(root);
	std::string jsonstr = root.toStyledString(true);
	dict.Add("json", jsonstr);
	char szBuffer[4096] = { 0 };
	sprintf_s(szBuffer, 4096, "hmset user %d %s", getUserId(), jsonstr.c_str());
	sRedisProxy.sendCmd(szBuffer, NULL, NULL);

	return true;
}

bool Player::onSavejson(Json::Value& root)
{
	Json::Value userJson;
	userJson["userId"] = getUserId();
	userJson["test"] = "test";
	userJson["offline"] = GetOfflineTimer();
	userJson["lasthost"] = session->getHost();

	root["user"] = userJson;

	return true;
}

bool Player::onEnterMap()
{
	Map* map = getMap();
	if (!map)
		return false;
	NetEntityEnterMapNotify nfy;
	nfy.name = this->getName();
	nfy.guid = getInstanceId();
	nfy.mapInsId = getMapInsId();
	nfy.mapId = getMapId();
	sendPacket(nfy);

	float32 cell = map->getMapCell();
	setPos(Utils::Point2ToVector2(getCellPos(), cell, cell));
	return true;
}

bool Player::onLeaveMap()
{
	NetEntityLeaveMapNotify nfy;
	nfy.guid = getInstanceId();
	nfy.mapInsId = getMapInsId();
	nfy.mapId = getMapId();
	sendPacket(nfy);
	return true;
}

bool Player::onEnterView(Entity* tar)
{
	Entity::onEnterView(tar);
	return tar->onEnterPlayerView(this);
}

bool Player::onLeaveView(Entity* tar)
{
	Entity::onLeaveView(tar);
	return tar->onLeavePlayerView(this);
}

bool Player::onEnterPlayerView(Player* plr)
{
	NetPlayerEnterViewNotify nfy;
	nfy.guid = this->getInstanceId();
	nfy.name = this->getName();

	nfy.charId = this->getCharId();
	nfy.entityType = this->getEntityType();
	nfy.mapId = this->getMapId();
	nfy.pos.x = this->getCellX();
	nfy.pos.y = this->getCellY();
	nfy.dir = this->getDirPosition();
	nfy.speed = this->getSpeed();
	nfy.status = this->mStatus->getCurrent();
	plr->sendPacket(nfy);
	//LOG_INFO("[%s] onEnterPlayerView [%s]", plr->getName().c_str(), getName().c_str());

	if (!isMoveTo())
		return true;

	NetEntityMoveToNotify moveNfy;
	moveNfy.guid = this->getInstanceId();
	moveNfy.x = mTarPos.x;
	moveNfy.y = mTarPos.y;
	plr->sendPacket(moveNfy);

	return true;
}

bool Player::onLeavePlayerView(Player* plr)
{
	NetPlayerLeaveViewNotify nfy;
	nfy.guid = this->getInstanceId();
	nfy.mapId = getMapId();
	plr->sendPacket(nfy);

	//LOG_INFO("[%s] onLeavePlayerView [%s]", plr->getName().c_str(), getName().c_str());
	return true;
}
