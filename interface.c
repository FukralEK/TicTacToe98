#include "resource.h"
#include "interface.h"
#include "game.h"

static InterfaceContext ctx;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	WNDCLASS wc = {0};
	MSG msg = {0};
	char toggle;

	ctx.hBitmapX = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_X));
	ctx.hBitmapO = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_O));
	
	ctx.blocked = 0;
	NewGame(0);

	wc.lpfnWndProc = TicProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "TicTacToeClass";
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_1));

	RegisterClass(&wc);
	
	ctx.hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_TIC_MENU));

	ctx.hWnd = CreateWindowEx(
		0,
		"TicTacToeClass",
		"Tic Tac Toe",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		300, 300,
		NULL,
		ctx.hMenu,
		hInstance,
		0);
	
	if (!ctx.hWnd)
	{
		return 1;
	}

	ShowWindow(ctx.hWnd, nCmdShow);
	
	toggle = 1;

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DestroyWindow(ctx.hWnd);

	return 0;
}

LRESULT CALLBACK TicProc(HWND hWnd,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	RECT rect;
	int width;
	int height;
	
	GetClientRect(hWnd, &rect);

	width = rect.right - rect.left;
    height = rect.bottom - rect.top;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		if (!ctx.blocked)
		{
			int row = HIWORD(lParam) / (height/3);
			int col = LOWORD(lParam) / (width/3);
			int cellIndex = row * 3 + col;

			TrySetCell(cellIndex);
			
			InvalidateRect(hWnd, NULL, TRUE);

			switch (CheckWin())
			{
			case WON_NOT_YET:
				break;
			case WON_X:
				ctx.blocked = 1;
				MessageBox(hWnd, "X won", "Finished", MB_OK);
				break;
			case WON_Y:
				ctx.blocked = 1;
				MessageBox(hWnd, "O won", "Finished", MB_OK);
				break;
			case WON_DRAW:
				ctx.blocked = 1;
				MessageBox(hWnd, "No one won", "Finished", MB_OK);
				break;
			}

		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			HPEN hPen;
			HPEN oldPen;
			HDC hMemDC;
			HBITMAP oldBitmap;
			int i;
			
			hdc = BeginPaint(hWnd, &ps);
			hMemDC = CreateCompatibleDC(hdc);
			
			FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW+1));

			oldBitmap = (HBITMAP)SelectObject(hMemDC, ctx.hBitmapX);
			
			for (i = 0; i < 9; i++)
			{
				BITMAP bmp;
				int x = (i % 3)*(width/3);
				int y = (i / 3)*(height/3);
				char cellState = GetCell(i);


				if (cellState == CELL_EMPTY) continue;
				else if (cellState == CELL_X) 
				{
					GetObject(ctx.hBitmapX, sizeof(BITMAP), &bmp);
					SelectObject(hMemDC, ctx.hBitmapX);
					StretchBlt(hdc, x, y, width/3, height/3, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
				}
				else if (cellState == CELL_O)
				{
					GetObject(ctx.hBitmapO, sizeof(BITMAP), &bmp);
					SelectObject(hMemDC, ctx.hBitmapO);
					StretchBlt(hdc, x, y, width/3, height/3, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
				}
				
			}

			hPen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			
			for (i = 1; i <= 2; i++)
			{
				int x = i * width / 3;
				MoveToEx(hdc, x, 0, NULL);
				LineTo(hdc, x, height);
			}

			for (i = 1; i <= 2; i++)
			{
				int y = i * height / 3;
				MoveToEx(hdc, 0, y, NULL);
				LineTo(hdc, width, y);
			}
			

			SelectObject(hMemDC, oldBitmap);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);

			DeleteDC(hMemDC);
			EndPaint(hWnd, &ps);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case ID_GAME_NEWGAME_LOCAL2PLAYERS:
				NewGame(0);
				ctx.blocked = 0;
				break;
			case ID_GAME_NEWGAME_EASY:
				NewGame(1);
				ctx.blocked = 0;
				break;
			case ID_GAME_NEWGAME_MEDIUM:
				NewGame(2);
				ctx.blocked = 0;
				break;
			case ID_GAME_NEWGAME_HARD:
				NewGame(3);
				ctx.blocked = 0;
				break;
			case ID_GAME_CLOSE:
				if (MessageBox(hWnd, "Are you sure?", "Close", MB_YESNO | MB_ICONWARNING) == IDYES)
				{
					PostQuitMessage(0);
				}
				break;

			case ID_HELP_ABOUT:
				MessageBox(hWnd, "TicTacToe by FukralEK", "About", MB_OK | MB_ICONINFORMATION);
				break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_CLOSE:
		if (MessageBox(hWnd, "Are you sure?", "Close", MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			PostQuitMessage(0);
		}
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}