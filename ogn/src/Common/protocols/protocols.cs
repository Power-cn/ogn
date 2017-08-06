using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

enum PACKET_ID_ENUM
{
ID_NetFirst,
ID_NetSessionEnterNotify,
ID_NetSessionLeaveNotify,
ID_NetPingNotify,
ID_NetLoginReq,
ID_NetLoginRes,
ID_NetChangeNameReq,
ID_NetChangeNameRes,
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
ID_NetRoomReadyReq,
ID_NetRoomReadyRes,
ID_NetRoomStartGameReq,
ID_NetRoomStartGameRes,
ID_NetRoomInfoNotify,
ID_NetGameStartNotify,
ID_NetGameCloseNotify,
ID_NetGameInfoNotify,
ID_NetGameOperateSeeReq,
ID_NetGameOperateSeeRes,
ID_NetGameOperateChipinReq,
ID_NetGameOperateChipinRes,
ID_NetGameOperateCallReq,
ID_NetGameOperateCallRes,
ID_NetGameOperateCompareReq,
ID_NetGameOperateCompareRes,
ID_NetEnd,

}

class PacketRegister
{
    public PacketRegister()
    {
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetFirst, "NetFirst");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetSessionEnterNotify, "NetSessionEnterNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetSessionLeaveNotify, "NetSessionLeaveNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetPingNotify, "NetPingNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetLoginReq, "NetLoginReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetLoginRes, "NetLoginRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetChangeNameReq, "NetChangeNameReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetChangeNameRes, "NetChangeNameRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGmMsg, "NetGmMsg");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetQueryRoleReq, "NetQueryRoleReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetQueryRoleRes, "NetQueryRoleRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetPropertyReq, "NetPropertyReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetPropertyRes, "NetPropertyRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEntityPropertyNotify, "NetEntityPropertyNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEntityEnterMapNotify, "NetEntityEnterMapNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEntityLeaveMapNotify, "NetEntityLeaveMapNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetPlayerEnterViewNotify, "NetPlayerEnterViewNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetPlayerLeaveViewNotify, "NetPlayerLeaveViewNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetNpcEnterViewNotify, "NetNpcEnterViewNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetNpcLeaveViewNotify, "NetNpcLeaveViewNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEntityMoveNotify, "NetEntityMoveNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEntityMoveToNotify, "NetEntityMoveToNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEntityAttackNotify, "NetEntityAttackNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEntityFollowNotify, "NetEntityFollowNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEntityCancelFollowNotify, "NetEntityCancelFollowNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetChatMsgNotify, "NetChatMsgNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetResponseMsgNotify, "NetResponseMsgNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetCreateTeamReq, "NetCreateTeamReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetCreateTeamRes, "NetCreateTeamRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetOrganizeTeamReq, "NetOrganizeTeamReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetOrganizeTeamRes, "NetOrganizeTeamRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetAgreeTeamReq, "NetAgreeTeamReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetTeamInfoRes, "NetTeamInfoRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetTeamListReq, "NetTeamListReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetTeamListRes, "NetTeamListRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetCreateRoomReq, "NetCreateRoomReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetCreateRoomRes, "NetCreateRoomRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEnterRoomReq, "NetEnterRoomReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEnterRoomRes, "NetEnterRoomRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEnterRoomNotify, "NetEnterRoomNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetLeaveRoomReq, "NetLeaveRoomReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetLeaveRoomRes, "NetLeaveRoomRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetChangeRoomMasterReq, "NetChangeRoomMasterReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetChangeRoomMasterRes, "NetChangeRoomMasterRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetRoomListReq, "NetRoomListReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetRoomListRes, "NetRoomListRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetRoomReadyReq, "NetRoomReadyReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetRoomReadyRes, "NetRoomReadyRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetRoomStartGameReq, "NetRoomStartGameReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetRoomStartGameRes, "NetRoomStartGameRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetRoomInfoNotify, "NetRoomInfoNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameStartNotify, "NetGameStartNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameCloseNotify, "NetGameCloseNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameInfoNotify, "NetGameInfoNotify");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameOperateSeeReq, "NetGameOperateSeeReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameOperateSeeRes, "NetGameOperateSeeRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameOperateChipinReq, "NetGameOperateChipinReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameOperateChipinRes, "NetGameOperateChipinRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameOperateCallReq, "NetGameOperateCallReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameOperateCallRes, "NetGameOperateCallRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameOperateCompareReq, "NetGameOperateCompareReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetGameOperateCompareRes, "NetGameOperateCompareRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetEnd, "NetEnd");

    }
}

public class DBAccountInfo : Header
{
	public DBAccountInfo()
	{
id = 0;
user = "";
password = new BinaryStream();
name = "";
email = "";
phone = "";

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(id);
bytes.Write(user);
bytes.Write(password);
bytes.Write(name);
bytes.Write(email);
bytes.Write(phone);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref id);
bytes.Read(ref user);
bytes.Read(password);
bytes.Read(ref name);
bytes.Read(ref email);
bytes.Read(ref phone);

