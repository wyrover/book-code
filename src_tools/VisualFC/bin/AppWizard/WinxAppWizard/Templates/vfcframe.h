// Windows Template Library - WTL version 8.0
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// This file is a part of the Windows Template Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/osi3.0/licenses/cpl1.0.php)
// which can be found in the file CPL.TXT at the root of this distribution.
// By using this software in any fashion, you are agreeing to be bound by
// the terms of this license. You must not remove this notice, or
// any other, from this software.

// Module: vfcframe.h
// Creator: visualfc 
// Email: visualfc@gmail.com
// open source: http://sf.net/projects/visualfc
// Date: 2007-12-28

#ifndef __VFCFRAME_H__
#define __VFCFRAME_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error vfcframe.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error vfcframe.h requires atlwin.h to be included first
#endif

#ifndef __ATLFRAME_H__
	#error vfcframe.h requires atlframe.h to be included first
#endif

#ifndef ATL_IDW_STATUS_EX_BAR
#define ATL_IDW_STATUS_EX_BAR	0xE801+0x00E0
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
//CFrameDialogBase
//CFrameDialog
//CFrameDialogImpl

namespace WTL
{

template <class T>
class ATL_NO_VTABLE CFrameDialogBase
{
public:
//	DECLARE_FRAME_WND_CLASS(NULL, 0)

// Data members
	HWND m_hWndToolBar;
	HWND m_hWndStatusBar;
//	HWND m_hWndClient;
	int	 m_nBarHeight;

	HACCEL m_hAccel;

#ifdef _WIN32_WCE
	HWND m_hWndCECommandBar;
#endif // _WIN32_WCE

	struct _AtlToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		//WORD aItems[wItemCount]

		WORD* items()
			{ return (WORD*)(this+1); }
	};

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	struct _ChevronMenuInfo
	{
		HMENU hMenu;
		LPNMREBARCHEVRON lpnm;
		bool bCmdBar;
	};
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)

// Constructor
	CFrameDialogBase() : 
#ifdef _WIN32_WCE
		m_hWndCECommandBar(NULL),
#endif // _WIN32_WCE
		m_hWndToolBar(NULL), 
		m_hWndStatusBar(NULL), 
//		m_hWndClient(NULL), 
		m_hAccel(NULL),
		m_nBarHeight(-1)
	{ }

// Methods
	static HWND CreateSimpleToolBarCtrl(HWND hWndParent, UINT nResourceID, BOOL bInitialSeparator = FALSE, 
			DWORD dwStyle = ATL_SIMPLE_TOOLBAR_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
		HINSTANCE hInst = ModuleHelper::GetResourceInstance();
		HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nResourceID), RT_TOOLBAR);
		if (hRsrc == NULL)
			return NULL;

		HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
		if (hGlobal == NULL)
			return NULL;

		_AtlToolBarData* pData = (_AtlToolBarData*)::LockResource(hGlobal);
		if (pData == NULL)
			return NULL;
		ATLASSERT(pData->wVersion == 1);

		WORD* pItems = pData->items();
		int nItems = pData->wItemCount + (bInitialSeparator ? 1 : 0);
		CTempBuffer<TBBUTTON, _WTL_STACK_ALLOC_THRESHOLD> buff;
		TBBUTTON* pTBBtn = buff.Allocate(nItems);
		ATLASSERT(pTBBtn != NULL);
		if(pTBBtn == NULL)
			return NULL;

		const int cxSeparator = 8;

		// set initial separator (half width)
		if(bInitialSeparator)
		{
			pTBBtn[0].iBitmap = cxSeparator / 2;
			pTBBtn[0].idCommand = 0;
			pTBBtn[0].fsState = 0;
			pTBBtn[0].fsStyle = TBSTYLE_SEP;
			pTBBtn[0].dwData = 0;
			pTBBtn[0].iString = 0;
		}

		int nBmp = 0;
		for(int i = 0, j = bInitialSeparator ? 1 : 0; i < pData->wItemCount; i++, j++)
		{
			if(pItems[i] != 0)
			{
				pTBBtn[j].iBitmap = nBmp++;
				pTBBtn[j].idCommand = pItems[i];
				pTBBtn[j].fsState = TBSTATE_ENABLED;
				pTBBtn[j].fsStyle = TBSTYLE_BUTTON;
				pTBBtn[j].dwData = 0;
				pTBBtn[j].iString = 0;
			}
			else
			{
				pTBBtn[j].iBitmap = cxSeparator;
				pTBBtn[j].idCommand = 0;
				pTBBtn[j].fsState = 0;
				pTBBtn[j].fsStyle = TBSTYLE_SEP;
				pTBBtn[j].dwData = 0;
				pTBBtn[j].iString = 0;
			}
		}

#ifndef _WIN32_WCE
		HWND hWnd = ::CreateWindowEx(0, TOOLBARCLASSNAME, NULL, dwStyle, 0, 0, 100, 100, hWndParent, (HMENU)LongToHandle(nID), ModuleHelper::GetModuleInstance(), NULL);
		if(hWnd == NULL)
		{
			ATLASSERT(FALSE);
			return NULL;
		}
#else // CE specific
		dwStyle;
		nID;
		// The toolbar must go onto the existing CommandBar or MenuBar
		HWND hWnd = hWndParent;
#endif // _WIN32_WCE

		::SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0L);

		// check if font is taller than our bitmaps
		CFontHandle font = (HFONT)::SendMessage(hWnd, WM_GETFONT, 0, 0L);
		if(font.IsNull())
			font = AtlGetDefaultGuiFont();
		LOGFONT lf = { 0 };
		font.GetLogFont(lf);
		WORD cyFontHeight = (WORD)abs(lf.lfHeight);

