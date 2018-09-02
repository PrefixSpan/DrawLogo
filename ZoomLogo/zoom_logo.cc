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
    CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

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
    GetCenterPoint(hWnd);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
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

// 绘制圆角矩形
void DrawRoundRectange(HDC hDC, float x, float y, float Width, float Height, 
                       float arcSize, COLORREF lineColor, float lineWidth)
{
  // 圆角矩形的半宽（hew）和半高（heh）
  float hew = Width / arcSize / 2;
  float heh = Height / arcSize / 2;

  // 圆角修正
  if(fabs(hew - heh) > 10)
  {
    hew = heh = hew > heh ? heh : hew;
  }

  // 边线修正
  int lineMove = 1;

  //////////////////////////////////////////////////////////////////////////
  // 画圆角
  //////////////////////////////////////////////////////////////////////////
  // 画左上圆角
  //Arc(hDC, x, y, x+2*hew, y+2*heh, 180, 90, 180, 90);
  //// 画右上圆角
  //Arc(hDC, x+Width-2*hew, y, x+Width, y+2*heh, 270, 90, 270, 90);
  //// 画右下圆角
  //Arc(hDC, x+Width-2*hew, y+Height-2*heh, x+Width, y+Height, 0, 90, 270, 90);
  //// 画左下圆角
  //Arc(hDC, x, y+Height-2*heh, x+2*hew, y+Height, 90, 90, 270, 90);

  ////////////////////////////////////////////////////////////////////////////
  //// 画直线（连接4个圆角）
  ////////////////////////////////////////////////////////////////////////////
  //// 画顶部横线
  //DrawLine(hDC, x + hew-lineMove, y, x+Width-hew+lineMove, y, PS_SOLID, 2, RGB(0,0,0));
  //// 画右侧竖线
  //DrawLine(hDC, x+Width, y + heh-lineMove, x+Width, y+Height-heh+lineMove, PS_SOLID, 2, RGB(0,0,0));
  //// 画底部横线
  //DrawLine(hDC, x+Width-hew+lineMove, y+Height, x+hew-lineMove, y+Height, PS_SOLID, 2, RGB(0,0,0));
  //// 画左侧竖线
  //DrawLine(hDC, x, y+Height-heh+lineMove, x, y+heh-lineMove, PS_SOLID, 2, RGB(0,0,0));
}

void DrawRoundRectangle(HDC hDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color)
{
  HRGN hrgn = CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);

  HBRUSH hBrush = CreateSolidBrush(color);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

  FillRgn(hDC, hrgn, hBrush);

  HPEN hPen = CreatePen(PS_SOLID, 1, color);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

  SelectObject(hDC, hOldBrush);
  DeleteObject(hPen);

  SelectObject(hDC, hOldPen);
  DeleteObject(hOldBrush);
}

void ZoomLogo::DrawLogo(HWND hWnd, HDC hDC)
{
    if (0)
    {

        static HDC hMemDC = CreateCompatibleDC(hDC);

        COLORREF color = RGB(90, 190, 90);
        HBRUSH hBrush = CreateSolidBrush(color);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hMemDC, hBrush);

        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hMemDC, hPen);

        static HBITMAP hMemBitmap = CreateCompatibleBitmap(hDC, 200, 200);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hMemBitmap);
        PatBlt(hMemDC, 0, 0, 200, 200, BLACKNESS);




        //   Rectangle(hDC, 0, 100, 400, 300);

        SetGraphicsMode(hDC, GM_ADVANCED);

        XFORM xForm;
        xForm.eM11 = 1.0;
        xForm.eM12 = 0.0;
        xForm.eM21 = 0.0;
        xForm.eM22 = 1.0;
        xForm.eDx = 100.0;
        xForm.eDy = 100.0;


        //  XFORM xForm2;
        //   CopyMemory(&xForm2, &xForm, sizeof(XFORM));
        SetWindowOrgEx(hDC, -00, -200, NULL);
        for (int i = 0; i < 200; i++)
        {
            xForm.eM22 = (200.0 + i) / 200.0;
            //  xForm.eDx = 0;
            SetWorldTransform(hDC, &xForm);
            BitBlt(hDC, i, -100, 1, 200, hMemDC, 0, 0, SRCCOPY);
        }

        SelectObject(hDC, hOldBrush);
        DeleteObject(hPen);

        SelectObject(hDC, hOldPen);
        DeleteObject(hOldBrush);

        return;
    }


  // Draw Circle
  int x_circle_center = center_point_.x;
  int y_circle_center = center_point_.y;
  int circle_radius1 = rect_size_ / 2;
  int circle_radius2 = circle_radius1 + 20;
  int circle_radius3 = circle_radius1 + 30;

  DrawCircle(hDC, x_circle_center, y_circle_center, circle_radius3, RGB(90, 90, 90));
  DrawCircle(hDC, x_circle_center, y_circle_center, circle_radius2, RGB(250, 250, 250));
  DrawCircle(hDC, x_circle_center, y_circle_center, circle_radius1, RGB(45, 182, 254));

  // Draw left rectangle 
  int rect_wight = rect_size_ * 8 / 33;
  int rect_height = rect_size_ * 6 / 32;
  int x_pos = x_circle_center - rect_wight  / 2;
  int y_pos = y_circle_center - rect_height / 2;
  int left_offset = rect_size_ / 20;
  x_pos -= left_offset;
//  int x_pos = x_circle_center - circle_radius1 * 2 / 5;

  DrawRoundRectangle(hDC, x_pos, y_pos, x_pos + rect_wight, y_pos + rect_height, 20, 20, RGB(255,255,255));

  POINT point1, point2, point3, point4;
  point1.x = x_pos + rect_wight + circle_radius1/40;
  point1.y = y_circle_center - rect_height / 5;
  point2.x = point1.x + rect_height / 2;
  point2.y = y_circle_center - rect_height / 2;
  point3.x = point2.x;
  point3.y = y_circle_center + rect_height / 2;
  point4.x = point1.x;
  point4.y = y_circle_center + rect_height / 5;

  // Draw right 
  DrawLine(hDC, point1.x, point1.y, point2.x, point2.y, PS_SOLID, 2, RGB(0,0,0));
  DrawLine(hDC, point2.x, point2.y, point3.x, point3.y, PS_SOLID, 2, RGB(0,0,0));
  DrawLine(hDC, point3.x, point3.y, point4.x, point4.y, PS_SOLID, 2, RGB(0,0,0));
  DrawLine(hDC, point4.x, point4.y, point1.x, point1.y, PS_SOLID, 2, RGB(0,0,0));

}

//绘制实心圆
void ZoomLogo::DrawCircle(HDC hDC, int x, int y, int len, COLORREF color)
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

