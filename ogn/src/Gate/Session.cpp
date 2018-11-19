#include "stdafx.h"

uint32 Session::sId = 0;

uint32 Session::getSocketId()
{
	return socket ? socket->getSocketId() : 0;
}

void Session::sendPacketToTarget(Packet& packet, Socket* target)
{
	if (!target)
		return;

	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << getSsnId();
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
	if (!socket)
		return;

	static char output[PACKET_MAX_LENGTH];
	BinaryStream  transform(output, PACKET_MAX_LENGTH);
	transform << getSsnId();
	transform << count;
	transform.write(data, count);
	target->sendBuffer(transform.datas(), transform.wpos());
}

void Session::sendPacket(Packet& packet)
{
	if (!socket)
		return;

	socket->sendPacket(packet);
}

void Session::sendBuffer(void* data, int32 count)
{
	if (!socket)
		return;

	socket->sendBuffer(data, count);
}

void Session::sendPacketToWorld(Packet& packet)
{
	if (!socket)
		return;

	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << getSsnId();
	int32 pos = in.wpos();
	in << packetCount;
	int32 offset = in.wpos();
	in << packet;
	packetCount = in.wpos() - offset;
	packetCount = Shared::htonl(packetCount);

	in.push(pos, &packetCount, sizeof(int32));
	socket->sendBuffer(in.datas(), in.wpos());
}

void Session::sendBufferToWorld(void* data, int32 count)
{
	if (!socket)
		return;

	static char output[PACKET_MAX_LENGTH];
	BinaryStream  transform(output, PACKET_MAX_LENGTH);
	transform << getSsnId();
	transform << count;
	transform.write(data, count);
	socket->sendBuffer(transform.datas(), transform.wpos());
}

Session::Session(Socket* s):
sessionId(0),
player(NULL),
socket(s)
{

}

Session::~Session()
{

}
