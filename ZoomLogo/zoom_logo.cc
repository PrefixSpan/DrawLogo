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


// ����Բ�Ǿ���
void DrawRoundRectange(HDC hDC, float x, float y, float Width, float Height, float arcSize, Color lineColor, float lineWidth)
{
  // Բ�Ǿ��εİ��hew���Ͱ�ߣ�heh��
  float hew = Width/arcSize/2;
  float heh = Height/arcSize/2;

  // Բ������
  if(fabs(hew-heh) > 10)
  {
    hew = heh = hew > heh ? heh : hew;
  }

  // ��������
  int lineMove = 1;

  //////////////////////////////////////////////////////////////////////////
  // ��Բ��
  //////////////////////////////////////////////////////////////////////////
  // ������Բ��
  Arc(hDC, x, y, 2*hew, 2*heh, 180, 90);
  // ������Բ��
  Arc(hDC, x+Width-2*hew, y, 2*hew, 2*heh, 270, 90);
  // ������Բ��
  Arc(hDC, x+Width-2*hew, y+Height-2*heh, 2*hew, 2*heh, 0, 90);
  // ������Բ��
  Arc(hDC, x, y+Height-2*heh, 2*hew, 2*heh, 90, 90);

  //////////////////////////////////////////////////////////////////////////
  // ��ֱ�ߣ�����4��Բ�ǣ�
  //////////////////////////////////////////////////////////////////////////
  // ����������
  DrawLine(hDC, x+hew-lineMove, y, x+Width-hew+lineMove, y);
  // ���Ҳ�����
  DrawLine(hDC, x+Width, y+heh-lineMove, x+Width, y+Height-heh+lineMove);
  // ���ײ�����
  DrawLine(hDC, x+Width-hew+lineMove, y+Height, x+hew-lineMove, y+Height);
  // ���������
  DrawLine(hDC, x, y+Height-heh+lineMove, x, y+heh-lineMove);

  // ����GDI+����
  //  Graphics  g(pDC->GetSafeHdc());

  //���û�ͼʱ���˲�ģʽΪ�����������
  //  g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

//   //��������
//   Pen pen(lineColor, lineWidth);
// 
//   //////////////////////////////////////////////////////////////////////////
//   // ��Բ��
//   //////////////////////////////////////////////////////////////////////////
//   // ������Բ��
//   g.DrawArc(&pen, x, y, 2*hew, 2*heh, 180, 90);
//   // ������Բ��
//   g.DrawArc(&pen, x+Width-2*hew, y, 2*hew, 2*heh, 270, 90);
//   // ������Բ��
//   g.DrawArc(&pen, x+Width-2*hew, y+Height-2*heh, 2*hew, 2*heh, 0, 90);
//   // ������Բ��
//   g.DrawArc(&pen, x, y+Height-2*heh, 2*hew, 2*heh, 90, 90);
// 
//   //////////////////////////////////////////////////////////////////////////
//   // ��ֱ�ߣ�����4��Բ�ǣ�
//   //////////////////////////////////////////////////////////////////////////
//   // ����������
//   g.DrawLine(&pen, x+hew-lineMove, y, x+Width-hew+lineMove, y);
//   // ���Ҳ�����
//   g.DrawLine(&pen, x+Width, y+heh-lineMove, x+Width, y+Height-heh+lineMove);
//   // ���ײ�����
//   g.DrawLine(&pen, x+Width-hew+lineMove, y+Height, x+hew-lineMove, y+Height);
//   // ���������
//   g.DrawLine(&pen, x, y+Height-heh+lineMove, x, y+heh-lineMove);
}

void ZoomLogo::DrawLogo(HWND hWnd, HDC hdc)
{
  DrawCircle(hdc, 300, 300, 350, RGB(90, 90, 90));
  DrawCircle(hdc, 300, 300, 340, RGB(250, 250, 250));
  DrawCircle(hdc, 300, 300, 320, RGB(45, 182, 254));
}

//����ʵ��Բ
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