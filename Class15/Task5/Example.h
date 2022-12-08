#pragma once

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void setconsolesize(int x, int y);
void hidecursor();
void clrscr();
void gotoxy(int column, int row);
int GetFontSize(HANDLE windowHandle, COORD* size);
int SetFontSize(HANDLE windowHandle, COORD size);
int example_main();


static char CHAR_MAP[11] = " .:-=+*#%@";
static HANDLE console_handle = 0;

void InitExample();
char getHPixel(int width, int color);
