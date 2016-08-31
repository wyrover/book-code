// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MenuExplorer.h"
#include "menusht.h"
#include "msglog.h"
#include "tinybutt.h"
#include "bitmaps.h"
#include "intedit.h"
#include "idcombo.h"
#include "gcombo.h"
#include "color.h"
#include "syscolor.h"
#include "menuchk.h"
#include "menuitem.h"

#include "menulist.h"
#include "menuinfo.h"

#include "menudlg.h"

#include "od.h"
#include "tpm.h"
#include "odmenu.h"

#include "MainFrm.h"

#include "uwm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
        //{{AFX_MSG_MAP(CMainFrame)
        ON_WM_CREATE()
        ON_COMMAND(IDM_MENUEXPLORER, OnMenuexplorer)
        ON_WM_INITMENU()
        ON_WM_INITMENUPOPUP()
        ON_WM_MENUSELECT()
        ON_WM_MENUCHAR()
        ON_WM_CONTEXTMENU()
        ON_WM_DRAWITEM()
        ON_WM_MEASUREITEM()
        ON_COMMAND(IDM_ENABLE_BAR, OnEnableBar)
        ON_UPDATE_COMMAND_UI(IDM_ENABLE_BAR, OnUpdateEnableBar)
        ON_COMMAND(IDM_ENABLE_BREAK, OnEnableBreak)
        ON_UPDATE_COMMAND_UI(IDM_ENABLE_BREAK, OnUpdateEnableBreak)
        //}}AFX_MSG_MAP
        // These macros are not handled by the ClassWizard and have
        // to be outside the AFX_MSG_MAP comments
        ON_WM_ENTERMENULOOP()
        ON_WM_EXITMENULOOP()
        // Global help commands
        ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
        ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
        ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
        ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
        ID_SEPARATOR,           // status line indicator
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
 m_bAutoMenuEnable = FALSE; // we want control for this program!
 sheet = NULL;
 exp   = NULL;
 tpm   = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
        if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
                return -1;
        
        if (!m_wndToolBar.Create(this) ||
                !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
        {
                TRACE0("Failed to create toolbar\n");
                return -1;      // fail to create
        }

        if (!m_wndStatusBar.Create(this) ||
                !m_wndStatusBar.SetIndicators(indicators,
                  sizeof(indicators)/sizeof(UINT)))
        {
                TRACE0("Failed to create status bar\n");
                return -1;      // fail to create
        }

        // TODO: Remove this if you don't want tool tips or a resizeable toolbar
        m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
                CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

        // TODO: Delete these three lines if you don't want the toolbar to
        //  be dockable
        m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
        EnableDocking(CBRS_ALIGN_ANY);
        DockControlBar(&m_wndToolBar);

        // Attach the owner-draw menu facility

        CMenu * mainmenu = GetMenu();
        odmenu.Attach(mainmenu->m_hMenu);  // owner-draw could appear anywhere

        return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
        CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
        CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


/****************************************************************************
*                           CMainFrame::OnCommand
* Inputs:
*       WPARAM wParam:
*       LPARAM lParam:
* Result: BOOL
*       
* Effect: 
*       Passes the message on to the logging facility
****************************************************************************/


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
     if(sheet != NULL && sheet->m_hWnd != NULL)
           sheet->SendMessage(UWM_COMMAND, wParam, lParam);
        
        return CMDIFrameWnd::OnCommand(wParam, lParam);
}

void CMainFrame::OnMenuexplorer() 
{
 // force underlying windo to top left corner of screen
 AfxGetMainWnd()->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

 // create new window
 if(sheet == NULL)
    { /* create new sheet */
     sheet = new CMenuSheet(IDS_MENUEXPLORER);
     exp = new CMenuExplorerDlg();
     tpm = new CTPM();
     info = new CMenuInfo();
     od = new COD();

     sheet->AddPage(exp);
     sheet->AddPage(tpm);
     sheet->AddPage(info);
     sheet->AddPage(od);

     if(!sheet->Create(AfxGetMainWnd()))
        { /* failed */
         DWORD lasterror = GetLastError();
         LPTSTR msg;
         if(lasterror != 0)
            if(::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                            FORMAT_MESSAGE_ALLOCATE_BUFFER,
                            NULL, lasterror, 0, (LPTSTR)&msg, 0, NULL) != 0)
               { /* report error */
                   AfxMessageBox(msg, MB_ICONERROR | MB_OK);
                   LocalFree(msg);
               } /* report error */
            else
               { /* failed for unknown reason */
                AfxMessageBox(_T("?"), MB_ICONERROR | MB_OK);
               } /* failed for unknown reason */
         delete exp;
         delete tpm;
         delete info;
         delete sheet;
         exp = NULL;
         tpm = NULL;
         info = NULL;
         sheet = NULL;
        } /* failed */
    } /* create new sheet */
 else
    { /* show it */
     sheet->ShowWindow(SW_NORMAL);
     sheet->SetFocus();
    } /* show it */
}


