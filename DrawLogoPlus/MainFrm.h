// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <GdiPlus.h>
#include <atlwinx.h>
using namespace Gdiplus;

#pragma comment (lib,"GdiPlus.lib")

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>,
  public CDoubleBufferImpl<CMainFrame>
{
public:
//	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	DECLARE_FRAME_WND_CLASS_EX(NULL, IDR_MAINFRAME, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, COLOR_WINDOWTEXT)

	CView m_view;
	CCommandBarCtrl m_CmdBar;

	BEGIN_MSG_MAP(CMainFrame)
    CHAIN_MSG_MAP(WTL::CDoubleBufferImpl<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

  void DoPaint(CDCHandle dc)
  {
    RECT rect;
    GetClientRect(&rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    Graphics graphics(dc.m_hDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    // DrawImage
    Image image(_T("zoomAppIcon.png"));

    int xpos_image = width / 2 - image.GetWidth() / 2;
    int ypos_image = height / 2 - image.GetHeight();

    Status status = 
      graphics.DrawImage(&image, xpos_image, ypos_image, image.GetWidth(), image.GetHeight());

    // Draw text
    LPCTSTR string_zoom = _T("Hello Zoom");
    Font draw_font(_T("Segoe UI"), 20);
    RectF boundRect; 
    graphics.MeasureString(string_zoom, _tcslen(string_zoom), &draw_font, PointF(0, 0), &boundRect);

    Color clr(255, 0, 0, 255);
    Gdiplus::SolidBrush blackBrush(clr);
    int xpos_string = width / 2 - boundRect.Width / 2;
    int ypos_string = height / 2 + image.GetHeight() / 3;

    graphics.DrawString(string_zoom, -1, &draw_font, PointF(xpos_string, ypos_string), &blackBrush);
  }
};
