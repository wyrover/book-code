// menu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MenuExplorer.h"
#include "msglog.h"
#include "intedit.h"
#include "tinybutt.h"
#include "bitmaps.h"
#include "menuitem.h"
#include "menulist.h"
#include "is95.h"

#include "xform.h"

#include "menudlg.h"
#include "tpm.h"
#include "menusht.h"

#include "menuchk.h"
#include "MenuInfo.h"
#include "idcombo.h"
#include "gcombo.h"
#include "color.h"
#include "syscolor.h"
#include "od.h"
#include "odmenu.h"

#include "MainFrm.h"
#include "uwm.h"
#include "odinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerDlg property page


IMPLEMENT_DYNCREATE(CMenuExplorerDlg, CPropertyPage)

CMenuExplorerDlg::CMenuExplorerDlg() : CPropertyPage(CMenuExplorerDlg::IDD)
{
        currentPopup = NULL;  // no current popup
        initialized = FALSE;
        usermenuid = 0;
        //{{AFX_DATA_INIT(CMenuExplorerDlg)
        //}}AFX_DATA_INIT
}


/****************************************************************************
*                        CMenuExplorerDlg::getMainWindow
* Result: CWnd *
*       The handle to the "main" window where the "top-level" menu is found
* Notes: 
*       This lets us play with changing where the "top-level" menu is found
****************************************************************************/

CWnd * CMenuExplorerDlg::getMainWindow()
    {
     return AfxGetMainWnd();
    }

/****************************************************************************
*                       CMenuExplorerDlg::getTopLevelMenu
* Result: CMenu *
*       Top-level menu object
* Notes: 
*       This lets us play with changing where the "top-level" menu is found
****************************************************************************/

CMenu * CMenuExplorerDlg::getTopLevelMenu()
    {
     return getMainWindow()->GetMenu();     
    }

void CMenuExplorerDlg::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CMenuExplorerDlg)
        DDX_Control(pDX, IDC_OWNERDRAW, c_OwnerDraw);
        DDX_Control(pDX, IDC_AUTOZOOM, c_AutoZoom);
        DDX_Control(pDX, IDC_MF_CHECKED, c_Checked);
        DDX_Control(pDX, IDC_NEWID_CAPTION, c_c_NewID);
        DDX_Control(pDX, IDC_SPINNEWID, c_SpinNewID);
        DDX_Control(pDX, IDC_RESTORE, c_Restore);
        DDX_Control(pDX, IDC_MODIFYMENU, c_ModifyMenu);
        DDX_Control(pDX, IDC_NEWID, c_NewID);
        DDX_Control(pDX, IDC_SET_POSITION, c_SetPosition);
        DDX_Control(pDX, IDC_SET_COMMAND, c_SetCommand);
        DDX_Control(pDX, IDC_CURRENT_POSITION, c_CurrentPosition);
        DDX_Control(pDX, IDC_CURRENT_COMMAND, c_CurrentCommand);
        DDX_Control(pDX, IDC_CHECKDIMENSIONS, c_CheckDimensions);
        DDX_Control(pDX, IDC_LIMIT, c_Limit);
        DDX_Control(pDX, IDC_CURRENTHANDLE, c_MenuHandle);
        DDX_Control(pDX, IDC_MENULIST, c_MenuList);
        DDX_Control(pDX, IDC_EXTEND, c_Extend);
        DDX_Control(pDX, IDC_WRAP, c_Wrap);
        DDX_Control(pDX, IDC_CLEARCONTEXTMENU, c_ClearContextMenu);
        DDX_Control(pDX, IDC_CONTEXTHANDLE, c_ContextHandle);
        DDX_Control(pDX, IDC_SETASCONTEXTMENU, c_SetAsContextMenu);
        DDX_Control(pDX, IDC_REMOVEMENU, c_RemoveMenu);
        DDX_Control(pDX, IDC_WATCH, c_Watch);
        DDX_Control(pDX, IDC_DRAWMENUBAR, c_DrawMenuBar);
        DDX_Control(pDX, IDC_BITMAPS, c_Bitmaps);
        DDX_Control(pDX, IDC_POPUPHANDLE, c_PopupHandle);
        DDX_Control(pDX, IDC_GETSYSTEMMENU, c_GetSystemMenu);
        DDX_Control(pDX, IDC_GETMENUSTRING, c_GetMenuString);
        DDX_Control(pDX, IDC_GETMENUITEMID, c_GetMenuItemID);
        DDX_Control(pDX, IDC_GETMENUITEMCOUNT, c_GetMenuItemCount);
        DDX_Control(pDX, IDC_CREATEMENU, c_CreateMenu);
        DDX_Control(pDX, IDC_SYSMENU_OPTION, c_SysMenuOption);
        DDX_Control(pDX, IDC_SETASPOPUP, c_SetAsPopup);
        DDX_Control(pDX, IDC_SUBMENU, c_SubMenu);
        DDX_Control(pDX, IDC_SPINSUBMENU, c_SpinSubMenu);
        DDX_Control(pDX, IDC_MF_POPUP, c_MF_Popup);
        DDX_Control(pDX, IDC_GETSUBMENU, c_GetSubMenu);
        DDX_Control(pDX, IDC_GETMENUSTATE, c_GetMenuState);
        DDX_Control(pDX, IDC_GETMENU, c_GetMenu);
        DDX_Control(pDX, IDC_ENABLEMENUITEM, c_EnableMenuItem);
        DDX_Control(pDX, IDC_ENABLE, c_Enable);
        DDX_Control(pDX, IDC_CHECKMENUITEM, c_CheckMenuItem);
        DDX_Control(pDX, IDC_CHECK, c_Check);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_INSERTMENU, c_InsertMenu);
        DDX_Control(pDX, IDC_DESTROYMENU, c_DestroyMenu);
        DDX_Control(pDX, IDC_APPENDMENU, c_AppendMenu);
        DDX_Control(pDX, IDC_DELETEMENU, c_DeleteMenu);
        DDX_Control(pDX, IDC_POSITION, c_Position);
        DDX_Control(pDX, IDC_COMMAND, c_Command);
        DDX_Control(pDX, IDC_MENUTEXT, c_MenuText);
        DDX_Control(pDX, IDC_SPINPOSITION, c_SpinPosition);
        DDX_Control(pDX, IDC_SPINCOMMAND, c_SpinCommand);
        DDX_Control(pDX, IDC_MF_NOBREAK, c_NoBreak);
        DDX_Control(pDX, IDC_MF_STRING, c_MF_String);
        DDX_Control(pDX, IDC_MF_SEPARATOR, c_MF_Separator);
        DDX_Control(pDX, IDC_MF_OWNERDRAW, c_MF_OwnerDraw);
        DDX_Control(pDX, IDC_MF_MENUBREAK, c_MF_MenuBreak);
        DDX_Control(pDX, IDC_MF_MENUBARBREAK, c_MF_MenuBarBreak);
        DDX_Control(pDX, IDC_MF_GRAYED, c_MF_Grayed);
        DDX_Control(pDX, IDC_MF_ENABLED, c_MF_Enabled);
        DDX_Control(pDX, IDC_MF_DISABLED, c_MF_Disabled);
        DDX_Control(pDX, IDC_MF_BYPOSITION, c_MF_ByPosition);
        DDX_Control(pDX, IDC_MF_BYCOMMAND, c_MF_ByCommand);
        DDX_Control(pDX, IDC_MF_BITMAP, c_MF_Bitmap);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuExplorerDlg, CPropertyPage)
        //{{AFX_MSG_MAP(CMenuExplorerDlg)
        ON_CBN_SELENDOK(IDC_MENUTEXT, OnSelendokMenutext)
        ON_BN_CLICKED(IDC_MF_BITMAP, OnMfBitmap)
        ON_BN_CLICKED(IDC_MF_BYCOMMAND, OnMfBycommand)
        ON_BN_CLICKED(IDC_MF_BYPOSITION, OnMfByposition)
        ON_BN_CLICKED(IDC_MF_DISABLED, OnMfDisabled)
        ON_CBN_EDITCHANGE(IDC_MENUTEXT, OnEditchangeMenutext)
        ON_BN_CLICKED(IDC_MF_ENABLED, OnMfEnabled)
        ON_BN_CLICKED(IDC_MF_GRAYED, OnMfGrayed)
        ON_BN_CLICKED(IDC_MF_MENUBARBREAK, OnMfMenubarbreak)
        ON_BN_CLICKED(IDC_MF_MENUBREAK, OnMfMenubreak)
        ON_BN_CLICKED(IDC_MF_OWNERDRAW, OnMfOwnerdraw)
        ON_BN_CLICKED(IDC_MF_SEPARATOR, OnMfSeparator)
        ON_BN_CLICKED(IDC_MF_STRING, OnMfString)
        ON_BN_CLICKED(IDC_MF_NOBREAK, OnNobreak)
        ON_BN_CLICKED(IDC_APPENDMENU, OnAppendmenu)
        ON_BN_CLICKED(IDC_CHECK, OnCheck)
        ON_BN_CLICKED(IDC_CHECKMENUITEM, OnCheckmenuitem)
        ON_BN_CLICKED(IDC_DELETEMENU, OnDeletemenu)
        ON_BN_CLICKED(IDC_DESTROYMENU, OnDestroymenu)
        ON_BN_CLICKED(IDC_ENABLE, OnEnable)
        ON_BN_CLICKED(IDC_ENABLEMENUITEM, OnEnablemenuitem)
        ON_BN_CLICKED(IDC_GETMENU, OnGetmenu)
        ON_BN_CLICKED(IDC_GETMENUSTATE, OnGetmenustate)
        ON_BN_CLICKED(IDC_GETSUBMENU, OnGetsubmenu)
        ON_BN_CLICKED(IDC_INSERTMENU, OnInsertmenu)
        ON_BN_CLICKED(IDC_MF_POPUP, OnMfPopup)
        ON_BN_CLICKED(IDC_GETMENUITEMCOUNT, OnGetmenuitemcount)
        ON_BN_CLICKED(IDC_GETMENUITEMID, OnGetmenuitemid)
        ON_BN_CLICKED(IDC_GETMENUSTRING, OnGetmenustring)
        ON_BN_CLICKED(IDC_SETASPOPUP, OnSetaspopup)
        ON_BN_CLICKED(IDC_GETSYSTEMMENU, OnGetsystemmenu)
        ON_WM_CLOSE()
        ON_WM_DESTROY()
        ON_BN_CLICKED(IDC_CREATEMENU, OnCreatemenu)
        ON_BN_CLICKED(IDC_CREATEPOPUPMENU, OnCreatepopupmenu)
        ON_BN_CLICKED(IDC_DRAWMENUBAR, OnDrawmenubar)
        ON_MESSAGE(UWM_LOGMSG, OnLogMessage)
        ON_MESSAGE(UWM_COMMAND, OnMainCommand)
        ON_MESSAGE(UWM_INITMENU, OnMainInitMenu)
        ON_MESSAGE(UWM_INITMENUPOPUP, OnMainInitMenuPopup)
        ON_MESSAGE(UWM_MENUSELECT, OnMainMenuSelect)
        ON_MESSAGE(UWM_MENUCHAR, OnMainMenuChar)
        ON_MESSAGE(UWM_ENTERMENULOOP, OnMainEnterMenuLoop)
        ON_MESSAGE(UWM_EXITMENULOOP, OnMainExitMenuLoop)
        ON_MESSAGE(UWM_ADD_OWNERDRAW, OnAddOwnerDraw)
        ON_EN_CHANGE(IDC_COMMAND, OnChangeCommand)
        ON_BN_CLICKED(IDC_REMOVEMENU, OnRemovemenu)
        ON_BN_CLICKED(IDC_CLEARCONTEXTMENU, OnClearcontextmenu)
        ON_BN_CLICKED(IDC_SETASCONTEXTMENU, OnSetascontextmenu)
        ON_WM_CONTEXTMENU()
        ON_WM_RBUTTONDOWN()
        ON_BN_CLICKED(IDC_EXTEND, OnExtend)
        ON_BN_CLICKED(IDC_WRAP, OnWrap)
        ON_NOTIFY(TVN_SELCHANGED, IDC_MENULIST, OnSelchangedMenulist)
        ON_NOTIFY(TVN_DELETEITEM, IDC_MENULIST, OnDeleteitemMenulist)
        ON_BN_CLICKED(IDC_TESTTREEWALK, OnTesttreewalk)
        ON_BN_CLICKED(IDC_CHECKDIMENSIONS, OnCheckdimensions)
        ON_BN_CLICKED(IDC_SET_COMMAND, OnSetCommand)
        ON_BN_CLICKED(IDC_SET_POSITION, OnSetPosition)
        ON_BN_CLICKED(IDC_RESTORE, OnRestore)
        ON_BN_CLICKED(IDC_MODIFYMENU, OnModifymenu)
        ON_BN_CLICKED(IDC_MF_CHECKED, OnMfChecked)
        ON_BN_CLICKED(IDC_AUTOZOOM, OnAutozoom)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerDlg message handlers


