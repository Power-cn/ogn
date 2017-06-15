#include "Shared.hpp"
#include <event.h>
Socket::Socket(void):
network(NULL),
packetCount(0)
{
	readStream = new CircularBuffer(PACKET_MAX_LENGTH * 2);
	readEvent = (struct event*)malloc(sizeof(event));
	writeEvent = (struct event*)malloc(sizeof(event));
	startSend = false;
	mSocketId = 0;
}

Socket::~Socket(void)
{
	SAFE_DELETE(readStream);
	event_free(readEvent);
	readEvent = NULL;
	event_free(writeEvent);
	writeEvent = NULL;

	if (mSocketId != INVALID_SOCKET && mSocketId != 0)
		evutil_closesocket(mSocketId);
	mSocketId = 0;
	while (queueSend.size())
	{
		StreamBuffer& packet = queueSend.front();
		queueSend.pop();
		free(packet.buffer);
	}
	network = NULL;
}

void Socket::sendBuffer(void* data, int32 count)
{
	network->postSend(this, data, count);
}

void Socket::sendPacket(Packet& packet)
{
	network->postSend(this, packet);
}

void Socket::sendPacket(void* data, int32 count)
{
	//network->postSendEncode(this, data, count);
}

