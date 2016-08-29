// DropDown.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "DropDown.h"
#include "is95.h"
#include "intedit.h"
#include "msgsheet.h"
#include "tinybutt.h"
#include "ixable.h"
#include "loggable.h"
#include "ComboOps.h"
#include "effects.h"
#include "ddtext.h"
#include "locales.h"
#include "ddstate.h"
#include "indexpg.h"
#include "ddndx.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof(x[0]))

/////////////////////////////////////////////////////////////////////////////
// CDropDownTest property page

IMPLEMENT_DYNCREATE(CDropDownTest, CPropertyPage)

CDropDownTest::CDropDownTest() : CPropertyPage(CDropDownTest::IDD)
{
        //{{AFX_DATA_INIT(CDropDownTest)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CDropDownTest::~CDropDownTest()
{
}

void CDropDownTest::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDropDownTest)
        DDX_Control(pDX, IDC_SMARTDROPDOWN, c_SmartDropdown);
        DDX_Control(pDX, IDC_ENABLE, c_Enable);
        DDX_Control(pDX, IDC_EXTENDEDUI, c_ExtendedUI);
        DDX_Control(pDX, IDC_WIDE, c_Wide);
        DDX_Control(pDX, IDC_SIMPLE, c_Simple);
        DDX_Control(pDX, IDC_DROPSIZE, c_DropSize);
        DDX_Control(pDX, IDC_DROPLIST, c_DropList);
        DDX_Control(pDX, IDC_DROPDOWN, c_DropDown);
        DDX_Control(pDX, IDC_DELETE, c_Delete);
        DDX_Control(pDX, IDC_COMBO, c_ComboDropDown);
        DDX_Control(pDX, IDC_ADD, c_Add);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDropDownTest, CPropertyPage)
        //{{AFX_MSG_MAP(CDropDownTest)
        ON_BN_CLICKED(IDC_ADD, OnAdd)
        ON_CBN_CLOSEUP(IDC_COMBO, OnCloseupCombo)
        ON_CBN_DBLCLK(IDC_COMBO, OnDblclkCombo)
        ON_CBN_DROPDOWN(IDC_COMBO, OnDropdownCombo)
        ON_CBN_EDITCHANGE(IDC_COMBO, OnEditchangeCombo)
        ON_CBN_EDITUPDATE(IDC_COMBO, OnEditupdateCombo)
        ON_CBN_ERRSPACE(IDC_COMBO, OnErrspaceCombo)
        ON_CBN_KILLFOCUS(IDC_COMBO, OnKillfocusCombo)
        ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
        ON_CBN_SELENDCANCEL(IDC_COMBO, OnSelendcancelCombo)
        ON_CBN_SELENDOK(IDC_COMBO, OnSelendokCombo)
        ON_CBN_SETFOCUS(IDC_COMBO, OnSetfocusCombo)
        ON_BN_CLICKED(IDC_DELETE, OnDelete)
        ON_BN_CLICKED(IDC_DROPDOWN, OnDropdown)
        ON_BN_CLICKED(IDC_DROPLIST, OnDroplist)
        ON_BN_CLICKED(IDC_SIMPLE, OnSimple)
        ON_BN_CLICKED(IDC_WIDE, OnWide)
        ON_BN_CLICKED(IDC_EXTENDEDUI, OnExtendedui)
        ON_BN_CLICKED(IDC_ENABLE, OnEnable)
        ON_CBN_CLOSEUP(IDC_SIMPLE_COMBO, OnCloseupCombo)
        ON_CBN_DBLCLK(IDC_SIMPLE_COMBO, OnDblclkCombo)
        ON_CBN_DROPDOWN(IDC_SIMPLE_COMBO, OnDropdownCombo)
        ON_CBN_EDITCHANGE(IDC_SIMPLE_COMBO, OnEditchangeCombo)
        ON_CBN_EDITUPDATE(IDC_SIMPLE_COMBO, OnEditupdateCombo)
        ON_CBN_ERRSPACE(IDC_SIMPLE_COMBO, OnErrspaceCombo)
        ON_CBN_KILLFOCUS(IDC_SIMPLE_COMBO, OnKillfocusCombo)
        ON_CBN_SELCHANGE(IDC_SIMPLE_COMBO, OnSelchangeCombo)
        ON_CBN_SELENDCANCEL(IDC_SIMPLE_COMBO, OnSelendcancelCombo)
        ON_CBN_SELENDOK(IDC_SIMPLE_COMBO, OnSelendokCombo)
        ON_CBN_SETFOCUS(IDC_SIMPLE_COMBO, OnSetfocusCombo)
        ON_CBN_CLOSEUP(IDC_DROPLIST_COMBO, OnCloseupCombo)
        ON_CBN_DBLCLK(IDC_DROPLIST_COMBO, OnDblclkCombo)
        ON_CBN_DROPDOWN(IDC_DROPLIST_COMBO, OnDropdownCombo)
        ON_CBN_EDITCHANGE(IDC_DROPLIST_COMBO, OnEditchangeCombo)
        ON_CBN_EDITUPDATE(IDC_DROPLIST_COMBO, OnEditupdateCombo)
        ON_CBN_ERRSPACE(IDC_DROPLIST_COMBO, OnErrspaceCombo)
        ON_CBN_KILLFOCUS(IDC_DROPLIST_COMBO, OnKillfocusCombo)
        ON_CBN_SELCHANGE(IDC_DROPLIST_COMBO, OnSelchangeCombo)
        ON_CBN_SELENDCANCEL(IDC_DROPLIST_COMBO, OnSelendcancelCombo)
        ON_CBN_SELENDOK(IDC_DROPLIST_COMBO, OnSelendokCombo)
        ON_CBN_SETFOCUS(IDC_DROPLIST_COMBO, OnSetfocusCombo)
        ON_BN_CLICKED(IDC_SENDMESSAGES, OnSendmessages)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropDownTest message handlers