/****************************************************************************
*                       CMenuExplorerDlg::getSelectedMenu
* Result: CMenu *
*       (Possibly temporary) menu object.  NULL if selection is not a menu
* Effect: 
*       May allocate a temporary menu object
****************************************************************************/

CMenu * CMenuExplorerDlg::getSelectedMenu()
    {
     menuitem * mi = getSelectedMenuItem();

     if(mi == NULL || mi->hmenu == NULL)
        return NULL;  // not a menu

     return CMenu::FromHandle(mi->hmenu); 
    }

/****************************************************************************
*                           CMenuExplorerDlg::getMenu
* Result: CMenu *
*       (Possibly temporary) menu object.  NULL if not a valid selection.
*       If the current selection is a command menu, returns its parent menu
* Effect: 
*       May allocate a temporary menu object
****************************************************************************/

CMenu * CMenuExplorerDlg::getMenu()
    {
     HTREEITEM ti = c_MenuList.GetSelectedItem();
     menuitem * mi = c_MenuList.GetItemData(ti);

     if(mi == NULL)
        return NULL;

     if((mi->flags & M_COMMAND) == 0)
        return CMenu::FromHandle(mi->hmenu);
     
     HTREEITEM parent = c_MenuList.GetParentItem(ti);
     menuitem * pi = c_MenuList.GetItemData(parent);

     if(pi == NULL || pi->hmenu == NULL)
        return NULL;

     return CMenu::FromHandle(pi->hmenu);
    }

/****************************************************************************
*                   CMenuExplorerDlg::getSelectedMenuPermanent
* Result: CMenu *
*       A permanent handle to a CMenu that holds the current selection
* Effect: 
*       Creates a new CMenu * object
****************************************************************************/

CMenu * CMenuExplorerDlg::getSelectedMenuPermanent()
    {
     menuitem * mi = getSelectedMenuItem();
     if(mi == NULL || mi->hmenu == NULL)
        return NULL;

     CMenu * menu = new CMenu;
     menu->Attach(mi->hmenu);
     return menu;
    }

/****************************************************************************
*                     CMenuExplorerDlg::getSelectedMenuItem
* Result: menuitem * mi
*       Selected menu item, or NULL if none selected
****************************************************************************/

menuitem * CMenuExplorerDlg::getSelectedMenuItem()
    {
     HTREEITEM ti = c_MenuList.GetSelectedItem();
     if(ti == NULL)
        return NULL;  // no selection

     return c_MenuList.GetItemData(ti);
    }

/****************************************************************************
*                          CMenuExplorerDlg::getEnabled
* Result: DWORD
*       MF_ENABLED or MF_GRAYED or MF_DISABLED
****************************************************************************/

DWORD CMenuExplorerDlg::getEnabled()
    {
     if(c_MF_Enabled.GetCheck())
        return MF_ENABLED;
     else
     if(c_MF_Grayed.GetCheck())
        return MF_GRAYED;
     else
     if(c_MF_Disabled.GetCheck())
        return MF_DISABLED;
     else
        return 0;  // should never happen
    }

/****************************************************************************
*                          CMenuExplorerDlg::getChecked
* Result: DWORD
*       MF_CHECKED | MF_UNCHECKED
****************************************************************************/

DWORD CMenuExplorerDlg::getChecked()
    {
     return (c_Checked.GetCheck() ? MF_CHECKED : MF_UNCHECKED);
    }

/****************************************************************************
*                           CMenuExplorerDlg::getBreak
* Result: DWORD
*       MF_MENUBREAK or MF_MENUBARBREAK or 0
****************************************************************************/

DWORD CMenuExplorerDlg::getBreak()
    {
     if(c_MF_MenuBarBreak.GetCheck())
        return MF_MENUBARBREAK;
     else
     if(c_MF_MenuBreak.GetCheck())
        return MF_MENUBREAK;
     else
        return 0; // None selected
    }

/****************************************************************************
*                           CMenuExplorerDlg::flagstr
* Inputs:
*       DWORD flags: Flag values to render as strings
* Result: CString
*       Flag string value
* Notes:
*       Because of how the bits are assigned (many values are 0) the table
*       used below encodes the 0-valued fields as the complement of the
*       bits which are NOT that value, and the 'mode' flag indicates whether
*       the test is for the presence of the bits or their absence
****************************************************************************/


        // 0 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0
        // | | | |   | | | |   | | | |   | | | |MF_GRAYED
        // | | | |   | | | |   | | | |   | | |MF_DISABLED
        // | | | |   | | | |   | | | |   | |MF_BITMAP
        // | | | |   | | | |   | | | |   |MF_CHECKED
        // | | | |   | | | |   | | | |MF_POPUP
        // | | | |   | | | |   | | |MF_MENUBARBREAK
        // | | | |   | | | |   | |MF_MENUBREAK
        // | | | |   | | | |   |MF_HILITE
        // | | | |   | | | |MF_OWNERDRAW
        // | | | |   | | |MF_USECHECKEDBITMAPS *
        // | | | |   | |MF_BYPOSITION
        // | | | |   |MF_OWNERDRAW
        // | | | |MF_DEFAULT
        // | | |MF_SYSMENU
        // | |MF_HELP
        // | |MF_RIGHTJUSTIFY    <= yes, it really is the same as MF_HELP!
        // |

static struct {BOOL mode; DWORD flag; int id; } flagstrings[] = {
        //----------------------------------------------------------------
        // selection
        //----------------------------------------------------------------
        { FALSE, MF_BYPOSITION,         IDS_MF_BYCOMMAND},      
        { TRUE,  MF_BYPOSITION,         IDS_MF_BYPOSITION},     // 0400

        //----------------------------------------------------------------
        // highlighting
        //----------------------------------------------------------------
        { TRUE, MF_HILITE,              IDS_MF_HILITE},         // 0080

        //----------------------------------------------------------------
        // default
        //----------------------------------------------------------------
        { TRUE, MF_DEFAULT,             IDS_MF_DEFAULT},        // 1000

        //----------------------------------------------------------------
        // display
        //----------------------------------------------------------------
        { FALSE, MF_BITMAP | MF_OWNERDRAW | MF_SEPARATOR | MF_POPUP, 
                                        IDS_MF_STRING},
        { TRUE,  MF_OWNERDRAW,          IDS_MF_OWNERDRAW},      // 0100
        { TRUE,  MF_SEPARATOR,          IDS_MF_SEPARATOR},      // 0800
        { TRUE,  MF_BITMAP,             IDS_MF_BITMAP},         // 0004
        { TRUE,  MF_POPUP,              IDS_MF_POPUP},          // 0010

        //----------------------------------------------------------------
        // break
        //----------------------------------------------------------------
        { TRUE,  MF_MENUBREAK,          IDS_MF_MENUBREAK},      // 0040
        { TRUE,  MF_MENUBARBREAK,       IDS_MF_MENUBARBREAK},   // 0020

        //----------------------------------------------------------------
        // enable
        //----------------------------------------------------------------
        { FALSE, MF_GRAYED | MF_DISABLED,
                                        IDS_MF_ENABLED},
        { TRUE,  MF_DISABLED,           IDS_MF_DISABLED},       // 0002
        { TRUE,  MF_GRAYED,             IDS_MF_GRAYED},         // 0001

        //----------------------------------------------------------------
        // checked
        //----------------------------------------------------------------
        { FALSE, MF_CHECKED,            IDS_MF_UNCHECKED},
        { TRUE,  MF_CHECKED,            IDS_MF_CHECKED},        // 0008

        //----------------------------------------------------------------
        // EOT
        //----------------------------------------------------------------
        { FALSE, 0,                     0} // EOT marker
                                              };

CString CMenuExplorerDlg::flagstr(DWORD flags, BOOL wantpos /* = TRUE */)
    {
     CString result;
     CString sep = _T("");

     for(int i = 0; flagstrings[i].id != 0; i++)
        { /* scan each */
         if(!wantpos && (flagstrings[i].id == IDS_MF_BYPOSITION ||
                         flagstrings[i].id == IDS_MF_BYCOMMAND))
            continue;  // skip position information

         if((flagstrings[i].mode && (flags & flagstrings[i].flag) != 0) ||
            (!flagstrings[i].mode && (flags & flagstrings[i].flag) == 0))
            { /* found it */
             CString t;
             t.LoadString(flagstrings[i].id);
             result += sep;
             result += t;
             sep = _T(" | ");
            } /* found it */
        } /* scan each */

     return result;
    }

void CMenuExplorerDlg::OnSelendokMenutext() 
{

}

void CMenuExplorerDlg::OnMfBitmap() 
{
 enableControls();
        
}

void CMenuExplorerDlg::OnMfBycommand() 
{
 enableControls();
        
}

void CMenuExplorerDlg::OnMfByposition() 
{
 enableControls();
        
}


void CMenuExplorerDlg::OnEditchangeMenutext() 
{
 enableControls();
}


void CMenuExplorerDlg::OnMfOwnerdraw() 
{
 enableControls();
}

void CMenuExplorerDlg::OnMfSeparator() 
{
 enableControls();
}

void CMenuExplorerDlg::OnMfString() 
{
 enableControls();
}


/****************************************************************************
*                          CMenuExplorerDlg::setupMenu
* Inputs:
*       menuparms * mp: Menu parameter block
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Sets up the menu parameter block output fields, based on the
*       menu parameter block input fields
****************************************************************************/

