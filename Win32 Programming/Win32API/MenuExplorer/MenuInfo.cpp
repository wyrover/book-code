// MenuInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MenuExplorer.h"
#include "intedit.h"
#include "menuchk.h"
#include "tinybutt.h"
#include "menuitem.h"
#include "menulist.h"
#include "bitmaps.h"
#include "MenuInfo.h"
#include "uwm.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof(x[0]))

/////////////////////////////////////////////////////////////////////////////
// CMenuInfo property page


IMPLEMENT_DYNCREATE(CMenuInfo, CPropertyPage)

CMenuInfo::CMenuInfo() : CPropertyPage(CMenuInfo::IDD)
{
	//{{AFX_DATA_INIT(CMenuInfo)
	//}}AFX_DATA_INIT
}


void CMenuInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuInfo)
	DDX_Control(pDX, IDC_BITMAPS, c_Bitmaps);
	DDX_Control(pDX, IDC_MENUTEXT, c_MenuText);
	DDX_Control(pDX, IDC_POSITION, c_Position);
	DDX_Control(pDX, IDC_RESULT, c_Result);
	DDX_Control(pDX, IDC_MENUHANDLE, c_MenuHandle);
	DDX_Control(pDX, IDC_DWTYPEDATA_CAPTION, c_c_dwTypeData);
	DDX_Control(pDX, IDC_CCH_CAPTION, c_c_cch);
	DDX_Control(pDX, IDC_WID_CAPTION, c_c_wID);
	DDX_Control(pDX, IDC_DWITEMDATA_CAPTION, c_c_dwItemData);
	DDX_Control(pDX, IDC_SPINPOSITION, c_SpinPosition);
	DDX_Control(pDX, IDC_SPINCOMMAND, c_SpinCommand);
	DDX_Control(pDX, IDC_MF_BYCOMMAND, c_ByCommand);
	DDX_Control(pDX, IDC_MF_BYPOSITION, c_ByPosition);
	DDX_Control(pDX, IDC_HSUBMENU_CAPTION, c_c_hSubmenu);
	DDX_Control(pDX, IDC_COMMAND, c_Command);
	DDX_Control(pDX, IDC_BMPUNCHECKED_CAPTION, c_c_hbmpUnchecked);
	DDX_Control(pDX, IDC_BMPCHECKED_CAPTION, c_c_hbmpChecked);
	DDX_Control(pDX, IDC_FSTATE_CAPTION, c_c_fState);
	DDX_Control(pDX, IDC_FTYPE_CAPTION, c_c_fType);
	DDX_Control(pDX, IDC_MFT_NOBREAK, c_MFT_NoBreak);
	DDX_Control(pDX, IDC_MENULIST, c_MenuList);
	DDX_Control(pDX, IDC_HBMPUNCHECKED, c_hbmpUnchecked);
	DDX_Control(pDX, IDC_HBMPCHECKED, c_hbmpChecked);
	DDX_Control(pDX, IDC_SETMENUINFO, c_SetMenuInfo);
	DDX_Control(pDX, IDC_GETMENUINFO, c_GetMenuInfo);
	DDX_Control(pDX, IDC_FSTATE, c_fState);
	DDX_Control(pDX, IDC_WID, c_wID);
	DDX_Control(pDX, IDC_MIIM_TYPE, c_MIIM_Type);
	DDX_Control(pDX, IDC_MIIM_SUBMENU, c_MIIM_Submenu);
	DDX_Control(pDX, IDC_MIIM_STATE, c_MIIM_State);
	DDX_Control(pDX, IDC_MIIM_ID, c_MIIM_ID);
	DDX_Control(pDX, IDC_MIIM_DATA, c_MIIM_Data);
	DDX_Control(pDX, IDC_MIIM_CHECKMARKS, c_MIIM_CheckMarks);
	DDX_Control(pDX, IDC_MFT_STRING, c_MFT_String);
	DDX_Control(pDX, IDC_MFT_SEPARATOR, c_MFT_Separator);
	DDX_Control(pDX, IDC_MFT_RIGHTJUSTIFY, c_MFT_RightJustify);
	DDX_Control(pDX, IDC_MFT_RADIOCHECK, c_MFT_RadioCheck);
	DDX_Control(pDX, IDC_MFT_OWNERDRAW, c_MFT_OwnerDraw);
	DDX_Control(pDX, IDC_MFT_MENUBREAK, c_MFT_MenuBreak);
	DDX_Control(pDX, IDC_MFT_MENUBARBREAK, c_MFT_MenuBarBreak);
	DDX_Control(pDX, IDC_MFT_BITMAP, c_MFT_Bitmap);
	DDX_Control(pDX, IDC_MFS_HILITE, c_MFS_Hilite);
	DDX_Control(pDX, IDC_MFS_GRAYED, c_MFS_Grayed);
	DDX_Control(pDX, IDC_MFS_DISABLED, c_MFS_Disabled);
	DDX_Control(pDX, IDC_MFS_DEFAULT, c_MFS_Default);
	DDX_Control(pDX, IDC_MFS_CHECKED, c_MFS_Checked);
	DDX_Control(pDX, IDC_HSUBMENU, c_hSubmenu);
	DDX_Control(pDX, IDC_FTYPE, c_fType);
	DDX_Control(pDX, IDC_FMASK, c_fMask);
	DDX_Control(pDX, IDC_DWTYPEDATA, c_dwTypeData);
	DDX_Control(pDX, IDC_DWITEMDATA, c_dwItemData);
	DDX_Control(pDX, IDC_CCH, c_cch);
	DDX_Control(pDX, IDC_CBSIZE, c_cbSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CMenuInfo)
	ON_EN_CHANGE(IDC_CCH, OnChangeCch)
	ON_EN_CHANGE(IDC_DWITEMDATA, OnChangeDwitemdata)
	ON_BN_CLICKED(IDC_GETMENUINFO, OnGetmenuinfo)
	ON_CBN_SELENDOK(IDC_HBMPCHECKED, OnSelendokHbmpchecked)
	ON_CBN_SELENDOK(IDC_HBMPUNCHECKED, OnSelendokHbmpunchecked)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MENULIST, OnSelchangedMenulist)
	ON_BN_CLICKED(IDC_MFS_CHECKED, OnMfsChecked)
	ON_BN_CLICKED(IDC_MFS_DEFAULT, OnMfsDefault)
	ON_BN_CLICKED(IDC_MFS_DISABLED, OnMfsDisabled)
	ON_BN_CLICKED(IDC_MFS_GRAYED, OnMfsGrayed)
	ON_BN_CLICKED(IDC_MFS_HILITE, OnMfsHilite)
	ON_BN_CLICKED(IDC_MFT_BITMAP, OnMftBitmap)
	ON_BN_CLICKED(IDC_MFT_MENUBARBREAK, OnMftMenubarbreak)
	ON_BN_CLICKED(IDC_MFT_MENUBREAK, OnMftMenubreak)
	ON_BN_CLICKED(IDC_MFT_OWNERDRAW, OnMftOwnerdraw)
	ON_BN_CLICKED(IDC_MFT_RADIOCHECK, OnMftRadiocheck)
	ON_BN_CLICKED(IDC_MFT_RIGHTJUSTIFY, OnMftRightjustify)
	ON_BN_CLICKED(IDC_MFT_SEPARATOR, OnMftSeparator)
	ON_BN_CLICKED(IDC_MFT_STRING, OnMftString)
	ON_BN_CLICKED(IDC_MIIM_CHECKMARKS, OnMiimCheckmarks)
	ON_BN_CLICKED(IDC_MIIM_DATA, OnMiimData)
	ON_BN_CLICKED(IDC_MIIM_ID, OnMiimId)
	ON_BN_CLICKED(IDC_MIIM_STATE, OnMiimState)
	ON_BN_CLICKED(IDC_MIIM_SUBMENU, OnMiimSubmenu)
	ON_BN_CLICKED(IDC_MIIM_TYPE, OnMiimType)
	ON_BN_CLICKED(IDC_SETMENUINFO, OnSetmenuinfo)
	ON_EN_CHANGE(IDC_WID, OnChangeWid)
	ON_BN_CLICKED(IDC_MFT_NOBREAK, OnMftNobreak)
	ON_MESSAGE(UWM_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuInfo message handlers


BOOL CMenuInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	// Load a NULL value up first, then call the load routine

 	changed = TRUE;  // force menu to load up on SetActive call

 	c_hbmpChecked.AddString(NULL);
	c_hbmpChecked.SetCurSel(0);
	c_hbmpChecked.load();

	c_hbmpUnchecked.AddString(NULL);
	c_hbmpUnchecked.SetCurSel(0);
	c_hbmpUnchecked.load();


 	c_cbSize.SetVal(sizeof(MENUITEMINFO));
	CheckRadioButton(IDC_MF_BYCOMMAND, IDC_MF_BYPOSITION, IDC_MF_BYCOMMAND);
	CheckRadioButton(IDC_MFT_NOBREAK, IDC_MFT_MENUBARBREAK, IDC_MFT_NOBREAK);
	CheckRadioButton(IDC_MFT_BITMAP, IDC_MFT_SEPARATOR, IDC_MFT_STRING);

	c_SpinCommand.SetRange(0,min(UD_MAXVAL, 65535));
	c_SpinPosition.SetRange(-1, 50);

	MFTtoControl();
	MFStoControl();
	MIIMtoControl();

	c_GetMenuInfo.EnableWindow(!Is3());
	c_SetMenuInfo.EnableWindow(!Is3());

	c_Bitmaps.load();

	enableControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMenuInfo::OnChangeCch() 
{
	// TODO: Add your control notification handler code here
	
}

void CMenuInfo::OnChangeDwitemdata() 
{
	// TODO: Add your control notification handler code here
	
}

void CMenuInfo::OnGetmenuinfo() 
{
 controlsToInfo(TRUE);
 int id = c_ByPosition.GetCheck() ? c_Position.GetVal() 
 				  : c_Command.GetVal(); 
 HMENU hmenu = (HMENU)c_MenuHandle.GetVal();

 BOOL result = ::GetMenuItemInfo(hmenu, id, c_ByPosition.GetCheck(), &info);

 logEvent(IDS_GETMENUITEMINFO, hmenu, id, result);

 if(!result)
    { /* failed */
     MessageBeep(0);
     if(GetLastError() == 0)
        { /* MS bug */
	 // Microsoft does not properly set the GetLastError code under
	 // some conditions of failure
	 logError(ERROR_INVALID_PARAMETER); // fake it out
	} /* MS bug */
     else
	logError(GetLastError());
    } /* failed */
 else
    { /* success */
     CString s;
     s.LoadString(IDS_OK);
     c_Result.SetWindowText(s);
     infoToControls();
     enableControls();
    } /* success */
}

void CMenuInfo::OnSelendokHbmpchecked() 
{
	// TODO: Add your control notification handler code here
	
}

void CMenuInfo::OnSelendokHbmpunchecked() 
{
	// TODO: Add your control notification handler code here
	
}

void CMenuInfo::OnSelchangedMenulist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	HTREEITEM ti = c_MenuList.GetSelectedItem();
	menuitem * mi = c_MenuList.GetItemData(ti);

	if(mi != NULL)
	   { /* has selection */
	    c_Command.SetVal(mi->id);
	    c_Position.SetVal(c_MenuList.getItemPos(ti));
	    if(mi->flags & M_COMMAND)
	       { /* command item */
		HTREEITEM parent = c_MenuList.GetParentItem(ti);
		menuitem * pi = c_MenuList.GetItemData(parent);
		if(pi != NULL)
		   c_MenuHandle.SetVal((int)pi->hmenu, _T("0x%08x"));
		else
		   c_MenuHandle.SetWindowText(_T(""));
	       } /* command item */
	    else
	       { /* menu item */
	        if(mi->flags & M_EMPTY)
		   c_MenuHandle.SetWindowText(_T(""));
		else
		   { /* real menu */
		    HTREEITEM parent = c_MenuList.GetParentItem(ti);
		    menuitem * pi = c_MenuList.GetItemData(parent);
		    if(pi != NULL)
		       c_MenuHandle.SetVal((int)pi->hmenu, _T("0x%08x"));
		    else
		       c_MenuHandle.SetWindowText(_T(""));
		   } /* real menu */
	       } /* menu item */
	   } /* has selection */
	else
	   { /* no selection */
	    c_MenuHandle.SetWindowText(_T(""));
	   } /* no selection */

	*pResult = 0;
}

