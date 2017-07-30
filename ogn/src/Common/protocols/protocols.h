#pragma once

enum PACKET_ID_ENUM {
ID_NetFirst,
ID_NetSessionEnterNotify,
ID_NetSessionLeaveNotify,
ID_NetPingNotify,
ID_NetLoginReq,
ID_NetLoginRes,
ID_NetGmMsg,
ID_NetQueryRoleReq,
ID_NetQueryRoleRes,
ID_NetPropertyReq,
ID_NetPropertyRes,
ID_NetEntityPropertyNotify,
ID_NetEntityEnterMapNotify,
ID_NetEntityLeaveMapNotify,
ID_NetPlayerEnterViewNotify,
ID_NetPlayerLeaveViewNotify,
ID_NetNpcEnterViewNotify,
ID_NetNpcLeaveViewNotify,
ID_NetEntityMoveNotify,
ID_NetEntityMoveToNotify,
ID_NetEntityAttackNotify,
ID_NetEntityFollowNotify,
ID_NetEntityCancelFollowNotify,
ID_NetChatMsgNotify,
ID_NetResponseMsgNotify,
ID_NetCreateTeamReq,
ID_NetCreateTeamRes,
ID_NetOrganizeTeamReq,
ID_NetOrganizeTeamRes,
ID_NetAgreeTeamReq,
ID_NetTeamInfoRes,
ID_NetTeamListReq,
ID_NetTeamListRes,
ID_NetCreateRoomReq,
ID_NetCreateRoomRes,
ID_NetEnterRoomReq,
ID_NetEnterRoomRes,
ID_NetEnterRoomNotify,
ID_NetLeaveRoomReq,
ID_NetLeaveRoomRes,
ID_NetChangeRoomMasterReq,
ID_NetChangeRoomMasterRes,
ID_NetRoomListReq,
ID_NetRoomListRes,
ID_NetEnd,

};

class DBAccountInfo : public Object {
public:
	DBAccountInfo() {
id = 0;
user = "";
password;
name = "";
email = "";
phone = "";

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << id);
CHECK(bytes << user);
CHECK(bytes << password);
CHECK(bytes << name);
CHECK(bytes << email);
CHECK(bytes << phone);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> id);
CHECK(bytes >> user);
CHECK(bytes >> password);
CHECK(bytes >> name);
CHECK(bytes >> email);
CHECK(bytes >> phone);

		return true;
	}
public:
int32 id;
std::string user;
BinaryStream password;
std::string name;
std::string email;
std::string phone;

};

class DBRoleInfo : public Object {
public:
	DBRoleInfo() {
id = 0;
accountId = 0;
name = "";
property;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << id);
CHECK(bytes << accountId);
CHECK(bytes << name);
CHECK(bytes << property);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> id);
CHECK(bytes >> accountId);
CHECK(bytes >> name);
CHECK(bytes >> property);

		return true;
	}
public:
int32 id;
int32 accountId;
std::string name;
BinaryStream property;

};

class Point : public Object {
public:
	Point() {
x = 0;
y = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << x);
CHECK(bytes << y);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> x);
CHECK(bytes >> y);

		return true;
	}
public:
int32 x;
int32 y;

};

class TeamEntityInfo : public Object {
public:
	TeamEntityInfo() {
name = "";
instanceId = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << instanceId);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> instanceId);

		return true;
	}
public:
std::string name;
int32 instanceId;

};

class TeamInfo : public Object {
public:
	TeamInfo() {
teamId = 0;
leaderInsId = 0;


	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << teamId);
CHECK(bytes << leaderInsId);
uint32 teamEntityInfos_Size = (uint32)teamEntityInfos.size();
bytes << teamEntityInfos_Size;
for (uint32 teamEntityInfos_i = 0; teamEntityInfos_i < teamEntityInfos_Size; ++teamEntityInfos_i) {
	bytes << teamEntityInfos[teamEntityInfos_i];
}

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> teamId);
CHECK(bytes >> leaderInsId);
uint32 teamEntityInfos_Size = 0;
bytes >> teamEntityInfos_Size;
for (uint32 teamEntityInfos_i = 0; teamEntityInfos_i < teamEntityInfos_Size; ++teamEntityInfos_i) {
	TeamEntityInfo teamEntityInfos_info;
	bytes >> teamEntityInfos_info;
	teamEntityInfos.push_back(teamEntityInfos_info);
}

		return true;
	}