BOOL CMenuExplorerDlg::setupMenu(menuparms * mp)
    {
     mp->menu = getMenu();
     if(mp->menu == NULL)
        return FALSE; // should be impossible

     // <<op>>Menu(hMenu, flags, newitem, lpnewitem)

     mp->flags = getEnabled() | getCmdPos() | getBreak() | getChecked();
     mp->newitem = c_NewID.GetVal(); // assume for now not MF_POPUP

     // If we are in an InsertMenu, we'll need this value; if we are
     // in an AppendMenu, it won't be used.
     mp->pos = c_Position.GetVal();

     // We set modid to the id for ModifyMenu.  If we were called from
     // OnModifyMenu, the caller will copy this value to the pos field
     mp->modid = getMenuID();
     CString fmt;

     if(c_MF_Popup.GetCheck())
        mp->flags |= MF_POPUP;

     if(c_MF_String.GetCheck())
        { /* MF_STRING */
         mp->flags |= MF_STRING;
         c_MenuText.GetWindowText(mp->s);
         mp->s = escToStr(mp->s);  // process \t to tab, for example
         mp->lpnewitem = (LPVOID)(LPCSTR)mp->s;

         //                             mp->msg        | mp->id  |
         // -------------------------------------------+---------+--------
         // AppendMenu(%1!08x!,        %3!08x!=%4!%s!, | %d,     | => %s
         // InsertMenu(%1!08x!, %2!d!, %3!08x!=%4!%s!, | %d,     | => %s 
         // 
         fmt.LoadString(mp->opid);
         //                         %1                 %2       %3         %4
         mp->msg.FormatMessage(fmt, mp->menu->m_hMenu, mp->pos, mp->flags, flagstr(mp->flags));
         fmt.LoadString(IDS_MENU_STRING);
         mp->suffix.Format(fmt, mp->s);
        } /* MF_STRING */
     else
     if(c_MF_Bitmap.GetCheck())
        { /* MF_BITMAP */
         mp->flags |= MF_BITMAP;
         int i = c_Bitmaps.GetCurSel();
         if(i == CB_ERR)
            return FALSE; // this should never happen
         mp->lpnewitem = (LPVOID)(HBITMAP)c_Bitmaps.GetItemData(i);;

         //                             mp->msg        | mp->id  |
         // -------------------------------------------+---------+--------
         // AppendMenu(%1!08x!,        %3!08x!=%4!%s!, | %d,     | => %s
         // InsertMenu(%1!08x!, %2!d!, %3!08x!=%4!%s!, | %d,     | => %s 
         // 
         fmt.LoadString(mp->opid);
         //                         %1                 %2       %3         %4
         mp->msg.FormatMessage(fmt, mp->menu->m_hMenu, mp->pos, mp->flags, flagstr(mp->flags));
         mp->suffix.LoadString(IDS_MENU_BITMAP);

        } /* MF_BITMAP */
     else
     if(c_MF_OwnerDraw.GetCheck())
        { /* MF_OWNERDRAW */
         mp->flags |= MF_OWNERDRAW;
         
         int sel = c_OwnerDraw.GetCurSel();
         mp->lpnewitem = (LPVOID)c_OwnerDraw.GetItemData(sel);

         //                             mp->msg        | mp->id  |
         // -------------------------------------------+---------+--------
         // AppendMenu(%1!08x!,        %3!08x!=%4!%s!, | 0x%08x, | => %s
         // InsertMenu(%1!08x!, %2!d!, %3!08x!=%4!%s!, | 0x%08x, | => %s 
         // 
         fmt.LoadString(mp->opid);
         //                         %1                 %2       %3         %4
         mp->msg.FormatMessage(fmt, mp->menu->m_hMenu, mp->pos, mp->flags, flagstr(mp->flags)); 
         fmt.LoadString(IDS_MENU_OWNERDRAW);
         mp->suffix.Format(fmt, mp->lpnewitem);
        } /* MF_OWNERDRAW */
     else
     if(c_MF_Separator.GetCheck())
        { /* MF_SEPARATOR */
         mp->flags |= MF_SEPARATOR;
         mp->lpnewitem = NULL; // ignored for separator
         mp->newitem = 0;      // must be 0 for separator

         //                             mp->msg        | mp->id  |
         // -------------------------------------------+---------+--------
         // AppendMenu(%1!08x!,        %3!08x!=%4!%s!, | 0,      | => %s
         // InsertMenu(%1!08x!, %2!d!, %3!08x!=%4!%s!, | 0,      | => %s 
         // 
         fmt.LoadString(mp->opid);
         //                         %1                 %2       %3         %4
         mp->msg.FormatMessage(fmt, mp->menu->m_hMenu, mp->pos, mp->flags, flagstr(mp->flags));
         mp->suffix.LoadString(IDS_MENU_SEPARATOR);
        } /* MF_SEPARATOR */
     else
        return FALSE; // impossible condition, we hope, but don't proceed if true

     if(c_MF_Popup.GetCheck())
        { /* MF_POPUP */
         // If a popup menu, the "newitem" field is not an ID but a handle
         mp->newitem = c_PopupHandle.GetVal();  // menu id is actually handle
         mp->id.Format(_T("0x%08x, "), mp->newitem);
        } /* MF_POPUP */
     else
        { /* not MF_POPUP */
         mp->id.Format(_T("%d, "), mp->newitem);
        } /* not MF_POPUP */

     return TRUE;
    }

/****************************************************************************
*                          CMenuExplorerDlg::finishMenu
* Inputs:
*       menuparms * mp: Menuparms block, prepared by setupMenu, and with
*                       the 'result' field filled in
* Result: void
*       
* Effect: 
*       Logs the event for the AppendMenu or InsertMenu operation
****************************************************************************/

void CMenuExplorerDlg::finishMenu(menuparms * mp)
    {
     CString rstr;
     rstr.LoadString(mp->result ? IDS_TRUE : IDS_FALSE);

     mp->msg += mp->id;
     mp->msg += mp->suffix;
     mp->msg += rstr;

     CString ok;
     ok.LoadString(mp->result ? IDS_OK : IDS_ERROR);
     c_Messages.addMessage(mp->menu->m_hMenu, ok, mp->msg);
     if(!mp->result)
        logError(mp->lasterror);

     if(mp->result && c_MF_Popup.GetCheck())
        { /* remove availability */
         HTREEITEM ti = c_MenuList.findItem( (HMENU)mp->newitem);
         menuitem * mi = c_MenuList.GetItemData(ti);
         mi->flags &= ~(M_UNASSIGNED | M_NOPARENT);
         clearCurrentPopup();
         c_MenuList.SetItemData(ti, NULL); // protect against deletion
         c_MenuList.DeleteItem(ti);  // remove it from unassigned list
        } /* remove availability */

     reloadMenuItems(mp->menu);
     c_MenuList.InvalidateRect(NULL);  //handle CTreeCtrl refresh bug
     enableControls();

     // <<op>>Menu(%08x, %08x=%s, %d, "%s") => %s       MF_STRING
     // <<op>>Menu(%08x, %08x=%s, %d, hbitmap) => %s    MF_BITMAP
     // <<op>>Menu(%08x, %08x=%s, %d, 0x%08x) => %s     MF_OWNERDRAW
     // <<op>>Menu(%08x, %08x=%s, 0, 0) => %s           MF_SEPARATOR
     // <<op>>Menu(%08x, %08x=%s, 0x%08x, 0) => %s      MF_POPUP
     
     notifyChange();
    }


void CMenuExplorerDlg::OnAppendmenu() 
{
 menuparms mp;
 mp.opid = IDS_APPENDMENU_PREFIX;
 if(!setupMenu(&mp))
    return;

 mp.result = mp.menu->AppendMenu(mp.flags, mp.newitem, (LPCTSTR)mp.lpnewitem);
 mp.lasterror = GetLastError();  // in case there was an error

 finishMenu(&mp);
}

void CMenuExplorerDlg::OnInsertmenu() 
{
 menuparms mp;
 mp.opid = IDS_INSERTMENU_PREFIX;
 if(!setupMenu(&mp))
    return;

 mp.result = mp.menu->InsertMenu(mp.pos, mp.flags, mp.newitem, 
                                                (LPCTSTR)mp.lpnewitem);

 mp.lasterror = GetLastError();  // in case there was an error
 finishMenu(&mp);
}

/****************************************************************************
*                         CMenuExplorerDlg::OnModifyMenu
* Result: void
*       
* Effect: 
*       Executes the ModifyMenu function
****************************************************************************/

void CMenuExplorerDlg::OnModifymenu() 
{
 menuparms mp;
 mp.opid = IDS_MODIFYMENU_PREFIX;
 if(!setupMenu(&mp))
    return;
        
 mp.pos = mp.modid;     // copy menu id (additional oddity of how ModifyMenu
                        // works requires we keep two values, but to use a
                        // single formatting/printing routine we need to 
                        // do this hack)

 mp.result = mp.menu->ModifyMenu(mp.pos, mp.flags, mp.newitem,
                                                (LPCTSTR)mp.lpnewitem);
 mp.lasterror = GetLastError();
 finishMenu(&mp);
}

void CMenuExplorerDlg::OnCheckmenuitem() 
{
 selection sel = getSelection();
 if(sel.menu == NULL)
    return;     // should never happen

 BOOL check = c_Check.GetCheck();
 int flags = sel.flags | (check ? MF_CHECKED : MF_UNCHECKED);

 UINT result = sel.menu->CheckMenuItem(sel.id, flags);
 
 CString fmt;
 fmt.LoadString(IDS_CHECKMENUITEM); // CheckMenuItem(0x%08x, %d, 0x%04x=%s) =>

 CString chkstr;
 chkstr.LoadString(check ? IDS_MF_CHECKED : IDS_MF_UNCHECKED);
 chkstr += _T(" | ");
 chkstr += getCmdPosStr(sel.flags);
 CString s;
 s.Format(fmt, sel.menu, sel.id, flags, chkstr);

 CString ok;
 DWORD lasterror = 0;
 if(result == (UINT)-1)
    { /* failed */
     lasterror = GetLastError();

     s += _T("-1");
     ok.LoadString(IDS_ERROR);
    } /* failed */
 else
    { /* success */
     chkstr.LoadString(result == MF_CHECKED ? IDS_MF_CHECKED : IDS_MF_UNCHECKED);
     s += chkstr;
     ok.LoadString(IDS_OK);
    } /* success */
 c_Messages.addMessage(sel.menu->m_hMenu, ok, s);
 if(lasterror != 0)
    logError(lasterror);
}

void CMenuExplorerDlg::OnDeletemenu() 
{
 selection sel = getSelection(SEL_POSITION);
 if(sel.menu == NULL)
    return; // should never happen

 // If it is a submenu, we can only delete by position in API3; 
 // save it for later so we can reassign it
 CMenu * sub = NULL;

 sub = sel.menu->GetSubMenu(sel.id);  // no guarantee it is submenu, may return NULL

 BOOL result = sel.menu->DeleteMenu(sel.id, sel.flags);
 DWORD lasterror = GetLastError();

 if(sub != NULL && result)
    { /* we deleted a submenu */
     HTREEITEM ti = c_MenuList.findItem(sub->m_hMenu);
     // Because this was a submenu, it has now been deleted from the
     // system and destroyed.

     c_MenuList.DeleteItem(ti);  // remove all traces from table
    } /* we deleted a submenu */

 // DeleteMenu(0x%08x, %d, %s) => %s
 CString t;
 t.LoadString(IDS_DELETEMENU);

 CString rstr;
 rstr.LoadString(result ? IDS_TRUE : IDS_FALSE);

 CString s;
 s.Format(t, sel.menu->m_hMenu, sel.id, getCmdPosStr(sel.flags), rstr);

 CString ok;
 ok.LoadString(result ? IDS_OK : IDS_ERROR);
 c_Messages.addMessage(sel.menu->m_hMenu, ok, s);
 if(!result)
    logError(lasterror);

 // Now update the menu from which we deleted it
 notifyChange();
 reloadMenuItems(sel.menu);
}