void CMenuInfo::OnMfsChecked() 
{
 MFStoControl();
}

void CMenuInfo::OnMfsDefault() 
{
 MFStoControl();
}

void CMenuInfo::OnMfsDisabled() 
{
 MFStoControl();
}

void CMenuInfo::OnMfsGrayed() 
{
 MFStoControl();
}

void CMenuInfo::OnMfsHilite() 
{
 MFStoControl();
}

void CMenuInfo::OnMftBitmap() 
{
 MFTtoControl();
 enableControls();
}

void CMenuInfo::OnMftMenubarbreak() 
{
 MFTtoControl();
}

void CMenuInfo::OnMftMenubreak() 
{
 MFTtoControl();
}

void CMenuInfo::OnMftNobreak() 
{
 MFTtoControl();
}

void CMenuInfo::OnMftOwnerdraw() 
{
 MFTtoControl();
 enableControls();
}

void CMenuInfo::OnMftRadiocheck() 
{
 MFTtoControl();
}

void CMenuInfo::OnMftRightjustify() 
{
 MFTtoControl();
}

void CMenuInfo::OnMftSeparator() 
{
 MFTtoControl();
 enableControls();
}

void CMenuInfo::OnMftString() 
{
 MFTtoControl();
 enableControls();
}

void CMenuInfo::OnMiimCheckmarks() 
{
 MIIMtoControl();
 enableControls();
}

