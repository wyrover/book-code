// FmtMsg.cpp : implementation file
//

#include "stdafx.h"
#include "CursorIconExplorer.h"
#include "FmtMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFmtMsg dialog


CFmtMsg::CFmtMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CFmtMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFmtMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFmtMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFmtMsg)
	DDX_Control(pDX, IDC_TESTBUFF, c_TestBuff);
	DDX_Control(pDX, IDC_RESULT, c_Result);
	DDX_Control(pDX, IDC_P3, c_P3);
	DDX_Control(pDX, IDC_P2, c_P2);
	DDX_Control(pDX, IDC_P1, c_P1);
	DDX_Control(pDX, IDC_FMT, c_Fmt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFmtMsg, CDialog)
	//{{AFX_MSG_MAP(CFmtMsg)
	ON_CBN_SELCHANGE(IDC_FMT, OnSelchangeFmt)
	ON_CBN_SELCHANGE(IDC_P1, OnSelchangeP1)
	ON_CBN_SELCHANGE(IDC_P2, OnSelchangeP2)
	ON_CBN_SELCHANGE(IDC_P3, OnSelchangeP3)
	ON_BN_CLICKED(IDC_MSGBOX, OnMsgbox)
	ON_BN_CLICKED(IDC_TESTBUFF, OnTestbuff)
	ON_BN_CLICKED(IDC_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFmtMsg message handlers

void CFmtMsg::Reformat()
    {
     TCHAR result[256];
     int size = c_TestBuff.GetCheck() ? 20 : sizeof(result);
     TCHAR p1[256];
     TCHAR p2[256];
     TCHAR p3[256];
     TCHAR fmt[256];
     LPTSTR args[3] = {p1, p2, p3};
     DWORD retval;

     c_P1.GetWindowText(p1, sizeof(p1) / sizeof(TCHAR));
     c_P2.GetWindowText(p2, sizeof(p2) / sizeof(TCHAR));
     c_P3.GetWindowText(p3, sizeof(p3) / sizeof(TCHAR));
     c_Fmt.GetWindowText(fmt, sizeof(fmt) / sizeof(TCHAR));
     retval = FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
     		   fmt,
                   0, 0, 
		   result, size,
		   (va_list *)args);
     if(retval != 0)
         { /* OK */
          c_Result.SetWindowText(result);
         } /* OK */
     else
         { /* error */
	  DWORD err = GetLastError();
	  LPTSTR errmsg;
	  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
	  		NULL,
			err,
			0,
			(LPTSTR) &errmsg,
			0,
			NULL);
	  c_Result.SetWindowText(errmsg);
	  LocalFree(errmsg);
         } /* error */

    }
	    

void CFmtMsg::OnSelchangeFmt() 
{
 Reformat();
	
}

void CFmtMsg::OnSelchangeP1() 
{
 Reformat();
}

void CFmtMsg::OnSelchangeP2() 
{
 Reformat();
	
}

void CFmtMsg::OnSelchangeP3() 
{
 Reformat();
	
}

BOOL CFmtMsg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
 	c_P1.SetCurSel(0);
	c_P2.SetCurSel(1);
	c_P3.SetCurSel(2);
	c_Fmt.SetCurSel(0);
	Reformat();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
// getString
//  Inputs:
//      HWND hwnd: Window handle.  Used to 
//                 GetWindowInstance if the hinst is
//                 needed and is NULL
//      HINSTANCE hinst: Instance handle of the 
//                       resource from which we get 
//                       the string
//      LPCTSTR str: A string pointer or a 
//                   MAKEINTRESOURCE of a string ID
//      LPTSTR buffer: If str is a MAKEINTRESOURCE
//                     this is a reference to a buffer
//                     into which the string will be
//                     loaded.
//      int size: The length of buffer, in characters.
// 
// RESULT: LPCTSTR
//      if str is a string pointer, then str
//      if str is a MAKEINTRESOURCE handle, then buffer
//      if any error, then NULL
// EFFECT:
//      If necessary, a string will be loaded and 
//      placed in buffer
#define GetWindowInstance(w) (HINSTANCE)GetWindowLong(w, GWL_HINSTANCE)
LPCTSTR getString(HWND hwnd, HINSTANCE hinst, 
            LPCTSTR str, LPTSTR buffer, int size)
    {
     if(HIWORD(str) == 0)
         { /* MAKEINTRESOURCE ( str ) */
          if(hinst == NULL)
             hinst = GetWindowInstance(hwnd);
          if(LoadString(hinst, LOWORD(str), 
                             buffer, size) == 0)
             return NULL ;
          return buffer ;
         } /* MAKEINTRESOURCE ( fmt )*/
     else
         return str ;
    }


int __cdecl FormatMessageBox(HWND parent, HINSTANCE hinst, UINT flags, LPCTSTR caption, LPCTSTR fmt, ...)
    {
     LPTSTR result;
     TCHAR localfmt[256];
     TCHAR localcaption[256];
     va_list args;
     DWORD fmtval;
     int retval;

     va_start(args, fmt);

     fmt = getString(parent, hinst, fmt, localfmt, 
	          sizeof(localfmt) / sizeof(TCHAR));
     if(fmt == NULL)
	 return -1;
     caption = getString(parent, hinst, caption,
	          localcaption,
		  sizeof(localcaption) / sizeof(TCHAR));
     if(caption == NULL)
	 return -1;
     fmtval = FormatMessage( FORMAT_MESSAGE_FROM_STRING |
	                     FORMAT_MESSAGE_ALLOCATE_BUFFER,
     		   fmt,
                   0, 0, 
		   (LPTSTR)&result, lstrlen(fmt) * sizeof(TCHAR),
		   &args);
     if(fmtval != 0)
         { /* OK */
          retval = MessageBox(parent, result, caption, flags);
	  LocalFree(result);
         } /* OK */
     else
         { /* error */
	  DWORD err = GetLastError();
	  TCHAR errmsg[256];
	  wsprintf(errmsg, _T("%d: 0x%08x"), err, err);
	  MessageBox(parent, fmt, errmsg, MB_ICONSTOP | MB_OK);
	  retval = -1;
         } /* error */
     va_end(args);
     return retval;
    }

void CFmtMsg::OnMsgbox() 
{
     TCHAR caption[256];
     TCHAR p1[256];
     TCHAR p2[256];
     TCHAR p3[256];
     TCHAR fmt[256];

     c_P1.GetWindowText(p1, sizeof(p1) / sizeof(TCHAR));
     c_P2.GetWindowText(p2, sizeof(p2) / sizeof(TCHAR));
     c_P3.GetWindowText(p3, sizeof(p3) / sizeof(TCHAR));
     c_Fmt.GetWindowText(fmt, sizeof(fmt) / sizeof(TCHAR));

     LoadString(AfxGetInstanceHandle(), AFX_IDS_APP_TITLE, caption, 
 				sizeof(caption)/sizeof(TCHAR));

     FormatMessageBox(m_hWnd, NULL, MB_ICONWARNING | MB_OKCANCEL, 
     			MAKEINTRESOURCE(AFX_IDS_APP_TITLE),
     			fmt, 
			p1, p2, p3);

	
}

void CFmtMsg::OnTestbuff() 
    {
     Reformat();
    }

void CFmtMsg::OnHelp() 
{
 AfxMessageBox(IDS_EXPLAIN);
	
}
