#include "stdafx.h"

IMPLEMENT_CLASS(MailModule, Module);

MailModule::MailModule()
{

}

MailModule::~MailModule()
{
}

bool MailModule::Initialize()
{
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