void CMenuInfo::OnMiimData() 
{
 MIIMtoControl();
 enableControls();
}

void CMenuInfo::OnMiimId() 
{
 MIIMtoControl();
 enableControls();
}

void CMenuInfo::OnMiimState() 
{
 MIIMtoControl();
 enableControls();
}

void CMenuInfo::OnMiimSubmenu() 
{
 MIIMtoControl();
 enableControls();
}

void CMenuInfo::OnMiimType() 
{
 MIIMtoControl();
 enableControls();
}

/****************************************************************************
*                             CMenuInfo::logError
* Inputs:
*       DWORD lasterror: Error code to log
* Result: void
*       
* Effect: 
*       Logs the error message
****************************************************************************/

void CMenuInfo::logError(DWORD lasterror)
    {
     LPTSTR msg;
     if(lasterror != 0 && ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
					  FORMAT_MESSAGE_ALLOCATE_BUFFER,
					  NULL, lasterror, 0, (LPTSTR)&msg,
					  0, NULL) != 0)
        { /* success */
	 // We have to strip off the completely gratuitous and pointless
	 // \r\n that is appended to the end of the string
	 while(msg[lstrlen(msg) - 1] < _T(' '))
	    msg[lstrlen(msg) - 1] = _T('\0');

	 c_Result.SetWindowText(msg);
	 LocalFree(msg);
	 
	 UWM_LOGMSG_DATA data;
	 data.handle = 0;
	 data.type = _T("");
	 data.msg = msg;
	 GetParent()->SendMessage(UWM_LOGMSG, 0, (LPARAM)&data);

	} /* success */
     
    }

