#pragma once
#include <Windows.h>

class ZoomLogo
{
public:
  ZoomLogo();
  virtual ~ZoomLogo();

  HWND CreateWnd(HINSTANCE hInstance, int nCmdShow);
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
  static void DrawLogo(HWND hWnd, HDC hdc);
  static void DrawCircle(HDC hDC, int x, int y, int len, COLORREF color);
  static void DrawGradients(HDC hdc, int x, int y, int len, COLORREF color);
  static void DrawRoundRectangle(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color);
  static void DrawPolygon(HDC hdc, int x, int y, int len, COLORREF color);

  static void DrawText(HDC hDC, int x, int y, LPCTSTR text, COLORREF color);

  static void OnPaint(HWND hWnd, HDC hDC);
  static void OnSize(HWND hWnd);
  static BOOL OnEraseBKGround(HWND hWnd, HDC hDC);

  static void GetCenterPoint(HWND hWnd);

private:
  HINSTANCE        cur_instance_;
  static POINT     center_point_;
  static int       rect_size_;
};