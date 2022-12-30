#include "pch.h"
#include "SocketHandler.h"
#pragma comment(lib, "ws2_32.lib")

SocketHandler::SocketHandler()
{
    initMask(&sendMask);
}

int SocketHandler::handleSend(SOCKET s, const char* buf, int len, int flags)
{
    _Packet _buf = InitPacket(len);
    Copy(_buf, buf);
    siftArray(&_buf, len, sendMask);
    return send(s, _buf, len, flags); 
}

void SocketHandler::initMask(PacketMask* mask)
{
    mask->arg = 0;
    mask->mask_mode = MASK_MODE_NULL;
    mask->pos = 0;
    mask->len = 0;
}

void SocketHandler::siftArray(_Packet* packetPtr, int len, PacketMask mask)
{
    if (mask.mask_mode == MASK_MODE_NULL) return;
    if (mask.len != len) return;
    if (mask.mask_mode == MASK_MODE_DEF)
        (*packetPtr)[mask.pos] = (*packetPtr)[mask.pos] + mask.arg;
    if (mask.mask_mode == MASK_MODE_BINARY)
        (*packetPtr)[mask.pos] = (*packetPtr)[mask.pos] ^ mask.arg;
    if (mask.mask_mode == MASK_MODE_REPLACE)
        (*packetPtr)[mask.pos] = mask.arg;
}

void SocketHandler::setSendMask(int pos, int len, int mask_mode, char arg)
{
    sendMask.arg = arg;
    sendMask.len = len;
    sendMask.mask_mode = mask_mode;
    sendMask.pos = pos;
}