void CMenuExplorerDlg::OnDestroymenu() 
{
        menuitem * mi = getSelectedMenuItem();
        if(mi == NULL)
           return;
        
        if(!(mi->flags & M_UNASSIGNED))
           return;  // cannot destroy menu contained in another menu
        
        CMenu * menu = CMenu::FromHandle(mi->hmenu);
        BOOL result = menu->DestroyMenu();
        DWORD lasterror = GetLastError();

        HTREEITEM ti = c_MenuList.findItem(mi->hmenu);

        c_MenuList.DeleteItem(ti);

        // DestroyMenu(0x%08x) => %s
        CString t;
        t.LoadString(IDS_DESTROYMENU);

        CString rstr;
        rstr.LoadString(result ? IDS_TRUE : IDS_FALSE);

        CString s;
        s.Format(t, mi->hmenu, rstr);

        CString ok;
        ok.LoadString(result ? IDS_OK : IDS_ERROR);
        c_Messages.addMessage(menu->m_hMenu, ok, s);
        if(!result)
           logError(lasterror);
}

void CMenuExplorerDlg::OnEnablemenuitem() 
{
 selection sel = getSelection();
 if(sel.menu == NULL)
    return;     // should never happen

 // The enable checkbox is 3-state, indicating disabled, enabled, grayed
 BOOL enable = c_Enable.GetCheck();
 int flags = sel.flags | (enable == 0 ? MF_DISABLED : 
                             enable == 1 ? MF_ENABLED : MF_GRAYED);

 UINT result = sel.menu->EnableMenuItem(sel.id, flags );
 DWORD lasterror = GetLastError();
 
 CString fmt;
 fmt.LoadString(IDS_ENABLEMENUITEM); // EnableMenuItem(0x%08x, %d, 0x%04x=%s) =>

 CString chkstr;
 chkstr.LoadString(enable == 0 ? IDS_MF_DISABLED : 
                        enable == 1 ? IDS_MF_ENABLED : IDS_MF_GRAYED);
 chkstr += _T(" | ");
 chkstr += getCmdPosStr(sel.flags);
 CString s;
 s.Format(fmt, sel.menu->m_hMenu, sel.id, flags, chkstr);

 CString ok;
 if(result == (UINT)-1)
    { /* failed */
     s += _T("-1");
     ok.LoadString(IDS_ERROR);
    } /* failed */
 else
    { /* success */
     chkstr.LoadString(result == MF_ENABLED ? IDS_MF_ENABLED : 
                        result == MF_DISABLED ? IDS_MF_DISABLED :
                                        IDS_MF_GRAYED);
     s += chkstr;
     ok.LoadString(IDS_OK);
    } /* success */
 c_Messages.addMessage(sel.menu->m_hMenu, ok, s);
 if(!result)
    logError(lasterror);
}

void CMenuExplorerDlg::OnGetmenu() 
{
 c_MenuList.Select(tree_main, TVGN_CARET);
 
 menuitem * mi = c_MenuList.GetItemData(tree_main);

 CString t;
 t.LoadString(IDS_GETMENU);

 CString s;
 s.Format(t, mi != NULL ? mi->hmenu : NULL, 
                mi != NULL ? mi->name : _T(""));

 CString ok;
 ok.LoadString(mi != NULL ? IDS_OK : IDS_ERROR);
 c_Messages.addMessage(mi != NULL ? mi->hmenu : NULL, ok, s);

 enableControls();
}

void CMenuExplorerDlg::OnGetmenustate() 
{
 selection sel = getSelection();
 if(sel.menu == NULL)
    return;  // should not happen
        
 CString str;
 UINT state = sel.menu->GetMenuState(sel.id, sel.flags);
 DWORD lasterror = GetLastError();

 // GetMenuState(0x%08x, %d, %s) => %d %s
 CString fmt;

 fmt.LoadString(IDS_GETMENUSTATE);
 
 CString s;
 s.Format(fmt, sel.menu->m_hMenu, sel.id, getCmdPosStr(sel.flags));

 if(state == (UINT)-1)
     s += _T("-1");
 else
    { /* expand */
     CString hex;
     hex.Format(_T("0x%04x=("), state);
     s += hex;

     if(state & MF_POPUP)
        { /* popup */
         s += flagstr(LOBYTE(state), FALSE);
         hex.Format(_T(", %d)"), HIBYTE(state));
         s += hex;
        } /* popup */
     else
        { /* command */
         s += flagstr(state, FALSE);
         s += _T(")");
        } /* command */
    } /* expand */

 CString ok;
 ok.LoadString(state == (UINT)-1 ? IDS_ERROR : IDS_OK);
 c_Messages.addMessage(sel.menu->m_hMenu, ok, s);
 
 if(state == (UINT)-1)
    logError(lasterror);
}

void CMenuExplorerDlg::OnGetsubmenu() 
{
 CMenu * menu = getSelectedMenu();
 if(menu == NULL)
    return;

 CMenu * sub = menu->GetSubMenu(c_SubMenu.GetVal());
 DWORD lasterror = GetLastError();

 if(sub != NULL && sub->m_hMenu != NULL)
    setSelectedMenu(sub->m_hMenu);

 CString t;
 CString s;
 CString ok;
 if(sub != NULL && sub->m_hMenu != NULL)
    { /* show sub */
     menuitem * mi = c_MenuList.findMenuItem(sub->m_hMenu);
     t.LoadString(IDS_GETSUBMENU);
     s.Format(t, menu->m_hMenu, c_SubMenu.GetVal(), sub->m_hMenu, mi->name);
     ok.LoadString(IDS_OK);
    } /* show sub */
 else
    { /* show NULL */
     t.LoadString(IDS_GETSUBMENU_NULL);
     s.Format(t, menu->m_hMenu, c_SubMenu.GetVal());
     ok.LoadString(IDS_ERROR);
    } /* show NULL */
 
 c_Messages.addMessage(menu->m_hMenu, ok, s);

 if(sub == NULL || sub->m_hMenu == NULL)
    logError(lasterror);
}

void CMenuExplorerDlg::OnMfPopup() 
{
 enableControls();
}

void CMenuExplorerDlg::OnGetmenuitemcount() 
{
 CMenu * menu = getSelectedMenu();
 if(menu == NULL)
    return;

 int c = menu->GetMenuItemCount();
 DWORD lasterror = GetLastError();

 CString t;
 t.LoadString(IDS_GETMENUITEMCOUNT);
 
 CString s;
 s.Format(t, menu->m_hMenu, c);
 CString ok;
 ok.LoadString(c >= 0 ? IDS_OK : IDS_ERROR);
 c_Messages.addMessage(menu->m_hMenu, ok, s);
 if(c < 0)
    logError(lasterror);
}

void CMenuExplorerDlg::OnGetmenuitemid() 
{
 selection sel = getSelection(SEL_POSITION);
 if(sel.menu == NULL)
    return;

 int id = sel.menu->GetMenuItemID(sel.id); // ID field will be positional
 DWORD lasterror = GetLastError();

 CString t;
 t.LoadString(IDS_GETMENUITEMID);

 CString s;
 s.Format(t, sel.menu->m_hMenu, sel.id, id);

 CString ok;
 ok.LoadString(id == -1 ? IDS_ERROR : IDS_OK);
 c_Messages.addMessage(sel.menu->m_hMenu, ok, s);

 if(id == -1)
    logError(lasterror);
}


/****************************************************************************
*                          CMenuExplorerDlg::getMenuID
* Result: int
*       if MF_BYPOSITION, returns the value of the Position variable
*       if MY_BYCOMMAND,  returns the value of the Command variable
****************************************************************************/

int CMenuExplorerDlg::getMenuID()
    {
     if(c_MF_ByCommand.GetCheck())
        { /* by command */
         return c_NewID.GetVal();
        } /* by command */
     else
        { /* by position */
         return c_Position.GetVal();
        } /* by position */
    }


/****************************************************************************
*                          CMenuExplorerDlg::getCmdPos
* Result: DWORD
*       MF_BYPOSITION or MF_BYCOMMAND, depending on user setting
****************************************************************************/

DWORD CMenuExplorerDlg::getCmdPos()
    {
     if(c_MF_ByCommand.GetCheck())
        return MF_BYCOMMAND;
     else
        return MF_BYPOSITION;
    }

/****************************************************************************
*                       CMenuExplorerDlg::setSelectedMenu
* Inputs:
*       HMENU hmenu: Menu to select, or NULL to deselect the menu
* Result: void
*       
* Effect: 
*       If hmenu is in the display, changes the attention to that
*       menu
****************************************************************************/

void CMenuExplorerDlg::setSelectedMenu(HMENU hmenu)
    {
     HTREEITEM t = c_MenuList.findItem(hmenu);
     c_MenuList.Select(t, TVGN_CARET);
    }

/****************************************************************************
*                         CMenuExplorerDlg::getCmdPosStr
* Inputs:
*       DWORD flags: 
* Result: CString
*       MF_BYPOSITION or MF_BYCOMMAND
****************************************************************************/

CString CMenuExplorerDlg::getCmdPosStr(DWORD flags)
    {
     CString s;

     if(flags & MF_BYPOSITION)
        s.LoadString(IDS_MF_BYPOSITION);
     else
        s.LoadString(IDS_MF_BYCOMMAND);

     return s;
    }

/****************************************************************************
*                         CMenuExplorerDlg::getCmdPosStr
* Result: CString
*       MF_BYPOSITION or MF_BYCOMMAND, depending on user setting
****************************************************************************/

CString CMenuExplorerDlg::getCmdPosStr()
    {
     CString s;

     if(c_MF_ByCommand.GetCheck())
        s.LoadString(IDS_MF_BYCOMMAND);
     else
        s.LoadString(IDS_MF_BYPOSITION);

     return s;
    }

/****************************************************************************
*                         CMenuExplorerDlg::getSelection
* Inputs:
*       int type: SEL_EITHER, SEL_POSITION or SEL_COMMAND
* Result: selection
*       
* Effect: 
*       Fills in a selection object for the current selection.  This may
*       involve reverse-engineering its position.  The result can be used
*       as the menu, id-or-position, and flags (ORed with whatever else is
*       needed) for an operation on the selected object
*       The menu field is NULL if there is an error.
****************************************************************************/

selection CMenuExplorerDlg::getSelection(int type)
    {
     selection sel = { NULL, 0, 0};
     HTREEITEM ti = c_MenuList.GetSelectedItem();
     if(ti == NULL)
        return sel;

     menuitem * mi = c_MenuList.GetItemData(ti);

     if(mi == NULL)
        return sel;

     // If we have a selection, return the handle of its parent menu,
     // and either MF_BYPOSITION and an index or MF_BYCOMMAND and a command
     // id

     if(mi->flags & M_NOPARENT)
        return sel;

     HTREEITEM parent = c_MenuList.GetParentItem(ti);
     menuitem * pi = c_MenuList.GetItemData(parent);

     sel.menu = CMenu::FromHandle(pi->hmenu);

     // Note: We currently implement only SEL_POSITION so there is no test
     // We may add SEL_COMMAND and hence SEL_EITHER at a later date
     sel.id = c_MenuList.getItemPos(ti);
     sel.flags = MF_BYPOSITION;

     if(type == SEL_EITHER || type == SEL_COMMAND)
        { /* favor by-command */
         int id = sel.menu->GetMenuItemID(sel.id);
         if(id != -1)
            { /* by-command possible */
             sel.flags = MF_BYCOMMAND;
             sel.id = id;
            } /* by-command possible */
        } /* favor by-command */
             
     return sel;
    }

void CMenuExplorerDlg::OnGetmenustring() 
{
 selection sel = getSelection();
 if(sel.menu == NULL)
    return;
        
 CString str;
 sel.menu->GetMenuString(sel.id, str, sel.flags);

 str = strToEsc(str);  // convert to \t, for example

 CString t;
 t.LoadString(IDS_GETMENUSTRING);
 
 CString s;
 s.Format(t, sel.menu->m_hMenu, sel.id, getCmdPosStr(sel.flags), str);

 CString ok;
 ok.LoadString(IDS_OK);
 c_Messages.addMessage(sel.menu->m_hMenu, ok, s);
}