		return true;
	}

public int id;
public string user;
public BinaryStream password;
public string name;
public string email;
public string phone;

}
public class DBRoleInfo : Header
{
	public DBRoleInfo()
	{
id = 0;
accountId = 0;
name = "";
property = new BinaryStream();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(id);
bytes.Write(accountId);
bytes.Write(name);
bytes.Write(property);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref id);
bytes.Read(ref accountId);
bytes.Read(ref name);
bytes.Read(property);

		return true;
	}

public int id;
public int accountId;
public string name;
public BinaryStream property;

}
public class Point : Header
{
	public Point()
	{
x = 0;
y = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(x);
bytes.Write(y);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref x);
bytes.Read(ref y);

		return true;
	}

public int x;
public int y;

}
public class TeamEntityInfo : Header
{
	public TeamEntityInfo()
	{
name = "";
instanceId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);
bytes.Write(instanceId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);
bytes.Read(ref instanceId);

		return true;
	}

public string name;
public int instanceId;

}
public class TeamInfo : Header
{
	public TeamInfo()
	{
teamId = 0;
leaderInsId = 0;
teamEntityInfos = new List<TeamEntityInfo>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(teamId);
bytes.Write(leaderInsId);
int teamEntityInfos_TEMP = teamEntityInfos.Count;
bytes.Write(teamEntityInfos_TEMP);
for (int i = 0; i < teamEntityInfos_TEMP; ++i)
{
	bytes.Write(teamEntityInfos[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref teamId);
bytes.Read(ref leaderInsId);
int teamEntityInfos_TEMP = 0;
bytes.Read(ref teamEntityInfos_TEMP);
for (int i = 0; i < teamEntityInfos_TEMP; ++i)
{
	TeamEntityInfo info_teamEntityInfos;
	info_teamEntityInfos = new TeamEntityInfo();
	bytes.Read(info_teamEntityInfos);
	teamEntityInfos.Add(info_teamEntityInfos);
}

		return true;
	}

public int teamId;
public int leaderInsId;
public List<TeamEntityInfo> teamEntityInfos;

}
public class RoomPlayerInfo : Header
{
	public RoomPlayerInfo()
	{
name = "";
userId = 0;
insId = 0;
state = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);
bytes.Write(userId);
bytes.Write(insId);
bytes.Write(state);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);
bytes.Read(ref userId);
bytes.Read(ref insId);
bytes.Read(ref state);

		return true;
	}

public string name;
public uint userId;
public ulong insId;
public sbyte state;

}
public class RoomInfo : Header
{
	public RoomInfo()
	{
roomId = 0;
masterUserId = 0;
password = "";
name = "";
maxCount = 0;
roomPlayerInfos = new List<RoomPlayerInfo>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomId);
bytes.Write(masterUserId);
bytes.Write(password);
bytes.Write(name);
bytes.Write(maxCount);
int roomPlayerInfos_TEMP = roomPlayerInfos.Count;
bytes.Write(roomPlayerInfos_TEMP);
for (int i = 0; i < roomPlayerInfos_TEMP; ++i)
{
	bytes.Write(roomPlayerInfos[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref roomId);
bytes.Read(ref masterUserId);
bytes.Read(ref password);
bytes.Read(ref name);
bytes.Read(ref maxCount);
int roomPlayerInfos_TEMP = 0;
bytes.Read(ref roomPlayerInfos_TEMP);
for (int i = 0; i < roomPlayerInfos_TEMP; ++i)
{
	RoomPlayerInfo info_roomPlayerInfos;
	info_roomPlayerInfos = new RoomPlayerInfo();
	bytes.Read(info_roomPlayerInfos);
	roomPlayerInfos.Add(info_roomPlayerInfos);
}

		return true;
	}

public uint roomId;
public uint masterUserId;
public string password;
public string name;
public uint maxCount;
public List<RoomPlayerInfo> roomPlayerInfos;

}
public class GameEntityInfo : Header
{
	public GameEntityInfo()
	{
userId = 0;
cards = new List<sbyte>();
userGold = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(userId);
int cards_TEMP = cards.Count;
bytes.Write(cards_TEMP);
for (int i = 0; i < cards_TEMP; ++i)
{
	bytes.Write(cards[i]);
}
bytes.Write(userGold);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref userId);
int cards_TEMP = 0;
bytes.Read(ref cards_TEMP);
for (int i = 0; i < cards_TEMP; ++i)
{
	sbyte info_cards;
	info_cards = 0;
	bytes.Read(ref info_cards);
	cards.Add(info_cards);
}
bytes.Read(ref userGold);