/****************************************************************************
*                         CDropDownTest::copyStrings
* Inputs:
*       CComboBox * cb: Source combo box
* Result: void
*       
* Effect: 
*       Copies all the strings from the source combo box to the destination
*       box which is in c_Combo
****************************************************************************/

void CDropDownTest::copyStrings(CComboBox * cb)
    {
     c_Combo->ResetContent();
     
     CString s;
     for(int i = 0; i < cb->GetCount(); i++)
        { /* copy strings */
         cb->GetLBText(i, s);
         c_Combo->AddString(s); // assumes sorted order
        } /* copy strings */
    }

/****************************************************************************
*                        CDropDownTest::getMaxCtlWidth
* Result: int
*       
* Effect: 
*       Maximum width for control type column
****************************************************************************/

int CDropDownTest::getMaxCtlWidth()
    {
     int width = 0;

     CString s;

     s.LoadString(IDS_SIMPLE);
     width = max(width, c_Messages.GetStringWidth(s));
     s.LoadString(IDS_DROPDOWN);
     width = max(width, c_Messages.GetStringWidth(s));
     s.LoadString(IDS_DROPDOWNLIST);
     width = max(width, c_Messages.GetStringWidth(s));

     return width;     
    }

/****************************************************************************
*                         CDropDownTest::OnInitDialog
* Result: BOOL
*       TRUE, always
* Effect: 
*       Initializes the various dropdown lists
****************************************************************************/

BOOL CDropDownTest::OnInitDialog()
    {
     CPropertyPage::OnInitDialog();

     c_Messages.setWrap(TRUE);

     c_Combo = &c_ComboDropDown;
     c_Enable.SetCheck(TRUE);  // start enabled
     c_SmartDropdown.SetCheck(TRUE);

     // This technique is a kludge that circumvents a bug in MFC.  It uses
     // a moderately weird approach to get around the bug, which causes
     // Windows/NT to crash (yep!) if I re-create the combo box window
     // with the following code:
     //         c_Combo.DestroyWindow();
     //         c_Combo.CreateWindow(...);
     //

     DWORD style = ::GetWindowLong(c_Combo->m_hWnd, GWL_STYLE);
     
     style &= ~(CBS_SIMPLE | CBS_DROPDOWN | CBS_DROPDOWNLIST);

     // make sure the two new windows are invisible
     style &= ~WS_VISIBLE;

     CRect r;
     c_DropSize.GetWindowRect(&r);  // screen coordinates
     ScreenToClient(&r);            // convert to client coords of current
                                    // window

     //----------------------------------------------------------------
     // Create a simple combobox object

     c_ComboSimple.Create(style | CBS_SIMPLE | WS_VSCROLL, r, this, IDC_SIMPLE_COMBO);
     c_ComboSimple.EnableWindow(FALSE);
     // Make sure we have the same font as our stock example
     c_ComboSimple.SetFont(c_DropDown.GetFont());

     //----------------------------------------------------------------
     // Create a dropdown list object
     c_ComboDropDownList.Create(style | CBS_DROPDOWNLIST | WS_VSCROLL, r, 
                                                  this, IDC_DROPLIST_COMBO);
     c_ComboDropDownList.EnableWindow(FALSE);
     // Make sure we have the same font as our stock example
     c_ComboDropDownList.SetFont(c_DropDown.GetFont());

     //----------------------------------------------------------------

     // Get the dropdown size and save it.
     DWORD savedsize = MAKELONG(r.bottom - r.top, r.right - r.left);

     c_Combo->GetDroppedControlRect(&r);
     ::SetWindowLong(c_Combo->m_hWnd, GWL_USERDATA, savedsize);
     ::SetWindowLong(c_ComboDropDownList.m_hWnd, GWL_USERDATA, savedsize);
     c_Messages.initialize(getMaxCtlWidth());
     c_Messages.setWrap(TRUE);

     CheckRadioButton(IDC_SIMPLE, IDC_DROPLIST, IDC_DROPDOWN);
     
     enableControls();

     return TRUE;
    }