void CMainFrame::OnInitMenu(CMenu* pMenu) 
{
        CMDIFrameWnd::OnInitMenu(pMenu);
        
        if(sheet != NULL && sheet->m_hWnd != NULL)
           sheet->SendMessage(UWM_INITMENU, (WPARAM)pMenu->m_hMenu, 0);
        
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
        CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
        
        if(sheet != NULL && sheet->m_hWnd != NULL)
           sheet->SendMessage(UWM_INITMENUPOPUP, (WPARAM)pPopupMenu->m_hMenu, 
                                                MAKELONG(nIndex, bSysMenu));
        
}

void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
        CMDIFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
        
        if(sheet != NULL && sheet->m_hWnd != NULL)
           sheet->SendMessage(UWM_MENUSELECT, MAKELONG(nItemID, nFlags), 
                                        (LPARAM)hSysMenu);
        
}

LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
     if(sheet != NULL && sheet->m_hWnd != NULL)
           sheet->SendMessage(UWM_MENUCHAR, MAKELONG(nChar, nFlags), 
                                                    (LPARAM)pMenu->m_hMenu);
     return CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
}

void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
{
        // TODO: Add your message handler code here
        
}

/****************************************************************************
*                         CMainFrame::OnExitMenuLoop
* Inputs:
*       BOOL popup: TRUE if popup, FALSE if not
* Result: void
*       
* Effect: 
*       Handles WM_ENTERMENULOOP
****************************************************************************/

void CMainFrame::OnExitMenuLoop(BOOL popup)
    {
     if(sheet != NULL && sheet->m_hWnd != NULL)
           sheet->SendMessage(UWM_EXITMENULOOP, (WPARAM)popup, 0);
     CMDIFrameWnd::OnExitMenuLoop(popup);
    }

/****************************************************************************
*                         CMainFrame::OnEnterMenuLoop
* Inputs:
*       BOOL popup: TRUE if popup, FALSE if not
* Result: void
*       
* Effect: 
*       Handles WM_ENTERMENULOOP
****************************************************************************/

void CMainFrame::OnEnterMenuLoop(BOOL popup)
    {
     if(sheet != NULL && sheet->m_hWnd != NULL)
           sheet->SendMessage(UWM_ENTERMENULOOP, (WPARAM)popup, 0);
     CMDIFrameWnd::OnEnterMenuLoop(popup);
    }



/****************************************************************************
*                             FindPopupMenuFromID
* Inputs:
*       CMenu * menu: parent menu
*       UINT id: ID of control we are searching for
* Result: COwnerDrawMenu *
*       A (possibly temporary) menu handle
* Notes: 
*       See the article "MFC Self-Drawing Menus".  We have to circumvent
*       the built-in MFC handling because of our dynamic creation and 
*       reassignment of menus.  
****************************************************************************/

COwnerDrawMenu * FindPopupMenuFromID(CMenu * menu, UINT id)
    {
     UINT items = menu->GetMenuItemCount();
     for(int i = 0; i < (int)items; i++)
        { /* scan each */
         COwnerDrawMenu * popup = (COwnerDrawMenu *)menu->GetSubMenu(i);
         if(popup != NULL)
            { /* submenu */
             popup = FindPopupMenuFromID(popup, id);
             if(popup != NULL)
                return popup;
            } /* submenu */
         else
            { /* menu item */
             if(menu->GetMenuItemID(i) == id)
                { /* found it */
                 return (COwnerDrawMenu *)CMenu::FromHandle(menu->m_hMenu);
                } /* found it */
            } /* menu item */
        } /* scan each */
     return NULL;
    }

/****************************************************************************
*                          CMainFrame::OnMeasureItem
* Inputs:
*       int nIDCtl:
*       LPMEASUREITEMSTRUCT mis: 
* Result: void
*       
* Effect: 
*       Attempts to call the virtual MeasureItem method
* Notes:
*       This is a direct ripoff of the CWnd::OnMeasureItem function
*       but we can't use that one because it relies on the permanent
*       menu handle map to track menus.  Our constant creation and reassignment
*       of menus makes it incredibly hairy to keep the permanent map 
*       consistent, so we fake it by simply knowing that we have only one
*       class of owner-draw menu in this application and we use
*       temporary handles to deal with that one class.
* 
*       This code comes directly from the article "MFC Self-Drawing Menus"
*       on the MSDN CD-ROM.
*
*       Strictly speaking, we don't need to create a menu object to hold
*       the popup menu we've found; but we're keeping this flexibility
*       for the future.  We could probably do this simply by attaching
*       any random menu item (say, for example, the top-level menu) and
*       doing the call.
****************************************************************************/

