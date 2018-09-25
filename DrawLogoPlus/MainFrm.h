// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <GdiPlus.h>
using namespace Gdiplus;

#pragma comment (lib,"GdiPlus.lib")


class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>//,
//	public CUpdateUI<CMainFrame>,
//	public CMessageFilter//, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CView m_view;
	CCommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

// 	virtual BOOL OnIdle()
// 	{
// //		UIUpdateToolBar();
// 		return FALSE;
// 	}

// 	BEGIN_UPDATE_UI_MAP(CMainFrame)
// 		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
// 		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
// 	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnErasBKGND)
	//	CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
    bHandled = FALSE;
    return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
//		pLoop->RemoveMessageFilter(this);
//		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
    bHandled = TRUE;

    PAINTSTRUCT ps;
		HDC hdc = BeginPaint(&ps);

		RECT rect;
		GetClientRect(&rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		HDC memdc = CreateCompatibleDC(hdc);
		HBITMAP bitmap = CreateCompatibleBitmap(hdc, width, height);
		HBITMAP old_bitmap = (HBITMAP)SelectObject(memdc, bitmap);

		Graphics graphics(memdc);
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

		BitBlt(hdc, 0, 0, width, height, memdc, 0, 0, SRCCOPY);

		SelectObject(memdc, old_bitmap);
		DeleteObject(bitmap);
    DeleteDC(memdc);

		EndPaint(&ps);

		return 1;
	}

	LRESULT OnErasBKGND(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = TRUE;
		return 1;
	}
};