public:
int32 teamId;
int32 leaderInsId;
std::vector<TeamEntityInfo> teamEntityInfos;

};

class RoomPlayerInfo : public Object {
public:
	RoomPlayerInfo() {
name = "";
userId = 0;
insId = 0;
state = 0;

	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << userId);
CHECK(bytes << insId);
CHECK(bytes << state);

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> userId);
CHECK(bytes >> insId);
CHECK(bytes >> state);

		return true;
	}
public:
std::string name;
uint32 userId;
uint64 insId;
uint8 state;

};

class RoomInfo : public Object {
public:
	RoomInfo() {
roomId = 0;
masterUserId = 0;
password = "";
name = "";
maxCount = 0;


	}

	bool operator >> (BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << masterUserId);
CHECK(bytes << password);
CHECK(bytes << name);
CHECK(bytes << maxCount);
uint32 roomPlayerInfos_Size = (uint32)roomPlayerInfos.size();
bytes << roomPlayerInfos_Size;
for (uint32 roomPlayerInfos_i = 0; roomPlayerInfos_i < roomPlayerInfos_Size; ++roomPlayerInfos_i) {
	bytes << roomPlayerInfos[roomPlayerInfos_i];
}

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> masterUserId);
CHECK(bytes >> password);
CHECK(bytes >> name);
CHECK(bytes >> maxCount);
uint32 roomPlayerInfos_Size = 0;
bytes >> roomPlayerInfos_Size;
for (uint32 roomPlayerInfos_i = 0; roomPlayerInfos_i < roomPlayerInfos_Size; ++roomPlayerInfos_i) {
	RoomPlayerInfo roomPlayerInfos_info;
	bytes >> roomPlayerInfos_info;
	roomPlayerInfos.push_back(roomPlayerInfos_info);
}

		return true;
	}
public:
uint32 roomId;
uint32 masterUserId;
std::string password;
std::string name;
uint32 maxCount;
std::vector<RoomPlayerInfo> roomPlayerInfos;

};

class NetFirst : public Packet {
public:
	NetFirst():
	Packet(ID_NetFirst) {


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << sbytes);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> sbytes);

		return true;
	}
public:
BinaryStream sbytes;

};

class NetSessionEnterNotify : public Packet {
public:
	NetSessionEnterNotify():
	Packet(ID_NetSessionEnterNotify) {
host = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << host);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> host);

		return true;
	}
public:
std::string host;

};

class NetSessionLeaveNotify : public Packet {
public:
	NetSessionLeaveNotify():
	Packet(ID_NetSessionLeaveNotify) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetPingNotify : public Packet {
public:
	NetPingNotify():
	Packet(ID_NetPingNotify) {
time = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << time);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> time);

		return true;
	}
public:
uint32 time;

};

class NetLoginReq : public Packet {
public:
	NetLoginReq():
	Packet(ID_NetLoginReq) {
user = "";
password = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << user);
CHECK(bytes << password);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> user);
CHECK(bytes >> password);

		return true;
	}
public:
std::string user;
std::string password;

};

class NetLoginRes : public Packet {
public:
	NetLoginRes():
	Packet(ID_NetLoginRes) {
result = 0;
guid = 0;
accountInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << guid);
CHECK(bytes << accountInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> guid);
CHECK(bytes >> accountInfo);

		return true;
	}
public:
int32 result;
int64 guid;
DBAccountInfo accountInfo;

};

