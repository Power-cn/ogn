#include "stdafx.h"

uint32 Session::sId = 0;

uint32 Session::getSocketId()
{
	return mSocket ? mSocket->getSocketId() : 0;
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
	static char input[PACKET_MAX_LENGTH] = { 0 };
	BinaryStream in(input, PACKET_MAX_LENGTH);
	int32 packetCount = 0;
	in << (int8)Snd_Ssn;
	in << getSsnId();
	int32 pos = in.wpos();
	in << packetCount;
	int32 offset = in.wpos();
	in << packet;
	packetCount = in.wpos() - offset;
	packetCount = Shared::htonl(packetCount);
	in.push(pos, &packetCount, sizeof(int32));
	GetDBServer()->sendBuffer(in.datas(), in.wpos());
}

void Session::sendPacketToWorld(Packet& packet)
{
	if (!mSocket) return;

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
	mSocket->sendBuffer(in.datas(), in.wpos());
}

void Session::sendBufferToWorld(void* data, int32 count)
{
	if (!mSocket) return;

	static char output[PACKET_MAX_LENGTH];
	BinaryStream  transform(output, PACKET_MAX_LENGTH);
	transform << getSsnId();
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