/****************************************************************************
*                             CMenuInfo::logEvent
* Inputs:
*       int msgid: ID of formatting string (Get/Set have same parameters)
*	HMENU hmenu: Menu handle argument for function
*	int id: Position or command id
*	BOOL result: Result of operation
* Result: void
*       
* Effect: 
*       Logs the message to the event log
****************************************************************************/

void CMenuInfo::logEvent(int msgid, HMENU hmenu, int id, BOOL result)
    {
     // [GS]etMenuItemInfo(0x%08x, %d, %s, &info) => %s

     CString t;
     t.LoadString(msgid);

     UWM_LOGMSG_DATA data;
     CString bypos;
     CString rstr;

     bypos.LoadString(c_ByPosition.GetCheck() ? IDS_TRUE : IDS_FALSE);
     rstr.LoadString(result ? IDS_TRUE : IDS_FALSE);

     data.type.LoadString(result ? IDS_OK : IDS_ERROR);
     data.msg.Format(t, (int)hmenu, id, bypos, rstr);
     data.handle = hmenu;

     GetParent()->SendMessage(UWM_LOGMSG, 0, (LPARAM)&data);
     
    }

void CMenuInfo::OnSetmenuinfo() 
{
 controlsToInfo(FALSE);
 int id = c_ByPosition.GetCheck() ? c_Position.GetVal() 
 				  : c_Command.GetVal(); 
 HMENU hmenu = (HMENU)c_MenuHandle.GetVal(); 

 BOOL result = ::SetMenuItemInfo(hmenu, id, c_ByPosition.GetCheck(), &info);

 // Log the message to the event window
 // SetMenuItemInfo(0x%08x, %d, %s, &info) => %s

 logEvent(IDS_SETMENUITEMINFO, hmenu, id, result);

 if(!result)
    { /* failed */
     MessageBeep(0);
     if(GetLastError() == 0)
        { /* MS bug */
	 // Microsoft does not properly set the GetLastError code under
	 // some conditions of failure
	 logError(ERROR_INVALID_PARAMETER); // fake it out
	} /* MS bug */
     else
	logError(GetLastError());
    } /* failed */
 else
    { /* success */
     CString s;
     s.LoadString(IDS_OK);
     c_Result.SetWindowText(s);
    } /* success */

}

