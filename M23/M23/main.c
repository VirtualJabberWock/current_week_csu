#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int bitSearch64(__int64 n);
__declspec(dllexport) int bitSearchAsm(__int64 n);
int bitSearchNative(__int64 n);

int main() {
	printf("Input number: ");
	__int64 n64 = 0;
	int s = scanf_s("%lld", &n64);
	if (s == 0) return -1;
	printf("bin64: %u\n", bitSearch64(n64));
	printf("native: %u\n", bitSearchNative(n64));
	printf("asm: %u\n", bitSearchAsm(n64));
	//test();
	return 0;
}

const int MASK32 = UINT_MAX / 0xf;
const int MASK32_2 = UINT_MAX ^ 0xf0f0f0f0;
const int MASK32_4 = UINT_MAX ^ 0xff00ff00;

//O(const) ��� O(Log(N)) ��� ������ ������
int bitSearch64(__int64 n)
{
	__int64 o =  n & -n; // ������� ������ ���� �������� ������ ������
	if (!o) return 0; 
	int t = o - 1;
	char t64 = (o > UINT_MAX); // ��� ������ ����� ������� ���� 32
	if (t64) t >> 32; // ����� �� 32
	t = (t & (MASK32*5)) + ((t >> 1) & (MASK32*5)); // ���������� �� 5(101), �.� 101010101010...
	t = (t & (MASK32*3)) + ((t >> 2) & (MASK32*3)); // ���������� �� 3(11), �.� 110011001100...
	t = (t & MASK32_2) + ((t >> 4) & MASK32_2); // ���������� �� 15(1111), �.� 1111000011110000...
	t = (t & MASK32_4) + ((t >> 8) & MASK32_4); // ���������� �� 31(11111111), �.� 1111111100000000...
	return ((t & 0xffff) + (t >> 16))+1+t64*32; // ������� ������� � ������ ������ � 1
}

//FASTER
__declspec(dllexport) int bitSearchAsm(__int64 n)
{
	__int64 o = n & -n;
	if (!o) return 0;
	__asm {
		bsr eax, esi 
		add eax, 1
	}
} 

int bitSearchNative(__int64 n)
{
	return __lzcnt()
}