		return true;
	}

public uint userId;
public List<sbyte> cards;
public uint userGold;

}
public class GameGoldenFlowerInfo : Header
{
	public GameGoldenFlowerInfo()
	{
insId = 0;
roomId = 0;
bankerUserId = 0;
curSpeakUserId = 0;
speakTime = 0;
curUseGold = 0;
curMaxUseGold = 0;
round = 0;
gameEntInfos = new List<GameEntityInfo>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(insId);
bytes.Write(roomId);
bytes.Write(bankerUserId);
bytes.Write(curSpeakUserId);
bytes.Write(speakTime);
bytes.Write(curUseGold);
bytes.Write(curMaxUseGold);
bytes.Write(round);
int gameEntInfos_TEMP = gameEntInfos.Count;
bytes.Write(gameEntInfos_TEMP);
for (int i = 0; i < gameEntInfos_TEMP; ++i)
{
	bytes.Write(gameEntInfos[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref insId);
bytes.Read(ref roomId);
bytes.Read(ref bankerUserId);
bytes.Read(ref curSpeakUserId);
bytes.Read(ref speakTime);
bytes.Read(ref curUseGold);
bytes.Read(ref curMaxUseGold);
bytes.Read(ref round);
int gameEntInfos_TEMP = 0;
bytes.Read(ref gameEntInfos_TEMP);
for (int i = 0; i < gameEntInfos_TEMP; ++i)
{
	GameEntityInfo info_gameEntInfos;
	info_gameEntInfos = new GameEntityInfo();
	bytes.Read(info_gameEntInfos);
	gameEntInfos.Add(info_gameEntInfos);
}

		return true;
	}

public uint insId;
public uint roomId;
public uint bankerUserId;
public uint curSpeakUserId;
public uint speakTime;
public uint curUseGold;
public uint curMaxUseGold;
public uint round;
public List<GameEntityInfo> gameEntInfos;

}
public class NetFirst : Packet
{
	public NetFirst():base((int)PACKET_ID_ENUM.ID_NetFirst)
	{
sbytes = new BinaryStream();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(sbytes);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(sbytes);

		return true;
	}

public BinaryStream sbytes;

}
public class NetSessionEnterNotify : Packet
{
	public NetSessionEnterNotify():base((int)PACKET_ID_ENUM.ID_NetSessionEnterNotify)
	{
host = "";

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(host);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref host);

		return true;
	}

public string host;

}
public class NetSessionLeaveNotify : Packet
{
	public NetSessionLeaveNotify():base((int)PACKET_ID_ENUM.ID_NetSessionLeaveNotify)
	{

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{

		return true;
	}


}
public class NetPingNotify : Packet
{
	public NetPingNotify():base((int)PACKET_ID_ENUM.ID_NetPingNotify)
	{
time = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(time);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref time);

		return true;
	}

public uint time;

}
public class NetLoginReq : Packet
{
	public NetLoginReq():base((int)PACKET_ID_ENUM.ID_NetLoginReq)
	{
user = "";
password = "";

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(user);
bytes.Write(password);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref user);
bytes.Read(ref password);

		return true;
	}

public string user;
public string password;

}
public class NetLoginRes : Packet
{
	public NetLoginRes():base((int)PACKET_ID_ENUM.ID_NetLoginRes)
	{
result = 0;
guid = 0;
accountInfo = new DBAccountInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(result);
bytes.Write(guid);
bytes.Write(accountInfo);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref result);
bytes.Read(ref guid);
bytes.Read(accountInfo);

		return true;
	}

public int result;
public long guid;
public DBAccountInfo accountInfo;

}
public class NetChangeNameReq : Packet
{
	public NetChangeNameReq():base((int)PACKET_ID_ENUM.ID_NetChangeNameReq)
	{
newName = "";

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(newName);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref newName);

		return true;
	}

public string newName;

}
public class NetChangeNameRes : Packet
{
	public NetChangeNameRes():base((int)PACKET_ID_ENUM.ID_NetChangeNameRes)
	{
result = 0;
newName = "";

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(result);
bytes.Write(newName);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref result);
bytes.Read(ref newName);

		return true;
	}

public sbyte result;
public string newName;

}
public class NetGmMsg : Packet
{
	public NetGmMsg():base((int)PACKET_ID_ENUM.ID_NetGmMsg)
	{
name = "";
gmParams = new List<string>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);
int gmParams_TEMP = gmParams.Count;
bytes.Write(gmParams_TEMP);
for (int i = 0; i < gmParams_TEMP; ++i)
{
	bytes.Write(gmParams[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);
int gmParams_TEMP = 0;
bytes.Read(ref gmParams_TEMP);
for (int i = 0; i < gmParams_TEMP; ++i)
{
	string info_gmParams;
	info_gmParams = "";
	bytes.Read(ref info_gmParams);
	gmParams.Add(info_gmParams);
}

		return true;
	}

public string name;
public List<string> gmParams;

}
public class NetQueryRoleReq : Packet
{
	public NetQueryRoleReq():base((int)PACKET_ID_ENUM.ID_NetQueryRoleReq)
	{
accountId = 0;
user = "";
roleCount = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(accountId);
bytes.Write(user);
bytes.Write(roleCount);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref accountId);
bytes.Read(ref user);
bytes.Read(ref roleCount);

