#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "Example.h"

void setconsolesize(int x, int y)
{
	HANDLE hCons;
	COORD crd;
	SMALL_RECT rct;
	crd.X = (short)x + 1;
	crd.Y = (short)y + 1;
	rct.Left = 0;
	rct.Top = 0;
	rct.Right = x;
	rct.Bottom = y;
	hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(hCons, crd);
	SetConsoleWindowInfo(hCons, 1, &rct);
}
void hidecursor()
{
	HANDLE hCons;
	CONSOLE_CURSOR_INFO cci;
	hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	cci.dwSize = 1;
	cci.bVisible = 0;
	SetConsoleCursorInfo(hCons, &cci);
}
void clrscr()
{
	HANDLE hCons;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD crd;
	int len;
	DWORD bRet;
	short x, y;
	char space = 0x20;
	hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hCons, &csbi);
	x = csbi.dwSize.X;
	y = csbi.dwSize.Y;
	len = (int)x * y;
	crd.X = 0;
	crd.Y = 0;
	FillConsoleOutputCharacter(hCons, space, len, crd, &bRet);
}
void gotoxy(int column, int row)
{
	HANDLE hCons;
	COORD crd;
	hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	//#ifdef MY_DEBUG
	//  if (hCons == INVALID_HANDLE_VALUE || hCons == 0) printf("\nERROR!!!\n");
	//#endif  	  
	crd.X = (short)column;
	crd.Y = (short)row;
	//#ifedf MY_DEBUG
	// if (SetConsoleCursorPosition(hCons,crd) == 0) printf("\nERROR2\n");
	//#else 
	SetConsoleCursorPosition(hCons, crd);
	//#endif
}
int GetFontSize(HANDLE windowHandle, COORD* size)
{
	CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };

	if (!GetCurrentConsoleFontEx(windowHandle, 0, &font))
	{
		return 0;
	}

	*size = font.dwFontSize;

	return 1;
}
int SetFontSize(HANDLE windowHandle, COORD size)
{
	CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };

	if (!GetCurrentConsoleFontEx(windowHandle, 0, &font))
	{
		return 0;
	}

	font.dwFontSize = size;

	if (!SetCurrentConsoleFontEx(windowHandle, 0, &font))
	{
		return 0;
	}

	return 1;
}

int example_main()
{
	setconsolesize(200, 200);
	clrscr();
	hidecursor();
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size;
	DWORD delay = 50;
	SetConsoleTextAttribute(h, FOREGROUND_BLUE | BACKGROUND_GREEN);
	printf("Game over my friend!\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | BACKGROUND_GREEN);
	printf("Hack\n");
	getchar();
	return 0;
}


char getHPixel(int width, int color)
{
	if (console_handle == 0) return;
	SetConsoleTextAttribute(console_handle, color);
	return CHAR_MAP[width % 10];
}

void InitExample()
{
	console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
}
