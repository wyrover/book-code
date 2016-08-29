// UDBuddy.cpp : implementation file
//

#include "stdafx.h"
#include <afxtempl.h>
#include "UDZ.h"
#include "ControlExplorer.h"
#include "TinyButt.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "UDOps.h"
#include "UDBuddy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpDownBuddy property page

IMPLEMENT_DYNCREATE(CUpDownBuddy, CUDOps)

CUpDownBuddy::CUpDownBuddy() : CUDOps(CUpDownBuddy::IDD)
{
        //{{AFX_DATA_INIT(CUpDownBuddy)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CUpDownBuddy::~CUpDownBuddy()
{
}

void CUpDownBuddy::DoDataExchange(CDataExchange* pDX)
{
        CUDOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CUpDownBuddy)
        DDX_Control(pDX, IDC_SETBASE, c_SetBase);
        DDX_Control(pDX, IDC_SETBUDDY, c_SetBuddy);
        DDX_Control(pDX, IDC_GETBUDDY, c_GetBuddy);
        DDX_Control(pDX, IDC_GETBASE, c_GetBase);
        DDX_Control(pDX, IDC_BUDDY, c_Buddy);
        DDX_Control(pDX, IDC_BASE, c_Base);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpDownBuddy, CUDOps)
        //{{AFX_MSG_MAP(CUpDownBuddy)
        ON_BN_CLICKED(IDC_GETBASE, OnGetbase)
        ON_BN_CLICKED(IDC_GETBUDDY, OnGetbuddy)
        ON_BN_CLICKED(IDC_SETBUDDY, OnSetbuddy)
        ON_BN_CLICKED(IDC_SETBASE, OnSetbase)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpDownBuddy message handlers


/****************************************************************************
*                            CUpDownBuddy::findSel
* Inputs:
*       int n: Value to find
* Result: int
*       Index of itemdata that matches n, or CB_ERR
* Notes:
*       This should actually be a member of our combo box class, but we're
*       pressed for time tonight... (sloppy, naughty, naughty...)
****************************************************************************/

int CUpDownBuddy::findSel(int n)
    {
     for(int i = 0; i < c_Base.GetCount(); i++)
        if((int)c_Base.GetItemData(i) == n)
           return i;

     return CB_ERR;
    }

BOOL CUpDownBuddy::OnInitDialog() 
{
        CUDOps::OnInitDialog();
        
        int index = c_Base.AddString(_T("10"));
        c_Base.SetItemData(index, 10);
        index = c_Base.AddString(_T("16"));
        c_Base.SetItemData(index, 16);

        index = findSel(10);
        c_Base.SetCurSel(index);
        
        
        CString s;
        s.LoadString(IDS_NONE);
        index = c_Buddy.AddString(s);
        c_Buddy.SetItemData(index, NULL);

        for(int i = 0; i < czarray->GetSize(); i++)
           { /* load each */
            index = c_Buddy.AddString(czarray->GetAt(i)->caption);
            c_Buddy.SetItemData(index, 
                                   (LPARAM)(czarray->GetAt(i)->wnd->m_hWnd));
           } /* load each */

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CUpDownBuddy::OnGetbase() 
{
 int result = ::SendMessage(c_UpDown->m_hWnd, UDM_GETBASE, 0, 0);
 showResult_N_N_d(IDS_UDM_GETBASE, result);

 int index = findSel(result);
 c_Base.SetCurSel(index);
}

/****************************************************************************
*                        CUpDownBuddy::getWindowString
* Inputs:
*       HWND hwnd: Window handle 
* Result: CString
*       Print string for window handle
* Effect: 
*       Creates a formatted string from the czarray data.
****************************************************************************/

CString CUpDownBuddy::getWindowString(HWND hwnd)
    {
     CString s;
     
     if(hwnd == NULL)
        { /* NULL */
         s.LoadString(IDS_NULL);
         return s;
        } /* NULL */

     for(int i = 0; i < czarray->GetSize(); i++)
        { /* scan for match */
         if(czarray->GetAt(i)->wnd->m_hWnd == hwnd)
            { /* found it */
             s.Format(_T("0x%08x [%s]"), (int)hwnd, 
                                czarray->GetAt(i)->caption);
             
             return s;
            } /* found it */
        } /* scan for match */

     s.Format(_T("0x%08x"), (int)hwnd);
     return s;
    }

void CUpDownBuddy::OnGetbuddy() 
{
 HWND buddy = (HWND)::SendMessage(c_UpDown->m_hWnd, UDM_GETBUDDY, 0, 0);

 for(int i = 0; i < c_Buddy.GetCount(); i++)
    { /* scan for match */
     if(c_Buddy.GetItemData(i) == (DWORD)buddy)
        { /* found it */
         c_Buddy.SetCurSel(i);
         break;
        } /* found it */
    } /* scan for match */

 showResult_N_N_s(IDS_UDM_GETBUDDY, getWindowString(buddy));
        
}

void CUpDownBuddy::OnSetbuddy() 
{
 int index = c_Buddy.GetCurSel();
 if(index != CB_ERR)
    { /* found it */
     HWND hwnd = (HWND)c_Buddy.GetItemData(index);
     HWND result = (HWND)::SendMessage(c_UpDown->m_hWnd, 
                                        UDM_SETBUDDY, (WPARAM)hwnd, 0);
     
     showResult_s_N_s(IDS_UDM_SETBUDDY, getWindowString(hwnd), 
                                                getWindowString(result));
    } /* found it */
        
}

void CUpDownBuddy::OnSetbase() 
{
 int index = c_Base.GetCurSel();
 if(index != CB_ERR)
    { /* select it */
     int n = c_Base.GetItemData(index);
     int result = ::SendMessage(c_UpDown->m_hWnd, UDM_SETBASE, n, 0);
     showResult_d_N_d(IDS_UDM_SETBASE, n, result);
    } /* select it */
        
}
