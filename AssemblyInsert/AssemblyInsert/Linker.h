#pragma once
#include <Windows.h>
#include <vector>

using namespace std;

class Linker
{
public:

	vector<BYTE> build();

	static vector<BYTE> getByteForm(int value);
	static vector<BYTE> getByteForm(__int64 value);
	static bool getPointerDistance(void* base, void* ptr, int* out);
	static __int64 getHeapPointerDistance(void* base, void* ptr);
};