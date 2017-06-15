#pragma once

enum EnumProperty
{
	ep_none,
	ep_entityType,
	ep_name,
	ep_insId,
	ep_CharId,
	ep_mapId,
	ep_posX,
	ep_posY,
	ep_speed,
	ep_dirPos,
	ep_Level,
	ep_MaxHp,
	ep_MaxMp,
	ep_Hp,
	ep_Mp,
	ep_Attack,
	ep_Defense,
};

class Property : public Object
{
	DECLARE_CLASS(Property);
public:
	static std::map<int32, std::string> sMapPropertyType;
	static void Register(int32 type, const std::string& name);
	static std::string GetPropertyName(int32 type);
};

#define REGISTER_PROPERTY(type) struct type##Struct \
{\
	type##Struct()\
	{\
		Property::Register(type, #type);\
	}\
};\
static type##Struct s_##type##Struct;




class EntityProperty :  public Property
{
	DECLARE_CLASS(EntityProperty);
protected:
	virtual bool serialize(BinaryStream& bitStream);
	virtual bool deSerialize(BinaryStream& bitStream);
public:
	int32			accountId;
	int32			roleId;
	int32			instanceId;
	int32			CharId;
	std::string		name;
	int32			mapId;
	Vector2			pos;
	int8			dir;
	
	uint8			mLevel;
	int32			mMaxHp;
	int32			mMaxMp;
	int32			mHp;
	int32			mMp;
	int32			mAttack;
	int32			mDefense;
};

class Entity;

class PropertyHelper
{
public:
	void setLevel(Entity* ent, uint8 value);
	uint8 getLevel(Entity* ent);

	void setSpeed(Entity* ent, float32 value);
	float32 getSpeed(Entity* ent);

	void setMaxHp(Entity* entity, int32 value);
	int32 getMaxHp(Entity* entity);

	void setMaxMp(Entity* entity, int32 value);
	int32 getMaxMp(Entity* entity);

	void setHp(Entity* entity, int32 value);
	int32 getHp(Entity* entity);

	void setMp(Entity* entity, int32 value);
	int32 getMp(Entity* entity);

	void setAttack(Entity* entity, int32 value);
	int32 getAttack(Entity* entity);

	void setDefense(Entity* entity, int32 value);
	int32 getDefense(Entity* entity);

	uint32 CalculateMaxHp(Entity* ent);
	uint32 CalculateMaxMp(Entity* ent);
	uint32 CalculateAttack(Entity* ent);
	uint32 CalculateDefense(Entity* ent);
};