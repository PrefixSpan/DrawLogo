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
    OnPaint(hWnd, hdc);
    EndPaint(hWnd, &ps);
    break;
  case WM_SIZE:
    OnSize(hWnd);
    break;
  case WM_ERASEBKGND:
    hdc = GetDC(hWnd);
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

void ZoomLogo::OnPaint(HWND hWnd, HDC hdc)
{
  DrawLogo(hWnd, hdc);
}

void ZoomLogo::OnSize(HWND hWnd)
{
  GetCenterPoint(hWnd);
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

void DrawRightPart(HDC hdc, int width, int hight, float factor, float y2, float x3, float y3,COLORREF color)
{
  HDC hMemDC = CreateCompatibleDC(hdc);
  HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, width, hight);
  HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hMemBitmap);
  PatBlt(hMemDC, 0, 0, width, hight, PATCOPY);

  SetGraphicsMode(hdc, GM_ADVANCED);

  XFORM xForm;
  xForm.eM11 = 1.0;
  xForm.eM12 = 0.0;
  xForm.eM21 = 0.0;
  xForm.eM22 = 1.0;
  xForm.eDx = x3;
  xForm.eDy = y3;

  SetWindowOrgEx(hdc, 0, -hight, NULL);
  for (int i = 0; i < width; i++)
  {
    xForm.eM22 = (factor + i) / factor;
    SetWorldTransform(hdc, &xForm);
    BitBlt(hdc, i, -hight/2, 1, hight, hMemDC, 0, 0, SRCCOPY);
  }
}

typedef unsigned char (*pGetColorFuncPtr)(unsigned char, unsigned char, int, int);  
extern unsigned char GetLinearColor( unsigned char first, unsigned char last, int areaHgh, int index );  
extern unsigned char GetSinColor( unsigned char first, unsigned char last, int areaHgh, int index );  

extern void LinearGradientFillingArea( unsigned char firRed, unsigned char firGreen, unsigned char firBlue,  
                               unsigned char latRed, unsigned char latGreen, unsigned char latBlue,  
                               HDC hdc,  
                               int areaX, int areaY, int areaWid, int areaHgh,  
                               pGetColorFuncPtr pFunc );  

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

  int circle_scale_factor1 = 20;
  int circle_radius2 = circle_radius1 + circle_scale_factor1;

  int circle_scale_factor2 = 10;
  int circle_radius3 = circle_radius2 + circle_scale_factor2;

  DrawCircle(hdc_mem, x_circle_center, y_circle_center, circle_radius3, RGB(190, 190, 190));
  DrawCircle(hdc_mem, x_circle_center, y_circle_center, circle_radius2, RGB(250, 250, 250));
  DrawCircle(hdc_mem, x_circle_center, y_circle_center, circle_radius1, RGB(45, 182, 254));

//  DrawGradients(hdc_mem, x_circle_center, y_circle_center, circle_radius1, RGB(45, 182, 254));

  // Draw text
  int xpos_text = center_point_.x;
  int ypos_text = center_point_.y + rect_size_ * 10 / 32;
  COLORREF text_color = RGB(0, 0, 255);
  LPCTSTR text = L"Hello Zoom";
  DrawText(hdc_mem, xpos_text, ypos_text, text, text_color);

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

  // Draw right 
  int right_margin = rect_height / 11;
  DrawPolygon(hdc_mem, x_pos + rect_wight + right_margin, y_circle_center, circle_radius1, RGB(255,255,255));

  BitBlt(hdc, 0, 0, rect_client.right, rect_client.bottom, hdc_mem, 0, 0, SRCCOPY);

  return;
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

void ZoomLogo::DrawGradients(HDC hdc, int x, int y, int len, COLORREF color)
{
  HRGN hrgn = CreateEllipticRgn(x-len/2, y-len/2, x+len/2, y+len/2);

  HBITMAP hbmp = CreateCompatibleBitmap(hdc, len, len);  
  HDC hBfr = CreateCompatibleDC(hdc);  
  SelectObject(hBfr, hbmp);  

  int point_count = 0;
  for (int i=x-len/2; i < x+len/2; i++)  
  {
    unsigned char temR = 0, temB = 0, temG = 0;  
    for (int j=y-len/2; j < y+len/2; j++)  
    {
      if (PtInRegion(hrgn, i, j))
      {
        //红色  
        //--------------------------------------------------  
        //   temR = pFunc(firRed, latRed, areaHgh, j);             

        //绿色  
        //--------------------------------------------------  
        //     temG = pFunc(firGreen, latGreen, areaHgh, j);         

        //蓝色  
        //--------------------------------------------------  
        //temB = pFunc(firBlue, latBlue, areaHgh, j);       

        char first = 239;
        char last = 119;
        unsigned char temC = abs( first - last);  

        double sinValue = sin(3.1415926 * (float)j / (float)len);  

        temB = first > last ? first - (unsigned char)( temC * sinValue ):  
          first + (unsigned char)( temC * sinValue );  

        //显示象素  
        //--------------------------------------------------  
        //创建一个图像pos(0, 0), size(areaWid, areaHgh)  
        SetPixelV( hBfr, i, j, RGB(0, 0, temB) ); 
        ++point_count;
      }
    }
  }

  StretchBlt( hdc, len, 0, len, len, hBfr, 0, 0, len , len, SRCCOPY );  
}

void ZoomLogo::DrawText(HDC hdc, int x, int y, LPCTSTR text, COLORREF color)
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