void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT mis) 
{
        if(mis->CtlType == ODT_MENU)
           { /* menu */
            // Because we are creating menus dynamically all over the place
            // we have to bypass the MFC ownerdraw reflection mechanism
            // which relies on relatively static menu structure.  We are
            // aided in this because we have only one type of owner-draw
            // menu, which simplifies our life immensely

            CMenu * root;

            _AFX_THREAD_STATE * threadstate = AfxGetThreadState();
            if(threadstate->m_hTrackingWindow == m_hWnd)
               root = CMenu::FromHandle(threadstate->m_hTrackingMenu);
            else
               root = GetMenu();

            COwnerDrawMenu * odm;
            odm = FindPopupMenuFromID(root, mis->itemID);
            if(odm != NULL)
               { /* found it */
                // This bizarre hack is required to map the virtual methods
                COwnerDrawMenu o;
                o.Attach(odm->Detach());
                o.MeasureItem(mis);
                o.Detach();  // avoid gratuitous destruction
               } /* found it */
           } /* menu */
        
        // Because we are not CWnd, but a child of CWnd, we don't call
        // Default() as CWnd::OnMeasureItem does, but we call our parent class

        CMDIFrameWnd::OnMeasureItem(nIDCtl, mis);
}

/****************************************************************************
*                           CMainFrame::OnDrawItem
* Inputs:
*       int nIDCtl:
*       LPDRAWITEMSTRUCT dis:
* Result: void
*       
* Effect: 
*       Handles the drawing of owner-draw messages.  See the extensive
*       comments in CMainFrame::OnMeasureItem for details of why this is
*       necessary
* Notes:
*       This is the same code that is found in OnMeasureItem, and is not
*       the same as the code found in CWnd::OnDrawItem.  But this code
*       relies upon CMenu objects in the permanent handle map, which we
*       don't have; we create temporary objects on the fly.
****************************************************************************/

void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT dis) 
{
 if(dis->CtlType == ODT_MENU)
    { /* menu */
     CMenu * root;

     _AFX_THREAD_STATE * threadstate = AfxGetThreadState();
     if(threadstate->m_hTrackingWindow == m_hWnd)
        root = CMenu::FromHandle(threadstate->m_hTrackingMenu);
     else
        root = GetMenu();

     COwnerDrawMenu * odm;
     odm = FindPopupMenuFromID(root, dis->itemID);
     if(odm != NULL)
        { /* found it */
         // This bizarre hack is required to map the virtual methods across!
         COwnerDrawMenu o;
         o.Attach(odm->Detach());
         o.DrawItem(dis);
         o.Detach();  // avoid gratuitous destruction
        } /* found it */
    } /* menu */
        
 // Because we are not CWnd, but a child of CWnd, we don't call
 // Default() as CWnd::OnDrawItem does, but we call our parent class
 CMDIFrameWnd::OnDrawItem(nIDCtl, dis);
}


/****************************************************************************
*                        CMainFrame::changeBreakState
* Inputs:
*       int id: Menu ID
*       UINT flag: Which flag to change (MF_MENUBREAK, MF_MENUBARBREAK)
* Result: void
*       
* Effect: 
*       Changes the menu break state
****************************************************************************/

void CMainFrame::changeBreakState(int id, UINT flag)
    {
     // Toggles the state of the Enable Menu (Bar) Break

     CMenu * main = AfxGetMainWnd()->GetMenu();

     UINT state = main->GetMenuState(id, MF_BYCOMMAND);

     if(state == (UINT)-1) 
        return;  // unknown ... this should not occur

     if(state & flag)
        state &= ~flag;
     else
        state |= flag;

     CString str;

     main->GetMenuString(id, str, MF_BYCOMMAND);

     main->ModifyMenu(id, MF_BYCOMMAND | state, 
                            id,  // use same ID as it had
                            str);

     
    }

void CMainFrame::OnEnableBar() 
{
 changeBreakState(IDM_ENABLE_BAR, MF_MENUBARBREAK);
}

void CMainFrame::OnUpdateEnableBar(CCmdUI* pCmdUI) 
{
 CMenu * main = AfxGetMainWnd()->GetMenu();

 UINT state = main->GetMenuState(IDM_ENABLE_BAR, MF_BYCOMMAND);

 if(state == (UINT)-1)
    return;  // impossible...

 pCmdUI->SetCheck((state & MF_MENUBARBREAK));
}

void CMainFrame::OnEnableBreak() 
{
 changeBreakState(IDM_ENABLE_BREAK, MF_MENUBREAK);
}

void CMainFrame::OnUpdateEnableBreak(CCmdUI* pCmdUI) 
{
 CMenu * main = AfxGetMainWnd()->GetMenu();

 UINT state = main->GetMenuState(IDM_ENABLE_BREAK, MF_BYCOMMAND);

 if(state == (UINT)-1)
    return;  // impossible...

 pCmdUI->SetCheck((state & MF_MENUBREAK));
        
}
