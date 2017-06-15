#pragma once

enum EntityType
{
	ET_None,
	ET_Player,
	ET_Npc,
};

class Player;
class Entity;

typedef std::set<Entity*> EntitySet;

class Entity : public Object
{
	friend class Map;
	friend class Player;
public:
	Entity(int8 type = 0);
	~Entity();
public:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy() { return true; }
	virtual std::string getName() { return mName; }
	virtual void setName(const std::string& name) { mName = name; }
	void setLine(uint8 l) { mLine = l; }
	uint8 getLine() { return mLine; }
public:
	virtual	void sendPacket(Packet& packet) {}
	virtual void sendBuffer(void* data, int32 count) {}
	virtual void sendPacketToView(Packet& packet) {}
	virtual void sendPacketToMap(Packet& packet) {}
	virtual void sendPacketToWorld(Packet& packet) {}
	virtual void sendPacketToTeam(Packet& packet) {}
	virtual void sendPacketToTarget(Packet& packet, Entity* tar) {}
	virtual void sendRespnoseMsg(int32 msgId, std::vector<std::string>* msgParams = NULL) {}
	virtual bool changeMapByMapInsId(int32 mapInsId);
	virtual bool changeMapByMapId(int32 mapId);
	virtual bool changeMapByMap(Map* map);
protected:
	virtual bool onEnterMap() { return true; }
	virtual bool onLeaveMap() { return true; }

	// 进入目标视野;
	virtual bool onEnterView(Entity* tar);
	virtual bool onLeaveView(Entity* tar);

	virtual bool onEnterPlayerView(Player* plr) { return true; }
	virtual bool onLeavePlayerView(Player* plr) { return true; }
public:
	Guid									instanceId;
	uint8									entityType;
	uint32									mCharId;
	uint8									mSex;
	Map*									mMap;
	Point2									mPos;
	Point2									mTarPos;
	float32									mSpeed;
	FSM*									mStatus;
	int8									mDirPosition;
	std::string								mName;
	std::list<Point2>						mPoints;
	float32									mCheckTime;
	Timer									mTimer;
	uint8									mLine;
public:
	Vector2 mPosition;
	Vector2 mTarPosition;
	Vector2 mDir;

	Vector2& getPos() { return mPosition; }
	Vector2& getTarPos() { return mTarPosition; }
	Vector2& getDir() { return mDir; }
	void setPos(const Vector2& pos) { mPosition = pos; }
	void setTarPos(const Vector2& tarPos) { mTarPosition = tarPos; }
	void setDir(const Vector2& dir) { mDir = dir; }
public:

	Point2& getCellPos() { return mPos; }
	int32 getCellX() { return mPos.x; }
	int32 getCellY() { return mPos.y; }
	int8 getDirPosition() { return mDirPosition; }
	void setDirPosition(int8 dir);
	void setCellPosition(Point2& pos);
	void setCellX(int32 x);
	void setCellY(int32 y);
	void setCellTarPos(Point2& pos) { mTarPos = pos; }
	void setCellTarX(int32 x) { mTarPos.x = x; }
	void setCellTarY(int32 y) { mTarPos.y = y; }
	Point2& getCellTarPos() { return mTarPos; }
	int32 getCellTarX() { return mTarPos.x; }
	int32 getCellTarY() { return mTarPos.y; }
	void setSpeed(float32 speed);
	float32 getSpeed() { return mSpeed; }
	void ChangeStatus(int32 status);
	void addChangeValue(const Variant& key, const Variant& value);
	std::list<Point2>& getPoints() { return mPoints; }
	void setCheckView(bool checkView) { mCheckView = checkView; }
public:
	EntitySet* getView();
	Map* getMap() { return mMap; }
	void setMap(Map* map) { mMap = map; }
	uint32 getMapId();
	void setMapId(uint32 mapId);
	uint32 getMapInsId();
	void setMapInsId(uint32 mapInsId);
	void setInstanceId(Guid uId) { instanceId = uId; }
	Guid getInstanceId() { return instanceId; }
	Guid getGuid() { return instanceId; }
	uint8 getEntityType() { return entityType; }

	void setCharId(uint32 charId) { mCharId = charId; }
	virtual uint32 getCharId() { return mCharId; }
	void setSex(uint8 sex) { mSex = sex; }
	uint8 getSex() { return mSex; }
	Property* getProperty(const std::string& name);
	Property* addProperty(Property* property);
	void removeProperty(const std::string& name);
	Variant& getVariant(const Variant& value);
	bool ContainsVariantKey(const Variant& key);
	void addVariant(const Variant& key, const Variant& value);
	void removeVariant(const Variant& key);
	Dictionary& getDictProperty() { mDictProperty; }

	bool serializeProperty(BinaryStream& stream);
	bool deSerializeProperty(BinaryStream& stream);

	bool MoveTo();
protected:
	int32 onTimerChangeProperty(TimerEvent& e);
	int32 onTimerCheckView(TimerEvent& e);
protected:
	void ChangePos(int32 lastX, int32 lastY, int32 x, int32 y);
	bool isMoveTo();
protected:
	std::map<std::string, Property*>		mMapProperty;
	Dictionary								mDictProperty;
	Dictionary								mDictChangePro;

	bool									mCheckView;
};

#define EntityGetProperty(tar, class_name) (class_name*)(tar)->getProperty(#class_name)