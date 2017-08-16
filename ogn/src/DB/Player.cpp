#include "stdafx.h"


void Player::sendPacket(Packet& packet) 
{
	if (ssn == NULL) return;
	ssn->sendPacketToWorld(packet);
}