/****************************************************************************
*                          CDropDownTest::findNotify
* Inputs:
*       int notification: Notification code
* Result: CString
*       
* Effect: 
*       Creates a temporary CString object
****************************************************************************/

CString CDropDownTest::findNotify(int notification)
    {
     CString s;
     switch(notification)
        { /* notification */
         case CBN_ERRSPACE:
                 s.LoadString(IDS_CBN_ERRSPACE);
                 break;
         case CBN_SELCHANGE:
                 s.LoadString(IDS_CBN_SELCHANGE);
                 break;
         case CBN_DBLCLK:
                 s.LoadString(IDS_CBN_DBLCLK);
                 break;
         case CBN_SETFOCUS:
                 s.LoadString(IDS_CBN_SETFOCUS);
                 break;
         case CBN_KILLFOCUS:
                 s.LoadString(IDS_CBN_KILLFOCUS);
                 break;
         case CBN_EDITCHANGE:
                 s.LoadString(IDS_CBN_EDITCHANGE);
                 break;
         case CBN_EDITUPDATE:
                 s.LoadString(IDS_CBN_EDITUPDATE);
                 break;
         case CBN_DROPDOWN:
                 s.LoadString(IDS_CBN_DROPDOWN);
                 break;
         case CBN_CLOSEUP:
                 s.LoadString(IDS_CBN_CLOSEUP);
                 break;
         case CBN_SELENDOK:
                 s.LoadString(IDS_CBN_SELENDOK);
                 break;
         case CBN_SELENDCANCEL:
                 s.LoadString(IDS_CBN_SELENDCANCEL);
                 break;
        } /* notification */
     return s;
    }

/****************************************************************************
*                         CDropDownTest::getComboType
* Inputs:
*       CComboBox * cb: Combo box object
* Result: CString
*       Style type
****************************************************************************/

CString CDropDownTest::getComboType(CComboBox * cb)
    {
     DWORD style = ::GetWindowLong(cb->m_hWnd, GWL_STYLE);
     CString s;

     switch(style & (CBS_SIMPLE | CBS_DROPDOWN | CBS_DROPDOWNLIST) )
        { /* style */
         case CBS_SIMPLE:
                 s.LoadString(IDS_SIMPLE);
                 break;
         case CBS_DROPDOWN:
                 s.LoadString(IDS_DROPDOWN);
                 break;
         case CBS_DROPDOWNLIST:
                 s.LoadString(IDS_DROPDOWNLIST);
                 break;
        } /* style */
     
     return s;
    }

/****************************************************************************
*                         CDropDownTest::logSelection
* Inputs:
*       CComboBox * cb: Combo box object
* Result: void
*       
* Effect: 
*       Logs the current selection to the log box
****************************************************************************/

void CDropDownTest::logSelection(CComboBox * cb)
    {
     CString s;

     CString CtlName;

     CtlName = getComboType(cb);

     int sel = cb->GetCurSel();
     if(sel >= 0)
        s.Format(_T("GetCurSel => %d"), sel);
     else
        s.LoadString(IDS_NO_SELECTION);

     c_Messages.addMessage(cb->m_hWnd, CtlName, s);

     CString selstr(_T(""));
     DWORD style = ::GetWindowLong(cb->m_hWnd, GWL_STYLE);
     
    }

