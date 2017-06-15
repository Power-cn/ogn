#include <event.h>
#include "Shared.hpp"

SocketClient::SocketClient():
buffer(NULL)
{

}

SocketClient::~SocketClient()
{
	if (buffer)
	{
		bufferevent_disable(buffer, EV_PERSIST);
		bufferevent_free(buffer);
	}
	buffer = NULL;
}

void SocketClient::sendPacket(Packet& packet)
{
	if (mSocket == NULL)
		return;

	mSocket->sendPacket(packet);
}

void SocketClient::sendBuffer(void* data, int32 count)
{
	if (mSocket == NULL)
		return;

	mSocket->sendBuffer(data, count);
}
