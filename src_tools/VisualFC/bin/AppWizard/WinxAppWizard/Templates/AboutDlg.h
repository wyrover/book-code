// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)
#define [!output FILE_NAME_SYMBOL]_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAboutDlg : public winx::ModelDialog<CAboutDlg, IDD_ABOUTBOX>
{
public:
	BOOL winx_msg OnInitDialog(HWND hDlg, HWND hDefaultFocus)
	{
		CenterWindow();
		return TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// [!output INSERT_LOCATION_COMMENT]

#endif // !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)