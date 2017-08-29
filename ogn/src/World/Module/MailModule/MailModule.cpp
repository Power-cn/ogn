#include "stdafx.h"

IMPLEMENT_CLASS(MailModule, Module);

MailModule::MailModule()
{

}

MailModule::~MailModule()
{
}

void MailModule::SendMail(Mail& aMail)
{
	switch (aMail.mailType)
	{
	case MailT_Target: {
		PlayerRecord* aPlrRcd = sFriends.FindPlrRecord(aMail.targetUserId);
		if (aPlrRcd == NULL) return;

		AddMail(aMail.targetUserId, aMail);
	}
		break;
	case MailT_Total: {
		MapPlayerRecord& mapPlayer = sFriends.GetMapPlayer();
		for (auto itr : mapPlayer)
		{
			PlayerRecord* aPlrRcd = itr.second;
			AddMail(aPlrRcd->GetUserId(), aMail);
		}
	}
		break;
	}

}

void MailModule::SendMail(cstring& title, cstring& conent)
{
	Mail aMail = {};
	aMail.mailType = MailT_Total;
	aMail.title = title;
	aMail.content = conent;
	SendMail(aMail);
}

bool MailModule::Initialize()
{
	sRedisProxy.addEventListener("OnRedisAuth", (EventCallback)&MailModule::onRedisAuth, this);
	return true;
}

bool MailModule::Update(float time, float delay)
{
	return true;
}

bool MailModule::Destroy()
{
	return true;
}

bool MailModule::onEnterWorld(Player* player, Dictionary& dict)
{
	std::vector<Mail>* lstMail = FindMail(player->getUserId());
	if (lstMail) {
		SendPlrMail(player, *lstMail);
	}
	return true;
}

bool MailModule::onLeaveWorld(Player* player, Dictionary& dict)
{

	return true;
}

bool MailModule::onLoad(Player* player, Dictionary& bytes)
{

	return true;
}

bool MailModule::onSave(Player* player, Dictionary& bytes)
{
	
	return true;
}

int32 MailModule::onRedisAuth(Event& e)
{
	char szBuffer[64] = { 0 };
	sprintf_s(szBuffer, 64, "hgetall %s", sMail);
	sRedisProxy.sendCmd(szBuffer, (EventCallback)&MailModule::onRedisAllPlr, this);
	return 0;
}

int32 MailModule::onRedisAllPlr(RedisEvent& e)
{
	float64 s0 = DateTime::GetNowAppUS();
	for (uint32 i = 0; i < e.backstr.size(); i += 2)
	{
		std::string keystr = e.backstr[i];
		std::string valuestr = e.backstr[i + 1];
		Json::Reader jsonReader;
		Json::Value root;
		jsonReader.parse(valuestr.c_str(), root);

		uint32 userId = Shared::strtoint32(keystr);
		for (uint32 j = 0; j < root.size(); ++i)
		{
			Mail aMail;
			aMail << root[j];
			AddMail(userId, aMail);
		}
	}

	return 0;
}

void MailModule::AddMail(uint32 userId, Mail& aMail)
{
	auto itr = mMapMail.find(userId);
	if (itr != mMapMail.end())
	{
		std::vector<Mail>& lstMail = itr->second;
		lstMail.push_back(aMail);
	}
	else
	{
		std::vector<Mail> lstMail;
		lstMail.push_back(aMail);
		mMapMail[userId] = lstMail;
	}
	{
		std::vector<Mail>& lstMail = mMapMail[userId];
		std::string str = WriteJson(lstMail);
		char szBuffer[4096] = {};
		sprintf_s(szBuffer, 4096, "hmset %s %d %s", sMail, userId, str.c_str());
		sRedisProxy.sendCmd(szBuffer, NULL, NULL);
		Player* aPlr = sWorld.FindPlrByUserId(userId);
		if (aPlr)
		{
			SendPlrMail(aPlr, lstMail);
		}
	}
}

std::vector<Mail>* MailModule::FindMail(uint32 userId)
{
	auto itr = mMapMail.find(userId);
	if (itr != mMapMail.end()) return &itr->second;
	return NULL;
}

std::string MailModule::WriteJson(std::vector<Mail>& lstMail)
{
	Json::Value aValue;
	for (Mail& aMail : lstMail)
	{
		Json::Value aChildValue;
		aMail >> aChildValue;
		aValue.append(aChildValue);
	}
	return aValue.toStyledString(true);
}

void MailModule::ReadJson(std::vector<Mail>& lstMail, cstring& jsonstr)
{
	Json::Reader reader;
	Json::Value aValue;
	reader.parse(jsonstr, aValue);
	for (uint32 i = 0; i < aValue.size(); ++i)
	{
		Mail aMail = {};
		aMail << aValue[i];
		lstMail.push_back(aMail);
	}
}

void MailModule::SendPlrMail(Player* aPlr, std::vector<Mail>& lstMail)
{
	NetMailListNotify nfy;
	for (Mail& aMail : lstMail)
	{
		MailInfo info;
		PlayerRecord* aPlrRcd = sFriends.FindPlrRecord(aMail.fromUserId);
		if (aPlrRcd)
		{
			info.fromUserName = aPlrRcd->GetName();
		}
		info.title = aMail.title;
		info.content = aMail.content;
		info.datastr = aMail.datastr;
		info.isDown = aMail.isDown;
		info.isRead = aMail.isRead;
		nfy.mailInfos.push_back(info);
	}
	aPlr->sendPacket(nfy);
}