void CMenuInfo::OnChangeWid() 
{
	// TODO: Add your control notification handler code here
	
}

/****************************************************************************
*                          CMenuInfo::controlsToInfo
* Inputs:
*	BOOL fetch: TRUE if we are doing GetMenuItemInfo, FALSE if we are
*		    doing SetMenuItemInfo
* Result: void
*       
* Effect: 
*       Transfers controls to MENUINFO structure
****************************************************************************/

void CMenuInfo::controlsToInfo(BOOL fetch)
    {
     DWORD flags = getMIIMfromControls();

     info.cbSize = sizeof(MENUITEMINFO);
     info.fMask = flags;

     if(!fetch)
	info.fType = getMFTfromControls();
     else
	info.fType = 0;

     if(!fetch)
	info.fState = getMFSfromControls();
     else
	info.fState = 0;

     if(!fetch)
	info.wID = (flags & MIIM_ID ? c_wID.GetVal() : 0);
     else
	info.wID = 0;

     if(!fetch)
        { /* setting submenu  */
	 info.hSubMenu = NULL;  // TODO: write more code here
	} /* setting submenu  */
     else
        { /* getting submenu */
	 info.hSubMenu = NULL;
	} /* getting submenu */

     if(!fetch)
        { /* setting bitmaps  */
	 info.hbmpChecked = c_hbmpChecked.GetCurItem();
	 info.hbmpUnchecked = c_hbmpUnchecked.GetCurItem();
	} /* setting bitmaps  */
     else
        { /* getting bitmaps */
	 info.hbmpChecked = NULL;
	 info.hbmpUnchecked = NULL;
	} /* getting bitmaps */

     if(!fetch)
	info.dwItemData = (flags & MIIM_DATA ? c_dwItemData.GetVal() : 0);
     else
	info.dwItemData = 0;

     if(fetch)
        { /* fetching */
	 // If we are a GetMenuItemInfo, we need to set dwTypeData and cch in
	 // case we have a string item.  Note that if we don't have a
	 // string data item, dwTypeData will be set to some other 32-bit
	 // value on the fetch; otherwise, it is a pointer to the buffer
     
	 info.cch = DIM(menutext);
	 info.dwTypeData = menutext;  // place to put result
	 return;
	} /* fetching */
     else
        { /* storing */
	 // The following sets the dwTypeData field, and is set only 
	 // when doing SetMenuItemInfo

	 if(c_MFT_Bitmap.GetCheck())
	    { /* bitmap */
	     info.dwTypeData = (LPTSTR)(HBITMAP)c_Bitmaps.GetCurItem();
	    } /* bitmap */
	 else
	 if(c_MFT_Separator.GetCheck())
	    { /* separator */
	     info.dwTypeData = 0;
	    } /* separator */
	 else
	 if(c_MFT_OwnerDraw.GetCheck())
	    { /* owner-draw */
	     info.dwTypeData = (LPTSTR)(flags & MIIM_TYPE ? c_dwTypeData.GetVal() 
							  : 0);
	    } /* owner-draw */
	 else
	 if(c_MFT_String.GetCheck())
	    { /* string */
	     info.cch = 0; // ignored on SetMenuItemInfo
	     info.dwTypeData = menutext;
	     c_MenuText.GetWindowText(menutext, DIM(menutext));
	    } /* string */
	 else
	    { /* impossible -- right? */
	     info.dwTypeData = 0;  // will probably fail for other reasons
	     info.cch        = (flags & MIIM_TYPE ? c_cch.GetVal() : 0);
	    } /* impossible -- right? */
	} /* storing */
    }

/****************************************************************************
*                          CMenuInfo::infoToControls
* Result: void
*       
* Effect: 
*       Transfers contents of menu info block to controls
****************************************************************************/