#ifndef _WIN32_WCE
		WORD bitsPerPixel = AtlGetBitmapResourceBitsPerPixel(nResourceID);
		if(bitsPerPixel > 4)
		{
			COLORREF crMask = CLR_DEFAULT;
			if(bitsPerPixel == 32)
			{
				// 32-bit color bitmap with alpha channel (valid for Windows XP and later)
				crMask = CLR_NONE;
			}
			HIMAGELIST hImageList = ImageList_LoadImage(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE(nResourceID), pData->wWidth, 1, crMask, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
			ATLASSERT(hImageList != NULL);
			::SendMessage(hWnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
		}
		else
#endif // !_WIN32_WCE
		{
			TBADDBITMAP tbab = { 0 };
			tbab.hInst = hInst;
			tbab.nID = nResourceID;
			::SendMessage(hWnd, TB_ADDBITMAP, nBmp, (LPARAM)&tbab);
		}

		::SendMessage(hWnd, TB_ADDBUTTONS, nItems, (LPARAM)pTBBtn);
		::SendMessage(hWnd, TB_SETBITMAPSIZE, 0, MAKELONG(pData->wWidth, max(pData->wHeight, cyFontHeight)));
		const int cxyButtonMargin = 7;
		::SendMessage(hWnd, TB_SETBUTTONSIZE, 0, MAKELONG(pData->wWidth + cxyButtonMargin, max(pData->wHeight, cyFontHeight) + cxyButtonMargin));

		return hWnd;
	}

#ifndef _WIN32_WCE
	static HWND CreateSimpleReBarCtrl(HWND hWndParent, DWORD dwStyle = ATL_SIMPLE_REBAR_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
		// Ensure style combinations for proper rebar painting
		if(dwStyle & CCS_NODIVIDER && dwStyle & WS_BORDER)
			dwStyle &= ~WS_BORDER;
		else if(!(dwStyle & WS_BORDER) && !(dwStyle & CCS_NODIVIDER))
			dwStyle |= CCS_NODIVIDER;

		// Create rebar window
		HWND hWndReBar = ::CreateWindowEx(0, REBARCLASSNAME, NULL, dwStyle, 0, 0, 100, 100, hWndParent, (HMENU)LongToHandle(nID), ModuleHelper::GetModuleInstance(), NULL);
		if(hWndReBar == NULL)
		{
			ATLTRACE2(atlTraceUI, 0, _T("Failed to create rebar.\n"));
			return NULL;
		}

		// Initialize and send the REBARINFO structure
		REBARINFO rbi = { 0 };
		rbi.cbSize = sizeof(REBARINFO);
		rbi.fMask  = 0;
		if(!::SendMessage(hWndReBar, RB_SETBARINFO, 0, (LPARAM)&rbi))
		{
			ATLTRACE2(atlTraceUI, 0, _T("Failed to initialize rebar.\n"));
			::DestroyWindow(hWndReBar);
			return NULL;
		}

		return hWndReBar;
	}

	BOOL CreateSimpleReBar(DWORD dwStyle = ATL_SIMPLE_REBAR_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
		T * pT = static_cast<T*>(this);
		ATLASSERT(!::IsWindow(m_hWndToolBar));
		m_hWndToolBar = CreateSimpleReBarCtrl(pT->m_hWnd, dwStyle, nID);
		return (m_hWndToolBar != NULL);
	}

	static BOOL AddSimpleReBarBandCtrl(HWND hWndReBar, HWND hWndBand, int nID = 0, LPCTSTR lpstrTitle = NULL, BOOL bNewRow = FALSE, int cxWidth = 0, BOOL bFullWidthAlways = FALSE)
	{
		ATLASSERT(::IsWindow(hWndReBar));   // must be already created
#ifdef _DEBUG
		// block - check if this is really a rebar
		{
			TCHAR lpszClassName[sizeof(REBARCLASSNAME)] = { 0 };
			::GetClassName(hWndReBar, lpszClassName, sizeof(REBARCLASSNAME));
			ATLASSERT(lstrcmp(lpszClassName, REBARCLASSNAME) == 0);
		}
#endif // _DEBUG
		ATLASSERT(::IsWindow(hWndBand));   // must be already created

		// Get number of buttons on the toolbar
		int nBtnCount = (int)::SendMessage(hWndBand, TB_BUTTONCOUNT, 0, 0L);

		// Set band info structure
		REBARBANDINFO rbBand = { RunTimeHelper::SizeOf_REBARBANDINFO() };
#if (_WIN32_IE >= 0x0400)
		rbBand.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_ID | RBBIM_SIZE | RBBIM_IDEALSIZE;
#else
		rbBand.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_ID | RBBIM_SIZE;
#endif // !(_WIN32_IE >= 0x0400)
		if(lpstrTitle != NULL)
			rbBand.fMask |= RBBIM_TEXT;
		rbBand.fStyle = RBBS_CHILDEDGE;
#if (_WIN32_IE >= 0x0500)
		if(nBtnCount > 0)   // add chevron style for toolbar with buttons
			rbBand.fStyle |= RBBS_USECHEVRON;
#endif // (_WIN32_IE >= 0x0500)
		if(bNewRow)
			rbBand.fStyle |= RBBS_BREAK;

		rbBand.lpText = (LPTSTR)lpstrTitle;
		rbBand.hwndChild = hWndBand;
		if(nID == 0)   // calc band ID
			nID = ATL_IDW_BAND_FIRST + (int)::SendMessage(hWndReBar, RB_GETBANDCOUNT, 0, 0L);
		rbBand.wID = nID;

		// Calculate the size of the band
		BOOL bRet = FALSE;
		RECT rcTmp = { 0 };
		if(nBtnCount > 0)
		{
			bRet = (BOOL)::SendMessage(hWndBand, TB_GETITEMRECT, nBtnCount - 1, (LPARAM)&rcTmp);
			ATLASSERT(bRet);
			rbBand.cx = (cxWidth != 0) ? cxWidth : rcTmp.right;
			rbBand.cyMinChild = rcTmp.bottom - rcTmp.top;
			if(bFullWidthAlways)
			{
				rbBand.cxMinChild = rbBand.cx;
			}
			else if(lpstrTitle == NULL)
			{
				bRet = (BOOL)::SendMessage(hWndBand, TB_GETITEMRECT, 0, (LPARAM)&rcTmp);
				ATLASSERT(bRet);
				rbBand.cxMinChild = rcTmp.right;
			}
			else
			{
				rbBand.cxMinChild = 0;
			}
		}
		else	// no buttons, either not a toolbar or really has no buttons
		{
			bRet = ::GetWindowRect(hWndBand, &rcTmp);
			ATLASSERT(bRet);
			rbBand.cx = (cxWidth != 0) ? cxWidth : (rcTmp.right - rcTmp.left);
			rbBand.cxMinChild = bFullWidthAlways ? rbBand.cx : 0;
			rbBand.cyMinChild = rcTmp.bottom - rcTmp.top;
		}

#if (_WIN32_IE >= 0x0400)
		rbBand.cxIdeal = rbBand.cx;
#endif // (_WIN32_IE >= 0x0400)

		// Add the band
		LRESULT lRes = ::SendMessage(hWndReBar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
		if(lRes == 0)
		{
			ATLTRACE2(atlTraceUI, 0, _T("Failed to add a band to the rebar.\n"));
			return FALSE;
		}

#if (_WIN32_IE >= 0x0501)
		DWORD dwExStyle = (DWORD)::SendMessage(hWndBand, TB_GETEXTENDEDSTYLE, 0, 0L);
		::SendMessage(hWndBand, TB_SETEXTENDEDSTYLE, 0, dwExStyle | TBSTYLE_EX_HIDECLIPPEDBUTTONS);
#endif // (_WIN32_IE >= 0x0501)

		return TRUE;
	}

	BOOL AddSimpleReBarBand(HWND hWndBand, LPCTSTR lpstrTitle = NULL, BOOL bNewRow = FALSE, int cxWidth = 0, BOOL bFullWidthAlways = FALSE)
	{
		ATLASSERT(::IsWindow(m_hWndToolBar));   // must be an existing rebar
		ATLASSERT(::IsWindow(hWndBand));        // must be created

		return AddSimpleReBarBandCtrl(m_hWndToolBar, hWndBand, 0, lpstrTitle, bNewRow, cxWidth, bFullWidthAlways);
	}

#if (_WIN32_IE >= 0x0400)
	void SizeSimpleReBarBands()
	{
		ATLASSERT(::IsWindow(m_hWndToolBar));   // must be an existing rebar

		int nCount = (int)::SendMessage(m_hWndToolBar, RB_GETBANDCOUNT, 0, 0L);

		for(int i = 0; i < nCount; i++)
		{
			REBARBANDINFO rbBand = { RunTimeHelper::SizeOf_REBARBANDINFO() };
			rbBand.fMask = RBBIM_SIZE;
			BOOL bRet = (BOOL)::SendMessage(m_hWndToolBar, RB_GETBANDINFO, i, (LPARAM)&rbBand);
			ATLASSERT(bRet);
			RECT rect = { 0, 0, 0, 0 };
			::SendMessage(m_hWndToolBar, RB_GETBANDBORDERS, i, (LPARAM)&rect);
			rbBand.cx += rect.left + rect.right;
			bRet = (BOOL)::SendMessage(m_hWndToolBar, RB_SETBANDINFO, i, (LPARAM)&rbBand);
			ATLASSERT(bRet);
		}
	}
#endif // (_WIN32_IE >= 0x0400)
#endif // _WIN32_WCE

#ifndef _WIN32_WCE
	BOOL CreateSimpleStatusBar(LPCTSTR lpstrText, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS /* | SBARS_SIZEGRIP*/, UINT nID = ATL_IDW_STATUS_EX_BAR)
#else // CE specific
	BOOL CreateSimpleStatusBar(LPCTSTR lpstrText, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, UINT nID = ATL_IDW_STATUS_EX_BAR)
#endif // _WIN32_WCE
	{
		T * pT = static_cast<T*>(this);
		ATLASSERT(!::IsWindow(m_hWndStatusBar));
		m_hWndStatusBar = ::CreateStatusWindow(dwStyle, lpstrText, pT->m_hWnd, nID);
		return (m_hWndStatusBar != NULL);
	}

#ifndef _WIN32_WCE
	BOOL CreateSimpleStatusBar(UINT nTextID = ATL_IDS_IDLEMESSAGE, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS /*| SBARS_SIZEGRIP*/, UINT nID = ATL_IDW_STATUS_EX_BAR)
#else // CE specific
	BOOL CreateSimpleStatusBar(UINT nTextID = ATL_IDS_IDLEMESSAGE, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, UINT nID = ATL_IDW_STATUS_EX_BAR)
#endif // _WIN32_WCE
	{
		const int cchMax = 128;   // max text length is 127 for status bars (+1 for null)
		TCHAR szText[cchMax];
		szText[0] = 0;
		::LoadString(ModuleHelper::GetResourceInstance(), nTextID, szText, cchMax);
		return CreateSimpleStatusBar(szText, dwStyle, nID);
	}

#ifdef _WIN32_WCE
	BOOL CreateSimpleCECommandBar(LPTSTR pszMenu = NULL, WORD iButton = 0, DWORD dwFlags = 0, int nCmdBarID = 1)
	{
		ATLASSERT(m_hWndCECommandBar == NULL);
		ATLASSERT(m_hWndToolBar == NULL);
		T * pT = static_cast<T*>(this);
		m_hWndCECommandBar = ::CommandBar_Create(ModuleHelper::GetModuleInstance(), pT->m_hWnd, nCmdBarID);
		if(m_hWndCECommandBar == NULL)
			return FALSE;

		m_hWndToolBar = m_hWndCECommandBar;

		BOOL bRet = TRUE;

		if(pszMenu != NULL)
			bRet &= ::CommandBar_InsertMenubarEx(m_hWndCECommandBar, IS_INTRESOURCE(pszMenu) ? ModuleHelper::GetResourceInstance() : NULL, pszMenu, iButton);

		bRet &= ::CommandBar_AddAdornments(m_hWndCECommandBar, dwFlags, 0);

		return bRet;
	}

#if defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__)
	BOOL CreateSimpleCEMenuBar(UINT nToolBarId = ATL_IDW_MENU_BAR, DWORD dwFlags = 0, int nBmpId = 0, int cBmpImages = 0)
	{
		ATLASSERT(m_hWndCECommandBar == NULL);
		T * pT = static_cast<T*>(this);
		SHMENUBARINFO mbi = { 0 };
		mbi.cbSize = sizeof(mbi);
		mbi.hwndParent = pT->m_hWnd;
		mbi.dwFlags = dwFlags;
		mbi.nToolBarId = nToolBarId;
		mbi.hInstRes  = ModuleHelper::GetResourceInstance();
		mbi.nBmpId = nBmpId;
		mbi.cBmpImages = cBmpImages;
		mbi.hwndMB = NULL;   // This gets set by SHCreateMenuBar

		BOOL bRet = ::SHCreateMenuBar(&mbi);
		if(bRet != FALSE)
		{
			m_hWndCECommandBar = mbi.hwndMB;
			SizeToMenuBar();
		}

		return bRet;
	}

	void SizeToMenuBar()   // for menu bar only
	{
		T * pT = static_cast<T*>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));
		ATLASSERT(::IsWindow(m_hWndCECommandBar));

		RECT rect = { 0 };
		pT->GetWindowRect(&rect);
		RECT rectMB = { 0 };
		::GetWindowRect(m_hWndCECommandBar, &rectMB);
		int cy = ::IsWindowVisible(m_hWndCECommandBar) ? rectMB.top - rect.top : rectMB.bottom - rect.top;
		pT->SetWindowPos(NULL, 0, 0, rect.right - rect.left, cy, SWP_NOZORDER | SWP_NOMOVE);
	}
