// DrawRoundWnd.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DrawRoundWnd.h"
#include <atlimage.h> 

using namespace Gdiplus;

#pragma comment (lib,"GdiPlus.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawLayeredWindow(HWND handle_wnd);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
  GdiplusStartupInput gdiplus_startup_input;
  ULONG_PTR gdi_token;

  GdiplusStartup(&gdi_token, &gdiplus_startup_input, NULL);

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAWROUNDWND, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWROUNDWND));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

  GdiplusShutdown(gdi_token);

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAWROUNDWND));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);//CreatePatternBrush(bitmap_image);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED,
	   szWindowClass,
	   szTitle, 
	   WS_OVERLAPPED,
	   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	   NULL,
	   NULL,
	   hInstance,
	   NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   DrawLayeredWindow(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_LBUTTONDOWN: 
		PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return 0;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void DrawLayeredWindow(HWND handle_wnd)
{	
  Image image(_T("zoomAppIcon.png"));
  int image_width = image.GetWidth();
  int image_height = image.GetHeight();

  HDC hdc = ::GetDC(handle_wnd);
  HDC mem_dc = ::CreateCompatibleDC(hdc);	

  HBITMAP mem_bitmap = (HBITMAP)::CreateCompatibleBitmap(hdc, image_width, image_height);	
  HBITMAP old_bitmap = (HBITMAP)::SelectObject(mem_dc, mem_bitmap); 	

  Graphics graphis(mem_dc);
  graphis.DrawImage(&image, 0, 0, image_width, image_height);
  
  CRect rcWindow;	
  GetWindowRect(GetDesktopWindow(), rcWindow);
  POINT pos_window = {(rcWindow.right-image_width)/2, (rcWindow.bottom-image_height)/2};	
  SIZE size_window = {image_width, image_height};	
  
  POINT point_src = {0, 0};
 
  BLENDFUNCTION bf;	
  bf.BlendOp = AC_SRC_OVER;	
  bf.BlendFlags = 0;	
  bf.SourceConstantAlpha = 255;
  bf.AlphaFormat = AC_SRC_ALPHA; 	
  
  ::UpdateLayeredWindow(handle_wnd, hdc, &pos_window, &size_window, mem_dc, &point_src, 0, &bf, ULW_ALPHA);
  
  ::SelectObject(mem_dc, old_bitmap); 	
  if(mem_dc) ::DeleteDC(mem_dc);
  if(mem_bitmap) ::DeleteObject(mem_bitmap);
  ::ReleaseDC(handle_wnd, hdc);
}
