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

private:
  HINSTANCE cur_instance;

};