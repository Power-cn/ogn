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
	int32 pos = in.wpos();
	in << packetCount;
	in << packet;
	packetCount = in.wpos() - sizeof(int32) - sizeof(uint64);
	packetCount = Shared::htonl(packetCount);

	in.push(pos, &packetCount, sizeof(int32));
	target->sendBuffer(in.datas(), in.wpos());
}

void Session::sendBufferToTarget(void* data, int32 count, Socket* target)
{
	if (!getSocket())
		return;

	static char output[PACKET_MAX_LENGTH];
	BinaryStream  transform(output, PACKET_MAX_LENGTH);
	transform << getSessionId();
	transform << count;
	transform.write(data, count);
	target->sendBuffer(transform.datas(), transform.wpos());
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

void Session::sendPacketToDB(Packet& packet)
{
	sApp.sendPacketToDB(packet, this);
}

void Session::sendPacketToWorld(Packet& packet)
{
	if (!mSocket)
		return;

	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << getSessionId();
	int32 pos = in.wpos();
	in << packetCount;
	in << packet;
	packetCount = in.wpos() - sizeof(int32) - sizeof(uint64);
	packetCount = Shared::htonl(packetCount);

	in.push(pos, &packetCount, sizeof(int32));
	mSocket->sendBuffer(in.datas(), in.wpos());
}

void Session::sendBufferToWorld(void* data, int32 count)
{
	if (!mSocket)
		return;

	static char output[PACKET_MAX_LENGTH];
	BinaryStream  transform(output, PACKET_MAX_LENGTH);
	transform << getSessionId();
	transform << count;
	transform.write(data, count);
	mSocket->sendBuffer(transform.datas(), transform.wpos());
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
