#include "stdafx.h"

RobotManager::RobotManager()
{
	mCell = 0.5f;
	mMapData = new char[100 * 100];
	memset(mMapData, 0, 100 * 100);

	//addEventListener(ID_NetPingNotify, (EventCallbackProcess)&RobotManager::onNetPingNotify, this);
	addEventListener(ID_NetLoginRes, (EventCallbackProcess)&RobotManager::onNetLoginRes, this);
	//addEventListener(ID_NetPropertyRes, (EventCallbackProcess)&SocketHandler::onNetPropertyRes, this);
	//addEventListener(ID_NetEntityEnterMapNotify, (EventCallbackProcess)&RobotManager::onNetEntityEnterMapNotify, this);
	//addEventListener(ID_NetEntityLeaveMapNotify, (EventCallbackProcess)&RobotManager::onNetEntityLeaveMapNotify, this);

	addEventListener(ID_NetPlayerEnterViewNotify, (EventCallbackProcess)&RobotManager::onNetPlayerEnterViewNotify, this);
	//addEventListener(ID_NetPlayerLeaveViewNotify, (EventCallbackProcess)&RobotManager::onNetPlayerLeaveViewNotify, this);

	addEventListener(ID_NetEntityPropertyNotify, (EventCallbackProcess)&RobotManager::onNetEntityPropertyNotify, this);
}

RobotManager::~RobotManager()
{
}

Robot* RobotManager::add(Socket* socket, Robot* robot)
{
	auto itr = mMapSocketRobat.find(socket);
	if (itr != mMapSocketRobat.end())
		return NULL;

	mMapSocketRobat.insert(std::make_pair(socket, robot));
	return robot;
}

void RobotManager::removeRobat(Socket* socket)
{
	auto itr = mMapSocketRobat.find(socket);
	if (itr != mMapSocketRobat.end())
		mMapSocketRobat.erase(itr);
}

Robot* RobotManager::getRobat(Socket* socket)
{
	auto itr = mMapSocketRobat.find(socket);
	if (itr != mMapSocketRobat.end())
		return itr->second;

	return NULL;
}

void RobotManager::update(float32 time, float32 delay)
{
	for (auto itr : mMapSocketRobat)
	{
		itr.second->update(time, delay);
	}
}

int RobotManager::onNetPingNotify(Robot* robot, NetPingNotify* nfy)
{
	std::string str;
	DateTime::getNow(str);
	LOG_INFO("[%s] ping [%d]", str.c_str(), GetTickCount() - nfy->time);

	return 0;
}

int RobotManager::onNetLoginRes(Robot* robot, NetLoginRes* res)
{
	if (res->result == 0)
	{
		LOG_DEBUG(LogSystem::csl_color_green, "user:%s accountId:%d instanceId:%d", res->accountInfo.user.c_str(), res->accountInfo.id, res->instanceId);
		NetPingNotify nfy;
		nfy.time = GetTickCount();
		robot->sendPacket(nfy);

		robot->mInstanceId = res->instanceId;
		robot->mAccountId = res->accountInfo.id;
		robot->mIsLogin = true;

		NetGmMsg msg;
		msg.name = "test";
		msg.gmParams.push_back("p1");
		msg.gmParams.push_back("p2");
		msg.gmParams.push_back("p3");
		robot->sendPacket(msg);


	}
	else
	{
		printf("login fail [%s]\n", INSTANCE(ConfigManager).getMsg(res->result).c_str());
	}
	return 0;
}

int RobotManager::onNetEntityEnterMapNotify(Robot* robot, NetEntityEnterMapNotify* nfy)
{
	//LOG_INFO("entity %s instanceId:%d enter mapId:%d", nfy->name.c_str(), nfy->instanceId, nfy->mapId);
	return 0;
}

int RobotManager::onNetEntityLeaveMapNotify(Robot* robot, NetEntityLeaveMapNotify* nfy)
{
	//LOG_INFO("player instanceId:%d leave mapId:%d", nfy->instanceId, nfy->mapId);
	return 0;
}

int RobotManager::onNetPlayerEnterViewNotify(Robot* robot, NetPlayerEnterViewNotify* nfy)
{
	if (nfy->guid == robot->mInstanceId)
	{
		LOG_INFO("entity %s instanceId:%d enter %s view", nfy->name.c_str(), nfy->guid, robot->user.c_str());

		robot->mPosX = nfy->pos.x;
		robot->mPosY = nfy->pos.y;
		robot->mInMap = true;
		Vector2 pos;
		pos = Utils::Point2ToVector2(nfy->pos.x, nfy->pos.y, mCell, mCell);
		robot->mPos = pos;
		robot->mSendPos.x = robot->mPosX;
		robot->mSendPos.y = robot->mPosY;
		robot->mSpeed = nfy->speed;
		INSTANCE(SocketHandler).createRobot();
	}


	return 0;
}

int RobotManager::onNetPlayerLeaveViewNotify(Robot* robot, NetPlayerLeaveViewNotify* nfy)
{
	LOG_INFO("player instanceId:%d leave %s view", nfy->guid, robot->user.c_str());
	return 0;
}

int RobotManager::onNetEntityPropertyNotify(Robot* robot, NetEntityPropertyNotify* nfy)
{
	Dictionary dict;
	nfy->property >> dict;

	LOG_DEBUG(LogSystem::csl_color_green, "Guid[%d][%s]", nfy->guid, robot->user.c_str());
	for (auto itr : dict.MapVariant())
	{
		const Variant& var = itr.first;
		const Variant& second = itr.second;

		if (var.type() == Variant::TypeString)
			LOG_INFO("type:[%s] value:[%s]", var.valueString(), second.toString().c_str());
		else
			LOG_INFO("type:[%s] value:[%s]", var.toString().c_str(), second.toString().c_str());
		//LOG_INFO("type:[%s] value:[%s]", Property::GetPropertyName(itr.first.valueInt32()).c_str(), itr.second.toString().c_str());
	}
	LOG_DEBUG(LogSystem::csl_color_green, "Guid");
	return 0;
}
