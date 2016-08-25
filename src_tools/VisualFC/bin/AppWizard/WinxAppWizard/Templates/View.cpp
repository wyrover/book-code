// [!output WINX_VIEW_FILE].cpp : implementation of the [!output WINX_VIEW_CLASS] class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "[!output WINX_VIEW_FILE].h"

[!if WINX_VIEWTYPE_SCROLL]
[!if WINX_USE_GDIPLUS]
LRESULT [!output WINX_VIEW_CLASS]::OnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
{
	m_image = NULL;
	return 0;
}

void [!output WINX_VIEW_CLASS]::OnDestroy(HWND hWnd)
{
	delete m_image;
	m_image = NULL;
}

BOOL [!output WINX_VIEW_CLASS]::OnEraseBkgnd(HWND hWnd, winx::DCHandle dc)
{
	winx::ClientRect rect(hWnd);
	int x = 0, y = 0;
	if (rect.right > m_sizeAll.cx)
	{
		RECT rectRight = rect;
		rectRight.left = x;
		rectRight.bottom = y;
		dc.FillRect(&rectRight, COLOR_WINDOW);
	}
	if (rect.bottom > m_sizeAll.cy)
	{
		RECT rectBottom = rect;
		rectBottom.top = y;
		dc.FillRect(&rectBottom, COLOR_WINDOW);
	}
	return TRUE;
}

void [!output WINX_VIEW_CLASS]::SetImage(Gdiplus::Image* image)
{
	delete m_image;
	m_image = image;
	
	SetScrollOffset(0, 0, FALSE);
	if(m_image)
		SetScrollSize(m_image->GetWidth()+1, m_image->GetHeight()+1);
	else
		SetScrollSize(1, 1);
}

void [!output WINX_VIEW_CLASS]::OnCmdFileOpen(HWND hWnd)
{
	winx::OpenFileDialog dlg(
		_T("Images files(*.jpg;*.png;*.tif;*.bmp;*.gif)\0*.jpg;*.png;*.tif;*.bmp;*.gif\0All files(*.*)\0*.*\0")
		);
	if (IDOK == dlg.DoModal(hWnd))
	{
		USES_CONVERSION;
		Gdiplus::Image* image = new Gdiplus::Image(T2CW(dlg.lpstrFile));
		if (image->GetLastStatus() != Gdiplus::Ok)
		{
			delete image;
			winx::ExMsgBoxTrace(hWnd, _T("Error"), _T("Can't load image from %s"), dlg.lpstrFile);
		}
		else
		{
			SetImage(image);
		}
	}
}

void [!output WINX_VIEW_CLASS]::DoPaint(winx::DCHandle dc)
{
	if (m_image)
	{
		Gdiplus::Graphics gr(dc);
		Gdiplus::Rect rect(0, 0, m_image->GetWidth(), m_image->GetHeight());
		gr.DrawImage(m_image, rect);
	}
}
[!else]
LRESULT [!output WINX_VIEW_CLASS]::OnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
{
	SetScrollSize(800, 800);
	return 0;
}

void [!output WINX_VIEW_CLASS]::DoPaint(winx::DCHandle dc)
{
	dc.TextOut(1, 1, _T("Hello, WINX!"));
	dc.TextOut(1, 300, _T("You are welcome!"));
}
[!endif]
[!endif]

[!if WINX_VIEWTYPE_GENERIC]
void [!output WINX_VIEW_CLASS]::OnPaint(HWND hWnd)
{
	winx::PaintDC dc(hWnd);
	dc.TextOut(1, 1, _T("Hello, WINX!"));
}
[!endif]

[!if WINX_VIEWTYPE_TREEVIEW]
void [!output WINX_VIEW_CLASS]::OnSubclass(HWND hWnd)
{
}
[!endif]

[!if !WINX_VIEWTYPE_SCROLL || !WINX_USE_GDIPLUS]
void [!output WINX_VIEW_CLASS]::OnCmdFileOpen(HWND hWnd)
{
	winx::OpenMultiFilesDialog dlg(
		_T("All supported files(*.txt;*.doc)\0*.txt;*.doc\0All files(*.*)\0*.*\0")
		);
	if (IDOK == dlg.DoModal(hWnd))
	{
		TCHAR szFile[MAX_PATH];
		UINT nCount = 0;
		winx::CString str;
		while (dlg.NextFile(szFile) == S_OK)
		{
			str += szFile;
			str += _T(";\n");
			++nCount;
		}
		winx::ExMsgBoxTrace(hWnd, _T("Information"), _T("Count = %d\n%s"), nCount, (LPCTSTR)str);
	}
}
[!endif]