class NetGmMsg : public Packet {
public:
	NetGmMsg():
	Packet(ID_NetGmMsg) {
name = "";


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
uint32 gmParams_Size = (uint32)gmParams.size();
bytes << gmParams_Size;
for (uint32 gmParams_i = 0; gmParams_i < gmParams_Size; ++gmParams_i) {
	bytes << gmParams[gmParams_i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
uint32 gmParams_Size = 0;
bytes >> gmParams_Size;
for (uint32 gmParams_i = 0; gmParams_i < gmParams_Size; ++gmParams_i) {
	std::string gmParams_info;
	bytes >> gmParams_info;
	gmParams.push_back(gmParams_info);
}

		return true;
	}
public:
std::string name;
std::vector<std::string> gmParams;

};

class NetQueryRoleReq : public Packet {
public:
	NetQueryRoleReq():
	Packet(ID_NetQueryRoleReq) {
accountId = 0;
user = "";
roleCount = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << accountId);
CHECK(bytes << user);
CHECK(bytes << roleCount);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> accountId);
CHECK(bytes >> user);
CHECK(bytes >> roleCount);

		return true;
	}
public:
int32 accountId;
std::string user;
int8 roleCount;

};

class NetQueryRoleRes : public Packet {
public:
	NetQueryRoleRes():
	Packet(ID_NetQueryRoleRes) {
accountId = 0;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << accountId);
uint32 roleInfos_Size = (uint32)roleInfos.size();
bytes << roleInfos_Size;
for (uint32 roleInfos_i = 0; roleInfos_i < roleInfos_Size; ++roleInfos_i) {
	bytes << roleInfos[roleInfos_i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> accountId);
uint32 roleInfos_Size = 0;
bytes >> roleInfos_Size;
for (uint32 roleInfos_i = 0; roleInfos_i < roleInfos_Size; ++roleInfos_i) {
	DBRoleInfo roleInfos_info;
	bytes >> roleInfos_info;
	roleInfos.push_back(roleInfos_info);
}

		return true;
	}
public:
int32 accountId;
std::vector<DBRoleInfo> roleInfos;

};

class NetPropertyReq : public Packet {
public:
	NetPropertyReq():
	Packet(ID_NetPropertyReq) {
guid = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);

		return true;
	}
public:
int64 guid;

};

class NetPropertyRes : public Packet {
public:
	NetPropertyRes():
	Packet(ID_NetPropertyRes) {
guid = 0;
property;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << property);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> property);

		return true;
	}
public:
int64 guid;
BinaryStream property;

};

class NetEntityPropertyNotify : public Packet {
public:
	NetEntityPropertyNotify():
	Packet(ID_NetEntityPropertyNotify) {
guid = 0;
property;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << property);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> property);

		return true;
	}
public:
int64 guid;
BinaryStream property;

};

class NetEntityEnterMapNotify : public Packet {
public:
	NetEntityEnterMapNotify():
	Packet(ID_NetEntityEnterMapNotify) {
name = "";
guid = 0;
mapInsId = 0;
mapId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << guid);
CHECK(bytes << mapInsId);
CHECK(bytes << mapId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> guid);
CHECK(bytes >> mapInsId);
CHECK(bytes >> mapId);

		return true;
	}
public:
std::string name;
int64 guid;
int32 mapInsId;
int32 mapId;

};

class NetEntityLeaveMapNotify : public Packet {
public:
	NetEntityLeaveMapNotify():
	Packet(ID_NetEntityLeaveMapNotify) {
guid = 0;
mapInsId = 0;
mapId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << mapInsId);
CHECK(bytes << mapId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> mapInsId);
CHECK(bytes >> mapId);

		return true;
	}
public:
int64 guid;
int32 mapInsId;
int32 mapId;

};

class NetPlayerEnterViewNotify : public Packet {
public:
	NetPlayerEnterViewNotify():
	Packet(ID_NetPlayerEnterViewNotify) {
name = "";
guid = 0;
entityType = 0;
mapId = 0;
charId = 0;
pos;
speed = 0.f;
status = 0;
dir = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << guid);
CHECK(bytes << entityType);
CHECK(bytes << mapId);
CHECK(bytes << charId);
CHECK(bytes << pos);
CHECK(bytes << speed);
CHECK(bytes << status);
CHECK(bytes << dir);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> guid);
CHECK(bytes >> entityType);
CHECK(bytes >> mapId);
CHECK(bytes >> charId);
CHECK(bytes >> pos);
CHECK(bytes >> speed);
CHECK(bytes >> status);
CHECK(bytes >> dir);

		return true;
	}
