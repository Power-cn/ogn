#include "stdafx.h"

IMPLEMENT_CLASS(Property, Object);
IMPLEMENT_CLASS(EntityProperty, Property);


std::map<int32, std::string> Property::sMapPropertyType;

void Property::Register(int32 type, const std::string& name)
{
	sMapPropertyType.insert(std::make_pair(type, name));
}

std::string Property::GetPropertyName(int32 type)
{
	auto itr = sMapPropertyType.find(type);
	if (itr == sMapPropertyType.end())
		return "";

	return itr->second;
}

REGISTER_PROPERTY(ep_entityType)
REGISTER_PROPERTY(ep_name)
REGISTER_PROPERTY(ep_insId)
REGISTER_PROPERTY(ep_CharId)
REGISTER_PROPERTY(ep_mapId)
REGISTER_PROPERTY(ep_posX)
REGISTER_PROPERTY(ep_posY)
REGISTER_PROPERTY(ep_speed)
REGISTER_PROPERTY(ep_dirPos)
REGISTER_PROPERTY(ep_Level)
REGISTER_PROPERTY(ep_MaxHp)
REGISTER_PROPERTY(ep_MaxMp)
REGISTER_PROPERTY(ep_Hp)
REGISTER_PROPERTY(ep_Mp)
REGISTER_PROPERTY(ep_Attack)
REGISTER_PROPERTY(ep_Defense)

bool EntityProperty::serialize(BinaryStream& bitStream)
{
	CHECK_RETURN(bitStream << accountId, false);
	CHECK_RETURN(bitStream << roleId, false);
	CHECK_RETURN(bitStream << instanceId, false);
	CHECK_RETURN(bitStream << CharId, false);
	
	CHECK_RETURN(bitStream << name, false);

	CHECK_RETURN(bitStream << mapId, false);
	CHECK_RETURN(bitStream << pos, false);
	CHECK_RETURN(bitStream << dir, false);
	return true;
}

bool EntityProperty::deSerialize(BinaryStream& bitStream)
{
	CHECK_RETURN(bitStream >> accountId, false);
	CHECK_RETURN(bitStream >> roleId, false);
	CHECK_RETURN(bitStream >> instanceId, false);
	CHECK_RETURN(bitStream >> CharId, false);
	CHECK_RETURN(bitStream >> name, false);

	CHECK_RETURN(bitStream >> mapId, false);
	CHECK_RETURN(bitStream >> pos, false);
	CHECK_RETURN(bitStream >> dir, false);
	return true;
}

void PropertyHelper::setLevel(Entity* ent, uint8 value)
{
	EntityProperty* pro = EntityGetProperty(ent, EntityProperty);
	if (pro == NULL)
		return;

	pro->mLevel = value;
	ent->addChangeValue(ep_Level, value);
}

uint8 PropertyHelper::getLevel(Entity* ent)
{
	EntityProperty* pro = EntityGetProperty(ent, EntityProperty);
	if (pro == NULL)
		return 0;
	return pro->mLevel;
}

void PropertyHelper::setSpeed(Entity* ent, float32 value)
{
	ent->setSpeed(value);
}

float32 PropertyHelper::getSpeed(Entity* ent)
{
	return ent->getSpeed();
}

void PropertyHelper::setMaxHp(Entity* entity, int32 value)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return;

	pro->mMaxHp = value;
	entity->addChangeValue(ep_MaxHp, value);
}

int32 PropertyHelper::getMaxHp(Entity* entity)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return 0;
	return pro->mMaxHp;
}

void PropertyHelper::setMaxMp(Entity* entity, int32 value)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return;
	pro->mMaxMp = value;
	entity->addChangeValue(ep_MaxMp, value);
}

int32 PropertyHelper::getMaxMp(Entity* entity)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return 0;
	return pro->mMaxMp;
}

void PropertyHelper::setHp(Entity* entity, int32 value)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return;
	pro->mHp = value;
	entity->addChangeValue(ep_Hp, value);
}

int32 PropertyHelper::getHp(Entity* entity)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return 0;
	return pro->mHp;
}

void PropertyHelper::setMp(Entity* entity, int32 value)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return;
	pro->mMp = value;
	entity->addChangeValue(ep_Mp, value);
}

int32 PropertyHelper::getMp(Entity* entity)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return 0;
	return pro->mMp;
}

void PropertyHelper::setAttack(Entity* entity, int32 value)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return;
	pro->mAttack = value;
	entity->addChangeValue(ep_Attack, value);
}

int32 PropertyHelper::getAttack(Entity* entity)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return 0;
	return pro->mAttack;
}

void PropertyHelper::setDefense(Entity* entity, int32 value)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return;

	pro->mDefense = value;
	entity->addChangeValue(ep_Defense, value);
}

int32 PropertyHelper::getDefense(Entity* entity)
{
	EntityProperty* pro = EntityGetProperty(entity, EntityProperty);
	if (pro == NULL)
		return 0;
	return pro->mDefense;
}

uint32 PropertyHelper::CalculateMaxHp(Entity* ent)
{
	CharJson* charJson = INSTANCE(ConfigManager).getCharJson(ent->getCharId());
	if (charJson == NULL)
		return 0;
	PropertyJson* propertyJson = INSTANCE(ConfigManager).getPropertyJson(charJson->PropertyId);
	if (propertyJson == NULL)
		return 0;

	return propertyJson->MaxHp;
}

uint32 PropertyHelper::CalculateMaxMp(Entity* ent)
{
	CharJson* charJson = INSTANCE(ConfigManager).getCharJson(ent->getCharId());
	if (charJson == NULL)
		return 0;
	PropertyJson* propertyJson = INSTANCE(ConfigManager).getPropertyJson(charJson->PropertyId);
	if (propertyJson == NULL)
		return 0;

	return propertyJson->MaxMp;
}

uint32 PropertyHelper::CalculateAttack(Entity* ent)
{
	CharJson* charJson = INSTANCE(ConfigManager).getCharJson(ent->getCharId());
	if (charJson == NULL)
		return 0;
	PropertyJson* propertyJson = INSTANCE(ConfigManager).getPropertyJson(charJson->PropertyId);
	if (propertyJson == NULL)
		return 0;

	return propertyJson->Attack;
}

uint32 PropertyHelper::CalculateDefense(Entity* ent)
{
	CharJson* charJson = INSTANCE(ConfigManager).getCharJson(ent->getCharId());
	if (charJson == NULL)
		return 0;
	PropertyJson* propertyJson = INSTANCE(ConfigManager).getPropertyJson(charJson->PropertyId);
	if (propertyJson == NULL)
		return 0;

	return propertyJson->Defense;
}
