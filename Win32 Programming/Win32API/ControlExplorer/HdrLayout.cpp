// HdrLayout.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "bitcombo.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "HdrLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHdrLayout property page

IMPLEMENT_DYNCREATE(CHdrLayout, CLoggingPage)

CHdrLayout::CHdrLayout() : CLoggingPage(CHdrLayout::IDD)
{
        //{{AFX_DATA_INIT(CHdrLayout)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CHdrLayout::~CHdrLayout()
{
}

static BITCOMBO t_flags[] = {
        { SWP_DRAWFRAME,      IDS_SWP_DRAWFRAME},
        { SWP_NOSIZE,         IDS_SWP_NOSIZE},
        { SWP_NOMOVE,         IDS_SWP_NOMOVE},
        { SWP_NOZORDER,       IDS_SWP_NOZORDER},
        { SWP_NOREDRAW,       IDS_SWP_NOREDRAW},
        { SWP_NOACTIVATE,     IDS_SWP_NOACTIVATE},
        { SWP_FRAMECHANGED,   IDS_SWP_FRAMECHANGED},
        { SWP_SHOWWINDOW,     IDS_SWP_SHOWWINDOW},
        { SWP_HIDEWINDOW,     IDS_SWP_HIDEWINDOW},
        { SWP_NOCOPYBITS,     IDS_SWP_NOCOPYBITS},
        { SWP_NOOWNERZORDER,  IDS_SWP_NOOWNERZORDER},
        { SWP_NOSENDCHANGING, IDS_SWP_NOSENDCHANGING},
        { SWP_DEFERERASE,     IDS_SWP_DEFERERASE},
        { SWP_ASYNCWINDOWPOS, IDS_SWP_ASYNCWINDOWPOS},
        { 0, 0} // EOT
                            };

void CHdrLayout::DoDataExchange(CDataExchange* pDX)
{
        CLoggingPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CHdrLayout)
        DDX_Control(pDX, IDC_SETWINDOWPOS, c_SetWindowPos);
        DDX_Control(pDX, IDC_Y_CAPTION, c_c_y);
        DDX_Control(pDX, IDC_X_CAPTION, c_c_x);
        DDX_Control(pDX, IDC_TOP_CAPTION, c_c_top);
        DDX_Control(pDX, IDC_RIGHT_CAPTION, c_c_right);
        DDX_Control(pDX, IDC_LPWINDOWPOS_CAPTION, c_c_LPWINDOWPOS);
        DDX_Control(pDX, IDC_LPRECT_CAPTION, c_c_LPRECT);
        DDX_Control(pDX, IDC_LEFT_CAPTION, c_c_left);
        DDX_Control(pDX, IDC_LAYOUT_CAPTION, c_c_Layout);
        DDX_Control(pDX, IDC_HWND_INSERTAFTER_CAPTION, c_c_hwndInsertAfter);
        DDX_Control(pDX, IDC_HWND_CAPTION, c_c_hwnd);
        DDX_Control(pDX, IDC_FLAGS_CAPTION, c_c_flags);
        DDX_Control(pDX, IDC_CY_CAPTION, c_c_cy);
        DDX_Control(pDX, IDC_CX_CAPTION, c_c_cx);
        DDX_Control(pDX, IDC_BOTTOM_CAPTION, c_c_bottom);
        DDX_Control(pDX, IDC_Y, c_y);
        DDX_Control(pDX, IDC_X, c_x);
        DDX_Control(pDX, IDC_TOP, c_top);
        DDX_Control(pDX, IDC_RIGHT, c_right);
        DDX_Control(pDX, IDC_LEFT, c_left);
        DDX_Control(pDX, IDC_HWNDINSERTAFTER, c_hwndInsertAfter);
        DDX_Control(pDX, IDC_HWND, c_hwnd);
        DDX_Control(pDX, IDC_FLAGS, c_flags);
        DDX_Control(pDX, IDC_CY, c_cy);
        DDX_Control(pDX, IDC_CX, c_cx);
        DDX_Control(pDX, IDC_BOTTOM, c_bottom);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHdrLayout, CLoggingPage)
        //{{AFX_MSG_MAP(CHdrLayout)
        ON_BN_CLICKED(IDC_HD_LAYOUT, OnHdLayout)
        ON_BN_CLICKED(IDC_SETWINDOWPOS, OnSetwindowpos)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHdrLayout message handlers


/****************************************************************************
*                         CHdrLayout::enableControls
* Inputs:
*       BOOL enable: TRUE to enable, FALSE to disable
* Result: void
*       
* Effect: 
*       Enables or disables all the controls and their labels
****************************************************************************/

void CHdrLayout::enableControls(BOOL enable)
    {
     c_y.EnableWindow(enable);
     c_c_y.EnableWindow(enable);
     c_x.EnableWindow(enable);
     c_c_x.EnableWindow(enable);
     c_top.EnableWindow(enable);
     c_c_top.EnableWindow(enable);
     c_right.EnableWindow(enable);
     c_c_right.EnableWindow(enable);
     c_left.EnableWindow(enable);
     c_c_left.EnableWindow(enable);

     c_hwndInsertAfter.EnableWindow(enable);
     c_c_hwndInsertAfter.EnableWindow(enable);
     c_hwnd.EnableWindow(enable);
     c_c_hwnd.EnableWindow(enable);
     c_flags.EnableWindow(enable);
     c_c_flags.EnableWindow(enable);
     c_cy.EnableWindow(enable);
     c_c_cy.EnableWindow(enable);
     c_cx.EnableWindow(enable);
     c_c_cx.EnableWindow(enable);
     c_bottom.EnableWindow(enable);
     c_c_bottom.EnableWindow(enable);

     c_c_LPWINDOWPOS.EnableWindow(enable);
     c_c_LPRECT.EnableWindow(enable);
     c_c_Layout.EnableWindow(enable);

     c_SetWindowPos.EnableWindow(enable);
    }


