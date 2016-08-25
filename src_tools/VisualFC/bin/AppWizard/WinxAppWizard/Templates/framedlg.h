// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
//
// Module: framedlg.h
// Creator: visualfc 
// Email: visualfc@gmail.com
// open source: http://sf.net/projects/visualfc
// Date: 2007-12-06

#ifndef __WINX_FRAMEDIALOG_H__
#define __WINX_FRAMEDIALOG_H__

//#define OnSetFocus	__WTL_OnSetFocus
//#define OnSize		__WTL_OnSize
//#define OnMenuSelect    __WTL_OnMenuSelect
//#define PreTranslateMessage __WTL_PreTranslateMessage
#include <winx/wtl/WrapperMessage.h>
#include <atlctrlw.h>
#include "vfcframe.h"
#include <winx/wtl/UnWrapperMessage.h>
//#undef OnSetFocus
//#undef OnMenuSelect
//#undef PreTranslateMessage
//#undef OnSize
// -------------------------------------------------------------------------

namespace winx
{

template <class T>
class FrameDialog : public WTL::CFrameDialog<T>
{
public:
	typedef FrameDialog<T>	FrameDialogClass;
public:

	void UpdateFrameTrueClient(LPRECT /*lpRect*/ )
	{
		T * pT = static_cast<T*>(this);
		RECT rc = {0};
		pT->GetClientRect(&rc);
		pT->_winx_resizer.DlgResize_UpdateLayout(rc.right-rc.left,rc.bottom-rc.top);
	}

	void UpdateFrameStatusBar(BOOL bVisible, int nHeight)
	{
		T * pT = static_cast<T*>(this);
		if (!bVisible)
			pT->_winx_resizer.m_sizeDialog.cy -= nHeight;
		else
			pT->_winx_resizer.m_sizeDialog.cy += nHeight;
	}

	BOOL ProcessFrameMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT & lResult)
	{
		typedef WTL::CFrameDialog<T> base;
		return base::ProcessWindowMessage(hWnd,message,wParam,lParam,lResult,0);
	}

	BOOL RepositionDialog()
	{
		typedef WTL::CFrameDialog<T> base;
		BOOL bRet = base::RepositionDialog();

		T * pT = static_cast<T*>(this);
		
		if (m_hWndStatusBar != NULL)
		{
		
			RECT rectSB = { 0 };
			::GetWindowRect(m_hWndStatusBar, &rectSB);
			pT->ScreenToClient(&rectSB);
			pT->_winx_resizer.m_sizeDialog.cy += rectSB.bottom-rectSB.top;
			
			pT->UpdateFrameLayout();

			if (pT->_winx_resizer.m_bGripper)
			{
				ATL::CWindow wndGripper = pT->GetDlgItem(ATL_IDW_STATUS_BAR);
				if( wndGripper.IsWindow() )
				{
					wndGripper.ShowWindow(SW_HIDE);
				}
				pT->_winx_resizer.m_bGripper = false;
			}				
		}	

		if (pT->_winx_resizer.UseMinTrackSize)
		{
			RECT rc = {0};
			pT->GetWindowRect(&rc);
			pT->_winx_resizer.m_ptMinTrackSize.y = rc.bottom-rc.top;
		}

		return bRet;
	}
};

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class FrameModelessDialog : public ModelessDialog<WindowClass,nDlgId,HandleClass>, public FrameDialog<WindowClass>
{
public:
	LRESULT winx_msg HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0;

		if (FrameDialogClass::ProcessFrameMessage(hWnd,message,wParam,lParam,lResult))
		{
			return lResult;
		}

		return WindowBase::HandleMessage(hWnd, message, wParam, lParam);
	}
};

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class FrameModelDialog : public ModelDialog<WindowClass,nDlgId,HandleClass>, public FrameDialog<WindowClass>
{
public:
	LRESULT winx_msg HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0;

		if (FrameDialogClass::ProcessFrameMessage(hWnd,message,wParam,lParam,lResult))
		{
			return lResult;
		}

		return WindowBase::HandleMessage(hWnd, message, wParam, lParam);
	}
};

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class AxFrameModelessDialog : public AxModelessDialog<WindowClass,nDlgId,HandleClass>, public FrameDialog<WindowClass>
{
public:
	LRESULT winx_msg HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0;

		if (FrameDialogClass::ProcessFrameMessage(hWnd,message,wParam,lParam,lResult))
		{
			return lResult;
		}

		return WindowBase::HandleMessage(hWnd, message, wParam, lParam);
	}
};

template <class WindowClass, int nDlgId = 0, class HandleClass = DefaultWindowHandle>
class AxFrameModelDialog : public AxModelDialog<WindowClass,nDlgId,HandleClass>, public FrameDialog<WindowClass>
{
public:
	LRESULT winx_msg HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0;

		if (FrameDialogClass::ProcessFrameMessage(hWnd,message,wParam,lParam,lResult))
		{
			return lResult;
		}

		return WindowBase::HandleMessage(hWnd, message, wParam, lParam);
	}
};



}; // namespace winx


#endif //__WINX_FRAMEDIALOG_H__