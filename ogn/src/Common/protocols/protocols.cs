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
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetOrganizeTeamReq, "NetOrganizeTeamReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetOrganizeTeamRes, "NetOrganizeTeamRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetAgreeTeamReq, "NetAgreeTeamReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetTeamInfoRes, "NetTeamInfoRes");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetTeamListReq, "NetTeamListReq");
PacketHelper.instance.RegisterPacket((int)PACKET_ID_ENUM.ID_NetTeamListRes, "NetTeamListRes");
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
public class NetFirst : Packet
{
	public NetFirst():base((int)PACKET_ID_ENUM.ID_NetFirst)
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
instanceId = 0;
accountInfo = new DBAccountInfo();

	}

	protected override bool OnSerialize(BinaryStream bytes)
	{
bytes.Write(result);
bytes.Write(instanceId);
bytes.Write(accountInfo);

		return true;
	}

	protected override bool OnDeserialize(BinaryStream bytes)
	{
bytes.Read(ref result);
bytes.Read(ref instanceId);
bytes.Read(accountInfo);

		return true;
	}

public int result;
public int instanceId;
public DBAccountInfo accountInfo;

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

public int guid;

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

public int guid;
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

public int guid;
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
public int guid;
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

public int guid;
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
public int guid;
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

public int guid;
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
public int guid;
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

public int guid;
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

public int guid;
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

public int guid;
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

public int guid;
public byte index;
public int tarGuid;
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

public int tarGuid;

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

public int tarGuid;

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

