#include <corecrt_malloc.h>
#include <iostream>
#include <stdio.h>

#include "WinMemUtils.h"
#include "ExecuteAsm.h"
#include "TestTranslator.h"
#include "base64.h"
#include "Linker.h"

typedef vector<BYTE> ByteV;

using namespace std;

extern "C" __declspec(dllexport) int test_template(int,int);
extern "C" __declspec(dllexport) int retro(int);

void printBytes(vector<BYTE> bytes);
char* makeFunc();

int main() {

	string a = "44 8B D2 33 D2 44 8B C2 45 85 D2 7E 13 45 8B CA 8D 04 0A 99 41 F7 FA 44 33 C2 49 83 E9 01 75 F0 41 8B C0 C3";
	
	char* reg = makeFunc();
	vector<int> args = vector<int>();
	args.push_back(35);
	args.push_back(12);
	int b = ExecuteAsm::executeIntInt(reg, 500, args);
	free(reg);

	printf("\nOkay");

	/*for (int i = 0; i < 100; i++) {
		b = ExecuteAsm::executeIntInt(reg, 500, args);
	}*/
	//cout << b << endl;
	return b;
}

extern "C" __declspec(dllexport) int test_template(int a, int b) {
	return retro(a + b);
}

extern "C" __declspec(dllexport) int retro(int a) {
	int c = a + (__security_cookie % 100);
	char buffer[64] = { 0 };
	buffer[63] = '\0';
	sprintf_s(buffer, 64, "Sum = %d", c);
	SetWindowTextW(GetConsoleWindow(), WinMemUtils::strToWide(buffer).c_str());
	return c;
}

void printBytes(vector<BYTE> bytes)
{
	printf("\n");
	for (int i = 0; i < bytes.size(); i++) {
		printf("%hhx ", bytes[i]);
	}
	printf("\n");
}

char* makeFunc()
{
	ByteV bytes = vector<BYTE>();
	ByteV temp;
	//            0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 ?? 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43
	string raw = "89 54 24 10 89 4C 24 08 48 83 EC 28 8B 44 24 38 8B 4C 24 30 03 C8 8B C1 8B C8 49 BB 00 00 00 00 00 00 00 00 41 FF D3 48 83 C4 28 C3";
	//49 BB E0 13 9A 22 F6 7F 00 00 41 FF D3
	bytes = TestTranslator::fromHexString(raw);
	char* region = (char*)malloc(sizeof(char) * 500);
	if (region == 0) exit(-20);
	for (int i = 0; i < bytes.size(); i++) {
		region[i] = bytes[i];
	}
	__int64 d1 = (__int64) retro;
	temp = Linker::getByteForm(d1);
	for (int i = 28; i < 36; i++) {
		region[i] = temp[i - 28];
	}
	region[100] = '\0';
	return region;
}
