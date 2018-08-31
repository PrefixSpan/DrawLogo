#include "stdafx.h"
#include <Windows.h>

static TCHAR szAppName[] = TEXT("GDI Test");
static LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  HWND     hWnd;
  MSG      msg;
  WNDCLASS wndclass;

  wndclass.style         = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc   = WndProc;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = hInstance;
  wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName  = NULL;
  wndclass.lpszClassName = szAppName;

  if (!RegisterClass(&wndclass))
  {
    MessageBox (NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
    return 0;
  }

  hWnd = CreateWindow(szAppName,            // window class name
    szAppName,           // window caption
    WS_OVERLAPPEDWINDOW, // window style
    CW_USEDEFAULT,       // initial x position
    CW_USEDEFAULT,       // initial y position
    400,                 // initial x size
    300,                 // initial y size
    NULL,                // parent window handle
    NULL,                // window menu handle
    hInstance,           // program instance handle
    NULL);               // creation parameters

  ShowWindow(hWnd, iCmdShow);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

//绘制指定属性的直线
static void DrawLine(HDC hDC, int x0, int y0, int x1, int y1, int style, int width, COLORREF color)
{
  HPEN hPen = CreatePen(style, width, color);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

  MoveToEx(hDC, x0, y0, NULL);
  LineTo(hDC, x1, y1);

  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);
}

//绘制实心圆
static void DrawCircle(HDC hDC, int x, int y, int len, COLORREF color)
{
  HBRUSH hBrush = CreateSolidBrush(color);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  HPEN hPen = CreatePen(PS_SOLID, 1, color);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

  Ellipse(hDC, x-len/2, y-len/2, x+len/2, y+len/2);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hPen);

  SelectObject(hDC, hOldPen);
  DeleteObject(hOldBrush);
}

//绘制填充矩形
static void DrawRect(HDC hDC, int left, int top, int width, int height, int style, COLORREF color)
{
  HBRUSH hBrush = CreateHatchBrush(style, color);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  Rectangle(hDC, left, top, left+width, top+height);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hOldBrush);
}

//绘制位图填充矩形
static void DrawBmpRect(HDC hDC, int left, int top, int width, int height, LPCTSTR file)
{
  HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
  HBRUSH hBrush = CreatePatternBrush(hBitmap);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  Rectangle(hDC, left, top, left+width, top+height);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hOldBrush);
  DeleteObject(hBitmap);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC         hDC;
  PAINTSTRUCT ps;

  switch (message)
  {
  case WM_CREATE:
    return 0;

  case WM_PAINT:
    {
      hDC = BeginPaint(hWnd, &ps);
      for (int i = 10; i < 50; i += 4)
      {
        SetPixel(hDC, i, 10, RGB(0, 0, 0)); //绘制像素点
      }

      if (0) {
        //绘制不同模式的直线
        DrawLine(hDC, 120, 30, 200, 30, PS_SOLID, 2, RGB(0,0,0));
        DrawLine(hDC, 120, 50, 200, 50, PS_DASH, 1, RGB(100,0,200));
        DrawLine(hDC, 120, 70, 200, 70, PS_DASHDOT, 1, RGB(100,250,100));
      }
        //绘制弧线、弦割线、饼图
        Arc(hDC, 10, 30, 40, 50, 40, 30, 10, 40);
        Chord(hDC, 10, 60, 40, 80, 40, 60, 10, 70);
        Pie(hDC, 10, 90, 40, 110, 40, 90, 10, 100);

        if (0) {
          POINT pt[4] = {{90,130},{60,40},{140,150},{160,80}};
        //绘制椭圆、矩形
        Ellipse(hDC,pt[0].x, pt[0].y, pt[1].x, pt[1].y);
        Rectangle(hDC, pt[2].x, pt[2].y, pt[3].x, pt[3].y);

        //绘制贝塞尔曲线
        PolyBezier(hDC, pt, 4);
        //标出贝塞尔曲线的四个锚点
        DrawCircle(hDC, pt[0].x, pt[0].y, 8, RGB(0,255,0));
        DrawCircle(hDC, pt[1].x, pt[1].y, 8, RGB(0,0,255));
        DrawCircle(hDC, pt[2].x, pt[2].y, 8, RGB(0,0,0));
        DrawCircle(hDC, pt[3].x, pt[3].y, 8, RGB(255,0,0));
        //绘制圆
        DrawCircle(hDC, 100, 180, 60, RGB(0, 250, 250));
        //绘制不同填充模式的矩形
        DrawRect(hDC, 220, 20, 60, 40, HS_BDIAGONAL, RGB(255,0,0));
        DrawRect(hDC, 220, 80, 60, 40, HS_CROSS, RGB(0,255,0));
        DrawRect(hDC, 290, 20, 60, 40, HS_DIAGCROSS, RGB(0,0,255));
        DrawRect(hDC, 290, 80, 60, 40, HS_VERTICAL, RGB(0,0,0));
        //绘制位图
        DrawBmpRect(hDC, 180, 140, 180, 100, TEXT("chenggong.bmp"));
        //绘制文本
        TextOut(hDC, 20, 220, TEXT("GDI画图输出测试程序"), 11);
      }
    }
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0 ;
  }

  return DefWindowProc (hWnd, message, wParam, lParam);
}