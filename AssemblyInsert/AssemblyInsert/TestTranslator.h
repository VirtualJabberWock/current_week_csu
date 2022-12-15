#pragma once
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

class TestTranslator
{
public:
	static vector<BYTE> fromHexString(string str);
private:
	static vector<BYTE> HexToBytes(string hex);
	static vector<string> split(string s, string delimiter);
};