void CMenuExplorerDlg::OnSetaspopup() 
{
 if(currentPopup != NULL)
    { /* delete it */
     clearCurrentPopup();
    } /* delete it */

 currentPopup = getSelectedMenuPermanent();
 c_PopupHandle.SetVal((int)currentPopup->m_hMenu, _T("0x%08x"));
 c_MF_Popup.SetCheck(TRUE);
 enableControls();
}

/****************************************************************************
*                     CMenuExplorerDlg::addSysMenuToDisplay
* Inputs:
*       HMENU hmenu: menu handle of system menu to add
* Result: HTREEITEM
*       Position at which menu was inserted into the display
* Effect: 
*       Adds the system menu to the display
****************************************************************************/

HTREEITEM CMenuExplorerDlg::addSysMenuToDisplay(HMENU hmenu)
    {
     menuitem * mi = c_MenuList.GetItemData(tree_system);
     mi->hmenu = hmenu;
     mi->flags &= ~M_EMPTY;
     mi->flags |= M_SYSMENU | M_NODELETE | M_NOPARENT;

     c_MenuList.insertMenuItems(CMenu::FromHandle(hmenu), tree_system);

     return tree_system;
    }

/****************************************************************************
*                  CMenuExplorerDlg::deleteSysMenuFromDisplay
* Result: void
*       
* Effect: 
*       Searches for a system menu object and if found, deletes it
****************************************************************************/

void CMenuExplorerDlg::deleteSysMenuFromDisplay()
    {
     menuitem * mi = c_MenuList.GetItemData(tree_system);
     mi->hmenu = NULL;
     mi->flags &= ~M_SYSMENU;
     mi->flags |= M_EMPTY;
     c_MenuList.deleteChildren(tree_system);
    }

void CMenuExplorerDlg::OnGetsystemmenu() 
{
 CWaitCursor cursor;
 BOOL revert = c_SysMenuOption.GetCheck();

 CMenu * tmenu = getMainWindow()->GetSystemMenu(revert);

 // If revert is true, the menu has been reset.  The result is undefined
 if(revert)
    { /* reverted */
     CString ok;
     ok.LoadString(IDS_OK);
     deleteSysMenuFromDisplay();
     CString s;
     s.LoadString(IDS_GETSYSMENUREVERT);
     c_Messages.addMessage(0, ok, s);
     c_MenuHandle.SetWindowText(_T(""));     
    } /* reverted */
 else
    { /* new sys menu */
     // If we were not reverting, the value is meaningful and is the
     // menu object representing the newly-created system menu

     // Given we have a new system menu, we need to see if there was one
     // there already; if so, we delete it and replace it with the
     // new menu item

     CString ok;
     ok.LoadString(IDS_OK);
     deleteSysMenuFromDisplay();
     addSysMenuToDisplay(tmenu->m_hMenu);
     setSelectedMenu(tmenu->m_hMenu);
     CString t;
     t.LoadString(IDS_GETSYSMENU);
     CString s;
     s.Format(t, tmenu->m_hMenu);
     c_Messages.addMessage(tmenu->m_hMenu, ok, s);
    } /* new sys menu */
 enableControls();
}

/****************************************************************************
*                       CMenuExplorerDlg::computeMenuName
* Inputs:
*       UINT flags: Flag values which dominate the name value
*       CString * name: Name to store, if flag hasn't already done it
*       int id: ID value, either menu command id or, for user-defined menus, 
*               the unique ID we assign to it
* Result: CString
*       The print name
* Effect: 
*       Computes a name for the menu.  This is later assigned
****************************************************************************/

CString CMenuExplorerDlg::computeMenuName(UINT flags, CString * name, int id)
    {
     CString type;
     if(flags & M_SYSMENU)
        type.LoadString(IDS_SYSMENU);
     else
     if(flags & M_MAIN)
        type.LoadString(IDS_MAIN);
     else
     if(flags & M_UNASSIGNED)
        { /* uassigned */
         CString fmt;
         fmt.LoadString(IDS_UNASSIGNED);
         type.Format(fmt, id);
        } /* uassigned */
     else
     if(flags & M_CONTEXT)
        type.LoadString(IDS_TRACKING);
     else
     if(name != NULL)
        type = *name;
     return type;
    }


/****************************************************************************
*                      CMenuExplorerDlg::addMenuToNode
* Inputs:
*       HMENU hmenu: Menu handle to add, NULL for command menu items
*       DWORD flags: Flags to set in itemdata field
*       CString * name: Name of menu, NULL permitted if no name
*       int id: Item ID, 0 for API3 non-command-items
*       int uid: User-defined item ID, 0 for non-user-defined menus
*       HTREEITEM node: Node to which to attach it
* Result: int
*       Location where it is inserted
* Effect: 
*       Inserts the menu item into the display list
****************************************************************************/

HTREEITEM CMenuExplorerDlg::addMenuToNode(HMENU hmenu, DWORD flags, 
                             CString * name, int id, int uid, HTREEITEM node)
    {
     CString t = computeMenuName(flags, name, uid);
     menuitem * item = new menuitem;
     if(item == NULL)
        return NULL; // error, should never happen
     item->flags = flags;
     item->hmenu = hmenu;
     item->name = t;
     item->id = id;
     item->uid   = uid;
     
     return c_MenuList.InsertItem(item, node);
    }

/****************************************************************************
*                       CMenuExplorerDlg::loadMenuDisplay
* Result: void
*       
* Effect: 
*       Loads the menu tree with all the existing menus
****************************************************************************/

void CMenuExplorerDlg::loadMenuDisplay()
    {
     CString s; 

     s.LoadString(IDS_MENUS);
     tree_root = c_MenuList.InsertItem(s, NULL);

     menuitem * mi = new menuitem;
     mi->flags = M_MAIN | M_NODELETE | M_NOPARENT;
     mi->hmenu = getTopLevelMenu()->m_hMenu;
     mi->name.LoadString(IDS_MAIN_MENU);
     
     tree_main = c_MenuList.InsertItem(mi, tree_root);
     c_MenuList.insertMenuItems(getTopLevelMenu(), tree_main);

     s.LoadString(IDS_UNASSIGNED_MENUS);
     tree_unassigned = c_MenuList.InsertItem(s, tree_root);

     mi = new menuitem;
     mi->flags = M_EMPTY | M_NOPARENT;
     mi->hmenu = NULL;
     mi->name.LoadString(IDS_SYSTEM_MENU);
     
     tree_system = c_MenuList.InsertItem(mi, tree_root);

     mi = new menuitem;
     mi->flags = M_EMPTY | M_NOPARENT;
     mi->hmenu = NULL;
     mi->name.LoadString(IDS_CONTEXT_MENU);

     tree_context = c_MenuList.InsertItem(mi, tree_root);
     
// The code below was needed only when we called this at arbitrary times
// Now that it is only called from OnInitDialog this is irrelevant
//     HMENU track = getTrackMenu();
//     if(track != NULL)
//        { /* has track */
//       CMenu * m = CMenu::FromHandle(track);
//       c_MenuList.insertMenuItems(m, tree_context);
//      } /* has track */

     c_MenuList.Expand(tree_root, TVE_EXPAND);
    }



/****************************************************************************
*                         CMenuExplorerDlg::getTrackMenu
* Result: HMENU
*       Menu handle of TrackPopupMenu menu, or NULL
****************************************************************************/

HMENU CMenuExplorerDlg::getTrackMenu()
    {
     return ((CMenuSheet *)GetParent())->trackMenu;
    }

/****************************************************************************
*                         CMenuExplorerDlg::setTrackMenu
* Inputs:
*       HMENU menu: Menu to set as tracking menu, or NULL
* Result: void
*       
* Effect: 
*       Replaces the current tracking menu
****************************************************************************/

void CMenuExplorerDlg::setTrackMenu(HMENU menu)
    {
     ((CMenuSheet *)GetParent())->trackMenu = menu;     
    }

/****************************************************************************
*                        CMenuExplorerDlg::enableControls
* Result: void
*       
* Effect: 
*       Enables or disables the controls based on the selection state
****************************************************************************/

void CMenuExplorerDlg::enableControls()
    {
     if(!initialized)
        return;  // avoid autobuddy startup problems

     c_MF_Popup.EnableWindow(c_PopupHandle.GetVal() != 0);
     c_MF_Bitmap.EnableWindow(c_Bitmaps.GetCurSel() != CB_ERR);

     c_ClearContextMenu.EnableWindow(getTrackMenu() != NULL);

     c_Command.EnableWindow(c_MF_ByCommand.GetCheck() && c_MenuHandle.GetVal() != 0);
     c_Position.EnableWindow(c_MenuHandle.GetVal() != 0);

     BOOL sel = FALSE;
     BOOL ismenu = FALSE;
     {
      HTREEITEM ti = c_MenuList.GetSelectedItem();
      menuitem * mi = c_MenuList.GetItemData(ti);

      if(ti != NULL && mi != NULL)
         { /* might be valid */
          if((mi->flags & M_EMPTY) == 0)
             { /* has content */
              sel = TRUE;
              if((mi->flags & M_COMMAND) == 0)
                 ismenu = TRUE;
             } /* has content */
         } /* might be valid */
     }

     c_GetMenuItemID.EnableWindow(sel);

     c_GetMenuString.EnableWindow(sel);
     c_GetMenuItemCount.EnableWindow(ismenu);

     c_CheckMenuItem.EnableWindow(sel);
     c_EnableMenuItem.EnableWindow(sel);
     c_GetMenuItemID.EnableWindow(sel);
     c_GetMenuState.EnableWindow(sel);
     c_GetSubMenu.EnableWindow(ismenu);


     menuitem * mi = NULL;
     CString text;
     c_MenuText.GetWindowText(text);
     text.TrimLeft();
     BOOL textavailable = c_MF_String.GetCheck() ? text.GetLength() > 0 : TRUE;

     BOOL hasid = c_MF_ByCommand.GetCheck() ? c_Command.GetVal() != 0 
                                : c_MF_ByPosition.GetCheck();

     if(sel)
        mi = getSelectedMenuItem();
     if(mi != NULL)
        sel = sel && !(mi->flags & M_EMPTY);

     BOOL same = sel && c_MF_Popup.GetCheck() && 
                        currentPopup->m_hMenu == getSelectedMenu()->m_hMenu;
     BOOL enable0 = sel &&  mi != NULL;

     BOOL enable1 = enable0/* && hasid */;
     BOOL enable2 = enable1 && textavailable;

     ismenu = mi != NULL && (mi->flags & M_COMMAND) == 0;

     /*

                       |...........enable2........|
                       |......enable1......|      |
                       |..enable0..|       |      |
                       |           |       |      |
                       | selection | hasid | name | ism |flags
     ------------------+-----------+-------+------+-----+-------------------
     SetAsPopup        |     *     |   --  |  --  |  *  | UNASSIGNED
     ------------------+-----------+-------+------+-----+-------------------
     SetAsContextMenu  |     *     |   --  |  --  |  *  | UNASSIGNED
     ------------------+-----------+-------+------+-----+-------------------
     AppendMenu        |     *     |   *   |  *   |  *  | ~READONLY
     ------------------+-----------+-------+------+-----+-------------------
     InsertMenu        |     *     |   *   |  *   |  *  | ~READONLY
     ------------------+-----------+-------+------+-----+-------------------
     ModifyMenu        |     *     |   *   |  *   |  *  | ~READONLY
     ------------------+-----------+-------+------+-----+-------------------
     RemoveMenu        |     *     |   *   |  --  | --  | ~UNASSIGNED
     ------------------+-----------+-------+------+-----+-------------------
     DeleteMenu        |     *     |   1   |  --  | --  |~(NODELETE | NOREMOVE)
     ------------------+-----------+-------+------+-----+-------------------
     DestroyMenu       |     *     |   1   |  --  |  *  |~(NODELETE | NOREMOVE)
     ------------------+-----------+-------+------+-----+-------------------
     name is either a string name (MF_STRING) or a bitmap selection  
     note 1: deletion of menu can be done to any unassigned menu, an
             id is not needed for an unassigned deletion.
     */
     c_SetAsPopup.EnableWindow(enable0 && ismenu && 
                                        (mi->flags & M_UNASSIGNED));
     c_SetAsContextMenu.EnableWindow(enable0 && ismenu && 
                                        (mi->flags & M_UNASSIGNED));
     c_AppendMenu.EnableWindow(enable2 && !same && ismenu &&
                                        !(mi->flags & M_READONLY));
     c_InsertMenu.EnableWindow(enable2 &&  !same && ismenu && 
                                        !(mi->flags & M_READONLY));
     c_ModifyMenu.EnableWindow(enable2 && !same && 
                                        !(mi->flags & M_READONLY));

     c_RemoveMenu.EnableWindow(sel && mi != NULL && 
                              !(mi->flags & M_UNASSIGNED) &&
                              !(mi->flags & M_NOPARENT));

     c_DeleteMenu.EnableWindow(enable1 && 
                   !(mi->flags & M_NODELETE) &&
                   !(mi->flags & M_NOREMOVE) &&
                   !(mi->flags & M_MAIN));
     c_DestroyMenu.EnableWindow(sel && ismenu &&
                                (mi->flags & M_UNASSIGNED));

     c_SetCommand.EnableWindow(c_CurrentCommand.GetWindowTextLength() > 0);
     c_SetPosition.EnableWindow(c_CurrentPosition.GetWindowTextLength() > 0);

     c_MenuText.EnableWindow(c_MF_String.GetCheck());
     c_Bitmaps.EnableWindow(c_MF_Bitmap.GetCheck());
     c_OwnerDraw.EnableWindow(c_MF_OwnerDraw.GetCheck());

     c_SpinNewID.EnableWindow(!c_MF_Separator.GetCheck());
     c_NewID.EnableWindow(!c_MF_Separator.GetCheck());
     c_c_NewID.EnableWindow(!c_MF_Separator.GetCheck());
    }

