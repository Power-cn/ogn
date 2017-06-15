#include "stdafx.h"

uint32 Session::sId = 0;

uint32 Session::getSocketId()
{
	return mSocket ? mSocket->getSocketId() : 0;
}

void Session::sendPacketToTarget(Packet& packet, Socket* target)
{
	if (!target)
		return;

	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << getSessionId();
	int32 pos = in.getWPostion();
	in << packetCount;
	in << packet;
	packetCount = in.getWPostion() - sizeof(int32) - sizeof(uint64);
	packetCount = Shared::htonl(packetCount);

	in.PushBytes(pos, &packetCount, sizeof(int32));
	target->sendBuffer(in.getPtr(), in.getWPostion());
}

void Session::sendBufferToTarget(void* data, int32 count, Socket* target)
{
	if (!getSocket())
		return;

	static char output[PACKET_MAX_LENGTH];
	BinaryStream  transform(output, PACKET_MAX_LENGTH);
	transform << getSessionId();
	transform << count;
	transform.WriteBytes(data, count);
	target->sendBuffer(transform.getPtr(), transform.getWPostion());
}

void Session::sendPacket(Packet& packet)
{
	if (!mSocket)
		return;

	mSocket->sendPacket(packet);
}

void Session::sendBuffer(void* data, int32 count)
{
	if (!mSocket)
		return;

	mSocket->sendBuffer(data, count);
}

void Session::sendPacketToWorld(Packet& packet)
{
	if (!mSocket)
		return;

	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << getSessionId();
	int32 pos = in.getWPostion();
	in << packetCount;
	in << packet;
	packetCount = in.getWPostion() - sizeof(int32) - sizeof(uint64);
	packetCount = Shared::htonl(packetCount);

	in.PushBytes(pos, &packetCount, sizeof(int32));
	mSocket->sendBuffer(in.getPtr(), in.getWPostion());
}

void Session::sendBufferToWorld(void* data, int32 count)
{
	if (!mSocket)
		return;

	static char output[PACKET_MAX_LENGTH];
	BinaryStream  transform(output, PACKET_MAX_LENGTH);
	transform << getSessionId();
	transform << count;
	transform.WriteBytes(data, count);
	mSocket->sendBuffer(transform.getPtr(), transform.getWPostion());
}

Session::Session(Socket* s):
mSessionId(0),
mPlayer(NULL),
mSocket(s)
{

}

Session::~Session()
{

}