#endif // defined(_AYGSHELL_H_) || defined(__AYGSHELL_H__)
#endif // _WIN32_WCE

	void UpdateFrameLayout(BOOL bResizeBars = TRUE, BOOL bNoCheckVisible = TRUE)
	{
		T * pT = static_cast<T*>(this);

		RECT rect = { 0 };
		pT->GetClientRect(&rect);

		// position bars and offset their dimensions
		UpdateBarsPosition(rect, bResizeBars);

		if(m_hWndToolBar != NULL &&
			 (((DWORD)::GetWindowLong(m_hWndToolBar, GWL_STYLE) & WS_VISIBLE) ) || bNoCheckVisible )
		{
			pT->UpdateFrameTrueClient(&rect);
		}
	}

	void UpdateBarsPosition(RECT& rect, BOOL bResizeBars = TRUE)
	{
		// resize toolbar
		if(m_hWndToolBar != NULL && ((DWORD)::GetWindowLong(m_hWndToolBar, GWL_STYLE) & WS_VISIBLE))
		{
			if(bResizeBars)
			{
				::SendMessage(m_hWndToolBar, WM_SIZE, 0, 0);
				::InvalidateRect(m_hWndToolBar, NULL, FALSE);
			}
			RECT rectTB = { 0 };
			::GetWindowRect(m_hWndToolBar, &rectTB);
			rect.top += rectTB.bottom - rectTB.top;
		}

		// resize status bar
		if(m_hWndStatusBar != NULL && ((DWORD)::GetWindowLong(m_hWndStatusBar, GWL_STYLE) & WS_VISIBLE))
		{
			if(bResizeBars)
				::SendMessage(m_hWndStatusBar, WM_SIZE, 0, 0);
			RECT rectSB = { 0 };
			::GetWindowRect(m_hWndStatusBar, &rectSB);
			rect.bottom -= rectSB.bottom - rectSB.top;
		}
	}

	BOOL PreTranslateMessage(MSG* pMsg, BOOL bDisibleEsc = TRUE)
	{
		if(bDisibleEsc && WM_KEYDOWN == pMsg->message &&
			(WPARAM)VK_ESCAPE ==  pMsg->wParam )
		{   
			return  TRUE;
		}   

		T * pT = static_cast<T*>(this);
		if(m_hAccel != NULL && ::TranslateAccelerator(pT->m_hWnd, m_hAccel, pMsg))
			return TRUE;
		if (::IsDialogMessage(pT->m_hWnd,pMsg))
			return TRUE;
		return FALSE;
	}

	BEGIN_MSG_MAP(CFrameDialogBase)
