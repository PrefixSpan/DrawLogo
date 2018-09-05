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
  // Register class
  WNDCLASSEX wcex;
  
  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc    = WndProc;
  wcex.cbClsExtra     = 0;
  wcex.cbWndExtra     = 0;
  wcex.hInstance      = hInstance;
  wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
  wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName   = NULL;
  wcex.lpszClassName  = L"wndZoomLogo";
  wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

  RegisterClassEx(&wcex);

  DWORD wnd_style = WS_OVERLAPPEDWINDOW;
  DWORD wnd_style_ex = 0;
  HWND hWnd = CreateWindowEx(wnd_style_ex, L"wndZoomLogo", L"ZoomLogo", wnd_style,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  if (hWnd)
  {
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
  }

  return hWnd;
}

LRESULT CALLBACK ZoomLogo::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    OnPaint(hWnd, hdc);
    EndPaint(hWnd, &ps);
    break;
  case WM_SIZE:
    OnSize(hWnd);
    break;
  case WM_ERASEBKGND:
  //  hdc = GetDC(hWnd);
 //   OnEraseBKGround(hWnd, hdc);
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

void ZoomLogo::GetCenterPoint(HWND hWnd)
{
  RECT rect;
  GetClientRect(hWnd, &rect);

  center_point_.x = rect.right / 2;
  center_point_.y = rect.bottom / 2;

  rect_size_ = rect.right < rect.bottom ? rect.right : rect.bottom;
}

BOOL ZoomLogo::OnEraseBKGround(HWND hWnd, HDC hdc)
{
  HBRUSH new_brush = CreateSolidBrush(RGB(1, 1, 1));
  HBRUSH old_brush = (HBRUSH)SelectObject(hdc, new_brush);

  RECT rect_client;
  GetClientRect(hWnd, &rect_client);
  PatBlt(hdc, rect_client.left, rect_client.top, rect_client.right - rect_client.left, 
    rect_client.bottom - rect_client.top, PATCOPY);

  SelectObject(hdc, old_brush);
  DeleteObject(new_brush);

  return TRUE;
}

void ZoomLogo::OnPaint(HWND hWnd, HDC hdc)
{
  DrawLogo(hWnd, hdc);
}

void ZoomLogo::DrawLogo(HWND hWnd, HDC hdc)
{
  HDC hdc_mem = CreateCompatibleDC(hdc);
  RECT rect_client;
  GetClientRect(hWnd, &rect_client);
  HBITMAP bitmap = CreateCompatibleBitmap(hdc, rect_client.right, rect_client.bottom);
  SelectObject(hdc_mem, bitmap);

  // Draw Circle
  int x_circle_center = center_point_.x;
  int y_circle_center = center_point_.y;
  int circle_radius1 = rect_size_ / 2;
  int circle_radius2 = circle_radius1 + 20;
  int circle_radius3 = circle_radius2 + 10;

  DrawCircle(hdc_mem, x_circle_center, y_circle_center, circle_radius3, RGB(190, 190, 190));
  DrawCircle(hdc_mem, x_circle_center, y_circle_center, circle_radius2, RGB(250, 250, 250));
//  DrawCircle(hdc_mem, x_circle_center, y_circle_center, circle_radius1, RGB(45, 182, 254));
  DrawGradients(hdc_mem, x_circle_center, y_circle_center, circle_radius1, RGB(45, 200, 255));

  // Draw left rectangle 
  int rect_wight = rect_size_ * 8 / 33;
  int rect_height = rect_size_ * 6 / 32;
  int x_pos = x_circle_center - rect_wight  / 2;
  int y_pos = y_circle_center - rect_height / 2;
  int left_offset = rect_size_ / 20;
  x_pos -= left_offset;
  int w = rect_size_ / 10;
  int h = rect_size_ / 10;

  DrawRoundRectangle(hdc_mem, x_pos, y_pos, x_pos + rect_wight, y_pos + rect_height, w, h, RGB(255,255,255));

  // Draw right polygon 
  int right_margin = rect_height / 11;
  DrawPolygon(hdc_mem, x_pos + rect_wight + right_margin, y_circle_center, circle_radius1, RGB(255,255,255));

  // Draw text
  int xpos_text = center_point_.x;
  int ypos_text = center_point_.y + rect_size_ * 10 / 32;
  COLORREF text_color = RGB(0, 0, 255);
  LPCTSTR text = L"Hello Zoom";
  DrawText(hdc_mem, xpos_text, ypos_text, text, text_color);

  BitBlt(hdc, 0, 0, rect_client.right, rect_client.bottom, hdc_mem, 0, 0, SRCCOPY);

  return;
}

