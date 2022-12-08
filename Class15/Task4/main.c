#include <stdio.h>
#include <stdlib.h>

#define loop(f, a) for(char a = 'A'; a < 'z'; a++) {f};

int virtual_f[3] = {
	-1519094430, //ApplE
	-827415954,  //EvenT
	-2075482121  //WorDs
};

unsigned int HashRot13(char* str);

int main() {
	char* s = (char*)calloc(6, sizeof(char));
	if (s == 0) return -1;
	s[5] = '\0';
	char i, a, b, c, d;
	loop(loop(loop(loop(loop(
	s[0] = i; s[1] = a; s[2] = b;
	s[3] = c; s[4] = d;
	if (HashRot13(s) == virtual_f[0]) printf("%s\n", s);
	if (HashRot13(s) == virtual_f[1]) printf("%s\n", s);
	if (HashRot13(s) == virtual_f[2]) printf("%s\n", s);
	, i), a), b), c), d);
}

unsigned int HashRot13(char* str)
{
	unsigned int hash = 0;
	for (; *str; str++)
	{
		hash += (unsigned char)(*str);
		hash -= (hash << 13) | (hash >> 19);
	}
	return hash;
}