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

bool Player::Initialize()
{
	Entity::Initialize();
	mTimer.addEventListener((EventCallback)&Player::onTimerSaveDB, this, 60.f);
	return true;
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
	sWorld.sendPacketToAll(packet);
}

void Player::sendPacketToTeam(Packet& packet)
{
	sTeam.sendPacketToTeam(packet, this);
}

void Player::sendPacketToRoom(Packet& packet)
{
	sRoom.sendPacketToTeam(packet, this);
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

	LOG_INFO("%s: %s", getName(), INSTANCE(ConfigManager).getMsg(msgId).c_str());

	sendPacket(nfy);
}

void Player::sendPacketToMsg(EnumChannel ec, const std::string& msg)
{
	sWorld.sendPacketToMsg((EnumChannel)ec, msg, this);
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

void Player::DoCreateCharacter(Dictionary& dict, DBRoleInfo& dbRoleInfo)
{
	Dictionary aPropertyDict;

	uint32 charId = INSTANCE(ConfigManager).getCharJsonRandId();
	float32 fSpeed = 0.f;
	uint32 MaxHp = 0;
	uint32 MaxMp = 0;

	CharJson* charJson = INSTANCE(ConfigManager).getCharJson(charId);
	if (charJson) {
		setSex(charJson->Sex);
		PropertyJson* propertyJson = INSTANCE(ConfigManager).getPropertyJson(charJson->PropertyId);
		if (propertyJson) {
			fSpeed = propertyJson->Speed;
			MaxHp = propertyJson->MaxHp;
			MaxMp = propertyJson->MaxMp;
		}
	}

	aPropertyDict.Add(ep_name, dbRoleInfo.name);
	aPropertyDict.Add(ep_CharId, charId);
	aPropertyDict.Add(ep_mapId, (uint32)1);
	aPropertyDict.Add(ep_speed, fSpeed);
	aPropertyDict.Add(ep_posX, (int32)0);
	aPropertyDict.Add(ep_posY, (int32)0);
	aPropertyDict.Add(ep_dirPos, (int8)D_UP);
	aPropertyDict.Add(ep_Level, (uint8)1);
	aPropertyDict.Add(ep_Hp, MaxHp);
	aPropertyDict.Add(ep_Mp, MaxMp);

	BinaryStream bytes;
	bytes << aPropertyDict;
	dict.Add("Property", bytes);
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
	onSaveProperty(dict);

	LOG_DEBUG(LogSystem::csl_color_green, "onSave[%s]", getName());
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
	onLoadProperty(dict);

	LOG_DEBUG(LogSystem::csl_color_green, "onLoad[%s]", getName());
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

bool Player::onLoadProperty(Dictionary& dict)
{
	Variant* varBytes = dict.GetVariant("Property");
	if (varBytes == NULL) return true;
	BinaryStream bytes = (BinaryStream&)varBytes->valueBytes();
	Dictionary aProperyDict;
	bytes >> aProperyDict;

	setName(aProperyDict[ep_name].valueString());
	setCharId(aProperyDict[ep_CharId].valueUint32());
	setSpeed(aProperyDict[ep_speed].valueFloat32());
	setCellX(aProperyDict[ep_posX].valueInt32());
	setCellTarX(getCellX());
	setCellY(aProperyDict[ep_posY].valueInt32());
	setCellTarY(getCellY());
	setDirPosition(aProperyDict[ep_dirPos].valueInt8());

	INSTANCE(PropertyHelper).setLevel(this, aProperyDict[ep_Level].valueUint8());
	INSTANCE(PropertyHelper).setHp(this, aProperyDict[ep_Hp].valueUint32());
	INSTANCE(PropertyHelper).setMp(this, aProperyDict[ep_Mp].valueUint32());

	INSTANCE(PropertyHelper).setMaxHp(this, INSTANCE(PropertyHelper).CalculateMaxHp(this));
	INSTANCE(PropertyHelper).setMaxMp(this, INSTANCE(PropertyHelper).CalculateMaxMp(this));
	INSTANCE(PropertyHelper).setAttack(this, INSTANCE(PropertyHelper).CalculateAttack(this));
	INSTANCE(PropertyHelper).setDefense(this, INSTANCE(PropertyHelper).CalculateDefense(this));
	return true;
}

bool Player::onSavejson(Dictionary& dict)
{
	Json::Value& root = GetJson();
	onSavejson(root);
	std::string jsonstr = root.toStyledString(true);
	dict.Add("json", jsonstr);
	char szBuffer[4096] = { 0 };
	sprintf_s(szBuffer, 4096, "hmset %s %d %s", sUser, getUserId(), jsonstr.c_str());
	sRedisProxy.sendCmd(szBuffer, NULL, NULL);
	return true;
}

bool Player::onSavejson(Json::Value& root)
{
	Json::Value userJson;
	userJson["userId"] = getUserId();
	userJson["test"] = "test";
	userJson["online"] = GetOfflineTimer();
	userJson["offline"] = GetOfflineTimer();
	userJson["lasthost"] = session->getHost();
	root["user"] = userJson;
	return true;
}

bool Player::onSaveProperty(Dictionary& dict)
{
	Dictionary aProperyDict;
	aProperyDict.Add(ep_name, getName());
	aProperyDict.Add(ep_CharId, getCharId());
	aProperyDict.Add(ep_mapId, getMapId());
	aProperyDict.Add(ep_speed, getSpeed());
	aProperyDict.Add(ep_posX, getCellX());
	aProperyDict.Add(ep_posY, getCellY());
	aProperyDict.Add(ep_dirPos, getDirPosition());
	aProperyDict.Add(ep_Level, INSTANCE(PropertyHelper).getLevel(this));
	aProperyDict.Add(ep_Hp, INSTANCE(PropertyHelper).getHp(this));
	aProperyDict.Add(ep_Mp, INSTANCE(PropertyHelper).getMp(this));

	BinaryStream bytes;
	bytes << aProperyDict;
	dict.Add("Property", bytes);
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

int32 Player::onTimerSaveDB(TimerEvent& e)
{
	Dictionary dict;
	sApp.doPlayerSave(this, dict);
	return 0;
}