public:
std::string name;
int64 guid;
int32 entityType;
int32 mapId;
int32 charId;
Point pos;
float32 speed;
int32 status;
int32 dir;

};

class NetPlayerLeaveViewNotify : public Packet {
public:
	NetPlayerLeaveViewNotify():
	Packet(ID_NetPlayerLeaveViewNotify) {
guid = 0;
mapId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << mapId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> mapId);

		return true;
	}
public:
int64 guid;
int32 mapId;

};

class NetNpcEnterViewNotify : public Packet {
public:
	NetNpcEnterViewNotify():
	Packet(ID_NetNpcEnterViewNotify) {
name = "";
guid = 0;
entityType = 0;
mapId = 0;
npcId = 0;
pos;
speed = 0.f;
status = 0;
dir = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << guid);
CHECK(bytes << entityType);
CHECK(bytes << mapId);
CHECK(bytes << npcId);
CHECK(bytes << pos);
CHECK(bytes << speed);
CHECK(bytes << status);
CHECK(bytes << dir);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> guid);
CHECK(bytes >> entityType);
CHECK(bytes >> mapId);
CHECK(bytes >> npcId);
CHECK(bytes >> pos);
CHECK(bytes >> speed);
CHECK(bytes >> status);
CHECK(bytes >> dir);

		return true;
	}
public:
std::string name;
int64 guid;
int32 entityType;
int32 mapId;
int32 npcId;
Point pos;
float32 speed;
int32 status;
int32 dir;

};

class NetNpcLeaveViewNotify : public Packet {
public:
	NetNpcLeaveViewNotify():
	Packet(ID_NetNpcLeaveViewNotify) {
guid = 0;
mapId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << mapId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> mapId);

		return true;
	}
public:
int64 guid;
int32 mapId;

};

class NetEntityMoveNotify : public Packet {
public:
	NetEntityMoveNotify():
	Packet(ID_NetEntityMoveNotify) {
guid = 0;
startDir = 0;
x = 0;
y = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << startDir);
CHECK(bytes << x);
CHECK(bytes << y);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> startDir);
CHECK(bytes >> x);
CHECK(bytes >> y);

		return true;
	}
public:
int64 guid;
int8 startDir;
int32 x;
int32 y;

};

class NetEntityMoveToNotify : public Packet {
public:
	NetEntityMoveToNotify():
	Packet(ID_NetEntityMoveToNotify) {
guid = 0;
x = 0;
y = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << x);
CHECK(bytes << y);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> x);
CHECK(bytes >> y);

		return true;
	}
public:
int64 guid;
int32 x;
int32 y;

};

class NetEntityAttackNotify : public Packet {
public:
	NetEntityAttackNotify():
	Packet(ID_NetEntityAttackNotify) {
guid = 0;
index = 0;
tarGuid = 0;
dirPos = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << guid);
CHECK(bytes << index);
CHECK(bytes << tarGuid);
CHECK(bytes << dirPos);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> guid);
CHECK(bytes >> index);
CHECK(bytes >> tarGuid);
CHECK(bytes >> dirPos);

		return true;
	}
public:
int64 guid;
int8 index;
int64 tarGuid;
int8 dirPos;

};

class NetEntityFollowNotify : public Packet {
public:
	NetEntityFollowNotify():
	Packet(ID_NetEntityFollowNotify) {
tarGuid = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << tarGuid);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> tarGuid);

		return true;
	}
public:
int64 tarGuid;

};

class NetEntityCancelFollowNotify : public Packet {
public:
	NetEntityCancelFollowNotify():
	Packet(ID_NetEntityCancelFollowNotify) {
tarGuid = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << tarGuid);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> tarGuid);

		return true;
	}
public:
int64 tarGuid;

};

class NetChatMsgNotify : public Packet {
public:
	NetChatMsgNotify():
	Packet(ID_NetChatMsgNotify) {
from = "";
to = "";
channelType = 0;
chatMsg = "";
chatData;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << from);
CHECK(bytes << to);
CHECK(bytes << channelType);
CHECK(bytes << chatMsg);
CHECK(bytes << chatData);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> from);
CHECK(bytes >> to);
CHECK(bytes >> channelType);
CHECK(bytes >> chatMsg);
CHECK(bytes >> chatData);

		return true;
	}