		return true;
	}

public int accountId;
public string user;
public byte roleCount;

}
public class NetQueryRoleRes : Packet
{
	public NetQueryRoleRes():base((int)PACKET_ID_ENUM.ID_NetQueryRoleRes)
	{
accountId = 0;
roleInfos = new List<DBRoleInfo>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(accountId);
int roleInfos_TEMP = roleInfos.Count;
bytes.Write(roleInfos_TEMP);
for (int i = 0; i < roleInfos_TEMP; ++i)
{
	bytes.Write(roleInfos[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref accountId);
int roleInfos_TEMP = 0;
bytes.Read(ref roleInfos_TEMP);
for (int i = 0; i < roleInfos_TEMP; ++i)
{
	DBRoleInfo info_roleInfos;
	info_roleInfos = new DBRoleInfo();
	bytes.Read(info_roleInfos);
	roleInfos.Add(info_roleInfos);
}

		return true;
	}

public int accountId;
public List<DBRoleInfo> roleInfos;

}
public class NetPropertyReq : Packet
{
	public NetPropertyReq():base((int)PACKET_ID_ENUM.ID_NetPropertyReq)
	{
guid = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);

		return true;
	}

public long guid;

}
public class NetPropertyRes : Packet
{
	public NetPropertyRes():base((int)PACKET_ID_ENUM.ID_NetPropertyRes)
	{
guid = 0;
property = new BinaryStream();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);
bytes.Write(property);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);
bytes.Read(property);

		return true;
	}

public long guid;
public BinaryStream property;

}
public class NetEntityPropertyNotify : Packet
{
	public NetEntityPropertyNotify():base((int)PACKET_ID_ENUM.ID_NetEntityPropertyNotify)
	{
guid = 0;
property = new BinaryStream();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);
bytes.Write(property);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);
bytes.Read(property);

		return true;
	}

public long guid;
public BinaryStream property;

}
public class NetEntityEnterMapNotify : Packet
{
	public NetEntityEnterMapNotify():base((int)PACKET_ID_ENUM.ID_NetEntityEnterMapNotify)
	{
name = "";
guid = 0;
mapInsId = 0;
mapId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);
bytes.Write(guid);
bytes.Write(mapInsId);
bytes.Write(mapId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);
bytes.Read(ref guid);
bytes.Read(ref mapInsId);
bytes.Read(ref mapId);

		return true;
	}

public string name;
public long guid;
public int mapInsId;
public int mapId;

}
public class NetEntityLeaveMapNotify : Packet
{
	public NetEntityLeaveMapNotify():base((int)PACKET_ID_ENUM.ID_NetEntityLeaveMapNotify)
	{
guid = 0;
mapInsId = 0;
mapId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);
bytes.Write(mapInsId);
bytes.Write(mapId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);
bytes.Read(ref mapInsId);
bytes.Read(ref mapId);

		return true;
	}

public long guid;
public int mapInsId;
public int mapId;

}
public class NetPlayerEnterViewNotify : Packet
{
	public NetPlayerEnterViewNotify():base((int)PACKET_ID_ENUM.ID_NetPlayerEnterViewNotify)
	{
name = "";
guid = 0;
entityType = 0;
mapId = 0;
charId = 0;
pos = new Point();
speed = 0.0F;
status = 0;
dir = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);
bytes.Write(guid);
bytes.Write(entityType);
bytes.Write(mapId);
bytes.Write(charId);
bytes.Write(pos);
bytes.Write(speed);
bytes.Write(status);
bytes.Write(dir);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);
bytes.Read(ref guid);
bytes.Read(ref entityType);
bytes.Read(ref mapId);
bytes.Read(ref charId);
bytes.Read(pos);
bytes.Read(ref speed);
bytes.Read(ref status);
bytes.Read(ref dir);

		return true;
	}

public string name;
public long guid;
public int entityType;
public int mapId;
public int charId;
public Point pos;
public float speed;
public int status;
public int dir;

}
public class NetPlayerLeaveViewNotify : Packet
{
	public NetPlayerLeaveViewNotify():base((int)PACKET_ID_ENUM.ID_NetPlayerLeaveViewNotify)
	{
guid = 0;
mapId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);
bytes.Write(mapId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);
bytes.Read(ref mapId);

		return true;
	}

public long guid;
public int mapId;

}
public class NetNpcEnterViewNotify : Packet
{
	public NetNpcEnterViewNotify():base((int)PACKET_ID_ENUM.ID_NetNpcEnterViewNotify)
	{
name = "";
guid = 0;
entityType = 0;
mapId = 0;
npcId = 0;
pos = new Point();
speed = 0.0F;
status = 0;
dir = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);
bytes.Write(guid);
bytes.Write(entityType);
bytes.Write(mapId);
bytes.Write(npcId);
bytes.Write(pos);
bytes.Write(speed);
bytes.Write(status);
bytes.Write(dir);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);
bytes.Read(ref guid);
bytes.Read(ref entityType);
bytes.Read(ref mapId);
bytes.Read(ref npcId);
bytes.Read(pos);
bytes.Read(ref speed);
bytes.Read(ref status);
bytes.Read(ref dir);

		return true;
	}

