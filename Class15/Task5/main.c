#include "Example.h"
#include "WindowsAPI.h"
#include "Image.h"

int main() {
	InitExample();
	maximizeConsoleWindow();
	setconsolesize(256,96);
	clrscr();
	hidecursor();
	COORD c;
	c.X = 10;
	c.Y = 10;
	SetFontSize(getConsoleHandle(), c);
	ImageCache* cache = malloc(sizeof(ImageCache)); // CACHE
	const char* FILENAME = "mono.bmp\0eye.bmp";
	if (cache == 0) return -1;
	cache->name = "__default__";
	for (int shade = 10; shade < 1000; shade+=5) {
		ChunkedImage* img = loadChunkedImage(FILENAME+(shade%100 >= 50)*9, shade % 50 + 100, cache);
		for (int x = 0; x < CI_W * 2; x++) {
			for (int y = CI_H-1; y > 0; y--) { 
				gotoxy(x, y);
				printf("%c", getHPixel((8 + img->color[CI_H - y][x/2]) % 3 + 7, img->color[CI_H - y][x/2]));
			}
		}
		free(img);
		//printf("shade %d / 250 | %s :: %d x %d", shade, FILENAME, CI_W, CI_H);
	}
	_getch();
	SetFontSize(getConsoleHandle(), c);
}