void CALLBACK Proc(int x, int y, LPARAM lpData)
{

  COLORREF color = RGB(255-x, 255-x, 100+y);
  SetPixel((HDC)lpData, x, y, color);
}

void ZoomLogo::DrawGradients(HDC hdc, int x, int y, int len, COLORREF color)
{
  int r_color = GetRValue(color);
  int g_color = GetGValue(color);
  int b_color = GetBValue(color);
  HRGN hrgn_circle = CreateEllipticRgn(x-len/2, y - len/2, x+len/2, y+len/2);
  for (int j = y - len/2; j < y+len/2; ++j)
  {
    int color_offset = (g_color-g_color/2)*j/(y+len/2);
    COLORREF color = RGB(r_color, g_color - color_offset, b_color);
    for (int i = x-len/2; i < x+len/2; ++i)
    {
      if (PtInRegion(hrgn_circle, i, j)) 
      {
        SetPixel(hdc, i, j, color);
      }
    }
  }

  return;
}

void ZoomLogo::DrawRoundRectangle(HDC hdc, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color)
{
  HRGN hrgn_roundrect = CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);

  HRGN hrgn_rect_lefttop = CreateRectRgn(x1, y1, x1 + x3, y1 + y3);
  CombineRgn(hrgn_roundrect, hrgn_rect_lefttop, hrgn_roundrect, RGN_OR);

  HRGN hrgn_rect_rightbottom = CreateRectRgn(x2 - x3, y2 - y3, x2 - 1, y2 - 1);
  CombineRgn(hrgn_roundrect, hrgn_rect_rightbottom, hrgn_roundrect, RGN_OR);

  HBRUSH new_brush = CreateSolidBrush(color);
  HBRUSH old_brush = (HBRUSH)SelectObject(hdc, new_brush);

  FillRgn(hdc, hrgn_roundrect, new_brush);

  SelectObject(hdc, old_brush);
  DeleteObject(new_brush);
}

void ZoomLogo::DrawPolygon(HDC hdc, int x, int y, int len, COLORREF color)
{
  int width = len * 8/45;
  int half_height_left = len * 8/100;
  int half_heidht_right = len * 8/40;
  POINT point[4];
  point[0].x = x;
  point[0].y = y - half_height_left;
  point[1].x = x + width;
  point[1].y = y - half_heidht_right;
  point[2].x = point[1].x;
  point[2].y = y + half_heidht_right;
  point[3].x = point[0].x;
  point[3].y = y + half_height_left;

  HRGN hrgn_polyrect = CreatePolygonRgn(point, sizeof(point)/sizeof(point[0]), WINDING);

  HBRUSH new_brush = CreateSolidBrush(color);
  HBRUSH old_brush = (HBRUSH)SelectObject(hdc, new_brush);

  FillRgn(hdc, hrgn_polyrect, new_brush);

  SelectObject(hdc, old_brush);
  DeleteObject(new_brush);
}

void ZoomLogo::DrawCircle(HDC hdc, int x, int y, int len, COLORREF color)
{
  HBRUSH hBrush = CreateSolidBrush(color);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

  HPEN hPen = CreatePen(PS_SOLID, 1, color);
  HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

  Ellipse(hdc, x-len/2, y-len/2, x+len/2, y+len/2);

  SelectObject(hdc, hOldBrush);
  DeleteObject(hOldBrush);

  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}

void ZoomLogo::DrawText(HDC hdc, int x, int y, LPCTSTR text, COLORREF color)
{
  int text_width = rect_size_ / 5;
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

  HFONT old_font = (HFONT)SelectObject(hdc, new_font);

  SetTextColor(hdc, color);
  SetBkColor(hdc, RGB(0, 0, 0));

  SIZE size;
  GetTextExtentPoint(hdc, text, lstrlen(text), &size);
  int xpos_offset = size.cx/2;

  TextOut(hdc, x - xpos_offset, y, text, lstrlen(text));

  SelectObject(hdc, old_font);
  DeleteObject(new_font);
}