public string name;
public long guid;
public int entityType;
public int mapId;
public int npcId;
public Point pos;
public float speed;
public int status;
public int dir;

}
public class NetNpcLeaveViewNotify : Packet
{
	public NetNpcLeaveViewNotify():base((int)PACKET_ID_ENUM.ID_NetNpcLeaveViewNotify)
	{
guid = 0;
mapId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);
bytes.Write(mapId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);
bytes.Read(ref mapId);

		return true;
	}

public long guid;
public int mapId;

}
public class NetEntityMoveNotify : Packet
{
	public NetEntityMoveNotify():base((int)PACKET_ID_ENUM.ID_NetEntityMoveNotify)
	{
guid = 0;
startDir = 0;
x = 0;
y = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);
bytes.Write(startDir);
bytes.Write(x);
bytes.Write(y);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);
bytes.Read(ref startDir);
bytes.Read(ref x);
bytes.Read(ref y);

		return true;
	}

public long guid;
public byte startDir;
public int x;
public int y;

}
public class NetEntityMoveToNotify : Packet
{
	public NetEntityMoveToNotify():base((int)PACKET_ID_ENUM.ID_NetEntityMoveToNotify)
	{
guid = 0;
x = 0;
y = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);
bytes.Write(x);
bytes.Write(y);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);
bytes.Read(ref x);
bytes.Read(ref y);

		return true;
	}

public long guid;
public int x;
public int y;

}
public class NetEntityAttackNotify : Packet
{
	public NetEntityAttackNotify():base((int)PACKET_ID_ENUM.ID_NetEntityAttackNotify)
	{
guid = 0;
index = 0;
tarGuid = 0;
dirPos = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(guid);
bytes.Write(index);
bytes.Write(tarGuid);
bytes.Write(dirPos);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref guid);
bytes.Read(ref index);
bytes.Read(ref tarGuid);
bytes.Read(ref dirPos);

		return true;
	}

public long guid;
public byte index;
public long tarGuid;
public byte dirPos;

}
public class NetEntityFollowNotify : Packet
{
	public NetEntityFollowNotify():base((int)PACKET_ID_ENUM.ID_NetEntityFollowNotify)
	{
tarGuid = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(tarGuid);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref tarGuid);

		return true;
	}

public long tarGuid;

}
public class NetEntityCancelFollowNotify : Packet
{
	public NetEntityCancelFollowNotify():base((int)PACKET_ID_ENUM.ID_NetEntityCancelFollowNotify)
	{
tarGuid = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(tarGuid);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref tarGuid);

		return true;
	}

public long tarGuid;

}
public class NetChatMsgNotify : Packet
{
	public NetChatMsgNotify():base((int)PACKET_ID_ENUM.ID_NetChatMsgNotify)
	{
from = "";
to = "";
channelType = 0;
chatMsg = "";
chatData = new BinaryStream();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(from);
bytes.Write(to);
bytes.Write(channelType);
bytes.Write(chatMsg);
bytes.Write(chatData);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref from);
bytes.Read(ref to);
bytes.Read(ref channelType);
bytes.Read(ref chatMsg);
bytes.Read(chatData);

		return true;
	}

public string from;
public string to;
public byte channelType;
public string chatMsg;
public BinaryStream chatData;

}
public class NetResponseMsgNotify : Packet
{
	public NetResponseMsgNotify():base((int)PACKET_ID_ENUM.ID_NetResponseMsgNotify)
	{
msgId = 0;
msgParams = new List<string>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(msgId);
int msgParams_TEMP = msgParams.Count;
bytes.Write(msgParams_TEMP);
for (int i = 0; i < msgParams_TEMP; ++i)
{
	bytes.Write(msgParams[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref msgId);
int msgParams_TEMP = 0;
bytes.Read(ref msgParams_TEMP);
for (int i = 0; i < msgParams_TEMP; ++i)
{
	string info_msgParams;
	info_msgParams = "";
	bytes.Read(ref info_msgParams);
	msgParams.Add(info_msgParams);
}

		return true;
	}

public int msgId;
public List<string> msgParams;

}
public class NetCreateTeamReq : Packet
{
	public NetCreateTeamReq():base((int)PACKET_ID_ENUM.ID_NetCreateTeamReq)
	{

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{

		return true;
	}


}
public class NetCreateTeamRes : Packet
{
	public NetCreateTeamRes():base((int)PACKET_ID_ENUM.ID_NetCreateTeamRes)
	{
teamInfo = new TeamInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(teamInfo);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(teamInfo);

		return true;
	}

public TeamInfo teamInfo;

}
public class NetOrganizeTeamReq : Packet
{
	public NetOrganizeTeamReq():base((int)PACKET_ID_ENUM.ID_NetOrganizeTeamReq)
	{
name = "";

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);

		return true;
	}

public string name;

}
public class NetOrganizeTeamRes : Packet
{
	public NetOrganizeTeamRes():base((int)PACKET_ID_ENUM.ID_NetOrganizeTeamRes)
	{
name = "";
isJoin = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);
bytes.Write(isJoin);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);
bytes.Read(ref isJoin);

		return true;
	}

public string name;
public byte isJoin;

}
public class NetAgreeTeamReq : Packet
{
	public NetAgreeTeamReq():base((int)PACKET_ID_ENUM.ID_NetAgreeTeamReq)
	{
name = "";
isJoin = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);
bytes.Write(isJoin);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);
bytes.Read(ref isJoin);

		return true;
	}

