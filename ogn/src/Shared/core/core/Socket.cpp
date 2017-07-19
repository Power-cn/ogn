#include "Shared.hpp"
Socket::Socket(void):
network(NULL),
angent(NULL),
readOverlapped(),
writeOverlapped()
{
	readStream = new CircularBuffer(PACKET_MAX_LENGTH * 2);
	startSend = false;
	mSocketId = 0;
}

Socket::~Socket(void)
{
	SAFE_DELETE(readStream);

	if (mSocketId != INVALID_SOCKET && mSocketId != 0)
		closesocket(mSocketId);
	mSocketId = 0;
	while (sendQueue.size())
	{
		StreamBuffer packet = sendQueue.front();
		sendQueue.pop();
		delete [] packet.buffer;
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