/****************************************************************************
*                        CMenuExplorerDlg::findBitmapName
* Inputs:
*       HBITMAP bm: Bitmap handle
* Result: CString
*       Bitmap name
****************************************************************************/

CString CMenuExplorerDlg::findBitmapName(HBITMAP bm)
    {
     CString s;
     for(int i = 0; i < c_Bitmaps.GetCount(); i++)
        { /* scan each */
         HBITMAP b = (HBITMAP)c_Bitmaps.GetItemData(i);
         if(b == bm)
            { /* found it */
             c_Bitmaps.GetLBText(i, s);
             return s;
            } /* found it */
        } /* scan each */
     return CString(_T("?"));
    }

/****************************************************************************
*                    CMenuExplorerDlg::reloadMenuItems
* Inputs:
*       CMenu * menu: Menu whose components are to be reloaded
* Result: void
*       
* Effect: 
*       Reloads the menu display, and tries to reselect the same
*       entry that was currently selected
****************************************************************************/

void CMenuExplorerDlg::reloadMenuItems(CMenu * menu)
    {
     HTREEITEM ti = c_MenuList.findItem(menu->m_hMenu);
     if(ti == NULL)
        return;

     c_MenuList.deleteChildren(ti);
     c_MenuList.insertMenuItems(menu, ti);
    }

BOOL CMenuExplorerDlg::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        CheckRadioButton(IDC_MF_BYCOMMAND, IDC_MF_BYPOSITION, IDC_MF_BYCOMMAND);
        CheckRadioButton(IDC_MF_NOBREAK, IDC_MF_MENUBARBREAK, IDC_MF_NOBREAK);
        CheckRadioButton(IDC_MF_STRING, IDC_MF_SEPARATOR, IDC_MF_STRING);
        CheckRadioButton(IDC_MF_ENABLED, IDC_MF_GRAYED, IDC_MF_ENABLED);

        c_SpinSubMenu.SetRange(0, 100);
        c_SpinPosition.SetRange(-1, 50);
        c_SpinCommand.SetRange(0, 50);
        c_SpinNewID.SetRange(0, min(UD_MAXVAL, 65535));

        CString t;
        int w;
        t.LoadString(IDS_OK);
        w = c_Messages.GetStringWidth(t);
        t.LoadString(IDS_ERROR);
        w = max(w, c_Messages.GetStringWidth(t));
        c_Messages.initialize(w);

        c_Watch.SetCheck(TRUE);

        c_Bitmaps.load();

        // Default to wrap mode on output.  Currently the controls are
        // invisible as they were only there for testing purposes.
        c_Wrap.SetCheck(TRUE);
        OnWrap();

        loadMenuDisplay();
        
        initialized = TRUE;
        enableControls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CMenuExplorerDlg::OnClose() 
{
 // do not call CPropertyPage::OnClose();

 // Because of the complexity of trying to track all the various changes
 // that might take place on each restart of the modeless dialog, we do
 // not actually allow the dialog to close.  Instead, we "fake" it by
 // hiding the dialog.  

 ShowWindow(SW_HIDE);
}

void CMenuExplorerDlg::OnDestroy() 
{
        CPropertyPage::OnDestroy();
        
        // TODO: Add your message handler code here
        
}

/****************************************************************************
*                        CMenuExplorerDlg::doCreateMenu
* Inputs:
*       int strid: String id of string to use for formatting
*       int type: lcm_menu or lcm_popupmenu
* Result: HTREEITEM
*       
* Effect: 
*       Logs the creation of the menu
****************************************************************************/

HTREEITEM CMenuExplorerDlg::doCreateMenu(int strid, int type)
    {
     CMenu newmenu;
     BOOL result;
     HTREEITEM ti = NULL;

     switch(type)
        { /* type */
         case lcm_popupmenu:
                 result = newmenu.CreatePopupMenu();  // create empty menu
                 break;
         case lcm_menu:
                 result = newmenu.CreateMenu();  // create empty menu
                 break;
        } /* type */

     DWORD lasterror = GetLastError();

     if(result)
        { /* created successfully */
         usermenuid++;  // assign serial number

         ti = addMenuToNode(newmenu.m_hMenu, M_USER | M_UNASSIGNED | M_NOPARENT, 
                                             NULL, 0, usermenuid, tree_unassigned);
         c_MenuList.insertMenuItems(&newmenu, ti);

         setSelectedMenu(newmenu.m_hMenu);
        } /* created successfully */

     CString fmt;
     fmt.LoadString(strid);

     CString s;
     s.Format(fmt, newmenu.m_hMenu);
     CString ok;
     ok.LoadString(result ? IDS_OK : IDS_ERROR);

     c_Messages.addMessage(newmenu.m_hMenu, ok, s);

     if(!result)
        logError(lasterror);

     newmenu.Detach();  // don't let menu be destroyed by destructor

     enableControls();

     return ti;
    }

void CMenuExplorerDlg::OnCreatemenu() 
{
 doCreateMenu(IDS_CREATEMENU, lcm_menu);
}

void CMenuExplorerDlg::OnCreatepopupmenu() 
{
 HTREEITEM ti = doCreateMenu(IDS_CREATEPOPUPMENU, lcm_popupmenu);
 if(ti != NULL)
    { /* mark as popup */
     menuitem * mi = c_MenuList.GetItemData(ti);
     mi->flags |= M_POPUP;
    } /* mark as popup */
}

BOOL CMenuExplorerDlg::OnKillActive() 
{
        // TODO: Add your specialized code here and/or call the base class
        
        return CPropertyPage::OnKillActive();
}

BOOL CMenuExplorerDlg::OnSetActive() 
{
        // TODO: Add your specialized code here and/or call the base class
        
        return CPropertyPage::OnSetActive();
}

/****************************************************************************
*                        CMenuExplorerDlg::OnMainCommand
* Inputs:
*       WPARAM wParam: wParam
*       LPARAM lParam: lParam
* Result: LRESULT
*       0, always
* Effect: 
*       Logs the message
****************************************************************************/

LRESULT CMenuExplorerDlg::OnMainCommand(WPARAM wParam, LPARAM lParam)
    {
     if(!c_Watch.GetCheck())
        return 0;

     // This odd test handles what is apparently an undocumented WM_USER
     // message that is sent to the property sheet or the property page
     // and has nothing to do with our protocol

     if(wParam == 0 && lParam == 0)
        return 0;

     CString fmt;
     fmt.LoadString(IDS_WM_COMMAND);
     CString msg;
     msg.LoadString(IDS_MSG);
     
     CString s;
     s.Format(fmt, wParam, LOWORD(wParam), HIWORD(wParam), lParam);
     c_Messages.addMessage((HANDLE)getMainWindow()->m_hWnd, msg, s);
     return 0;
    }

void CMenuExplorerDlg::OnDrawmenubar() 
{
 getMainWindow()->DrawMenuBar();
 CString fmt;
 fmt.LoadString(IDS_DRAWMENUBAR);
 CString ok;
 ok.LoadString(IDS_OK);
 CString s;
 s.Format(fmt, getMainWindow()->m_hWnd);
 c_Messages.addMessage((HANDLE)getMainWindow()->m_hWnd, ok, s);
}

/****************************************************************************
*                          CMenuExplorerDlg::OnMainInitMenuPopup
* Inputs:
*       WPARAM wParam:
*       LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuExplorerDlg::OnMainInitMenuPopup(WPARAM wParam, LPARAM lParam)
    {
     if(!c_Watch.GetCheck())
        return 0;

     CString fmt;
     fmt.LoadString(IDS_WM_INITMENUPOPUP);
     CString msg;
     msg.LoadString(IDS_MSG);
     
     CString s;
     s.Format(fmt, wParam, lParam, LOWORD(lParam), HIWORD(lParam));
     c_Messages.addMessage((HANDLE)getMainWindow()->m_hWnd, msg, s);
     return 0;
    }

/****************************************************************************
*                          CMenuExplorerDlg::OnMainInitMenu
* Inputs:
*       WPARAM wParam:
*       LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuExplorerDlg::OnMainInitMenu(WPARAM wParam, LPARAM lParam)
    {
     if(!c_Watch.GetCheck())
        return 0;

     CString fmt;
     fmt.LoadString(IDS_WM_INITMENU);
     CString msg;
     msg.LoadString(IDS_MSG);
     
     CString s;
     s.Format(fmt, wParam); 
     c_Messages.addMessage((HANDLE)getMainWindow()->m_hWnd, msg, s);
     return 0;
    }

/****************************************************************************
*                          CMenuExplorerDlg::OnMainMenuSelect
* Inputs:
*       WPARAM wParam:
*       LPARAM lParam:
* Result: LRESULT
*       0, always
* Effect: 
*       Sends the message to each of the CPropertyPage objects
****************************************************************************/

LRESULT CMenuExplorerDlg::OnMainMenuSelect(WPARAM wParam, LPARAM lParam)
    {
     if(!c_Watch.GetCheck())
        return 0;

     CString fmt;
     fmt.LoadString(IDS_WM_MENUSELECT);
     CString msg;
     msg.LoadString(IDS_MSG);
     
     CString s;
     s.Format(fmt, wParam, LOWORD(wParam), HIWORD(wParam), lParam);
     c_Messages.addMessage((HANDLE)getMainWindow()->m_hWnd, msg, s);
     return 0;
    }