/****************************************************************************
*                       void CDropDownTest::logNotification
* Inputs:
*       CComboBox * cb: combo box
*       int notification: notification code
* Result: void
*       
* Effect: 
*       Logs the message sent to the control
****************************************************************************/

void CDropDownTest::logNotification(CComboBox * cb, int notification)
    {
     // Format of message:
     // 0000 SIMPLE  CBN_xxxx
     // 0000 DROPDOWN
     // 0000 DROPLIST

     CString s;
     CString msgid = findNotify(notification);


     s = getComboType(cb);
     c_Messages.addMessage(cb->m_hWnd, s, msgid);

     switch(notification)
        { /* notification */
         case CBN_KILLFOCUS:
                 break;
         default:
                 logSelection(cb);
        } /* notification */
    }

/****************************************************************************
*                            CDropDownTest::OnAdd
* Result: void
*       
* Effect: 
*       Adds the string to the combo box
* Notes:
*       This code critically depends upon the fact that the "Add" button
*       will never become enabled for empty text or text which is already
*       in the combo box so that we don't add duplicates.  It unconditionally
*       adds the text, whether duplicate text or even completely empty text.
****************************************************************************/

void CDropDownTest::OnAdd() 
{
 CString text;
 c_Combo->GetWindowText(text);

 c_Combo->AddString(text);
 enableAddDelete();
}

void CDropDownTest::OnCloseupCombo() 
{
 logNotification(c_Combo, CBN_CLOSEUP);
}

void CDropDownTest::OnDblclkCombo() 
{
 logNotification(c_Combo, CBN_DBLCLK);
}

void CDropDownTest::OnEditchangeCombo() 
{
 logNotification(c_Combo, CBN_EDITCHANGE);
}

void CDropDownTest::OnEditupdateCombo() 
{
 logNotification(c_Combo, CBN_EDITUPDATE);
 enableAddDelete();
}

void CDropDownTest::OnErrspaceCombo() 
{
 logNotification(c_Combo, CBN_ERRSPACE);
}

void CDropDownTest::OnKillfocusCombo() 
{
 logNotification(c_Combo, CBN_KILLFOCUS);
}

void CDropDownTest::OnSelchangeCombo() 
{
 logNotification(c_Combo, CBN_SELCHANGE);
 enableAddDelete();
}

void CDropDownTest::OnSelendcancelCombo() 
{
 logNotification(c_Combo, CBN_SELENDCANCEL);
 enableAddDelete();
}

void CDropDownTest::OnSelendokCombo() 
{
 logNotification(c_Combo, CBN_SELENDOK);
 enableAddDelete();
}

void CDropDownTest::OnSetfocusCombo() 
{
 logNotification(c_Combo, CBN_SETFOCUS);
 enableAddDelete();
}

/****************************************************************************
*                       CDropDownTest::OnDropdownCombo
* Result: void
*       
* Effect: 
*       Computes the required size of the dropdown list.  This size is then
*       computed against the original size of the combo box which has been
*       stored in the GWL_USERDATA word.  The combo box will not be extended
*       beyond the original size.  It will be at least tall enough to hold
*       one object. 
* Notes:
*       This is written more like C code so it can be used as an example
*       in the book.
****************************************************************************/

void CDropDownTest::OnDropdownCombo() 
{
    logNotification(c_Combo, CBN_DROPDOWN);

    HWND hDlg = m_hWnd; // MFC
    HWND combo = c_Combo->m_hWnd;
    int items;
    int lineheight;
    HDC dc;
    TEXTMETRIC tm;
    RECT r;
    int required_height;
    int permitted_height;
    CFont * font;

    // Compute the size of the combo box
    // This assumes that the combo box is NOT 
    // owner-draw!
    items = c_Combo->GetCount();
    dc = ::GetDC(combo);
    font = c_Combo->GetFont();
    if(font != NULL)
        ::SelectObject(dc, font->m_hObject);
    ::GetTextMetrics(dc, &tm);
    ::ReleaseDC(combo, dc);
    lineheight = tm.tmHeight + tm.tmExternalLeading;
    required_height = (2 + max(items, 1)) * lineheight;
    permitted_height = (int)LOWORD(GetWindowLong(combo, GWL_USERDATA));

    c_Combo->SetDroppedWidth((int)HIWORD(GetWindowLong(combo, GWL_USERDATA)));

    // If the required height is less than or equal to the saved height,
    // use the required height.  If it is greater, use the
    // saved height
    c_Combo->GetDroppedControlRect(&r);
    ScreenToClient(&r);
    if(required_height <= permitted_height && c_SmartDropdown.GetCheck())
        { /* smaller than default setting */
             r.bottom = r.top + required_height;
        } /* smaller than default setting */
    else
        { /* larger than default setting */
             r.bottom = r.top + permitted_height;
        } /* larger than default setting */

    c_Combo->MoveWindow(r.left, r.top,
                     r.right - r.left, r.bottom - r.top, TRUE);
}


