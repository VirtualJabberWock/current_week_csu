#include "WindowsAPI.h"
#include <stdio.h>

HANDLE getConsoleHandle()
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}

void maximizeConsoleWindow()
{
	HWND hWnd = GetConsoleWindow();
	SetWindowTextW(hWnd, L"Injection test");
	ShowWindow(hWnd, SW_MAXIMIZE);
}
