#include <winsock.h>
#pragma once
#pragma comment(lib, "ws2_32.lib")

#define _Packet char*
#define Packet const char*
#define InitPacket(len) new char[len]
#define Copy(_b, b) for (int i = 0; i < len; i++) _b[i] = b[i];

class SocketHandler
{


public:

	static const int MASK_MODE_NULL = -1;
	static const int MASK_MODE_DEF = 0;
	static const int MASK_MODE_BINARY = 1;
	static const int MASK_MODE_REPLACE = 2;

	SocketHandler();

	int len_filter = -1;

	struct PacketMask {
		int pos;
		int len;
		int mask_mode;
		char arg;
	};
	
	PacketMask sendMask = PacketMask();

	int handleSend(SOCKET, const char*, int, int);
	void setSendMask(int pos, int len, int mask_mode, char arg);

private:

	void initMask(PacketMask* mask);
	void siftArray(_Packet* packetPtr, int len, PacketMask mask);
	

};

