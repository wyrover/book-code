// [!output WINX_VIEW_FILE].h : interface of the [!output WINX_VIEW_CLASS] class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)
#define [!output FILE_NAME_SYMBOL]_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class [!output WINX_VIEW_CLASS] : public [!output WINX_VIEW_BASE_CLASS]<[!output WINX_VIEW_CLASS]>
{
	WINX_CLASS("[!output WINX_VIEW_CLASS_REG]");

[!if WINX_USE_ACCEL]
	WINX_ACCEL(IDR_ACCEL_VIEW); // accelerator

[!endif]
	WINX_CMDS_BEGIN()
		WINX_CMD(ID_FILE_OPEN, OnCmdFileOpen)
	WINX_CMDS_END();

public:
[!if WINX_VIEWTYPE_SCROLL]
[!if WINX_USE_GDIPLUS]

private:
	Gdiplus::Image* m_image;

public:
[!if WINX_USE_CPP_FILES]
	LRESULT OnCreate(HWND hWnd, LPCREATESTRUCT lpCS);
[!else]
	LRESULT OnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
	{
		m_image = NULL;
		return 0;
	}
[!endif]
[!if WINX_USE_CPP_FILES]
	void OnDestroy(HWND hWnd);
[!else]
	void OnDestroy(HWND hWnd)
	{
		delete m_image;
		m_image = NULL;
	}
[!endif]
[!if WINX_USE_CPP_FILES]
	BOOL OnEraseBkgnd(HWND hWnd, winx::DCHandle dc);
[!else]
	BOOL OnEraseBkgnd(HWND hWnd, winx::DCHandle dc)
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
[!endif]
[!if WINX_USE_CPP_FILES]
	void SetImage(Gdiplus::Image* image);
[!else]
	void SetImage(Gdiplus::Image* image)
	{
		delete m_image;
		m_image = image;

		SetScrollOffset(0, 0, FALSE);
		if(m_image)
			SetScrollSize(m_image->GetWidth()+1, m_image->GetHeight()+1);
		else
			SetScrollSize(1, 1);
	}
[!endif]
[!if WINX_USE_CPP_FILES]
	void OnCmdFileOpen(HWND hWnd);
[!else]
	void OnCmdFileOpen(HWND hWnd)
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
[!endif]
[!if WINX_USE_CPP_FILES]
	void DoPaint(winx::DCHandle dc);
[!else]
	void DoPaint(winx::DCHandle dc)
	{
		if (m_image)
		{
			Gdiplus::Graphics gr(dc);
			Gdiplus::Rect rect(0, 0, m_image->GetWidth(), m_image->GetHeight());
			gr.DrawImage(m_image, rect);
		}
	}
[!endif]
[!else]
[!if WINX_USE_CPP_FILES]
	LRESULT OnCreate(HWND hWnd, LPCREATESTRUCT lpCS);
[!else]
	LRESULT OnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
	{
		SetScrollSize(800, 800);
		return 0;
	}
[!endif]
[!if WINX_USE_CPP_FILES]
	void DoPaint(winx::DCHandle dc);
[!else]
	void DoPaint(winx::DCHandle dc)
	{
		dc.TextOut(1, 1, _T("Hello, WINX!"));
		dc.TextOut(1, 300, _T("You are welcome!"));
	}
[!endif]
[!endif]
[!endif]
[!if WINX_VIEWTYPE_GENERIC]
[!if WINX_USE_CPP_FILES]
	void OnPaint(HWND hWnd);
[!else]
	void OnPaint(HWND hWnd)
	{
		winx::PaintDC dc(hWnd);
		dc.TextOut(1, 1, _T("Hello, WINX!"));
	}
[!endif]
[!endif]
[!if WINX_VIEWTYPE_LISTVIEW]
[!if WINX_USE_CPP_FILES]
	void OnSubclass(HWND hWnd);
[!else]
	void OnSubclass(HWND hWnd)
	{
		InsertItem(0, _T("Hello, WINX!"));
		InsertItem(1, _T("You"));
		InsertItem(2, _T("Are"));
		InsertItem(3, _T("Welcome!!"));
	}
[!endif]
[!endif]
[!if WINX_VIEWTYPE_TREEVIEW]
[!if WINX_USE_CPP_FILES]
	void OnSubclass(HWND hWnd);
[!else]
	void OnSubclass(HWND hWnd)
	{
	}
[!endif]
[!endif]
[!if !WINX_VIEWTYPE_SCROLL || !WINX_USE_GDIPLUS]
[!if WINX_USE_CPP_FILES]
	void OnCmdFileOpen(HWND hWnd);
[!else]
	void OnCmdFileOpen(HWND hWnd)
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
[!endif]
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// [!output INSERT_LOCATION_COMMENT]

#endif // !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)