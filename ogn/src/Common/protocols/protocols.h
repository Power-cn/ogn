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
ID_NetOrganizeTeamReq,
ID_NetOrganizeTeamRes,
ID_NetAgreeTeamReq,
ID_NetTeamInfoRes,
ID_NetTeamListReq,
ID_NetTeamListRes,
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
uint32 teamEntityInfos_TEMP = teamEntityInfos.size();
bytes << teamEntityInfos_TEMP;
for (uint32 i = 0; i < teamEntityInfos_TEMP; ++i) {
	bytes << teamEntityInfos[i];
}

		return true;
	}

	bool operator << (BinaryStream& bytes) {
CHECK(bytes >> teamId);
CHECK(bytes >> leaderInsId);
uint32 teamEntityInfos_TEMP = 0;
bytes >> teamEntityInfos_TEMP;
for (uint32 i = 0; i < teamEntityInfos_TEMP; ++i) {
	TeamEntityInfo info;
	bytes >> info;
	teamEntityInfos.push_back(info);
}

		return true;
	}
public:
int32 teamId;
int32 leaderInsId;
std::vector<TeamEntityInfo> teamEntityInfos;

};

class NetFirst : public Packet {
public:
	NetFirst():
	Packet(ID_NetFirst) {

	}

	bool OnSerialize(BinaryStream& bytes) {

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {

		return true;
	}
public:

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
instanceId = 0;
accountInfo;

	}

	bool OnSerialize(BinaryStream& bytes) {
CHECK(bytes << result);
CHECK(bytes << instanceId);
CHECK(bytes << accountInfo);

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> result);
CHECK(bytes >> instanceId);
CHECK(bytes >> accountInfo);

		return true;
	}
public:
int32 result;
int32 instanceId;
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
uint32 gmParams_TEMP = gmParams.size();
bytes << gmParams_TEMP;
for (uint32 i = 0; i < gmParams_TEMP; ++i) {
	bytes << gmParams[i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> name);
uint32 gmParams_TEMP = 0;
bytes >> gmParams_TEMP;
for (uint32 i = 0; i < gmParams_TEMP; ++i) {
	std::string info;
	bytes >> info;
	gmParams.push_back(info);
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
uint32 roleInfos_TEMP = roleInfos.size();
bytes << roleInfos_TEMP;
for (uint32 i = 0; i < roleInfos_TEMP; ++i) {
	bytes << roleInfos[i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> accountId);
uint32 roleInfos_TEMP = 0;
bytes >> roleInfos_TEMP;
for (uint32 i = 0; i < roleInfos_TEMP; ++i) {
	DBRoleInfo info;
	bytes >> info;
	roleInfos.push_back(info);
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
int32 guid;

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
int32 guid;
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
int32 guid;
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
int32 guid;
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
int32 guid;
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
int32 guid;
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
int32 guid;
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
int32 guid;
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
int32 guid;
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
int32 guid;
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
int32 guid;
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
int32 guid;
int8 index;
int32 tarGuid;
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
int32 tarGuid;

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
int32 tarGuid;

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
uint32 msgParams_TEMP = msgParams.size();
bytes << msgParams_TEMP;
for (uint32 i = 0; i < msgParams_TEMP; ++i) {
	bytes << msgParams[i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
CHECK(bytes >> msgId);
uint32 msgParams_TEMP = 0;
bytes >> msgParams_TEMP;
for (uint32 i = 0; i < msgParams_TEMP; ++i) {
	std::string info;
	bytes >> info;
	msgParams.push_back(info);
}

		return true;
	}
public:
int32 msgId;
std::vector<std::string> msgParams;

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
uint32 teamInfos_TEMP = teamInfos.size();
bytes << teamInfos_TEMP;
for (uint32 i = 0; i < teamInfos_TEMP; ++i) {
	bytes << teamInfos[i];
}

		return true;
	}

	bool OnDeserialize(BinaryStream& bytes) {
uint32 teamInfos_TEMP = 0;
bytes >> teamInfos_TEMP;
for (uint32 i = 0; i < teamInfos_TEMP; ++i) {
	TeamInfo info;
	bytes >> info;
	teamInfos.push_back(info);
}

		return true;
	}
public:
std::vector<TeamInfo> teamInfos;

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
REGISTER_PACKET_HELPER(ID_NetOrganizeTeamReq, NetOrganizeTeamReq);
REGISTER_PACKET_HELPER(ID_NetOrganizeTeamRes, NetOrganizeTeamRes);
REGISTER_PACKET_HELPER(ID_NetAgreeTeamReq, NetAgreeTeamReq);
REGISTER_PACKET_HELPER(ID_NetTeamInfoRes, NetTeamInfoRes);
REGISTER_PACKET_HELPER(ID_NetTeamListReq, NetTeamListReq);
REGISTER_PACKET_HELPER(ID_NetTeamListRes, NetTeamListRes);
REGISTER_PACKET_HELPER(ID_NetEnd, NetEnd);
