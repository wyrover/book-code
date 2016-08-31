// menu.h : main header file for the MENU application
//

#ifndef __AFXWIN_H__
        #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerDlg dialog

typedef struct  {
        // Set up by setupMenu, used by finishMenu
        CMenu * menu;
        int pos;                // %1: InsertMenu              
        int modid;              // %1:                         ModifyMenu
        DWORD flags;            // %2: InsertMenu  AppendMenu  ModifyMenu
        int newitem;            // %3: InsertMenu  AppendMenu  ModifyMenu
        LPVOID lpnewitem;       // %4: InsertMenu  AppendMenu  ModifyMenu

        // Set up by caller, used to format the logging strings
        int opid;  // IDS_APPENDMENU_PREFIX or IDS_INSERTMENU_PREFIX

        // Set up by setupMenu, used to show the logging strings
        CString s;
        CString msg;
        CString suffix;
        CString id;

        // Filled in by executing the function
        BOOL result;
        DWORD lasterror; // only meaningful if failure
                } menuparms;

typedef struct {
                CMenu * menu;   // menu in which it is found
                DWORD flags;  // MF_BYPOSITION or MF_BYCOMMAND
                int   id;     // position      or command id
               } selection;

class CMenuExplorerDlg : public CPropertyPage
{
        DECLARE_DYNCREATE(CMenuExplorerDlg)
// Construction
public:
        CMenuExplorerDlg();   // standard constructor

// Dialog Data
        //{{AFX_DATA(CMenuExplorerDlg)
        enum { IDD = IDD_MENU };
        CComboBox       c_OwnerDraw;
        CButton c_AutoZoom;
        CButton c_Checked;
        CStatic c_c_NewID;
        CSpinButtonCtrl c_SpinNewID;
        CTinyButton     c_Restore;
        CTinyButton     c_ModifyMenu;
        CIntEdit        c_NewID;
        CButton c_SetPosition;
        CButton c_SetCommand;
        CIntEdit        c_CurrentPosition;
        CIntEdit        c_CurrentCommand;
        CTinyButton     c_CheckDimensions;
        CIntEdit        c_Limit;
        CIntEdit        c_MenuHandle;
        CMenuList       c_MenuList;
        CButton c_Extend;
        CButton c_Wrap;
        CTinyButton     c_ClearContextMenu;
        CIntEdit        c_ContextHandle;
        CTinyButton     c_SetAsContextMenu;
        CTinyButton     c_RemoveMenu;
        CButton c_Watch;
        CTinyButton     c_DrawMenuBar;
        CBitmaps        c_Bitmaps;
        CStatic c_c_Command;
        CIntEdit        c_PopupHandle;
        CTinyButton     c_GetSystemMenu;
        CTinyButton     c_GetMenuString;
        CTinyButton     c_GetMenuItemID;
        CTinyButton     c_GetMenuItemCount;
        CTinyButton     c_CreateMenu;
        CButton         c_SysMenuOption;
        CTinyButton     c_SetAsPopup;
        CIntEdit        c_SubMenu;
        CSpinButtonCtrl c_SpinSubMenu;
        CButton         c_MF_Popup;
        CTinyButton     c_GetSubMenu;
        CTinyButton     c_GetMenuState;
        CTinyButton     c_GetMenu;
        CTinyButton     c_EnableMenuItem;
        CTinyButton     c_Enable;
        CTinyButton     c_CheckMenuItem;
        CButton         c_Check;
        CMessageLog     c_Messages;
        CTinyButton     c_InsertMenu;
        CTinyButton     c_DestroyMenu;
        CTinyButton     c_AppendMenu;
        CTinyButton     c_DeleteMenu;
        CIntEdit        c_Position;
        CIntEdit        c_Command;
        CComboBox       c_MenuText;
        CSpinButtonCtrl c_SpinPosition;
        CSpinButtonCtrl c_SpinCommand;
        CStatic         c_c_Position;
        CButton         c_NoBreak;
        CButton         c_MF_String;
        CButton         c_MF_Separator;
        CButton         c_MF_OwnerDraw;
        CButton         c_MF_MenuBreak;
        CButton         c_MF_MenuBarBreak;
        CButton         c_MF_Grayed;
        CButton         c_MF_Enabled;
        CButton         c_MF_Disabled;
        CButton         c_MF_ByPosition;
        CButton         c_MF_ByCommand;
        CButton         c_MF_Bitmap;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMenuExplorerDlg)
        public:
        virtual BOOL OnKillActive();
        virtual BOOL OnSetActive();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        BOOL initialized;

        CString findBitmapName(HBITMAP bm);
        void loadMenuDisplay();
        void reloadMenuItems(CMenu * menu);
        void enableControls();
        HTREEITEM addMenuToNode(HMENU menu, DWORD flags, CString * name, 
                                            int id, int uid, HTREEITEM node);
        void addToCommandDropdown(CMenu * menu);
        CMenu * getSelectedMenu();
        CMenu * getSelectedMenuPermanent();
        CMenu * getMenu();
        menuitem * getSelectedMenuItem();
        int getMenuID();
        DWORD getCmdPos();
        CString getCmdPosStr();
        CString getCmdPosStr(DWORD flags);
        void setSelectedMenu(HMENU menu);
        HTREEITEM addSysMenuToDisplay(HMENU hmenu);
        void deleteSysMenuFromDisplay();
        DWORD getEnabled();
        DWORD getChecked();
        DWORD getBreak();
        CString flagstr(DWORD flags, BOOL wantpos = TRUE);
        CWnd * getMainWindow();
        CMenu * getTopLevelMenu();
        BOOL setupMenu(menuparms * mp);
        void removeContextMenu();
        void finishMenu(menuparms * mp);
        HMENU getTrackMenu();
        void setTrackMenu(HMENU menu);
        CString computeMenuName(UINT flags, CString * name, int id);
        void park(HMENU menu);
        HMENU unpark(HMENU menu);
        void insertMenuItems(CMenu * menu, HTREEITEM parent);
        void traceTree(HTREEITEM ti, int depth);
        void logError(DWORD lasterror);
        HTREEITEM moveItem(HTREEITEM dest, HTREEITEM src);
        void clearCurrentPopup();
        void notifyChange();