#ifndef _WIN32_WCE
		MESSAGE_HANDLER(WM_MENUSELECT, OnMenuSelect)
#endif // !_WIN32_WCE
//		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
//		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
#ifndef _WIN32_WCE
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFOA, OnToolTipTextA)
		NOTIFY_CODE_HANDLER(TTN_GETDISPINFOW, OnToolTipTextW)
#endif // !_WIN32_WCE
	END_MSG_MAP()

#ifndef _WIN32_WCE
	LRESULT OnMenuSelect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		if(m_hWndStatusBar == NULL)
			return 1;

		WORD wFlags = HIWORD(wParam);
		if(wFlags == 0xFFFF && lParam == NULL)   // menu closing
		{
			::SendMessage(m_hWndStatusBar, SB_SIMPLE, FALSE, 0L);
		}
		else
		{
			const int cchBuff = 256;
			TCHAR szBuff[cchBuff];
			szBuff[0] = 0;
			if(!(wFlags & MF_POPUP))
			{
				WORD wID = LOWORD(wParam);
				// check for special cases
				if(wID >= 0xF000 && wID < 0xF1F0)   // system menu IDs
					wID = (WORD)(((wID - 0xF000) >> 4) + ATL_IDS_SCFIRST);
				else if(wID >= ID_FILE_MRU_FIRST && wID <= ID_FILE_MRU_LAST)   // MRU items
					wID = ATL_IDS_MRU_FILE;
				else if(wID >= ATL_IDM_FIRST_MDICHILD && wID <= ATL_IDM_LAST_MDICHILD)   // MDI child windows
					wID = ATL_IDS_MDICHILD;

				int nRet = ::LoadString(ModuleHelper::GetResourceInstance(), wID, szBuff, cchBuff);
				for(int i = 0; i < nRet; i++)
				{
					if(szBuff[i] == _T('\n'))
					{
						szBuff[i] = 0;
						break;
					}
				}
			}
			::SendMessage(m_hWndStatusBar, SB_SIMPLE, TRUE, 0L);
			::SendMessage(m_hWndStatusBar, SB_SETTEXT, (255 | SBT_NOBORDERS), (LPARAM)szBuff);
		}

		return 1;
	}
