#include "zoom_logo.h"
#include <math.h>

POINT ZoomLogo::center_point_;
int   ZoomLogo::rect_size_ = 0;

ZoomLogo::ZoomLogo()
{

}

ZoomLogo::~ZoomLogo()
{

}

HWND ZoomLogo::CreateWnd(HINSTANCE hInstance, int nCmdShow)
{
  cur_instance_ = hInstance;
  
  // Register class
  WNDCLASSEX wcex;
  
  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style        = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc  = WndProc;
  wcex.cbClsExtra   = 0;
  wcex.cbWndExtra   = 0;
  wcex.hInstance    = hInstance;
  wcex.hIcon      = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
  wcex.hCursor    = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName  = L"wndZoomLogo";
  wcex.hIconSm    = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

  RegisterClassEx(&wcex);

  HWND hWnd = NULL;
  DWORD wnd_style = WS_OVERLAPPEDWINDOW;
  DWORD wnd_style_ex = 0;
  hWnd = CreateWindowEx(wnd_style_ex, L"wndZoomLogo", L"ZoomLogo", wnd_style,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  if (hWnd)
  {
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
  }

  return hWnd;
}

void ZoomLogo::GetCenterPoint(HWND hWnd)
{
  RECT rect;
  GetClientRect(hWnd, &rect);

  center_point_.x = rect.right / 2;
  center_point_.y = rect.bottom / 2;

  rect_size_ = rect.right < rect.bottom ? rect.right : rect.bottom;
}

LRESULT CALLBACK ZoomLogo::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    DrawLogo(hWnd, hdc);
    EndPaint(hWnd, &ps);
    break;
  case WM_SIZE:
    OnSize(hWnd);
    break;
  case WM_ERASEBKGND:
    hdc = GetDC(hWnd);
    OnEraseBKGround(hWnd, hdc);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}

void ZoomLogo::OnSize(HWND hWnd)
{
  GetCenterPoint(hWnd);
}

BOOL ZoomLogo::OnEraseBKGround(HWND hWnd, HDC hDC)
{
  HBRUSH hBrush = CreateSolidBrush(RGB(1,1,1));
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  RECT rect_client;
  GetClientRect(hWnd, &rect_client);
  PatBlt(hDC, rect_client.left, rect_client.top, rect_client.right - rect_client.left, 
    rect_client.bottom - rect_client.top, PATCOPY);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hBrush);

  return TRUE;
}

void DrawRoundRectangle(HDC hDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color)
{
  HRGN hrgn_roundrect = CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);

  HRGN hrgn_rect_lefttop = CreateRectRgn(x1, y1, x1 + x3, y1 + y3);
  CombineRgn(hrgn_roundrect, hrgn_rect_lefttop, hrgn_roundrect, RGN_OR);

  HRGN hrgn_rect_rightbottom = CreateRectRgn(x2 - x3, y2 - y3, x2 - 1, y2 - 1);
  CombineRgn(hrgn_roundrect, hrgn_rect_rightbottom, hrgn_roundrect, RGN_OR);

  HBRUSH hBrush = CreateSolidBrush(color);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  FillRgn(hDC, hrgn_roundrect, hBrush);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hOldBrush);
}

void DrawRightPart(HDC hDC, int width, int hight, float factor, float y2, float x3, float y3,COLORREF color)
{
  HDC hMemDC = CreateCompatibleDC(hDC);
  HBITMAP hMemBitmap = CreateCompatibleBitmap(hDC, width, hight);
  HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hMemBitmap);
  PatBlt(hMemDC, 0, 0, width, hight, PATCOPY);

  SetGraphicsMode(hDC, GM_ADVANCED);

  XFORM xForm;
  xForm.eM11 = 1.0;
  xForm.eM12 = 0.0;
  xForm.eM21 = 0.0;
  xForm.eM22 = 1.0;
  xForm.eDx = x3;
  xForm.eDy = y3;

  SetWindowOrgEx(hDC, 0, -hight, NULL);
  for (int i = 0; i < width; i++)
  {
    xForm.eM22 = (factor + i) / factor;
    SetWorldTransform(hDC, &xForm);
    BitBlt(hDC, i, -hight/2, 1, hight, hMemDC, 0, 0, SRCCOPY);
  }
}

void ZoomLogo::DrawLogo(HWND hWnd, HDC hDC)
{
  // Draw Circle
  int x_circle_center = center_point_.x;
  int y_circle_center = center_point_.y;
  int circle_radius1 = rect_size_ / 2;

  int circle_scale_factor1 = 20;
  int circle_radius2 = circle_radius1 + circle_scale_factor1;

  int circle_scale_factor2 = 10;
  int circle_radius3 = circle_radius2 + circle_scale_factor2;

  DrawCircle(hDC, x_circle_center, y_circle_center, circle_radius3, RGB(190, 190, 190));
  DrawCircle(hDC, x_circle_center, y_circle_center, circle_radius2, RGB(250, 250, 250));
  DrawCircle(hDC, x_circle_center, y_circle_center, circle_radius1, RGB(45, 182, 254));

  // Draw text
  int xpos_text = center_point_.x;
  int ypos_text = center_point_.y + rect_size_ / 3;
  COLORREF text_color = RGB(0, 0, 255);
  LPCTSTR text = L"Hello Zoom";
  DrawText(hDC, xpos_text, ypos_text, text, text_color);

  // Draw left rectangle 
  int rect_wight = rect_size_ * 8 / 33;
  int rect_height = rect_size_ * 6 / 32;
  int x_pos = x_circle_center - rect_wight  / 2;
  int y_pos = y_circle_center - rect_height / 2;
  int left_offset = rect_size_ / 20;
  x_pos -= left_offset;
  int w = rect_size_ / 10;
  int h = rect_size_ / 10;

  DrawRoundRectangle(hDC, x_pos, y_pos, x_pos + rect_wight, y_pos + rect_height, w, h, RGB(255,255,255));

  // Draw right 
  COLORREF right_color = RGB(100, 255, 255);
  int right_margin = rect_height / 10;
  int right_rect_width = rect_wight / 3;
  int right_rect_height = rect_height / 2;
  float scale_faltor = (float)right_rect_width;
  DrawRightPart(hDC, right_rect_width, right_rect_height, scale_faltor, 0, 
    x_pos + rect_wight + right_margin, y_pos, right_color);

  return;
}

void ZoomLogo::DrawCircle(HDC hDC, int x, int y, int len, COLORREF color)
{
  HBRUSH hBrush = CreateSolidBrush(color);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  HPEN hPen = CreatePen(PS_SOLID, 1, color);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

  Ellipse(hDC, x-len/2, y-len/2, x+len/2, y+len/2);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hOldBrush);

  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);
}

void ZoomLogo::DrawText(HDC hDC, int x, int y, LPCTSTR text, COLORREF color)
{
  int text_width = x / 5;
  int text_heigth = text_width*5 / 12;

  HFONT new_font = CreateFont(
    text_width,
    text_heigth,
    0,
    0,
    FW_LIGHT,
    0,
    0,
    0,
    GB2312_CHARSET,
    OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,
    L"Segoe UI");

  HFONT old_font = (HFONT)SelectObject(hDC, new_font);

  SetTextColor(hDC, color);
  SetBkColor(hDC, RGB(0, 0, 0));

  SIZE size;
  GetTextExtentPoint(hDC, text, lstrlen(text), &size);
  int xpos_offset = size.cx/2;

  TextOut(hDC, x - xpos_offset, y, text, lstrlen(text));

  SelectObject(hDC, old_font);
  DeleteObject(new_font);
}