/****************************************************************************
*                        CHdrLayout::layoutToControls
* Result: void
*       
* Effect: 
*       Transfers the values in the 'layout' structure to the controls
****************************************************************************/

void CHdrLayout::layoutToControls()
    {
     c_left.SetWindowText(layout.prc->left);
     c_right.SetWindowText(layout.prc->right);
     c_bottom.SetWindowText(layout.prc->bottom);
     c_top.SetWindowText(layout.prc->top);

     c_x.SetWindowText(layout.pwpos->x);
     c_cx.SetWindowText(layout.pwpos->cx);
     c_y.SetWindowText(layout.pwpos->y);
     c_cy.SetWindowText(layout.pwpos->cy);

     if(layout.pwpos->hwnd != NULL)
        c_hwnd.SetWindowText((int)layout.pwpos->hwnd, _T("%08x"));
     else
        { /* null */
         CString null;
         null.LoadString(IDS_NULL);
         c_hwnd.SetWindowText(null);
        } /* null */

     if(layout.pwpos->hwndInsertAfter != NULL)
        c_hwndInsertAfter.SetWindowText((int)layout.pwpos->hwndInsertAfter,
                                                                  _T("%08x"));
     else
        { /* null */
         CString null;
         null.LoadString(IDS_NULL);
         c_hwndInsertAfter.SetWindowText(null);
        } /* null */

     c_flags.AddBits(layout.pwpos->flags, t_flags);
    }

/****************************************************************************
*                        CHdrLayout::controlsToLayout
* Result: void
*       
* Effect: 
*       Copies selected values from the controls to the layout structure.
*
* Notes:
*       At the moment the only fields that are settable are x, y, cx, cy
****************************************************************************/

void CHdrLayout::controlsToLayout()
    {
     layout.pwpos->x = c_x.GetWindowInt();
     layout.pwpos->y = c_y.GetWindowInt();
     layout.pwpos->cx = c_cx.GetWindowInt();
     layout.pwpos->cy = c_cy.GetWindowInt();
    }

/****************************************************************************
*                          CHdrLayout::clearControls
* Result: void
*       
* Effect: 
*       Clears all the controls
****************************************************************************/

void CHdrLayout::clearControls()
    {
     c_left.Blank();
     c_right.Blank();
     c_bottom.Blank();
     c_top.Blank();
     c_x.Blank();
     c_y.Blank();
     c_cx.Blank();
     c_cy.Blank();
    }

BOOL CHdrLayout::OnInitDialog() 
{
        CLoggingPage::OnInitDialog();
        
        enableControls(FALSE);
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CHdrLayout::OnHdLayout() 
{
 CString fmt;
 CString ctlName;
 ctlName.LoadString(IDS_PARENT);

 CString s;

 c_Header->GetParent()->GetClientRect(&rc);
 fmt.LoadString(IDS_FORMAT_GETCLIENTRECT);
 s.Format(fmt, rc.left, rc.top, rc.right, rc.bottom);
 
 c_Messages->addMessage(c_Header->GetParent()->m_hWnd, ctlName, s);

 layout.prc = &rc;
 layout.pwpos = &pos;
 LRESULT result = ::SendMessage(c_Header->m_hWnd, HDM_LAYOUT, 0, 
                        (LPARAM)&layout);
 CString res;
 if(result)
    { /* success */
     layoutToControls();
     enableControls(TRUE);
     res.LoadString(IDS_TRUE);
    } /* success */
 else
    { /* failure */
     clearControls();
     enableControls(FALSE);
     res.LoadString(IDS_FALSE);
    } /* failure */
 
 fmt.LoadString(IDS_FORMAT_HDM_LAYOUT);
 // HDM_LAYOUT(0, &layout) => %s

 ctlName.LoadString(IDS_HEADER);
 s.Format(fmt, res);
 c_Messages->addMessage(c_Header->m_hWnd, ctlName, s);
}

void CHdrLayout::OnSetwindowpos() 
{
 controlsToLayout();

 BOOL result = c_Header->SetWindowPos(
                        CWnd::FromHandle(layout.pwpos->hwndInsertAfter),
                        layout.pwpos->x,
                        layout.pwpos->y,
                        layout.pwpos->cx,
                        layout.pwpos->cy,
                        layout.pwpos->flags | SWP_SHOWWINDOW);
  
 CString fmt;
 fmt.LoadString(IDS_FORMAT_SETWINDOWPOS);

 CString ctlName;
 ctlName.LoadString(IDS_HEADER);

 CString res;
 res.LoadString(result ? IDS_TRUE : IDS_FALSE);
 CString s;
 s.Format(fmt, layout.pwpos->x, layout.pwpos->y,
               layout.pwpos->cx,layout.pwpos->cy, _T("..."), res);
 c_Messages->addMessage(c_Header->m_hWnd, ctlName, s);

        
}