public string name;
public byte isJoin;

}
public class NetTeamInfoRes : Packet
{
	public NetTeamInfoRes():base((int)PACKET_ID_ENUM.ID_NetTeamInfoRes)
	{
teamInfo = new TeamInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(teamInfo);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(teamInfo);

		return true;
	}

public TeamInfo teamInfo;

}
public class NetTeamListReq : Packet
{
	public NetTeamListReq():base((int)PACKET_ID_ENUM.ID_NetTeamListReq)
	{

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{

		return true;
	}


}
public class NetTeamListRes : Packet
{
	public NetTeamListRes():base((int)PACKET_ID_ENUM.ID_NetTeamListRes)
	{
teamInfos = new List<TeamInfo>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
int teamInfos_TEMP = teamInfos.Count;
bytes.Write(teamInfos_TEMP);
for (int i = 0; i < teamInfos_TEMP; ++i)
{
	bytes.Write(teamInfos[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
int teamInfos_TEMP = 0;
bytes.Read(ref teamInfos_TEMP);
for (int i = 0; i < teamInfos_TEMP; ++i)
{
	TeamInfo info_teamInfos;
	info_teamInfos = new TeamInfo();
	bytes.Read(info_teamInfos);
	teamInfos.Add(info_teamInfos);
}

		return true;
	}

public List<TeamInfo> teamInfos;

}
public class NetCreateRoomReq : Packet
{
	public NetCreateRoomReq():base((int)PACKET_ID_ENUM.ID_NetCreateRoomReq)
	{
name = "";

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(name);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref name);

		return true;
	}

public string name;

}
public class NetCreateRoomRes : Packet
{
	public NetCreateRoomRes():base((int)PACKET_ID_ENUM.ID_NetCreateRoomRes)
	{
roomInfo = new RoomInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomInfo);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(roomInfo);

		return true;
	}

public RoomInfo roomInfo;

}
public class NetEnterRoomReq : Packet
{
	public NetEnterRoomReq():base((int)PACKET_ID_ENUM.ID_NetEnterRoomReq)
	{
roomId = 0;
password = "";

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomId);
bytes.Write(password);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref roomId);
bytes.Read(ref password);

		return true;
	}

public uint roomId;
public string password;

}
public class NetEnterRoomRes : Packet
{
	public NetEnterRoomRes():base((int)PACKET_ID_ENUM.ID_NetEnterRoomRes)
	{
roomInfo = new RoomInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomInfo);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(roomInfo);

		return true;
	}

public RoomInfo roomInfo;

}
public class NetEnterRoomNotify : Packet
{
	public NetEnterRoomNotify():base((int)PACKET_ID_ENUM.ID_NetEnterRoomNotify)
	{
roomId = 0;
roomPlayerInfo = new RoomPlayerInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomId);
bytes.Write(roomPlayerInfo);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref roomId);
bytes.Read(roomPlayerInfo);

		return true;
	}

public uint roomId;
public RoomPlayerInfo roomPlayerInfo;

}
public class NetLeaveRoomReq : Packet
{
	public NetLeaveRoomReq():base((int)PACKET_ID_ENUM.ID_NetLeaveRoomReq)
	{
roomId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref roomId);

		return true;
	}

public uint roomId;

}
public class NetLeaveRoomRes : Packet
{
	public NetLeaveRoomRes():base((int)PACKET_ID_ENUM.ID_NetLeaveRoomRes)
	{
roomId = 0;
userId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomId);
bytes.Write(userId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref roomId);
bytes.Read(ref userId);

		return true;
	}

public uint roomId;
public uint userId;

}
public class NetChangeRoomMasterReq : Packet
{
	public NetChangeRoomMasterReq():base((int)PACKET_ID_ENUM.ID_NetChangeRoomMasterReq)
	{
roomId = 0;
userId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomId);
bytes.Write(userId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref roomId);
bytes.Read(ref userId);

		return true;
	}

