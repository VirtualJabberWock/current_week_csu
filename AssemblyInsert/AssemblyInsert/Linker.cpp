#include "Linker.h"

vector<BYTE> Linker::build()
{
    return vector<BYTE>();
}

vector<BYTE> Linker::getByteForm(int value)
{
    vector<BYTE> arrayOfByte(4);
    for (int i = 0; i < 4; i++)
        arrayOfByte[i] = (value >> (i * 8));
    return arrayOfByte;
}

vector<BYTE> Linker::getByteForm(__int64 value)
{
    vector<BYTE> arrayOfByte(8);
    for (int i = 0; i < 8; i++)
        arrayOfByte[i] = (value >> (i * 8));
    return arrayOfByte;
}

bool Linker::getPointerDistance(void* base, void* ptr, int* out)
{
    __int64 z =  (__int64)ptr - (__int64)base;
    if (z < INT_MIN || z > INT_MAX) return false;
    *out = z;
    return true;
}

__int64 Linker::getHeapPointerDistance(void* base, void* ptr)
{
    return (__int64)ptr - (__int64)base;
}
