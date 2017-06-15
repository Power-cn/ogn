#include <event.h>
#include "Shared.hpp"
SocketAngent::SocketAngent() :
mSocketId(0),
mSocket(NULL)
{
}

SocketAngent::~SocketAngent()
{
	if (mSocketId != INVALID_SOCKET && mSocketId != 0)
		evutil_closesocket(mSocketId);
	mSocketId = 0;
}
