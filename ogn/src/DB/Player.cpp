#include "stdafx.h"


void Player::sendPacket(Packet& packet) 
{
	if (session == NULL) return;
	session->sendPacketToWorld(packet);
}