void CMenuInfo::infoToControls()
    {
     DWORD flags = getMIIMfromControls();
     
     // cbSize is already set, don't need to do it...

     // fmask is already set based on setup, will not change

     if(flags & MIIM_TYPE)
        { /* type */
	 int selection = IDC_MFT_STRING; // assume string
	 if(info.fType & MFT_BITMAP)
	     selection = IDC_MFT_BITMAP;
	 else
	 if(info.fType & MFT_SEPARATOR)
	    selection = IDC_MFT_SEPARATOR;
         else
	 if(info.fType & MFT_OWNERDRAW)
	    selection = IDC_MFT_OWNERDRAW;
	 else
	    { /* string */
	     c_MenuText.SetWindowText(menutext);
	    } /* string */

         CheckRadioButton(IDC_MFT_BITMAP, IDC_MFT_SEPARATOR, selection);
  
         selection = IDC_MFT_NOBREAK;
	 if(info.fType & MFT_MENUBREAK)
	    selection = IDC_MFT_MENUBREAK;
         else
         if(info.fType & MFT_MENUBARBREAK)
	    selection = IDC_MFT_MENUBARBREAK;

         CheckRadioButton(IDC_MFT_NOBREAK, IDC_MFT_MENUBARBREAK, selection);
         
	 c_MFT_RadioCheck.SetCheck(info.fType & MFT_RADIOCHECK);
	 c_MFT_RightJustify.SetCheck(info.fType & MFT_RIGHTJUSTIFY);
	} /* type */


     if(flags & MIIM_STATE)
        { /* state */
	 c_MFS_Checked.SetCheck( (info.fState & MFS_CHECKED));
	 c_MFS_Default.SetCheck( (info.fState & MFS_DEFAULT));
	 c_MFS_Disabled.SetCheck((info.fState & MFS_DISABLED) == MFS_DISABLED);
	 c_MFS_Grayed.SetCheck(  (info.fState & MFS_GRAYED) == MFS_GRAYED);
	 c_MFS_Hilite.SetCheck(  (info.fState & MFS_HILITE));
	} /* state */

     if(flags & MIIM_ID)
	c_wID.SetVal(info.wID);

     if(flags & MIIM_SUBMENU)
        { /* has submenu */
	 c_hSubmenu.SetVal((int)info.hSubMenu, _T("0x%08x"));
	} /* has submenu */

     if(flags & MIIM_CHECKMARKS)
        { /* has checkmarks */
	 // TODO: write this
	} /* has checkmarks */

     if(flags & MIIM_DATA)
	c_dwItemData.SetVal(info.dwItemData, _T("0x%08x"));

     if(flags & MIIM_TYPE)
	c_cch.SetVal(info.cch);

     if(info.fType & MFT_BITMAP)
        { /* bitmap values */
	 c_Bitmaps.Select((HMENU)info.dwTypeData);
	} /* bitmap values */

     // Note: dwTypeData is updated by enableControls()

    }

/****************************************************************************
*                        CMenuInfo::getMFTfromControls
* Result: DWORD
*       result of reading the controls
****************************************************************************/

DWORD CMenuInfo::getMFTfromControls()
    {
     DWORD flags = 0;
     if(c_MFT_Bitmap.GetCheck())
	flags = MFT_BITMAP;
     else
     if(c_MFT_OwnerDraw.GetCheck())
	flags = MFT_OWNERDRAW;
     else
     if(c_MFT_String.GetCheck())
	flags = MFT_STRING;
     else
     if(c_MFT_Separator.GetCheck())
	flags = MFT_SEPARATOR;

     if(c_MFT_MenuBreak.GetCheck())
	flags |= MFT_MENUBREAK;
     else
     if(c_MFT_MenuBarBreak.GetCheck())
	flags |= MFT_MENUBARBREAK;

     if(c_MFT_RadioCheck.GetCheck())
	flags |= MFT_RADIOCHECK;

     if(c_MFT_RightJustify.GetCheck())
	flags |= MFT_RIGHTJUSTIFY;
     
     return flags;
    }

/****************************************************************************
*                        CMenuInfo::getMFSfromControls
* Result: DWORD
*       MFS flags from individual controls
****************************************************************************/

DWORD CMenuInfo::getMFSfromControls()
    {
     DWORD flags = 0;
     if(c_MFS_Checked.GetCheck())
	flags |= MFS_CHECKED;

     if(c_MFS_Default.GetCheck())
	flags |= MFS_DEFAULT;

     if(c_MFS_Disabled.GetCheck())
	flags |= MFS_DISABLED;

     if(c_MFS_Grayed.GetCheck())
	flags |= MFS_GRAYED;

     if(c_MFS_Hilite.GetCheck())
	flags |= MFS_HILITE;

     return flags;
    }

