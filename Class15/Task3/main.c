#include <stdio.h>
#include <stdlib.h>
#include "../../aquaUtils/utils/DebugUtils.h"
#include "extended.h"


typedef struct tagPiece {
	int x;
	int y;
} Piece;

int main() {
	Piece f[8] = { 0 };
	for (int i = 0; i < 8; i++) {
		printf("(x, y)> ");
		if (!scanf_s("%d %d", &f[i].x, &f[i].y)) panic("Invalid input");
		if (f[i].x < 0 || f[i].x >= 8 || f[i].y < 0 || f[i].y >= 8) panic("Invalid coords");
	}
	for (int factor = 1; factor < 5; factor++) {
		int j = 8 - factor;
		int i = 0;
		while (i != 8 - factor) {
			if (j == i) continue;
			int a = (f[i].x == f[j].x);
			a = a || (f[i].y == f[j].y);
			a = a || (abs(f[i].x - f[j].x) == abs(f[i].y - f[j].y));
			if (a) {
				return printf("There is linked pair!");
			}
			j = i;
			i = (i + factor) % 8;
		}
	}
	printf("There is no linked pairs");
	return 0;
}

