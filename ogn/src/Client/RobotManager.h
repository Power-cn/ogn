#pragma once

class RobotManager :  public EventDispatcher
{
public:
	RobotManager();
	~RobotManager();

	Robot* add(Socket* socket, Robot* robot);
	void removeRobat(Socket* socket);
	Robot* getRobat(Socket* socket);
	void update(float32 time, float32 delay);
protected:
	int onNetPingNotify(Robot* robot, NetPingNotify* nfy);
	int onNetLoginRes(Robot* robot, NetLoginRes* ref);
	int onNetEntityEnterMapNotify(Robot* robot, NetEntityEnterMapNotify* nfy);
	int onNetEntityLeaveMapNotify(Robot* robot, NetEntityLeaveMapNotify* nfy);
	int onNetPlayerEnterViewNotify(Robot* robot, NetPlayerEnterViewNotify* nfy);
	int onNetPlayerLeaveViewNotify(Robot* robot, NetPlayerLeaveViewNotify* nfy);
	int onNetEntityPropertyNotify(Robot* robot, NetEntityPropertyNotify* nfy);
	int onNetChatMsgNotify(Robot* robot, NetChatMsgNotify* nfy);
protected:
	int32 onGmCmd(CmdEvent& e);
protected:
	std::map<Socket*, Robot*>		mMapSocketRobat;
	std::map<int32, Robot*>			mMapRobat;
public:
	Robot*							mCurRobot;
	char*			mMapData;
	float			mCell;
};