#endif // !_WIN32_WCE

#ifndef _WIN32_WCE
	LRESULT OnToolTipTextA(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMTTDISPINFOA pDispInfo = (LPNMTTDISPINFOA)pnmh;
		pDispInfo->szText[0] = 0;

		if((idCtrl != 0) && !(pDispInfo->uFlags & TTF_IDISHWND))
		{
			const int cchBuff = 256;
			char szBuff[cchBuff];
			szBuff[0] = 0;
			int nRet = ::LoadStringA(ModuleHelper::GetResourceInstance(), idCtrl, szBuff, cchBuff);
			for(int i = 0; i < nRet; i++)
			{
				if(szBuff[i] == '\n')
				{
					SecureHelper::strncpyA_x(pDispInfo->szText, _countof(pDispInfo->szText), &szBuff[i + 1], _TRUNCATE);
					break;
				}
			}
#if (_WIN32_IE >= 0x0300)
			if(nRet > 0)   // string was loaded, save it
				pDispInfo->uFlags |= TTF_DI_SETITEM;
#endif // (_WIN32_IE >= 0x0300)
		}

		return 0;
	}

	LRESULT OnToolTipTextW(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPNMTTDISPINFOW pDispInfo = (LPNMTTDISPINFOW)pnmh;
		pDispInfo->szText[0] = 0;

		if((idCtrl != 0) && !(pDispInfo->uFlags & TTF_IDISHWND))
		{
			const int cchBuff = 256;
			wchar_t szBuff[cchBuff];
			szBuff[0] = 0;
			int nRet = ::LoadStringW(ModuleHelper::GetResourceInstance(), idCtrl, szBuff, cchBuff);
			for(int i = 0; i < nRet; i++)
			{
				if(szBuff[i] == L'\n')
				{
					SecureHelper::strncpyW_x(pDispInfo->szText, _countof(pDispInfo->szText), &szBuff[i + 1], _TRUNCATE);
					break;
				}
			}
#if (_WIN32_IE >= 0x0300)
			if(nRet > 0)   // string was loaded, save it
				pDispInfo->uFlags |= TTF_DI_SETITEM;
#endif // (_WIN32_IE >= 0x0300)
		}

		return 0;
	}
#endif // !_WIN32_WCE