public:
std::string from;
std::string to;
int8 channelType;
std::string chatMsg;
BinaryStream chatData;

};

class NetResponseMsgNotify : public Packet {
public:
	NetResponseMsgNotify():
	Packet(ID_NetResponseMsgNotify) {
msgId = 0;


	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << msgId);
uint32 msgParams_Size = (uint32)msgParams.size();
bytes << msgParams_Size;
for (uint32 msgParams_i = 0; msgParams_i < msgParams_Size; ++msgParams_i) {
	bytes << msgParams[msgParams_i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> msgId);
uint32 msgParams_Size = 0;
bytes >> msgParams_Size;
for (uint32 msgParams_i = 0; msgParams_i < msgParams_Size; ++msgParams_i) {
	std::string msgParams_info;
	bytes >> msgParams_info;
	msgParams.push_back(msgParams_info);
}

		return true;
	}
public:
int32 msgId;
std::vector<std::string> msgParams;

};

class NetCreateTeamReq : public Packet {
public:
	NetCreateTeamReq():
	Packet(ID_NetCreateTeamReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetCreateTeamRes : public Packet {
public:
	NetCreateTeamRes():
	Packet(ID_NetCreateTeamRes) {
teamInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << teamInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> teamInfo);

		return true;
	}
public:
TeamInfo teamInfo;

};

class NetOrganizeTeamReq : public Packet {
public:
	NetOrganizeTeamReq():
	Packet(ID_NetOrganizeTeamReq) {
name = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);

		return true;
	}
public:
std::string name;

};

class NetOrganizeTeamRes : public Packet {
public:
	NetOrganizeTeamRes():
	Packet(ID_NetOrganizeTeamRes) {
name = "";
isJoin = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << isJoin);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> isJoin);

		return true;
	}
public:
std::string name;
int8 isJoin;

};

class NetAgreeTeamReq : public Packet {
public:
	NetAgreeTeamReq():
	Packet(ID_NetAgreeTeamReq) {
name = "";
isJoin = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);
CHECK(bytes << isJoin);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
CHECK(bytes >> isJoin);

		return true;
	}
public:
std::string name;
int8 isJoin;

};

class NetTeamInfoRes : public Packet {
public:
	NetTeamInfoRes():
	Packet(ID_NetTeamInfoRes) {
teamInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << teamInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> teamInfo);

		return true;
	}
public:
TeamInfo teamInfo;

};

class NetTeamListReq : public Packet {
public:
	NetTeamListReq():
	Packet(ID_NetTeamListReq) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};

class NetTeamListRes : public Packet {
public:
	NetTeamListRes():
	Packet(ID_NetTeamListRes) {


	}

	bool OnSerialize(BinaryStream& bytes) {
uint32 teamInfos_Size = (uint32)teamInfos.size();
bytes << teamInfos_Size;
for (uint32 teamInfos_i = 0; teamInfos_i < teamInfos_Size; ++teamInfos_i) {
	bytes << teamInfos[teamInfos_i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
uint32 teamInfos_Size = 0;
bytes >> teamInfos_Size;
for (uint32 teamInfos_i = 0; teamInfos_i < teamInfos_Size; ++teamInfos_i) {
	TeamInfo teamInfos_info;
	bytes >> teamInfos_info;
	teamInfos.push_back(teamInfos_info);
}

		return true;
	}
public:
std::vector<TeamInfo> teamInfos;

};

class NetCreateRoomReq : public Packet {
public:
	NetCreateRoomReq():
	Packet(ID_NetCreateRoomReq) {
name = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << name);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);

		return true;
	}
public:
std::string name;

};

class NetCreateRoomRes : public Packet {
public:
	NetCreateRoomRes():
	Packet(ID_NetCreateRoomRes) {
roomInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomInfo);

		return true;
	}
public:
RoomInfo roomInfo;

};