void CDropDownTest::OnDelete() 
{
 int sel = c_Combo->GetCurSel();
 if(sel == CB_ERR)
         return;
 c_Combo->DeleteString(sel);
 enableAddDelete();     
}

/****************************************************************************
*                           CDropDownTest::changeTo
* Inputs:
*       CComboBox * newCombo: Combo box to switch to
* Result: void
*       
* Effect: 
*       Switches the visible combo box.  Since the combo boxes already exist
*       this is no major hassle.  Copies all the strings from the source
*       combo box to the new combo box so it looks like we just switched shape
****************************************************************************/

void CDropDownTest::changeTo(CComboBox * newCombo)
    {
     CComboBox * source = c_Combo;      // save original value
     
     c_ComboSimple.ShowWindow(newCombo->m_hWnd == c_ComboSimple.m_hWnd
                                                          ? SW_SHOW : SW_HIDE);
     c_ComboSimple.EnableWindow(c_Enable.GetCheck() &&
                                newCombo->m_hWnd == c_ComboSimple.m_hWnd);

     c_ComboDropDown.ShowWindow(newCombo->m_hWnd == c_ComboDropDown.m_hWnd
                                                          ? SW_SHOW : SW_HIDE);
     c_ComboDropDown.EnableWindow(c_Enable.GetCheck() &&
                                   newCombo->m_hWnd == c_ComboDropDown.m_hWnd);

     c_ComboDropDownList.ShowWindow(
                                 newCombo->m_hWnd == c_ComboDropDownList.m_hWnd
                                        ? SW_SHOW : SW_HIDE);
     c_ComboDropDownList.EnableWindow(c_Enable.GetCheck() && 
                               newCombo->m_hWnd == c_ComboDropDownList.m_hWnd);

     c_Combo = newCombo;
     enableControls();
     copyStrings(source);
     
    }

void CDropDownTest::OnDropdown() 
{
 if(c_Combo->m_hWnd == c_ComboDropDown.m_hWnd)
    return;  // redundant click on radio button

 changeTo(&c_ComboDropDown);
}

void CDropDownTest::OnDroplist() 
{
 if(c_Combo->m_hWnd == c_ComboDropDownList.m_hWnd)
    return;  // redundant click on radio button

 changeTo(&c_ComboDropDownList);
}

void CDropDownTest::OnSimple() 
{
 if(c_Combo->m_hWnd == c_Simple.m_hWnd)
    return;  // redundant click on radio button

 changeTo(&c_ComboSimple);
}

void CDropDownTest::OnWide() 
{
        DWORD ht_wd = ::GetWindowLong(c_Combo->m_hWnd,
                                                GWL_USERDATA);

        if(c_Wide.GetCheck())
           { /* wide dropdown */
                ht_wd = MAKELONG(LOWORD(ht_wd),
                                     HIWORD(ht_wd) * 2);
           } /* wide dropdown */
        else
           { /* narrow dropdown */
                ht_wd = MAKELONG(LOWORD(ht_wd),
                                     HIWORD(ht_wd) / 2);
           } /* narrow dropdown */
        ::SetWindowLong(c_Combo->m_hWnd, GWL_USERDATA, ht_wd);
}

void CDropDownTest::OnExtendedui() 
{
 c_Combo->SetExtendedUI(c_ExtendedUI.GetCheck());
        
}

/****************************************************************************
*                                   isBlank
* Inputs:
*       LPCTSTR s: Pointer to string
* Result: BOOL
*       TRUE if string is empty or entirely whitespace
*       FALSE if string is nonempty
****************************************************************************/

