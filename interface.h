#ifndef INTERFACE_H
#define INTERFACE_H

#include <windows.h>

LRESULT CALLBACK TicProc(HWND hWnd,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam);

typedef struct {
	HWND hWnd;
	HMENU hMenu;
	HBITMAP hBitmapX;
	HBITMAP hBitmapO;
	char blocked;
} InterfaceContext;

#endif