public uint roomId;
public uint userId;

}
public class NetChangeRoomMasterRes : Packet
{
	public NetChangeRoomMasterRes():base((int)PACKET_ID_ENUM.ID_NetChangeRoomMasterRes)
	{
roomId = 0;
masterUserId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomId);
bytes.Write(masterUserId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref roomId);
bytes.Read(ref masterUserId);

		return true;
	}

public uint roomId;
public uint masterUserId;

}
public class NetRoomListReq : Packet
{
	public NetRoomListReq():base((int)PACKET_ID_ENUM.ID_NetRoomListReq)
	{
start = 0;
count = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(start);
bytes.Write(count);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref start);
bytes.Read(ref count);

		return true;
	}

public uint start;
public uint count;

}
public class NetRoomListRes : Packet
{
	public NetRoomListRes():base((int)PACKET_ID_ENUM.ID_NetRoomListRes)
	{
roomInfoInfos = new List<RoomInfo>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
int roomInfoInfos_TEMP = roomInfoInfos.Count;
bytes.Write(roomInfoInfos_TEMP);
for (int i = 0; i < roomInfoInfos_TEMP; ++i)
{
	bytes.Write(roomInfoInfos[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
int roomInfoInfos_TEMP = 0;
bytes.Read(ref roomInfoInfos_TEMP);
for (int i = 0; i < roomInfoInfos_TEMP; ++i)
{
	RoomInfo info_roomInfoInfos;
	info_roomInfoInfos = new RoomInfo();
	bytes.Read(info_roomInfoInfos);
	roomInfoInfos.Add(info_roomInfoInfos);
}

		return true;
	}

public List<RoomInfo> roomInfoInfos;

}
public class NetRoomReadyReq : Packet
{
	public NetRoomReadyReq():base((int)PACKET_ID_ENUM.ID_NetRoomReadyReq)
	{
isReady = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(isReady);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref isReady);

		return true;
	}

public sbyte isReady;

}
public class NetRoomReadyRes : Packet
{
	public NetRoomReadyRes():base((int)PACKET_ID_ENUM.ID_NetRoomReadyRes)
	{
userId = 0;
result = 0;
isReady = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(userId);
bytes.Write(result);
bytes.Write(isReady);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref userId);
bytes.Read(ref result);
bytes.Read(ref isReady);

		return true;
	}

public uint userId;
public sbyte result;
public sbyte isReady;

}
public class NetRoomStartGameReq : Packet
{
	public NetRoomStartGameReq():base((int)PACKET_ID_ENUM.ID_NetRoomStartGameReq)
	{

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{

		return true;
	}


}
public class NetRoomStartGameRes : Packet
{
	public NetRoomStartGameRes():base((int)PACKET_ID_ENUM.ID_NetRoomStartGameRes)
	{
result = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(result);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref result);

		return true;
	}

public sbyte result;

}
public class NetRoomInfoNotify : Packet
{
	public NetRoomInfoNotify():base((int)PACKET_ID_ENUM.ID_NetRoomInfoNotify)
	{
roomInfo = new RoomInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(roomInfo);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(roomInfo);

		return true;
	}

public RoomInfo roomInfo;

}
public class NetGameStartNotify : Packet
{
	public NetGameStartNotify():base((int)PACKET_ID_ENUM.ID_NetGameStartNotify)
	{
info = new GameGoldenFlowerInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(info);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(info);

		return true;
	}

public GameGoldenFlowerInfo info;

}
public class NetGameCloseNotify : Packet
{
	public NetGameCloseNotify():base((int)PACKET_ID_ENUM.ID_NetGameCloseNotify)
	{
winUserId = 0;
winGold = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(winUserId);
bytes.Write(winGold);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref winUserId);
bytes.Read(ref winGold);

		return true;
	}

public uint winUserId;
public uint winGold;

}
public class NetGameInfoNotify : Packet
{
	public NetGameInfoNotify():base((int)PACKET_ID_ENUM.ID_NetGameInfoNotify)
	{
info = new GameGoldenFlowerInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(info);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(info);

		return true;
	}

public GameGoldenFlowerInfo info;

}
public class NetGameOperateSeeReq : Packet
{
	public NetGameOperateSeeReq():base((int)PACKET_ID_ENUM.ID_NetGameOperateSeeReq)
	{

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{

		return true;
	}


}
public class NetGameOperateSeeRes : Packet
{
	public NetGameOperateSeeRes():base((int)PACKET_ID_ENUM.ID_NetGameOperateSeeRes)
	{
result = 0;
cards = new List<sbyte>();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(result);
int cards_TEMP = cards.Count;
bytes.Write(cards_TEMP);
for (int i = 0; i < cards_TEMP; ++i)
{
	bytes.Write(cards[i]);
}

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref result);
int cards_TEMP = 0;
bytes.Read(ref cards_TEMP);
for (int i = 0; i < cards_TEMP; ++i)
{
	sbyte info_cards;
	info_cards = 0;
	bytes.Read(ref info_cards);
	cards.Add(info_cards);
}