BOOL isBlank(LPCTSTR s)
    {
     LPCTSTR p = s;
     while(*p)
        { /* check blank */
         if(*p != _T(' ') && *p != _T('\t'))
            return FALSE;
         p++;
        } /* check blank */
     return TRUE;
    }

/****************************************************************************
*                           CDropDownTest::hasEdit
* Result: BOOL
*       TRUE if box has edit control
*       FALSE if box has no edit control
****************************************************************************/

BOOL CDropDownTest::hasEdit()
    {
     DWORD style = ::GetWindowLong(c_Combo->m_hWnd, GWL_STYLE);

     switch(style & (CBS_SIMPLE | CBS_DROPDOWN | CBS_DROPDOWNLIST) )
        { /* style */
         case CBS_SIMPLE:
                 return TRUE;
         case CBS_DROPDOWN:
                 return TRUE;
         case CBS_DROPDOWNLIST:
                 return FALSE;
        } /* style */
     return FALSE; // should never get here
    }

/****************************************************************************
*                           CDropDownTest::hasDrop
* Result: BOOL
*       TRUE if combo box has dropdow
*       FALSE if combo box doesn't have dropdown
****************************************************************************/

BOOL CDropDownTest::hasDrop()
    {
     DWORD style = ::GetWindowLong(c_Combo->m_hWnd, GWL_STYLE);

     switch(style & (CBS_SIMPLE | CBS_DROPDOWN | CBS_DROPDOWNLIST) )
        { /* style */
         case CBS_SIMPLE:
                 return FALSE;
         case CBS_DROPDOWN:
                 return TRUE;
         case CBS_DROPDOWNLIST:
                 return TRUE;
        } /* style */
     return FALSE; // should never get here
    }

/****************************************************************************
*                       CDropDownTest::enableAddDelete
* Result: void
*       
* Effect: 
*       Enables the Add button and Delete button
* Notes:
*       To enable the Add button, the following must be true:
*               - the control must have an edit control
*               - the text of the edit control must be non-blank
*               - the text must not already be in the list
*       To enable the Delete button
*               - the control must have one or more items
*               - there must be a current selection
****************************************************************************/

void CDropDownTest::enableAddDelete()
    {
     BOOL validtext = FALSE;
     CString text;
     c_Combo->GetWindowText(text);

     if(!isBlank(text))
        { /* check text */
         if(c_Combo->FindStringExact(-1, text) == CB_ERR)
            { /* not present */
             validtext = TRUE;
            } /* not present */
        } /* check text */

     c_Add.EnableWindow(hasEdit() && validtext);

     c_Delete.EnableWindow(c_Combo->GetCount() > 0 && 
                           c_Combo->GetCurSel() != CB_ERR);
    }

/****************************************************************************
*                        CDropDownTest::enableControls
* Result: void
*       
* Effect: 
*       Enables or disables relevant controls
****************************************************************************/

void CDropDownTest::enableControls()
    {
     BOOL hasdrop = hasDrop();
     BOOL hasedit = hasEdit();

     c_Wide.EnableWindow(hasdrop && !Is3());
     c_ExtendedUI.EnableWindow(hasdrop);

     enableAddDelete();
    }

void CDropDownTest::OnEnable() 
{
 c_Combo->EnableWindow((BOOL)c_Enable.GetCheck());
        
}

void CDropDownTest::OnSendmessages() 
{
 CMessageSheet sheet(IDS_SEND_DROPDOWN_MESSAGES);
 CComboIndex ndx;
 CDDState ddstate;
 CDDText  ddtext;
 CDDEffects ddeffects;

 sheet.AddPage(&ndx);
 sheet.AddPage(&ddstate);
 sheet.AddPage(&ddtext);
 sheet.AddPage(&ddeffects);

 ddstate.c_Messages = &c_Messages;
 ddstate.combo = c_Combo;

 ddtext.c_Messages = &c_Messages;
 ddtext.combo = c_Combo;

 ddeffects.c_Messages = &c_Messages;
 ddeffects.combo = c_Combo;

 CRect r;
 GetParent()->GetWindowRect(&r);
 GetParent()->SetWindowPos(NULL, 0, 
                        ::GetSystemMetrics(SM_CYSCREEN) - r.Height(), 0, 0, 
                        SWP_NOSIZE | SWP_NOZORDER);
                        
 sheet.DoModal();
}