/****************************************************************************
*                        CMenuExplorerDlg::OnMainMenuChar
* Inputs:
*       WPARAM:
*       LPARAM:
* Result: LRESULT
*       0, always
* Effect: 
*       Logs the WM_MENUCHAR message
****************************************************************************/

LRESULT CMenuExplorerDlg::OnMainMenuChar(WPARAM wParam, LPARAM lParam)
    {
     if(!c_Watch.GetCheck())
        return 0;

     CString fmt;
     fmt.LoadString(IDS_WM_MENUCHAR);
     CString msg;
     msg.LoadString(IDS_MSG);
     
     CString menu;
     menu.LoadString(HIWORD(wParam) == MF_SYSMENU ? IDS_MF_SYSMENU 
                                                  : IDS_MF_POPUP);

     CString s;
     // WM_MENUCHAR(0x%08x=('%c', %s), 0x%08x)
     s.Format(fmt, wParam, LOWORD(wParam), menu, lParam);
     c_Messages.addMessage((HANDLE)getMainWindow()->m_hWnd, msg, s);
     return 0;
     
    }


/****************************************************************************
*                        CMenuExplorerDlg::OnMainEnterMenuLoop
* Inputs:
*       WPARAM: Boolean value, TRUE if a popup menu, FALSE if not
*       LPARAM: ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Logs the WM_MENUCHAR message
****************************************************************************/

LRESULT CMenuExplorerDlg::OnMainEnterMenuLoop(WPARAM wParam, LPARAM)
    {
     if(!c_Watch.GetCheck())
        return 0;

     CString fmt;
     fmt.LoadString(IDS_WM_ENTERMENULOOP);
     CString msg;
     msg.LoadString(IDS_MSG);
     
     CString popup;
     popup.LoadString(wParam ? IDS_TRUE : IDS_FALSE); 

     CString s;
     // WM_ENTERMENULOOP(%s, 0)
     s.Format(fmt, popup);
     c_Messages.addMessage((HANDLE)getMainWindow()->m_hWnd, msg, s);
     return 0;
     
    }

/****************************************************************************
*                      CMenuExplorerDlg::OnMainExitMenuLoop
* Inputs:
*       WPARAM: Boolean value, TRUE if a popup menu, FALSE if not
*       LPARAM: ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Logs the WM_MENUCHAR message
****************************************************************************/

LRESULT CMenuExplorerDlg::OnMainExitMenuLoop(WPARAM wParam, LPARAM)
    {
     if(!c_Watch.GetCheck())
        return 0;

     CString fmt;
     fmt.LoadString(IDS_WM_EXITMENULOOP);
     CString msg;
     msg.LoadString(IDS_MSG);
     
     CString popup;
     popup.LoadString(wParam ? IDS_TRUE : IDS_FALSE); 

     CString s;
     // WM_EXITMENULOOP(%s, 0)
     s.Format(fmt, popup);
     c_Messages.addMessage((HANDLE)getMainWindow()->m_hWnd, msg, s);
     return 0;
     
    }

void CMenuExplorerDlg::OnChangeCommand() 
{
 enableControls();
}

/****************************************************************************
*                         CMenuExplorerDlg::OnRemoveMenu
* Result: void
*       
* Effect: 
*       Takes the current menu item, whatever it is, whereever it is, and
*       removes it from whatever menu it is in.  If it is a submenu, it then
*       installs this menu as an unassigned menu so it can be later put back
*       somewhere else.  Think of the "unassigned menus" as sort of a
*       "clipboard list" and you have the right idea.
* Notes:
*       RemoveMenu can remove any menu item.
****************************************************************************/

void CMenuExplorerDlg::OnRemovemenu() 
{
 selection sel = getSelection(SEL_POSITION);
 if(sel.menu == NULL)
    return; // should never happen

 // If it is a submenu, we can only delete by position in API3; 
 // save it for later so we can reassign it
 CMenu * sub = NULL;

 sub = sel.menu->GetSubMenu(sel.id);  // no guarantee it is submenu, may return NULL

 BOOL result = sel.menu->RemoveMenu(sel.id, sel.flags);
 DWORD lasterror = GetLastError();

 if(sub != NULL)
    { /* we removed a submenu */
     HTREEITEM ti = c_MenuList.findItem(sub->m_hMenu);
     menuitem * mi = c_MenuList.GetItemData(ti);

     mi->flags |= M_UNASSIGNED | M_NOPARENT | M_POPUP;
 
     mi->name = computeMenuName(mi->flags, &mi->name, mi->uid);

     // Make sure the menu items under the submenu appear in the list
     HTREEITEM ni = moveItem(tree_unassigned, ti);
     mi = c_MenuList.GetItemData(ni);
     reloadMenuItems(CMenu::FromHandle(mi->hmenu));
    } /* we removed a submenu */
 else
    { /* we removed a command item */
     // Nothing special to do here
    } /* we removed a command item */

 // RemoveMenu(0x%08x, %d, %s) => %s
 CString t;
 t.LoadString(IDS_REMOVEMENU);

 CString rstr;
 rstr.LoadString(result ? IDS_TRUE : IDS_FALSE);

 CString s;
 s.Format(t, sel.menu->m_hMenu, sel.id, getCmdPosStr(sel.flags), rstr);

 CString ok;
 ok.LoadString(result ? IDS_OK : IDS_ERROR);
 c_Messages.addMessage(sel.menu->m_hMenu, ok, s);
 if(!result)
    logError(lasterror);

 // Now update the menu from which we deleted it
 reloadMenuItems(sel.menu);
 notifyChange(); 
}

/****************************************************************************
*                           CMenuExplorerDlg::logError
* Inputs:
*       DWORD lasterror: Error code to log
* Result: void
*       
* Effect: 
*       Translates the error code and logs it to the message log
****************************************************************************/

void CMenuExplorerDlg::logError(DWORD lasterror)
    {
     if(lasterror == 0)
        return; // no error to log
     LPTSTR msg;
     if(::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                        FORMAT_MESSAGE_ALLOCATE_BUFFER,
                        NULL,
                        lasterror, 0,
                        (LPTSTR)&msg,
                        0, 
                        NULL) != 0)
        { /* success */
         CString err;
         err.LoadString(IDS_ERROR);
         c_Messages.addMessage((HANDLE)lasterror, err, msg);
         LocalFree(msg);
        } /* success */
    }

/****************************************************************************
*                      CMenuExplorerDlg::removeContextMenu
* Result: void
*       
* Effect: 
*       Removes the current context menu from its binding and updates the
*       display
****************************************************************************/

void CMenuExplorerDlg::removeContextMenu()
    {
     HMENU trackMenu = getTrackMenu();
     if(trackMenu != NULL)
        { /* remove old one */
         menuitem * mi = c_MenuList.GetItemData(tree_context);
         mi->flags &= ~M_CONTEXT;  // remove context flag
         mi->flags |= M_UNASSIGNED | M_NOPARENT; // mark as unassigned
         c_ContextHandle.SetWindowText(_T(""));
         c_MenuList.deleteChildren(tree_context);
         c_MenuList.InvalidateRect(NULL);  //handle CTreeCtrl refresh bug
         setTrackMenu(NULL);
         addMenuToNode(trackMenu, mi->flags, &mi->name, mi->id, 
                                        mi->uid, tree_unassigned);
         reloadMenuItems(CMenu::FromHandle(trackMenu));
         setSelectedMenu(trackMenu);
         c_MenuList.SetItemData(tree_context, NULL);
         delete mi;
        } /* remove old one */
    }

/****************************************************************************
*                      CMenuExplorerDlg::OnClearContextMenu
* Result: void
*       
* Effect: 
*       Removes the context menu by moving it back to the unassigned menu pool
****************************************************************************/

void CMenuExplorerDlg::OnClearcontextmenu() 
{
 removeContextMenu();

 enableControls();
}

/****************************************************************************
*                      CMenuExplorerDlg::clearCurrentPopup
* Inputs:
*       HTREEITEM ti: Tree item
* Result: void
*       
* Effect: 
*       If the current popup menu (the menu that would be assigned via an 
*       MF_POPUP selection) is defined, we must remove that definition because
*       we have either just assigned it via an Append/InsertMenu or we have
*       made it the current right-button context menu.
****************************************************************************/

void CMenuExplorerDlg::clearCurrentPopup()
    {
     if(currentPopup == NULL)
        return;

     HTREEITEM ti = c_MenuList.findItem(currentPopup->m_hMenu);
     currentPopup->Detach();  // so delete doesn't kill menu
     delete currentPopup;
     currentPopup = NULL;
     c_PopupHandle.SetWindowText(_T(""));
     c_MF_Popup.SetCheck(FALSE);
     
    }

/****************************************************************************
*                           CMenuExplorerDlg::moveItem
* Inputs:
*       HTREEITEM dest: Place under which item is to be inserted
*       HTREEITEM src: Source item which is to be removed from its current
*                       place and moved to the new place
* Result: HTREEITEM
*       New item id
* Effect: 
*       Moves the item in the tree
****************************************************************************/

HTREEITEM CMenuExplorerDlg::moveItem(HTREEITEM dest, HTREEITEM src)
    {
     menuitem * mi = c_MenuList.GetItemData(src);
     c_MenuList.SetItemData(src, NULL);  // detach so DeleteItem doesn't
                                         // free it up
     c_MenuList.DeleteItem(src);
     return c_MenuList.InsertItem(mi, dest);
    }

void CMenuExplorerDlg::OnSetascontextmenu() 
{
 HTREEITEM ti = c_MenuList.GetSelectedItem();
 if(ti == NULL)
    return;

 // Check to see if this really is a popup menu.  If not, warn the user.
 // Note that we *don't* disable the control but use this counter-to-GUI
 // style because we *want* the user to see the effect of using a bad
 // menu, that is, a bogus popup

 menuitem * mi = c_MenuList.GetItemData(ti);

 // insert in dummy menu
 if(!(mi->flags & M_POPUP))
    { /* not popup */
     // Tell the user the problem.  Options are:
     //         Yes - convert to MF_POPUP menu
     //         No  - don't convert to MF_POPUP menu
     //         Cancel - terminate this operation
     int result;
     result = AfxMessageBox(IDS_BAD_POPUP, MB_ICONWARNING | MB_YESNOCANCEL);

     CMenu dummy;

     switch(result)
        { /* handle response */
         case IDCANCEL:
                 return;  // do nothing, terminate
         case IDYES:
                 dummy.CreateMenu();
                 dummy.InsertMenu(0, MF_POPUP, (int)mi->hmenu, (LPCTSTR)NULL);
                 // Now that the MF_POPUP flag is set, remove it again
                 // This is because we are going to destroy the dummy menu
                 // and we don't want to destroy its child menus as well

                 // note that mi->hmenu is still the same handle!
                 dummy.RemoveMenu(0, MF_BYPOSITION);
                 mi->flags |= M_POPUP;

                 // We no longer need the dummy menu
                 dummy.DestroyMenu();
                 break;
         case IDNO:
                 // We're not going to do anything.  
                 break;
        } /* handle response */
    } /* not popup */

 removeContextMenu();  // remove any existing definition

 mi->flags &= ~M_UNASSIGNED;
 mi->flags |= M_CONTEXT;
 setTrackMenu(mi->hmenu);
 c_ContextHandle.SetVal((int)mi->hmenu, _T("0x%08x"));

 // If we had assigned the menu as the MF_POPUP assignable menu as well, we 
 // must clear this factoid.

 if(currentPopup != NULL && currentPopup->m_hMenu == mi->hmenu)
    { /* no longer valid as popup */
     clearCurrentPopup();
    } /* no longer valid as popup */

 mi->name = computeMenuName(mi->flags, &mi->name, mi->uid);

 // Install the menu as the popup item
 c_MenuList.SetItemData(ti, NULL);  // protect against deletion
 c_MenuList.SetItemData(tree_context, mi); // move properties to new location
 c_MenuList.DeleteItem(ti);  // remove previous definition

 CMenu * menu = CMenu::FromHandle(mi->hmenu);
 c_MenuList.insertMenuItems(menu, tree_context);
 enableControls();
}

void CMenuExplorerDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
        // TODO: Add your message handler code here
        
}

void CMenuExplorerDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
        GetParent()->SendMessage(UWM_TRACKPOPUPMENU, 0, 
                                                MAKELONG(point.x, point.y));
        
        CPropertyPage::OnRButtonDown(nFlags, point);
}

void CMenuExplorerDlg::OnExtend() 
{
 c_Messages.setAutoExtend(c_Extend.GetCheck());
}

void CMenuExplorerDlg::OnWrap() 
{
 c_Messages.setWrap(c_Wrap.GetCheck());
}

void CMenuExplorerDlg::OnSelchangedMenulist(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

        HTREEITEM ti = c_MenuList.GetSelectedItem();
        
        menuitem * mi = c_MenuList.GetItemData(ti);

        if(mi != NULL)
           { /* has selection */
            if(mi->flags & M_NOPARENT)
               { /* logical top-level menu */
                c_MenuHandle.SetVal((int)mi->hmenu, _T("0x%08x"));
                c_CurrentPosition.SetWindowText(_T(""));
                c_CurrentCommand.SetWindowText(_T(""));
               } /* logical top-level menu */
            else
               { /* lower member */
                HTREEITEM parent = c_MenuList.GetParentItem(ti);
                menuitem * pi = c_MenuList.GetItemData(parent);

                // The menu is a member of a higher-level menu
                // we can query its id and other properties

                // set the ID as the current command ID -- get actual 
                // ID from menu
                int pos = c_MenuList.getItemPos(ti);
                c_CurrentPosition.SetVal(pos);
                c_CurrentCommand.SetVal(::GetMenuItemID(pi->hmenu, pos));

                if(mi->flags & M_COMMAND)
                   { /* command item */
                    if(pi != NULL)
                       { /* set parent handle */
                        c_MenuHandle.SetVal((int)pi->hmenu, _T("0x%08x"));

                       } /* set parent handle */
                    else
                       { /* no parent handle */
                        c_CurrentPosition.SetWindowText(_T(""));
                        c_CurrentCommand.SetWindowText(_T(""));
                       } /* no parent handle */
                   } /* command item */
                else
                   { /* menu item */
                    c_MenuHandle.SetVal((int)mi->hmenu, _T("0x%08x"));
                   } /* menu item */
               } /* lower member */
           } /* has selection */
        else
           { /* no selection */
            c_MenuHandle.SetWindowText(_T(""));
           } /* no selection */

        enableControls();
        
        *pResult = 0;
}

/****************************************************************************
*                     CMenuExplorerDlg::OnDeleteitemMenulist
* Inputs:
*       NMHDR * pNMHDR:
*       LRESULT * pResult: 
* Result: void
*       Result is passed back via the pResult pointer, and is always 0
* Effect: 
*       Frees up the menuitem object associated with the entry
* Notes:
*       To prevent the freeing of menuitem objects that are about to be
*       reassigned to a new tree node, you must first SetItemData(ti, NULL)
*       before calling DeleteItem(ti).  This keeps the object from being
*       freed in this function (but don't forget to do it yourself if you
*       end up not using it, or make sure that eventually there is one path
*       that deletes an object and doesn't set its itemdata first, thus 
*       allowing this function to free the data).
****************************************************************************/

void CMenuExplorerDlg::OnDeleteitemMenulist(NMHDR* pNMHDR, LRESULT* pResult) 
{
        NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
        

        // There is something wrong here; I get what appears to be a valid
        // reference to mi, but the delete causes all sorts of grief.  It
        // appears that the documentation is either incorrect or unreadable
        //
        /*
           pNMTreeView on a TVN_DELETEITEM notification says: 

           Pointer to an NM_TREEVIEW structure. The itemOld member is a
           TV_ITEM structure containing valid information about the item that
           was deleted in the hItem and lParam members.
        */
        if(pNMTreeView->itemOld.mask & TVIF_PARAM)
           { /* has lParam */
            menuitem * mi = (menuitem *)pNMTreeView->itemOld.lParam;
            if(mi != NULL)
              delete mi;
           } /* has lParam */


        *pResult = 0;
}

/****************************************************************************
*                          CMenuExplorerDlg::traceTree
* Inputs:
*       HTREEITEM ti: Root of tree
*       int depth: Current depth
* Result: void
*       
* Effect: 
*       Traces the tree.  This is strictly for debugging purposes.
****************************************************************************/

void CMenuExplorerDlg::traceTree(HTREEITEM ti, int depth)
    {
     CString s;
     CString t;
     CString cnt;
     static int count;

     if(ti != NULL)
        t = c_MenuList.GetItemText(ti);

     cnt.Format(_T("=>%d"), depth);
     s.Format(_T("%d: 0x%08x %s"), count, ti, t);
     c_Messages.addMessage(ti, cnt, s);

     if(ti == NULL)
        { /* recur */
         count = 0;
         traceTree(c_MenuList.GetRootItem(), count + 1);
         cnt.Format(_T("<=%d"), depth);
         c_Messages.addMessage(ti, cnt, _T(""));
         return;
        } /* recur */

     count++;

     if(count > c_Limit.GetVal())
        return;

     if(c_MenuList.ItemHasChildren(ti))
        { /* process child window */
         traceTree(c_MenuList.GetChildItem(ti), depth+1);
        } /* process child window */

     c_Messages.addMessage(0, _T(""), _T("Siblings"));

     for(HTREEITEM ci = c_MenuList.GetNextSiblingItem(ti); 
                ci != NULL; 
                        ci = c_MenuList.GetNextSiblingItem(ci))
        { /* try each */
         t = c_MenuList.GetItemText(ci);
         cnt.Format(_T("%d"), depth);
         s.Format(_T("%d: 0x%08x %s"), count, ci, t);
         c_Messages.addMessage(ti, cnt, s);

         if(c_MenuList.ItemHasChildren(ci))
            traceTree(ci, depth+1);
        } /* try each */

     cnt.Format(_T("<=%d"), depth);
     s.Format(_T("%d: return"), count);
     c_Messages.addMessage(0, cnt, s);
    }

void CMenuExplorerDlg::OnTesttreewalk() 
{
 traceTree(NULL,0);
}

/****************************************************************************
*                         CMenuExplorerDlg::OnLogMessage
* Inputs:
*       WPARAM: unused
*       LPARAM lParam: (UWM_LOGMSG_DATA *)
* Result: LRESULT
*       0, always (logically void)
* Effect: 
*       Logs the message using the specified data
****************************************************************************/

LRESULT CMenuExplorerDlg::OnLogMessage(WPARAM, LPARAM lParam)
    {
     UWM_LOGMSG_DATA * data = (UWM_LOGMSG_DATA *)lParam;

     c_Messages.addMessage(data->handle, data->type, data->msg);
     return 0;
    }

void CMenuExplorerDlg::OnCheckdimensions() 
{
 if(Is3())
    { /* API 3 */
     DWORD dim = GetMenuCheckMarkDimensions();

     CString t;
     t.LoadString(IDS_GETMENUCHECKMARKDIMENSIONS);
     CString s;
     s.Format(t, dim, LOWORD(dim), HIWORD(dim));
     c_Messages.addMessage(0, _T(""), s);
    } /* API 3 */
 else
    { /* API 4 */
     int n = ::GetSystemMetrics(SM_CXMENUCHECK);

     CString t;
     t.LoadString(IDS_GETCXMENUCHECK);
     CString s;
     s.Format(t, n);
     c_Messages.addMessage(0, _T(""), s);

     n = ::GetSystemMetrics(SM_CYMENUCHECK);
     t.LoadString(IDS_GETCYMENUCHECK);
     s.Format(t, n);
     c_Messages.addMessage(0, _T(""), s);
    } /* API 4 */
}

/****************************************************************************
*                         CMenuExplorerDlg::notifyChange
* Result: void
*       
* Effect: 
*       Sends a menu change notification to all siblings
****************************************************************************/

void CMenuExplorerDlg::notifyChange()
    {
     GetParent()->SendMessage(UWM_CHANGE, 0, (LPARAM)m_hWnd);
    }

void CMenuExplorerDlg::OnSetCommand() 
{
 int n;
 n = c_CurrentCommand.GetVal();
 c_Command.SetVal(n);
}

void CMenuExplorerDlg::OnSetPosition() 
{
 int n;
 n = c_CurrentPosition.GetVal();
 c_Position.SetVal(n);
}

void CMenuExplorerDlg::OnRestore() 
{
 CMenu oldmenu;
 // This is a bit of a kludge; it requires that we know that the menu IDs
 // are used for either 0 or > 0 child windows.  We should really do this
 // by sending some sort of message to the main frame to get the menu id
 // of the menu currently loaded.  That's too complex for right now.

 
 oldmenu.LoadMenu(IDR_MAINFRAME);  // load menu
        
 menuitem * mi = c_MenuList.GetItemData(tree_main);

 mi->hmenu = oldmenu.m_hMenu;

 getMainWindow()->SetMenu(&oldmenu);

 reloadMenuItems(&oldmenu);

 oldmenu.Detach();  // avoid deletion on destructor
}


void CMenuExplorerDlg::OnAutozoom() 
{
 c_MenuList.autoresize = c_AutoZoom.GetCheck();
}


/****************************************************************************
*                          CMenuExplorerDlg::OnAddOwnerDraw
* Inputs:
*       WPARAM wParam: (WPARAM)(odinfo *)
*       LPARAM: ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Creates a copy of the data and adds it to the list
****************************************************************************/

LRESULT CMenuExplorerDlg::OnAddOwnerDraw(WPARAM wParam, LPARAM)
    {
     odinfo * mi = new odinfo;
     *mi = *(odinfo *)wParam;

     int index = c_OwnerDraw.AddString(mi->s);
     if(index != CB_ERR)
        c_OwnerDraw.SetItemData(index, (LPARAM)mi);
     return 0;
    }



// Miscellaneous functions we probably don't need to fill in the bodies for
void CMenuExplorerDlg::OnCheck() 
{
        // TODO: Add your control notification handler code here
}

void CMenuExplorerDlg::OnMfChecked() 
{
        // TODO: Add your control notification handler code here
        
}

void CMenuExplorerDlg::OnMfDisabled() 
{
        // TODO: Add your control notification handler code here
}

void CMenuExplorerDlg::OnMfEnabled() 
{
        // TODO: Add your control notification handler code here
}

void CMenuExplorerDlg::OnMfGrayed() 
{
        // TODO: Add your control notification handler code here
}

void CMenuExplorerDlg::OnMfMenubarbreak() 
{
        // TODO: Add your control notification handler code here
}

void CMenuExplorerDlg::OnMfMenubreak() 
{
        // TODO: Add your control notification handler code here
}

void CMenuExplorerDlg::OnNobreak() 
{
        // TODO: Add your control notification handler code here
}

void CMenuExplorerDlg::OnEnable() 
{
        // TODO: Add your control notification handler code here
}