		return true;
	}

public sbyte result;
public List<sbyte> cards;

}
public class NetGameOperateChipinReq : Packet
{
	public NetGameOperateChipinReq():base((int)PACKET_ID_ENUM.ID_NetGameOperateChipinReq)
	{
gold = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(gold);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref gold);

		return true;
	}

public uint gold;

}
public class NetGameOperateChipinRes : Packet
{
	public NetGameOperateChipinRes():base((int)PACKET_ID_ENUM.ID_NetGameOperateChipinRes)
	{
result = 0;
userId = 0;
gold = 0;
state = 0;
nextSpeakUserId = 0;
speakTime = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(result);
bytes.Write(userId);
bytes.Write(gold);
bytes.Write(state);
bytes.Write(nextSpeakUserId);
bytes.Write(speakTime);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref result);
bytes.Read(ref userId);
bytes.Read(ref gold);
bytes.Read(ref state);
bytes.Read(ref nextSpeakUserId);
bytes.Read(ref speakTime);

		return true;
	}

public sbyte result;
public uint userId;
public uint gold;
public sbyte state;
public uint nextSpeakUserId;
public uint speakTime;

}
public class NetGameOperateCallReq : Packet
{
	public NetGameOperateCallReq():base((int)PACKET_ID_ENUM.ID_NetGameOperateCallReq)
	{

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{

		return true;
	}


}
public class NetGameOperateCallRes : Packet
{
	public NetGameOperateCallRes():base((int)PACKET_ID_ENUM.ID_NetGameOperateCallRes)
	{
result = 0;
userId = 0;
gold = 0;
state = 0;
nextSpeakUserId = 0;
speakTime = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(result);
bytes.Write(userId);
bytes.Write(gold);
bytes.Write(state);
bytes.Write(nextSpeakUserId);
bytes.Write(speakTime);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref result);
bytes.Read(ref userId);
bytes.Read(ref gold);
bytes.Read(ref state);
bytes.Read(ref nextSpeakUserId);
bytes.Read(ref speakTime);

		return true;
	}

public sbyte result;
public uint userId;
public uint gold;
public sbyte state;
public uint nextSpeakUserId;
public uint speakTime;

}
public class NetGameOperateCompareReq : Packet
{
	public NetGameOperateCompareReq():base((int)PACKET_ID_ENUM.ID_NetGameOperateCompareReq)
	{
tarUserId = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(tarUserId);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref tarUserId);

		return true;
	}

public uint tarUserId;

}
public class NetGameOperateCompareRes : Packet
{
	public NetGameOperateCompareRes():base((int)PACKET_ID_ENUM.ID_NetGameOperateCompareRes)
	{
result = 0;
userId = 0;
cards = new List<sbyte>();
gold = 0;
state = 0;
tarUserId = 0;
tarCards = new List<sbyte>();
tarState = 0;
nextSpeakUserId = 0;
speakTime = 0;

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(result);
bytes.Write(userId);
int cards_TEMP = cards.Count;
bytes.Write(cards_TEMP);
for (int i = 0; i < cards_TEMP; ++i)
{
	bytes.Write(cards[i]);
}
bytes.Write(gold);
bytes.Write(state);
bytes.Write(tarUserId);
int tarCards_TEMP = tarCards.Count;
bytes.Write(tarCards_TEMP);
for (int i = 0; i < tarCards_TEMP; ++i)
{
	bytes.Write(tarCards[i]);
}
bytes.Write(tarState);
bytes.Write(nextSpeakUserId);
bytes.Write(speakTime);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref result);
bytes.Read(ref userId);
int cards_TEMP = 0;
bytes.Read(ref cards_TEMP);
for (int i = 0; i < cards_TEMP; ++i)
{
	sbyte info_cards;
	info_cards = 0;
	bytes.Read(ref info_cards);
	cards.Add(info_cards);
}
bytes.Read(ref gold);
bytes.Read(ref state);
bytes.Read(ref tarUserId);
int tarCards_TEMP = 0;
bytes.Read(ref tarCards_TEMP);
for (int i = 0; i < tarCards_TEMP; ++i)
{
	sbyte info_tarCards;
	info_tarCards = 0;
	bytes.Read(ref info_tarCards);
	tarCards.Add(info_tarCards);
}
bytes.Read(ref tarState);
bytes.Read(ref nextSpeakUserId);
bytes.Read(ref speakTime);

		return true;
	}

public sbyte result;
public uint userId;
public List<sbyte> cards;
public uint gold;
public sbyte state;
public uint tarUserId;
public List<sbyte> tarCards;
public sbyte tarState;
public uint nextSpeakUserId;
public uint speakTime;

}
public class NetEnd : Packet
{
	public NetEnd():base((int)PACKET_ID_ENUM.ID_NetEnd)
	{

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{

		return true;
	}


}