// Implementation - chevron menu support
#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	bool PrepareChevronMenu(_ChevronMenuInfo& cmi)
	{
		// get rebar and toolbar
		REBARBANDINFO rbbi = { RunTimeHelper::SizeOf_REBARBANDINFO() };
		rbbi.fMask = RBBIM_CHILD;
		BOOL bRet = (BOOL)::SendMessage(cmi.lpnm->hdr.hwndFrom, RB_GETBANDINFO, cmi.lpnm->uBand, (LPARAM)&rbbi);
		ATLASSERT(bRet);

		// assume the band is a toolbar
		ATL::CWindow wnd = rbbi.hwndChild;
		int nCount = (int)wnd.SendMessage(TB_BUTTONCOUNT);
		if(nCount <= 0)   // probably not a toolbar
			return false;

		// check if it's a command bar
		CMenuHandle menuCmdBar = (HMENU)wnd.SendMessage(CBRM_GETMENU);
		cmi.bCmdBar = (menuCmdBar.m_hMenu != NULL);

		// build a menu from hidden items
		CMenuHandle menu;
		bRet = menu.CreatePopupMenu();
		ATLASSERT(bRet);
		RECT rcClient = { 0 };
		bRet = wnd.GetClientRect(&rcClient);
		ATLASSERT(bRet);
		for(int i = 0; i < nCount; i++)
		{
			TBBUTTON tbb = { 0 };
			bRet = (BOOL)wnd.SendMessage(TB_GETBUTTON, i, (LPARAM)&tbb);
			ATLASSERT(bRet);
			// skip hidden buttons
			if((tbb.fsState & TBSTATE_HIDDEN) != 0)
				continue;
			RECT rcButton = { 0 };
			bRet = (BOOL)wnd.SendMessage(TB_GETITEMRECT, i, (LPARAM)&rcButton);
			ATLASSERT(bRet);
			bool bEnabled = ((tbb.fsState & TBSTATE_ENABLED) != 0);
			if(rcButton.right > rcClient.right)
			{
				if(tbb.fsStyle & BTNS_SEP)
				{
					if(menu.GetMenuItemCount() > 0)
						menu.AppendMenu(MF_SEPARATOR);
				}
				else if(cmi.bCmdBar)
				{
					const int cchBuff = 200;
					TCHAR szBuff[cchBuff] = { 0 };
					CMenuItemInfo mii;
					mii.fMask = MIIM_TYPE | MIIM_SUBMENU;
					mii.dwTypeData = szBuff;
					mii.cch = cchBuff;
					bRet = menuCmdBar.GetMenuItemInfo(i, TRUE, &mii);
					ATLASSERT(bRet);
					// Note: CmdBar currently supports only drop-down items
					ATLASSERT(::IsMenu(mii.hSubMenu));
					bRet = menu.AppendMenu(MF_STRING | MF_POPUP | (bEnabled ? MF_ENABLED : MF_GRAYED), (UINT_PTR)mii.hSubMenu, mii.dwTypeData);
					ATLASSERT(bRet);
				}
				else
				{
					// get button's text
					const int cchBuff = 200;
					TCHAR szBuff[cchBuff] = { 0 };
					LPTSTR lpstrText = szBuff;
					TBBUTTONINFO tbbi = { 0 };
					tbbi.cbSize = sizeof(TBBUTTONINFO);
					tbbi.dwMask = TBIF_TEXT;
					tbbi.pszText = szBuff;
					tbbi.cchText = cchBuff;
					if(wnd.SendMessage(TB_GETBUTTONINFO, tbb.idCommand, (LPARAM)&tbbi) == -1 || lstrlen(szBuff) == 0)
					{
						// no text for this button, try a resource string
						lpstrText = _T("");
						int nRet = ::LoadString(ModuleHelper::GetResourceInstance(), tbb.idCommand, szBuff, cchBuff);
						for(int n = 0; n < nRet; n++)
						{
							if(szBuff[n] == _T('\n'))
							{
								lpstrText = &szBuff[n + 1];
								break;
							}
						}
					}
					bRet = menu.AppendMenu(MF_STRING | (bEnabled ? MF_ENABLED : MF_GRAYED), tbb.idCommand, lpstrText);
					ATLASSERT(bRet);
				}
			}
		}

		if(menu.GetMenuItemCount() == 0)   // no hidden buttons after all
		{
			menu.DestroyMenu();
			::MessageBeep((UINT)-1);
			return false;
		}

		cmi.hMenu = menu;
		return true;
	}

	void DisplayChevronMenu(_ChevronMenuInfo& cmi)
	{
#ifndef TPM_VERPOSANIMATION
		const UINT TPM_VERPOSANIMATION = 0x1000L;   // Menu animation flag
#endif
		// convert chevron rect to screen coordinates
		ATL::CWindow wndFrom = cmi.lpnm->hdr.hwndFrom;
		POINT pt = { cmi.lpnm->rc.left, cmi.lpnm->rc.bottom };
		wndFrom.MapWindowPoints(NULL, &pt, 1);
		RECT rc = cmi.lpnm->rc;
		wndFrom.MapWindowPoints(NULL, &rc);
		// set up flags and rect
		UINT uMenuFlags = TPM_LEFTBUTTON | TPM_VERTICAL | TPM_LEFTALIGN | TPM_TOPALIGN | (!AtlIsOldWindows() ? TPM_VERPOSANIMATION : 0);
		TPMPARAMS TPMParams = { 0 };
		TPMParams.cbSize = sizeof(TPMPARAMS);
		TPMParams.rcExclude = rc;
		// check if this window has a command bar
		T * pT = static_cast<T*>(this);
		HWND hWndCmdBar = (HWND)::SendMessage(pT->m_hWnd, CBRM_GETCMDBAR, 0, 0L);
		if(::IsWindow(hWndCmdBar))
		{
			CBRPOPUPMENU CBRPopupMenu = { sizeof(CBRPOPUPMENU), cmi.hMenu, uMenuFlags, pt.x, pt.y, &TPMParams };
			::SendMessage(hWndCmdBar, CBRM_TRACKPOPUPMENU, 0, (LPARAM)&CBRPopupMenu);
		}
		else
		{
			CMenuHandle menu = cmi.hMenu;
			menu.TrackPopupMenuEx(uMenuFlags, pt.x, pt.y, pT->m_hWnd, &TPMParams);
		}
	}

	void CleanupChevronMenu(_ChevronMenuInfo& cmi)
	{
		CMenuHandle menu = cmi.hMenu;
		// if menu is from a command bar, detach submenus so they are not destroyed
		if(cmi.bCmdBar)
		{
			for(int i = menu.GetMenuItemCount() - 1; i >=0; i--)
				menu.RemoveMenu(i, MF_BYPOSITION);
		}
		// destroy menu
		menu.DestroyMenu();
		// convert chevron rect to screen coordinates
		ATL::CWindow wndFrom = cmi.lpnm->hdr.hwndFrom;
		RECT rc = cmi.lpnm->rc;
		wndFrom.MapWindowPoints(NULL, &rc);
		// eat next message if click is on the same button
		T * pT = static_cast<T*>(this);
		MSG msg = { 0 };
		if(::PeekMessage(&msg, pT->m_hWnd, WM_LBUTTONDOWN, WM_LBUTTONDOWN, PM_NOREMOVE) && ::PtInRect(&rc, msg.pt))
			::PeekMessage(&msg, pT->m_hWnd, WM_LBUTTONDOWN, WM_LBUTTONDOWN, PM_REMOVE);
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
};


template <class T >
class ATL_NO_VTABLE CFrameDialog : public CFrameDialogBase<T>
{
public:
	void LockBands(bool bLock)
	{
		CReBarCtrl ctrl = m_hWndToolBar;
		ctrl.LockBands(bLock);
	}

	BOOL LoadSimpleAccel(UINT nAccelID)
	{
		m_hAccel = ::LoadAccelerators(ModuleHelper::GetResourceInstance(), MAKEINTRESOURCE(nAccelID));
		return (m_hAccel != NULL);
	}