class NetEnterRoomReq : public Packet {
public:
	NetEnterRoomReq():
	Packet(ID_NetEnterRoomReq) {
roomId = 0;
password = "";

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << password);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> password);

		return true;
	}
public:
uint32 roomId;
std::string password;

};

class NetEnterRoomRes : public Packet {
public:
	NetEnterRoomRes():
	Packet(ID_NetEnterRoomRes) {
roomInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomInfo);

		return true;
	}
public:
RoomInfo roomInfo;

};

class NetEnterRoomNotify : public Packet {
public:
	NetEnterRoomNotify():
	Packet(ID_NetEnterRoomNotify) {
roomId = 0;
roomPlayerInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << roomPlayerInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> roomPlayerInfo);

		return true;
	}
public:
uint32 roomId;
RoomPlayerInfo roomPlayerInfo;

};

class NetLeaveRoomReq : public Packet {
public:
	NetLeaveRoomReq():
	Packet(ID_NetLeaveRoomReq) {
roomId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);

		return true;
	}
public:
uint32 roomId;

};

class NetLeaveRoomRes : public Packet {
public:
	NetLeaveRoomRes():
	Packet(ID_NetLeaveRoomRes) {
roomId = 0;
userId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << userId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> userId);

		return true;
	}
public:
uint32 roomId;
uint32 userId;

};

class NetChangeRoomMasterReq : public Packet {
public:
	NetChangeRoomMasterReq():
	Packet(ID_NetChangeRoomMasterReq) {
roomId = 0;
userId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << userId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> userId);

		return true;
	}
public:
uint32 roomId;
uint32 userId;

};

class NetChangeRoomMasterRes : public Packet {
public:
	NetChangeRoomMasterRes():
	Packet(ID_NetChangeRoomMasterRes) {
roomId = 0;
masterUserId = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << roomId);
CHECK(bytes << masterUserId);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> roomId);
CHECK(bytes >> masterUserId);

		return true;
	}
public:
uint32 roomId;
uint32 masterUserId;

};

class NetRoomListReq : public Packet {
public:
	NetRoomListReq():
	Packet(ID_NetRoomListReq) {
start = 0;
count = 0;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << start);
CHECK(bytes << count);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> start);
CHECK(bytes >> count);

		return true;
	}
public:
uint32 start;
uint32 count;

};

class NetRoomListRes : public Packet {
public:
	NetRoomListRes():
	Packet(ID_NetRoomListRes) {


	}