/****************************************************************************
*                        CMenuInfo::getMIIMfromControls
* Result: DWORD
*       MIIM flags from individual controls
****************************************************************************/

DWORD CMenuInfo::getMIIMfromControls()
    {
     DWORD flags = 0;
     if(c_MIIM_CheckMarks.GetCheck())
	flags |= MIIM_CHECKMARKS;

     if(c_MIIM_Data.GetCheck())
	flags |= MIIM_DATA;

     if(c_MIIM_ID.GetCheck())
	flags |= MIIM_ID;

     if(c_MIIM_State.GetCheck())
	flags |= MIIM_STATE;

     if(c_MIIM_Submenu.GetCheck())
	flags |= MIIM_SUBMENU;

     if(c_MIIM_Type.GetCheck())
	flags |= MIIM_TYPE;

     return flags;
    }

/****************************************************************************
*                           CMenuInfo::MFTtoControl
* Result: void
*       
* Effect: 
*       Looks at all the MFT bit selections and creates a hex value which
*	it stores in the control
****************************************************************************/

void CMenuInfo::MFTtoControl()
    {
     DWORD flags = getMFTfromControls();
     c_fType.SetVal(flags, _T("0x%08x"));
    }

/****************************************************************************
*                           CMenuInfo::MFStoControl
* Result: void
*       
* Effect: 
*       Looks at all the MFT bit selections and creates a hex value which
*	it stores in the control
****************************************************************************/

void CMenuInfo::MFStoControl()
    {
     DWORD flags = getMFSfromControls();
     c_fState.SetVal(flags, _T("0x%08x"));
    }

/****************************************************************************
*                           CMenuInfo::MIIMtoControl
* Result: void
*       
* Effect: 
*       Looks at all the MFT bit selections and creates a hex value which
*	it stores in the control
****************************************************************************/

void CMenuInfo::MIIMtoControl()
    {
     DWORD flags = getMIIMfromControls();
     c_fMask.SetVal(flags, _T("0x%08x"));
    }


/****************************************************************************
*                          CMenuInfo::enableControls
* Result: void
*       
* Effect: 
*       Based on the available flags and other criteria, show/hide the
*	various controls
****************************************************************************/