	void LoadSimpleIcon(UINT nIconID)
	{
		T * pT = static_cast<T*>(this);
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(nIconID), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		pT->SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(nIconID), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		pT->SetIcon(hIconSmall, FALSE);
	}

	BOOL GetTrueClientRect(LPRECT lpRect)
	{
		T * pT = static_cast<T*>(this);
		pT->GetClientRect(lpRect);
		// position bars and offset their dimensions
		UpdateBarsPosition(*lpRect, FALSE);
		return TRUE;
	}

	void UpdateFrameTrueClient(LPRECT /*lpRect*/)
	{
		// T is request inheritance CDialogResize<> or implement methods
		// void UpdateFrameClient(LPRECT lpRect)
		RECT rc = {0};
		T * pT = static_cast<T*>(this);
		pT->GetClientRect(&rc);
		pT->DlgResize_UpdateLayout(rc.right-rc.top,rc.bottom-rc.top);
	}

	void UpdateFrameStatusBar(BOOL bVisible, int nHeight)
	{
		// T is request inheritance CDialogResize<> or implement methods
		// void UpdateFrameStatusBar(BOOL bVisible, int nHeight)
		T * pT = static_cast<T*>(this);
		if (!bVisible)
			pT->CDialogResize<CMainDlg>::m_sizeDialog.cy -= nHeight;
		else
			pT->CDialogResize<CMainDlg>::m_sizeDialog.cy += nHeight;
	}

	BOOL ViewStatusBar(BOOL bVisible)
	{
		if (m_hWndStatusBar == NULL)
			return FALSE;
		
		BOOL bNowVisible = (((DWORD)::GetWindowLong(m_hWndStatusBar, GWL_STYLE) & WS_VISIBLE) != 0);
		if (bNowVisible == bVisible)
			return FALSE;
		
		T * pT = static_cast<T*>(this);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);

		RECT rectSB = { 0 };
		::GetWindowRect(m_hWndStatusBar, &rectSB);
		pT->ScreenToClient(&rectSB);
		
		pT->UpdateFrameStatusBar(bVisible,rectSB.bottom-rectSB.top);

		pT->UpdateFrameLayout();

		return TRUE;
	}

	BOOL ViewReBarBand(int index, BOOL bVisible)
	{
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + index);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);

		T * pT = static_cast<T*>(this);
		pT->UpdateFrameLayout();

		return !bVisible;
	}

	BOOL ViewToolBar(BOOL bVisible)
	{
		T * pT = static_cast<T*>(this);
		::ShowWindow(m_hWndToolBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);

		pT->OnRepositionDialog();

		pT->UpdateFrameLayout();

		return !bVisible;
	}

	BOOL OnRepositionDialog()
	{
		if (m_nBarHeight == -1)
			return FALSE;

		T * pT = static_cast<T*>(this);

		RECT rectTB = { 0 };
		if(m_hWndToolBar != NULL && ((DWORD)::GetWindowLong(m_hWndToolBar, GWL_STYLE) & WS_VISIBLE))
		{
			::GetWindowRect(m_hWndToolBar, &rectTB);
			pT->ScreenToClient(&rectTB);
		}

		int nHeight = rectTB.bottom-rectTB.top;

		if (m_nBarHeight != nHeight)
		{
			POINT ptOffset = {0, nHeight-m_nBarHeight};
			m_nBarHeight = nHeight;
			RECT rcChild;
			CWindow wndChild = ::GetWindow(pT->m_hWnd, GW_CHILD);
			while (wndChild)
			{
				if (wndChild != m_hWndToolBar  && wndChild != m_hWndStatusBar)
				{
					wndChild.GetWindowRect(&rcChild);
					pT->ScreenToClient(&rcChild);
					::OffsetRect(&rcChild,ptOffset.x,ptOffset.y);
					wndChild.MoveWindow(&rcChild,FALSE);
				}
				wndChild = ::GetNextWindow(wndChild,GW_HWNDNEXT);
			}			
		}
		return TRUE;
	}

	BOOL RepositionDialog()
	{
		T * pT = static_cast<T*>(this);
		if (m_hWndToolBar != NULL)
		{
			RECT rectTB = { 0 };
			::GetWindowRect(m_hWndToolBar, &rectTB);
			
			POINT ptOffset = {0,rectTB.bottom-rectTB.top};

			m_nBarHeight = rectTB.bottom-rectTB.top;
			
			RECT rcChild;
			CWindow wndChild = ::GetWindow(pT->m_hWnd, GW_CHILD);
			while (wndChild)
			{
				if (wndChild != m_hWndToolBar)
				{
					wndChild.GetWindowRect(&rcChild);
					pT->ScreenToClient(&rcChild);
					::OffsetRect(&rcChild,ptOffset.x,ptOffset.y);
					wndChild.MoveWindow(&rcChild,FALSE);
				}
				wndChild = ::GetNextWindow(wndChild,GW_HWNDNEXT);
			}

			RECT rcWindow;
			pT->GetWindowRect(&rcWindow);

			RECT rcClient;
			pT->GetClientRect(&rcClient);
			pT->ClientToScreen(&rcClient);

			rcWindow.right += ptOffset.x;
			rcWindow.bottom += ptOffset.y;

			//test use cmdbar and attach menu
			
			int bUse = rcClient.top - rcWindow.top - GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CYCAPTION);
			if (bUse <= 0 && pT->GetMenu() == NULL)
			{
				rcWindow.bottom -= GetSystemMetrics(SM_CYMENU);
			}
			if ( (pT->GetStyle() & WS_CHILD) == WS_CHILD)
			{
				CWindow parent = pT->GetParent();
				if (parent.m_hWnd)
				{
					parent.ScreenToClient(&rcWindow);
				}
			}
			pT->MoveWindow(&rcWindow, FALSE);
		}

		if (m_hWndStatusBar != NULL)
		{
			RECT rectSB = { 0 };
			::GetWindowRect(m_hWndStatusBar, &rectSB);
			pT->ScreenToClient(&rectSB);
			
			RECT rcWindow;
			pT->GetWindowRect(&rcWindow);
			rcWindow.bottom += rectSB.bottom-rectSB.top;

			if ( (pT->GetStyle() & WS_CHILD) == WS_CHILD)
			{
				CWindow parent = pT->GetParent();
				if (parent.m_hWnd)
				{
					parent.ScreenToClient(&rcWindow);
				}
			}
			pT->MoveWindow(&rcWindow, FALSE);	
			
			::SendMessage(m_hWndStatusBar, WM_SIZE, 0, 0);
		}
		return TRUE;
	}

	BOOL CreateSimpleToolBar(UINT nResourceID , DWORD dwStyle = ATL_SIMPLE_TOOLBAR_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
		T * pT = static_cast<T*>(this);
//		if(nResourceID == 0)
//			nResourceID = T::GetWndClassInfo().m_uCommonResourceID;
#ifndef _WIN32_WCE
		ATLASSERT(!::IsWindow(m_hWndToolBar));
		m_hWndToolBar = T::CreateSimpleToolBarCtrl(pT->m_hWnd, nResourceID, TRUE, dwStyle, nID);
		return (m_hWndToolBar != NULL);
#else // CE specific
		HWND hWnd= T::CreateSimpleToolBarCtrl(m_hWndCECommandBar, nResourceID, TRUE, dwStyle, nID);
		return (hWnd != NULL);
#endif // _WIN32_WCE
	}

	HWND CreateToolBarCtrl(UINT nResourceID , DWORD dwStyle = ATL_SIMPLE_TOOLBAR_PANE_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
		T * pT = static_cast<T*>(this);
//		if(nResourceID == 0)
//			nResourceID = T::GetWndClassInfo().m_uCommonResourceID;
#ifndef _WIN32_WCE
		HWND hWnd = T::CreateSimpleToolBarCtrl(pT->m_hWnd, nResourceID, FALSE, dwStyle, nID);
		return hWnd;
#else // CE specific
		HWND hWnd= T::CreateSimpleToolBarCtrl(m_hWndCECommandBar, nResourceID, FALSE, dwStyle, nID);
		return hWnd;
#endif // _WIN32_WCE
	}