	bool OnSerialize(BinaryStream& bytes) {
uint32 roomInfoInfos_Size = (uint32)roomInfoInfos.size();
bytes << roomInfoInfos_Size;
for (uint32 roomInfoInfos_i = 0; roomInfoInfos_i < roomInfoInfos_Size; ++roomInfoInfos_i) {
	bytes << roomInfoInfos[roomInfoInfos_i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
uint32 roomInfoInfos_Size = 0;
bytes >> roomInfoInfos_Size;
for (uint32 roomInfoInfos_i = 0; roomInfoInfos_i < roomInfoInfos_Size; ++roomInfoInfos_i) {
	RoomInfo roomInfoInfos_info;
	bytes >> roomInfoInfos_info;
	roomInfoInfos.push_back(roomInfoInfos_info);
}

		return true;
	}
public:
std::vector<RoomInfo> roomInfoInfos;

};

class NetEnd : public Packet {
public:
	NetEnd():
	Packet(ID_NetEnd) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

};



REGISTER_PACKET_HELPER(ID_NetFirst, NetFirst);
REGISTER_PACKET_HELPER(ID_NetSessionEnterNotify, NetSessionEnterNotify);
REGISTER_PACKET_HELPER(ID_NetSessionLeaveNotify, NetSessionLeaveNotify);
REGISTER_PACKET_HELPER(ID_NetPingNotify, NetPingNotify);
REGISTER_PACKET_HELPER(ID_NetLoginReq, NetLoginReq);
REGISTER_PACKET_HELPER(ID_NetLoginRes, NetLoginRes);
REGISTER_PACKET_HELPER(ID_NetGmMsg, NetGmMsg);
REGISTER_PACKET_HELPER(ID_NetQueryRoleReq, NetQueryRoleReq);
REGISTER_PACKET_HELPER(ID_NetQueryRoleRes, NetQueryRoleRes);
REGISTER_PACKET_HELPER(ID_NetPropertyReq, NetPropertyReq);
REGISTER_PACKET_HELPER(ID_NetPropertyRes, NetPropertyRes);
REGISTER_PACKET_HELPER(ID_NetEntityPropertyNotify, NetEntityPropertyNotify);
REGISTER_PACKET_HELPER(ID_NetEntityEnterMapNotify, NetEntityEnterMapNotify);
REGISTER_PACKET_HELPER(ID_NetEntityLeaveMapNotify, NetEntityLeaveMapNotify);
REGISTER_PACKET_HELPER(ID_NetPlayerEnterViewNotify, NetPlayerEnterViewNotify);
REGISTER_PACKET_HELPER(ID_NetPlayerLeaveViewNotify, NetPlayerLeaveViewNotify);
REGISTER_PACKET_HELPER(ID_NetNpcEnterViewNotify, NetNpcEnterViewNotify);
REGISTER_PACKET_HELPER(ID_NetNpcLeaveViewNotify, NetNpcLeaveViewNotify);
REGISTER_PACKET_HELPER(ID_NetEntityMoveNotify, NetEntityMoveNotify);
REGISTER_PACKET_HELPER(ID_NetEntityMoveToNotify, NetEntityMoveToNotify);
REGISTER_PACKET_HELPER(ID_NetEntityAttackNotify, NetEntityAttackNotify);
REGISTER_PACKET_HELPER(ID_NetEntityFollowNotify, NetEntityFollowNotify);
REGISTER_PACKET_HELPER(ID_NetEntityCancelFollowNotify, NetEntityCancelFollowNotify);
REGISTER_PACKET_HELPER(ID_NetChatMsgNotify, NetChatMsgNotify);
REGISTER_PACKET_HELPER(ID_NetResponseMsgNotify, NetResponseMsgNotify);
REGISTER_PACKET_HELPER(ID_NetCreateTeamReq, NetCreateTeamReq);
REGISTER_PACKET_HELPER(ID_NetCreateTeamRes, NetCreateTeamRes);
REGISTER_PACKET_HELPER(ID_NetOrganizeTeamReq, NetOrganizeTeamReq);
REGISTER_PACKET_HELPER(ID_NetOrganizeTeamRes, NetOrganizeTeamRes);
REGISTER_PACKET_HELPER(ID_NetAgreeTeamReq, NetAgreeTeamReq);
REGISTER_PACKET_HELPER(ID_NetTeamInfoRes, NetTeamInfoRes);
REGISTER_PACKET_HELPER(ID_NetTeamListReq, NetTeamListReq);
REGISTER_PACKET_HELPER(ID_NetTeamListRes, NetTeamListRes);
REGISTER_PACKET_HELPER(ID_NetCreateRoomReq, NetCreateRoomReq);
REGISTER_PACKET_HELPER(ID_NetCreateRoomRes, NetCreateRoomRes);
REGISTER_PACKET_HELPER(ID_NetEnterRoomReq, NetEnterRoomReq);
REGISTER_PACKET_HELPER(ID_NetEnterRoomRes, NetEnterRoomRes);
REGISTER_PACKET_HELPER(ID_NetEnterRoomNotify, NetEnterRoomNotify);
REGISTER_PACKET_HELPER(ID_NetLeaveRoomReq, NetLeaveRoomReq);
REGISTER_PACKET_HELPER(ID_NetLeaveRoomRes, NetLeaveRoomRes);
REGISTER_PACKET_HELPER(ID_NetChangeRoomMasterReq, NetChangeRoomMasterReq);
REGISTER_PACKET_HELPER(ID_NetChangeRoomMasterRes, NetChangeRoomMasterRes);
REGISTER_PACKET_HELPER(ID_NetRoomListReq, NetRoomListReq);
REGISTER_PACKET_HELPER(ID_NetRoomListRes, NetRoomListRes);
REGISTER_PACKET_HELPER(ID_NetEnd, NetEnd);