void CMenuInfo::enableControls()
    {
     DWORD mask = getMIIMfromControls();

     c_fType.ShowWindow(mask & MIIM_TYPE ? SW_SHOW : SW_HIDE);
     c_c_fType.EnableWindow(mask & MIIM_TYPE);
     c_MFT_Bitmap.EnableWindow(mask & MIIM_TYPE);
     c_MFT_OwnerDraw.EnableWindow(mask & MIIM_TYPE);
     c_MFT_String.EnableWindow(mask & MIIM_TYPE);
     c_MFT_Separator.EnableWindow(mask & MIIM_TYPE);
     c_MFT_NoBreak.EnableWindow(mask & MIIM_TYPE);
     c_MFT_MenuBarBreak.EnableWindow(mask & MIIM_TYPE);
     c_MFT_MenuBreak.EnableWindow(mask & MIIM_TYPE);
     c_MFT_RadioCheck.EnableWindow(mask & MIIM_TYPE);
     c_MFT_RightJustify.EnableWindow(mask & MIIM_TYPE);

     c_dwTypeData.ShowWindow(mask & MIIM_TYPE ? SW_SHOW : SW_HIDE);
     c_c_dwTypeData.EnableWindow(mask & MIIM_TYPE);

     // based on the selected type, change what we show
     // for MFT_BITMAP, we select a bitmap
     // for MFT_SEPARATOR, we disable it and display a NULL value
     // for MFT_STRING, we make it read-only referencing an internal string
     // for MFT_OWNERDRAW, it is user-definable so it is fully enabled
     if(c_MFT_Bitmap.GetCheck())
        { /* bitmap */
	 HBITMAP bm = NULL;
	 bm = c_Bitmaps.GetCurItem();
         c_dwTypeData.SetReadOnly(TRUE);
	 c_dwTypeData.SetVal((int)bm, _T("0x%08x"));
	} /* bitmap */
     else
     if(c_MFT_Separator.GetCheck())
        { /* separator */
	 CString s;
	 s.LoadString(IDS_NULL);
	 c_dwTypeData.SetReadOnly(TRUE);
	 c_dwTypeData.SetWindowText(s);
	} /* separator */
     else
     if(c_MFT_OwnerDraw.GetCheck())
        { /* owner-draw */
	 c_dwTypeData.SetReadOnly(FALSE);
	 c_dwTypeData.SetWindowText(_T(""));
	} /* owner-draw */
     else
     if(c_MFT_String.GetCheck())
        { /* string */
	 c_dwTypeData.SetReadOnly(TRUE);
	 CString s;
	 c_MenuText.GetWindowText(s);
	 s = _T("\"") + s + _T("\"");
	 c_dwTypeData.SetWindowText(s);
	} /* string */
     else
        { /* impossible -- right? */
	 c_dwTypeData.SetReadOnly(TRUE);
	 c_dwTypeData.SetWindowText(_T(""));
	} /* impossible -- right? */


     c_cch.ShowWindow(mask & MIIM_TYPE ? SW_SHOW : SW_HIDE);
     c_c_cch.EnableWindow(mask & MIIM_TYPE);

     c_fState.ShowWindow(mask & MIIM_STATE ? SW_SHOW : SW_HIDE);
     c_c_fState.EnableWindow(mask & MIIM_STATE);
     c_MFS_Checked.EnableWindow(mask & MIIM_STATE);
     c_MFS_Default.EnableWindow(mask & MIIM_STATE);
     c_MFS_Disabled.EnableWindow(mask & MIIM_STATE);
     c_MFS_Grayed.EnableWindow(mask & MIIM_STATE);
     c_MFS_Hilite.EnableWindow(mask & MIIM_STATE);

     c_wID.ShowWindow(mask & MIIM_ID ? SW_SHOW : SW_HIDE);
     c_c_wID.EnableWindow(mask & MIIM_ID);

     c_hSubmenu.ShowWindow(mask & MIIM_SUBMENU ? SW_SHOW : SW_HIDE);
     c_c_hSubmenu.EnableWindow(mask & MIIM_SUBMENU);

     c_hbmpChecked.ShowWindow(mask & MIIM_CHECKMARKS ? SW_SHOW : SW_HIDE);
     c_hbmpUnchecked.ShowWindow(mask & MIIM_CHECKMARKS ? SW_SHOW : SW_HIDE);
     c_c_hbmpChecked.EnableWindow(mask & MIIM_CHECKMARKS);
     c_c_hbmpUnchecked.EnableWindow(mask & MIIM_CHECKMARKS);

     c_dwItemData.ShowWindow(mask & MIIM_DATA ? SW_SHOW : SW_HIDE);
     c_c_dwItemData.EnableWindow(mask & MIIM_DATA);

     c_MenuText.EnableWindow(c_MFT_String.IsWindowEnabled() 
     			     && c_MFT_String.GetCheck());
     c_Bitmaps.EnableWindow(c_MFT_Bitmap.GetCheck());

    }

/****************************************************************************
*			  CMenuInfo::loadMenuDisplay
* Result: void
*       
* Effect: 
*       Loads the menu tree
****************************************************************************/

void CMenuInfo::loadMenuDisplay()
    {
     CString s;

     c_MenuList.DeleteAllItems();

     s.LoadString(IDS_MENUS);
     tree_root = c_MenuList.InsertItem(s, NULL);

     menuitem * mi = new menuitem;
     mi->flags = M_MAIN | M_NODELETE;
     mi->hmenu = AfxGetMainWnd()->GetMenu()->m_hMenu;
     mi->name.LoadString(IDS_MAIN_MENU);

     tree_main = c_MenuList.InsertItem(mi, tree_root);
     c_MenuList.insertMenuItems(CMenu::FromHandle(mi->hmenu), tree_main);

     // Force the tree to show the main menu top-level items
     c_MenuList.Expand(tree_root, TVE_EXPAND);
     c_MenuList.Expand(tree_main, TVE_EXPAND);
    }


BOOL CMenuInfo::OnSetActive() 
{
     if(changed)
	loadMenuDisplay();
	
	return CPropertyPage::OnSetActive();
}

/****************************************************************************
*                             CMenuInfo::OnChange
* Inputs:
*       WPARAM: ignored
*	LPARAM: window handle of caller (check to see if we're sending to
*		ourself)
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Sets the 'changed' flag to indicate the menus need to be reloaded
****************************************************************************/

LRESULT CMenuInfo::OnChange(WPARAM, LPARAM lParam)
    {
     if((HWND)lParam != m_hWnd)
	changed = TRUE;
     return 0;
    }