#ifdef _WIN32_WCE
	// CE specific variant that returns the handle of the toolbar
	HWND CreateSimpleCEToolBar(UINT nResourceID = 0, DWORD dwStyle = ATL_SIMPLE_TOOLBAR_STYLE, UINT nID = ATL_IDW_TOOLBAR)
	{
	//	if(nResourceID == 0)
	//		nResourceID = T::GetWndClassInfo().m_uCommonResourceID;
		return T::CreateSimpleToolBarCtrl(m_hWndCECommandBar, nResourceID, TRUE, dwStyle, nID);
	}
#endif // _WIN32_WCE

// message map and handlers
	BEGIN_MSG_MAP(CFrameDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
#ifndef _ATL_NO_REBAR_SUPPORT
#if (_WIN32_IE >= 0x0400)
		NOTIFY_CODE_HANDLER(RBN_AUTOSIZE, OnReBarAutoSize)
#endif // (_WIN32_IE >= 0x0400)
#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
		NOTIFY_CODE_HANDLER(RBN_CHEVRONPUSHED, OnChevronPushed)
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
#endif // !_ATL_NO_REBAR_SUPPORT
		CHAIN_MSG_MAP(CFrameDialogBase<T>)
	END_MSG_MAP()

	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(wParam != SIZE_MINIMIZED)
		{
			T* pT = static_cast<T*>(this);
			pT->UpdateFrameLayout(TRUE, FALSE);
		}
		bHandled = FALSE;
		return 1;
	}

#ifndef _ATL_NO_REBAR_SUPPORT
#if (_WIN32_IE >= 0x0400)
	LRESULT OnReBarAutoSize(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_hWnd && m_hWndToolBar)
		{
			pT->OnRepositionDialog();
			pT->UpdateFrameLayout(FALSE, FALSE);
		}
		return 0;
	}
#endif // (_WIN32_IE >= 0x0400)

#if (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
	LRESULT OnChevronPushed(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);
		_ChevronMenuInfo cmi = { NULL, (LPNMREBARCHEVRON)pnmh, false };
		if(!pT->PrepareChevronMenu(cmi))
		{
			bHandled = FALSE;
			return 1;
		}
		// display a popup menu with hidden items
		pT->DisplayChevronMenu(cmi);
		// cleanup
		pT->CleanupChevronMenu(cmi);
		return 0;
	}
#endif // (_WIN32_IE >= 0x0500) && !defined(_WIN32_WCE)
#endif // !_ATL_NO_REBAR_SUPPORT
};

//
#ifndef _WIN32_WCE
template <class T, class TBase = CWindow>
class ATL_NO_VTABLE CFrameDialogImpl : public CDialogImpl<T, TBase>, public CFrameDialog<T>
{
public:
	BEGIN_MSG_MAP(CFrameDialogImpl)
		CHAIN_MSG_MAP(CFrameDialog<T>)
	END_MSG_MAP()
};

template <class T, class TBase = CWindow>
class ATL_NO_VTABLE CAxFrameDialogImpl : public CAxDialogImpl<T, TBase>, public CFrameDialog<T>
{
public:
	BEGIN_MSG_MAP(CAxFrameDialogImpl)
		CHAIN_MSG_MAP(CFrameDialog<T>)
	END_MSG_MAP()
};
#endif //_WIN32_WCE

}; //namespace WTL

#endif