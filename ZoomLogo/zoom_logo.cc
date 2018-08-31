#include "zoom_logo.h"

ZoomLogo::ZoomLogo()
{
}

ZoomLogo::~ZoomLogo()
{

}

HWND ZoomLogo::CreateWnd(HINSTANCE hInstance, int nCmdShow)
{
  cur_instance = hInstance;
  
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
  DWORD wnd_style_ex = WS_EX_TOPMOST;
  hWnd = CreateWindowEx(wnd_style_ex, L"wndZoomLogo", L"ZoomLogo", wnd_style,
    CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

  if (hWnd)
  {
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
  }

  return hWnd;
}

LRESULT CALLBACK ZoomLogo::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int wmId, wmEvent;
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    DrawLogo(hWnd, hdc);
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


// 绘制圆角矩形
void DrawRoundRectange(HDC hDC, float x, float y, float Width, float Height, float arcSize, Color lineColor, float lineWidth)
{
  // 圆角矩形的半宽（hew）和半高（heh）
  float hew = Width/arcSize/2;
  float heh = Height/arcSize/2;

  // 圆角修正
  if(fabs(hew-heh) > 10)
  {
    hew = heh = hew > heh ? heh : hew;
  }

  // 边线修正
  int lineMove = 1;

  //////////////////////////////////////////////////////////////////////////
  // 画圆角
  //////////////////////////////////////////////////////////////////////////
  // 画左上圆角
  Arc(hDC, x, y, 2*hew, 2*heh, 180, 90);
  // 画右上圆角
  Arc(hDC, x+Width-2*hew, y, 2*hew, 2*heh, 270, 90);
  // 画右下圆角
  Arc(hDC, x+Width-2*hew, y+Height-2*heh, 2*hew, 2*heh, 0, 90);
  // 画左下圆角
  Arc(hDC, x, y+Height-2*heh, 2*hew, 2*heh, 90, 90);

  //////////////////////////////////////////////////////////////////////////
  // 画直线（连接4个圆角）
  //////////////////////////////////////////////////////////////////////////
  // 画顶部横线
  DrawLine(hDC, x+hew-lineMove, y, x+Width-hew+lineMove, y);
  // 画右侧竖线
  DrawLine(hDC, x+Width, y+heh-lineMove, x+Width, y+Height-heh+lineMove);
  // 画底部横线
  DrawLine(hDC, x+Width-hew+lineMove, y+Height, x+hew-lineMove, y+Height);
  // 画左侧竖线
  DrawLine(hDC, x, y+Height-heh+lineMove, x, y+heh-lineMove);

  // 创建GDI+对象
  //  Graphics  g(pDC->GetSafeHdc());

  //设置画图时的滤波模式为消除锯齿现象
  //  g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

//   //创建画笔
//   Pen pen(lineColor, lineWidth);
// 
//   //////////////////////////////////////////////////////////////////////////
//   // 画圆角
//   //////////////////////////////////////////////////////////////////////////
//   // 画左上圆角
//   g.DrawArc(&pen, x, y, 2*hew, 2*heh, 180, 90);
//   // 画右上圆角
//   g.DrawArc(&pen, x+Width-2*hew, y, 2*hew, 2*heh, 270, 90);
//   // 画右下圆角
//   g.DrawArc(&pen, x+Width-2*hew, y+Height-2*heh, 2*hew, 2*heh, 0, 90);
//   // 画左下圆角
//   g.DrawArc(&pen, x, y+Height-2*heh, 2*hew, 2*heh, 90, 90);
// 
//   //////////////////////////////////////////////////////////////////////////
//   // 画直线（连接4个圆角）
//   //////////////////////////////////////////////////////////////////////////
//   // 画顶部横线
//   g.DrawLine(&pen, x+hew-lineMove, y, x+Width-hew+lineMove, y);
//   // 画右侧竖线
//   g.DrawLine(&pen, x+Width, y+heh-lineMove, x+Width, y+Height-heh+lineMove);
//   // 画底部横线
//   g.DrawLine(&pen, x+Width-hew+lineMove, y+Height, x+hew-lineMove, y+Height);
//   // 画左侧竖线
//   g.DrawLine(&pen, x, y+Height-heh+lineMove, x, y+heh-lineMove);
}

void ZoomLogo::DrawLogo(HWND hWnd, HDC hdc)
{
  DrawCircle(hdc, 300, 300, 350, RGB(90, 90, 90));
  DrawCircle(hdc, 300, 300, 340, RGB(250, 250, 250));
  DrawCircle(hdc, 300, 300, 320, RGB(45, 182, 254));
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