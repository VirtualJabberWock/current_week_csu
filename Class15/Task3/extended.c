#include "extended.h"

typedef struct tagPiece {
	char x;
	char y;
} Piece;

int check(Piece f[8]);

int extended() {
	Piece input[8] = { 0 };
	for (int i = 0; i < 8; i++) {
		int x = 0; int y = 0;
		printf("(x, y)> ");
		if (!scanf_s("%d %d", &x, &y)) panic("Invalid input");
		if (x < 0 || x >= 8 || y < 0 || y >= 8) panic("Invalid coords");
		input[i].x = x;
		input[i].y = y;
	}
	int a = check(input);
	if (a) printf("Pair no found!");
	else printf("There is linked pair!");
	return 0;
}



int check(Piece f[8])
{
	for (int factor = 1; factor < 5; factor++) {
		int j = 8 - factor;
		for (int i = 0; i != 8 - factor; (i + factor) % 8) {
			int a =
				(f[i].x == f[j].x) || (f[i].y == f[j].y) || (abs(f[i].x - f[j].x) == abs(f[i].y - f[j].y));
			if (a) return 0;
			j = i;
		}
	}
	return 1; // 4 * 8 operations
}

//char b0 = 0;
//char b1 = 1;
//for (int i = 0; i < 8; i++) {
//b0 ^= f[i]; // horizontal 
//	b1 &= ((f[i] > 0) && (f[i] & (f[i] - 1)) == 0); // vertical
//	if (b1 == 0) return 0;
//}
//if (b0 != 0b11111111) return 0;