#define lcm_popupmenu 1
#define lcm_menu      2
        HTREEITEM doCreateMenu(int strid, int type);


        CMenu * currentPopup;
        int usermenuid;

        // GetSelection values
#define SEL_EITHER   0  // can return MF_BYCOMMAND or MF_BYPOSITION
#define SEL_POSITION 1  // must return MF_BYPOSITION
#define SEL_COMMAND  2  // must return MY_BYCOMMAND

        selection getSelection(int type = SEL_EITHER);

        // Major nodes in the tree control
        HTREEITEM tree_root;
        HTREEITEM tree_main;
        HTREEITEM tree_unassigned;
        HTREEITEM tree_context;
        HTREEITEM tree_system;

        // Generated message map functions
        //{{AFX_MSG(CMenuExplorerDlg)
        afx_msg void OnSelendokMenutext();
        afx_msg void OnMfBitmap();
        afx_msg void OnMfBycommand();
        afx_msg void OnMfByposition();
        afx_msg void OnMfDisabled();
        afx_msg void OnEditchangeMenutext();
        afx_msg void OnMfEnabled();
        afx_msg void OnMfGrayed();
        afx_msg void OnMfMenubarbreak();
        afx_msg void OnMfMenubreak();
        afx_msg void OnMfOwnerdraw();
        afx_msg void OnMfSeparator();
        afx_msg void OnMfString();
        afx_msg void OnNobreak();
        afx_msg void OnAppendmenu();
        afx_msg void OnCheck();
        afx_msg void OnCheckmenuitem();
        afx_msg void OnDeletemenu();
        afx_msg void OnDestroymenu();
        afx_msg void OnEnable();
        afx_msg void OnEnablemenuitem();
        afx_msg void OnGetmenu();
        afx_msg void OnGetmenustate();
        afx_msg void OnGetsubmenu();
        afx_msg void OnInsertmenu();
        afx_msg void OnMfPopup();
        afx_msg void OnGetmenuitemcount();
        afx_msg void OnGetmenuitemid();
        afx_msg void OnGetmenustring();
        afx_msg void OnSetaspopup();
        afx_msg void OnGetsystemmenu();
        virtual BOOL OnInitDialog();
        afx_msg void OnClose();
        afx_msg void OnDestroy();
        afx_msg void OnCreatemenu();
        afx_msg void OnCreatepopupmenu();
        afx_msg void OnDrawmenubar();
        afx_msg LRESULT OnLogMessage(WPARAM wParam, LPARAM lParam); 
        afx_msg LRESULT OnMainCommand(WPARAM wParam, LPARAM lParam); 
        afx_msg LRESULT OnMainInitMenu(WPARAM wParam, LPARAM lParam); 
        afx_msg LRESULT OnMainInitMenuPopup(WPARAM wParam, LPARAM lParam); 
        afx_msg LRESULT OnMainMenuSelect(WPARAM wParam, LPARAM lParam); 
        afx_msg LRESULT OnMainMenuChar(WPARAM wParam, LPARAM lParam); 
        afx_msg LRESULT OnMainEnterMenuLoop(WPARAM wParam, LPARAM lParam); 
        afx_msg LRESULT OnMainExitMenuLoop(WPARAM wParam, LPARAM lParam); 
        afx_msg LRESULT OnAddOwnerDraw(WPARAM, LPARAM);
        afx_msg void OnChangeCommand();
        afx_msg void OnRemovemenu();
        afx_msg void OnClearcontextmenu();
        afx_msg void OnSetascontextmenu();
        afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnExtend();
        afx_msg void OnWrap();
        afx_msg void OnSelchangedMenulist(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnDeleteitemMenulist(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnTesttreewalk();
        afx_msg void OnCheckdimensions();
        afx_msg void OnSetCommand();
        afx_msg void OnSetPosition();
        afx_msg void OnRestore();
        afx_msg void OnModifymenu();
        afx_msg void OnMfChecked();
        afx_msg void OnAutozoom();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
