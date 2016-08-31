// ListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ListBox.h"
#include "dlgcodes.h"

#include "indexpg.h"
#include "ixable.h"
#include "loggable.h"
#include "listops.h"

#include "IntEdit.h"
#include "listndx.h"
#include "TinyButt.h"
#include "NumList.h"

#include "locales.h"
#include "ListStr.h"
#include "ListStat.h"
#include "ListQ.h"
#include "ListDir.h"
#include "ListRect.h"
#include "ListTabs.h"

#include "msgsheet.h"
#include "animate.h"
#include "xform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof(x[0]))

#ifdef UNICODE
#define HEXCHAR _T("%04x")
#else
#define HEXCHAR _T("%02x")
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxTest property page

IMPLEMENT_DYNCREATE(CListBoxTest, CPropertyPage)

CListBoxTest::CListBoxTest() : CPropertyPage(CListBoxTest::IDD)
{
        //{{AFX_DATA_INIT(CListBoxTest)
        //}}AFX_DATA_INIT
}

CListBoxTest::~CListBoxTest()
{
}

void CListBoxTest::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CListBoxTest)
        DDX_Control(pDX, IDC_NO_ANIMATION, c_NoAnimation);
        DDX_Control(pDX, IDC_LBS_MULTICOLUMN, c_LBSMultiColumn);
        DDX_Control(pDX, IDC_WS_BORDER, c_WSBorder);
        DDX_Control(pDX, IDC_ENABLED, c_Enable);
        DDX_Control(pDX, IDC_LBS_NOTIFY, c_LBSNotify);
        DDX_Control(pDX, IDC_LBS_NOINTEGRALHEIGHT, c_LBSNoIntegralHeight);
        DDX_Control(pDX, IDC_LBS_SORT, c_LBSSort);
        DDX_Control(pDX, IDC_LBS_USETABSTOPS, c_LBSUseTabStops);
        DDX_Control(pDX, IDC_LBS_WANTKEYBOARDINPUT, c_LBSWantKeyboardInput);
        DDX_Control(pDX, IDC_LBS_DISABLENOSCROLL, c_LBSDisableNoScroll);
        DDX_Control(pDX, IDC_SINGLE_CAPTION, c_c_Single);
        DDX_Control(pDX, IDC_NONE_CAPTION, c_c_None);
        DDX_Control(pDX, IDC_MULTIPLE_CAPTION, c_c_Multiple);
        DDX_Control(pDX, IDC_EXTENDED_CAPTION, c_c_Extended);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_SINGLE, c_Single);
        DDX_Control(pDX, IDC_NONE, c_None);
        DDX_Control(pDX, IDC_MULTIPLE, c_Multiple);
        DDX_Control(pDX, IDC_EXTENDED, c_Extended);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListBoxTest, CPropertyPage)
        //{{AFX_MSG_MAP(CListBoxTest)
        ON_WM_TIMER()
        ON_LBN_DBLCLK(IDC_EXTENDED, OnDblclkExtended)
        ON_LBN_KILLFOCUS(IDC_EXTENDED, OnKillfocusExtended)
        ON_LBN_SELCANCEL(IDC_EXTENDED, OnSelcancelExtended)
        ON_LBN_SELCHANGE(IDC_EXTENDED, OnSelchangeExtended)
        ON_LBN_SETFOCUS(IDC_EXTENDED, OnSetfocusExtended)
        ON_LBN_DBLCLK(IDC_MULTIPLE, OnDblclkMultiple)
        ON_LBN_KILLFOCUS(IDC_MULTIPLE, OnKillfocusMultiple)
        ON_LBN_SELCANCEL(IDC_MULTIPLE, OnSelcancelMultiple)
        ON_LBN_SELCHANGE(IDC_MULTIPLE, OnSelchangeMultiple)
        ON_LBN_SETFOCUS(IDC_MULTIPLE, OnSetfocusMultiple)
        ON_LBN_DBLCLK(IDC_NONE, OnDblclkNone)
        ON_LBN_KILLFOCUS(IDC_NONE, OnKillfocusNone)
        ON_LBN_SELCANCEL(IDC_NONE, OnSelcancelNone)
        ON_LBN_SELCHANGE(IDC_NONE, OnSelchangeNone)
        ON_LBN_SETFOCUS(IDC_NONE, OnSetfocusNone)
        ON_LBN_DBLCLK(IDC_SINGLE, OnDblclkSingle)
        ON_LBN_KILLFOCUS(IDC_SINGLE, OnKillfocusSingle)
        ON_LBN_SELCANCEL(IDC_SINGLE, OnSelcancelSingle)
        ON_LBN_SELCHANGE(IDC_SINGLE, OnSelchangeSingle)
        ON_LBN_SETFOCUS(IDC_SINGLE, OnSetfocusSingle)
        ON_WM_VKEYTOITEM()
        ON_WM_CHARTOITEM()
        ON_BN_CLICKED(IDC_ENABLED, OnEnable)
        ON_BN_CLICKED(IDC_SENDMESSAGES, OnSendmessages)
        ON_BN_CLICKED(IDC_LBS_DISABLENOSCROLL, OnLbsDisablenoscroll)
        ON_BN_CLICKED(IDC_LBS_MULTICOLUMN, OnLbsMulticolumn)
        ON_BN_CLICKED(IDC_LBS_NOINTEGRALHEIGHT, OnLbsNointegralheight)
        ON_BN_CLICKED(IDC_LBS_NOTIFY, OnLbsNotify)
        ON_BN_CLICKED(IDC_LBS_SORT, OnLbsSort)
        ON_BN_CLICKED(IDC_LBS_USETABSTOPS, OnLbsUsetabstops)
        ON_BN_CLICKED(IDC_LBS_WANTKEYBOARDINPUT, OnLbsWantkeyboardinput)
        ON_BN_CLICKED(IDC_WS_BORDER, OnWsBorder)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxTest message handlers

/****************************************************************************
*                         CChap8Dlg::addSampleStrings
* Inputs:
*       int ctl: Control ID
*       int sortctl: Sort control specification
* Result: void
*       
* Effect: 
*       Loads sample controls
****************************************************************************/

void CListBoxTest::addSampleStrings(int ctl, int sortctl)
{
    CListBox * lb = (CListBox *)GetDlgItem(ctl);
    CButton * sb = NULL;
    if(sortctl != 0)
        sb = (CButton *)GetDlgItem(sortctl);
    DWORD style = GetWindowLong(lb->m_hWnd, GWL_STYLE);
    lb->ResetContent();
    for(int i = IDS_FIRST_SAMPLE; i < IDS_LAST_SAMPLE; i++)
        { /* add string */
         CString text;
         if(text.LoadString(i) == 0)
             continue;
         int Index = lb->AddString(text);
         lb->SetItemData(Index, i);
        } /* add string */
     if(sb != NULL)
         { /* set check */
          sb->SetCheck(style & LBS_SORT ? 1 : 0);
         } /* set check */ 

}

/****************************************************************************
*                           CDropDowTest::setParms
* Inputs:
*       int index: Index to set
*       CListBox * lb: List box to set up
*       CStatic * lbl: List box label
* Result: void
*       
* Effect: 
*       Initializes the table used for the control descriptions
****************************************************************************/

void CListBoxTest::setParms(int index, CListBox * lb, CStatic * lbl)
    {
     CClientDC dc(&c_Messages);

     ctls[index].lb = lb;
     ctls[index].CtlID = lb->GetDlgCtrlID();
     lbl->GetWindowText(ctls[index].CtlName);
     CSize sz;
     sz = dc.GetTextExtent(ctls[index].CtlName);
     ctls[index].width = sz.cx;
    }

/****************************************************************************
*                        CListBoxTest::getMaxCtlWidth
* Result: int
*       
* Effect: 
*       Returns the width of the largest ctldesc string
* Notes:
*       Cannot be called before setParms is used to load the table
****************************************************************************/

int CListBoxTest::getMaxCtlWidth()
    {
     int width = 0;

     for(int i = 0; i < DIM(ctls); i++)
        { /* check each */
         width = max(width, c_Messages.GetStringWidth(ctls[i].CtlName));
        } /* check each */

     return width;
    }

/****************************************************************************
*                         CListBoxTest::OnInitDialog
* Result: BOOL
*       TRUE, always
* Effect: 
*       Initializes the various ListBox lists
****************************************************************************/

BOOL CListBoxTest::OnInitDialog()
    {
     CPropertyPage::OnInitDialog();

        addSampleStrings(IDC_SINGLE, IDC_SINGLE_SORT);
        addSampleStrings(IDC_MULTIPLE, 0);
        addSampleStrings(IDC_EXTENDED, 0);
        addSampleStrings(IDC_NONE, 0);
     
        setParms(0, &c_Single, &c_c_Single);
        setParms(1, &c_Extended, &c_c_Extended);
        setParms(2, &c_Multiple, &c_c_Multiple);
        setParms(3, &c_None, &c_c_None);

        // The generic listbox styles are the same for all the list boxes.
        // We can set the controls based on just c_Single

        c_LBSMultiColumn.SetCheck(c_Single.GetStyle() & LBS_MULTICOLUMN 
                                                           ? TRUE : FALSE);
        c_LBSNotify.SetCheck(c_Single.GetStyle() & LBS_NOTIFY 
                                                           ? TRUE : FALSE);
        c_LBSNoIntegralHeight.SetCheck(c_Single.GetStyle() & LBS_NOINTEGRALHEIGHT 
                                                           ? TRUE : FALSE);
        c_LBSSort.SetCheck(c_Single.GetStyle() & LBS_SORT
                                                           ? TRUE : FALSE);
        c_LBSUseTabStops.SetCheck(c_Single.GetStyle() & LBS_USETABSTOPS
                                                           ? TRUE : FALSE);

        c_LBSWantKeyboardInput.SetCheck(c_Single.GetStyle() & LBS_WANTKEYBOARDINPUT
                                                           ? TRUE : FALSE);

        c_LBSDisableNoScroll.SetCheck(c_Single.GetStyle() & LBS_DISABLENOSCROLL
                                                           ? TRUE : FALSE);
        c_Enable.SetCheck(TRUE);
        // Because we may have been subclassed by CTL3D, we probably have
        // other than the standard font selected.  Save this in case we
        // need a font again.  We need only one of the fonts because all
        // the boxes are the same from the font perspective.
        orgfont = c_Single.GetFont();

        c_Messages.initialize(getMaxCtlWidth());
        c_Messages.setWrap(TRUE);

        return TRUE;
    }

void CListBoxTest::OnDblclkExtended() 
{
 logNotification(IDC_EXTENDED, LBN_DBLCLK);
        
}

void CListBoxTest::OnKillfocusExtended() 
{
 logNotification(IDC_EXTENDED, LBN_KILLFOCUS);
        
}

void CListBoxTest::OnSelcancelExtended() 
{
 logNotification(IDC_EXTENDED, LBN_SELCANCEL);
        
}

void CListBoxTest::OnSelchangeExtended() 
{
 logNotification(IDC_EXTENDED, LBN_SELCHANGE);
        
}

void CListBoxTest::OnSetfocusExtended() 
{
 logNotification(IDC_EXTENDED, LBN_SETFOCUS);
        
}

void CListBoxTest::OnDblclkMultiple() 
{
 logNotification(IDC_MULTIPLE, LBN_DBLCLK);
        
}

void CListBoxTest::OnKillfocusMultiple() 
{
 logNotification(IDC_MULTIPLE, LBN_KILLFOCUS);
        
}

void CListBoxTest::OnSelcancelMultiple() 
{
 logNotification(IDC_MULTIPLE, LBN_SELCANCEL);
        
}

void CListBoxTest::OnSelchangeMultiple() 
{
 logNotification(IDC_MULTIPLE, LBN_SELCHANGE);
        
}

void CListBoxTest::OnSetfocusMultiple() 
{
 logNotification(IDC_MULTIPLE, LBN_SETFOCUS);
        
}

void CListBoxTest::OnDblclkNone() 
{
 logNotification(IDC_NONE, LBN_DBLCLK);
        
}

void CListBoxTest::OnKillfocusNone() 
{
 logNotification(IDC_NONE, LBN_KILLFOCUS);
        
}

void CListBoxTest::OnSelcancelNone() 
{
 logNotification(IDC_NONE, LBN_SELCANCEL);
        
}

void CListBoxTest::OnSelchangeNone() 
{
 logNotification(IDC_NONE, LBN_SELCHANGE);
        
}

void CListBoxTest::OnSetfocusNone() 
{
 logNotification(IDC_NONE, LBN_SETFOCUS);
        
}

void CListBoxTest::OnDblclkSingle() 
{
 logNotification(IDC_SINGLE, LBN_DBLCLK);
        
}

void CListBoxTest::OnKillfocusSingle() 
{
 logNotification(IDC_SINGLE, LBN_KILLFOCUS);
        
}

void CListBoxTest::OnSelcancelSingle() 
{
 logNotification(IDC_SINGLE, LBN_SELCANCEL);
        
}

void CListBoxTest::OnSelchangeSingle() 
{
 logNotification(IDC_SINGLE, LBN_SELCHANGE);
        
}

void CListBoxTest::OnSetfocusSingle() 
{
 logNotification(IDC_SINGLE, LBN_SETFOCUS);
        
}

/****************************************************************************
*                           CListBoxTest::findCtl
* Inputs:
*       int id: Control id
* Result: ctldesc *
*       Reference to control display parameters, or NULL if error
****************************************************************************/

ctldesc * CListBoxTest::findCtl(int id)
    {
     for(int i = 0; i < DIM(ctls); i++)
        { /* scan it */
         if(ctls[i].CtlID == id)
            return &ctls[i];
        } /* scan it */
     return NULL;
    }

/****************************************************************************
*                          CListBoxTest::findNotify
* Inputs:
*       int notification: Notification code
* Result: CString
*       
* Effect: 
*       Creates a temporary CString object
****************************************************************************/

CString CListBoxTest::findNotify(int notification)
    {
     CString s;
     switch(notification)
        { /* notification */
         case LBN_ERRSPACE:
                 s.LoadString(IDS_LBN_ERRSPACE);
                 break;
         case LBN_SELCHANGE:
                 s.LoadString(IDS_LBN_SELCHANGE);
                 break;
         case LBN_DBLCLK:
                 s.LoadString(IDS_LBN_DBLCLK);
                 break;
         case LBN_SELCANCEL:
                 s.LoadString(IDS_LBN_SELCANCEL);
                 break;
         case LBN_SETFOCUS:
                 s.LoadString(IDS_LBN_SETFOCUS);
                 break;
         case LBN_KILLFOCUS:
                 s.LoadString(IDS_LBN_KILLFOCUS);
                 break;
        } /* notification */
     return s;
    }

/****************************************************************************
*                         CListBoxTest::logSelection
* Inputs:
*       ctldesc * desc: control descriptor
* Result: void
*       
* Effect: 
*       Logs the current selection to the log box
****************************************************************************/

void CListBoxTest::logSelection(ctldesc * desc)
    {
     CString s;

     int sel = desc->lb->GetCurSel();
     if(sel != LB_ERR)
        s.Format(_T("GetCurSel => %d"),sel); 
     else
        s.LoadString(IDS_NO_SELECTION);

     c_Messages.addMessage(desc->lb->m_hWnd, desc->CtlName, s);

     // if it is a multiselect box, show the multiselections
     CString selstr(_T(""));
     DWORD style = ::GetWindowLong(desc->lb->m_hWnd, GWL_STYLE);
     
     if(style & LBS_NOSEL)
        return;  // no selections

     if(style & LBS_EXTENDEDSEL || style & LBS_MULTIPLESEL)
        { /* multiselect */
         int selcount = desc->lb->GetSelCount();
         if(selcount == 0)
            { /* no selections */
             s.Format(_T("GetSelCount=> %d"), selcount);
             c_Messages.addMessage(desc->lb->m_hWnd, desc->CtlName, s);
             return;
            } /* no selections */
         else
            { /* has selections */
             CString selections(_T(""));
             CString sep = _T("");

             int * sels = new(int[selcount]);
             desc->lb->GetSelItems(selcount, sels);
             for(int i = 0; i < selcount; i++)
                { /* form string */
                 CString t;
                 t.Format(_T("%s%d"), sep, sels[i]);
                 selections += t;
                 sep = _T(", ");
                } /* form string */
             delete sels;

             s.Format(_T("GetSelCount=> %d: [%s]"),
                                                selcount,
                                                selections);

             c_Messages.addMessage(desc->lb->m_hWnd, desc->CtlName, s);
            } /* has selections */
        } /* multiselect */
    }

/****************************************************************************
*                          CListBoxTest::logToItem
* Inputs:
*       int msg: WM_CHARTOITEM or WM_VKEYTOITEM
*       UINT key: key or VK
*       CListBox * lb: List box
*       UINT nIndex: Index
*       int result: Result of calling parent
* Result: 
*       void
* Effect: 
*       Logs the message
****************************************************************************/

void CListBoxTest::logToItem(int msg, UINT key, CListBox * lb, UINT nIndex, int result)
    {
     int cmd;

     switch(msg)
        { /* msg */
         case WM_VKEYTOITEM:
                 cmd = IDS_WM_VKEYTOITEM;
                 break;
         case WM_CHARTOITEM:
                 cmd = IDS_WM_CHARTOITEM;
                 break;
        } /* msg */

     CString s;
     ctldesc * desc = findCtl(lb->GetDlgCtrlID());
     showResult_vkey_LBERR(cmd, lb, desc->CtlName, key, nIndex, result);
    }

/****************************************************************************
*                       void CListBoxTest::logNotification
* Inputs:
*       int id: control id
*       int notification: notification code
* Result: void
*       
* Effect: 
*       Logs the message sent to the control
****************************************************************************/

void CListBoxTest::logNotification(int id, int notification)
    {
     // Format of message:
     // 0000 MULTIPLE  LBN_xxxx
     // 0000 SINGLE
     // 0000 EXTENDED
     // 0000 NONE      LBN_xxxx

     ctldesc * desc = findCtl(id);
     if(desc == NULL)
        return; // ignore messages until startup completes

     CString s;
     CString msgid = findNotify(notification);
     
     c_Messages.addMessage(desc->lb->m_hWnd, desc->CtlName, msgid);;

     switch(notification)
        { /* notification */
         case LBN_SELCHANGE:
                 logSelection(desc);
                 break;
         case LBN_SELCANCEL:
         case LBN_DBLCLK:
         case LBN_SETFOCUS:
                 logSelection(desc);
                 break;
         case LBN_KILLFOCUS:
                 break;
        } /* notification */
    }

int CListBoxTest::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
        int result =  CPropertyPage::OnVKeyToItem(nKey, pListBox, nIndex);

        logToItem(WM_VKEYTOITEM, nKey, pListBox, nIndex, result);

        return result;
}

int CListBoxTest::OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex) 
{
        int result = CPropertyPage::OnCharToItem(nChar, pListBox, nIndex);

        logToItem(WM_CHARTOITEM, nChar, pListBox, nIndex, result);

        return result;
}

void CListBoxTest::OnSendmessages() 
{
 CMessageSheet sheet(IDS_LISTBOX_MESSAGES);

 CListIndex ndx;
 CListStrings str;
 CListState state;
 CListQuery q;
 CListDir   dir;
 CListboxRect  rect;
 CListTabs tabs;

 str.c_Messages = &c_Messages;
 str.c_Single = &c_Single;
 str.c_Multiple = &c_Multiple;
 str.c_Extended = &c_Extended;
 str.c_None = &c_None;
 
 state.c_Messages = &c_Messages;
 state.c_Single = &c_Single;
 state.c_Multiple = &c_Multiple;
 state.c_Extended = &c_Extended;
 state.c_None = &c_None;
 
 q.c_Messages = &c_Messages;
 q.c_Single = &c_Single;
 q.c_Multiple = &c_Multiple;
 q.c_Extended = &c_Extended;
 q.c_None = &c_None;
 
 rect.c_Messages = &c_Messages;
 rect.c_Single = &c_Single;
 rect.c_Multiple = &c_Multiple;
 rect.c_Extended = &c_Extended;
 rect.c_None = &c_None;
 
 dir.c_Messages = &c_Messages;
 dir.c_Single = &c_Single;
 dir.c_Multiple = &c_Multiple;
 dir.c_Extended = &c_Extended;
 dir.c_None = &c_None;
 
 tabs.c_Messages = &c_Messages;
 tabs.c_Single = &c_Single;
 tabs.c_Multiple = &c_Multiple;
 tabs.c_Extended = &c_Extended;
 tabs.c_None = &c_None;

 sheet.AddPage(&ndx);
 sheet.AddPage(&str);
 sheet.AddPage(&state);
 sheet.AddPage(&q);
 sheet.AddPage(&rect);
 sheet.AddPage(&dir);
 sheet.AddPage(&tabs);

 // Move us down to the bottom left corner
 CRect r;
 GetParent()->GetWindowRect(&r);
 GetParent()->SetWindowPos(NULL, 0, 
                        ::GetSystemMetrics(SM_CYSCREEN) - r.Height(), 0, 0, 
                        SWP_NOSIZE | SWP_NOZORDER);
 sheet.DoModal();
        
}

void CListBoxTest::OnLbsDisablenoscroll() 
{
 recreateWindows();
}

void CListBoxTest::OnLbsMulticolumn() 
{
 recreateWindows();
}

void CListBoxTest::OnLbsNointegralheight() 
{
 recreateWindows();
}

void CListBoxTest::OnLbsNotify() 
{
 recreateWindows();
}

void CListBoxTest::OnLbsSort() 
{
 recreateWindows();
}

void CListBoxTest::OnLbsUsetabstops() 
{
 recreateWindows();
}

void CListBoxTest::OnLbsWantkeyboardinput() 
{
 recreateWindows();
}

void CListBoxTest::OnWsBorder() 
{
 recreateWindows();
}

void CListBoxTest::OnEnable() 
{
 c_Single.EnableWindow(c_Enable.GetCheck());
 c_Multiple.EnableWindow(c_Enable.GetCheck());
 c_Extended.EnableWindow(c_Enable.GetCheck());
 c_None.EnableWindow(c_Enable.GetCheck());
}

/****************************************************************************
*                         CListBoxTest::recreateWindow
* Inputs:
*       CButton * button: Button to recreate
*       int id: ID to display in log
*       DWORD style: Styles to re-create it
* Result: void
*       
* Effect: 
*       Creates a new window whose style is specified by the 'style' 
*       member, less the visibility flag.  For visual effect, we display
*       some cute icons to indicate that the window is being reconstructed
*       so the reconstruction is evident.  This visual effect is purely
*       gratuitous.
* Notes:
*       We save the current selection so we can reset it.  We also save the
*       font that was used because we want to make sure the same font is
*       used in the new window.
****************************************************************************/

void CListBoxTest::recreateWindow(CListBox * box, int id, DWORD style)
    {
     CListBox newWindow;
     CFont * font;
     UINT dlgcodes;
     CRect rect;

     box->GetWindowRect(&rect);
     ScreenToClient(&rect);

     dlgcodes = getDlgCode(box->m_hWnd);

     CWnd * oldparent = box->GetParent();

     font = box->GetFont();

     // Add in the orthogonal style bits
     // WS_BORDER
     // WS_DISABLED

     // LBS_NOTIFY
     // LBS_SORT
     // LBS_USETABSTOPS
     // LBS_NOINTEGRALHEIGHT
     // LBS_MULTICOLUMN
     // LBS_WANTKEYBOARDINPUT
     // LBS_DISABLENOSCROLL

     if(c_WSBorder.GetCheck())
        style |= WS_BORDER;
     else
        style &= ~WS_BORDER;

     if(!c_Enable.GetCheck())
        style |= WS_DISABLED;
     else
        style &= ~WS_DISABLED;

     if(c_LBSNotify.GetCheck())
        style |= LBS_NOTIFY;
     else
        style &= ~LBS_NOTIFY;

     if(c_LBSMultiColumn.GetCheck())
        style |= LBS_MULTICOLUMN;
     else
        style &= ~LBS_MULTICOLUMN;

     if(c_LBSNoIntegralHeight.GetCheck())
        style |= LBS_NOINTEGRALHEIGHT;
     else
        style &= ~LBS_NOINTEGRALHEIGHT;

     if(c_LBSSort.GetCheck())
        style |= LBS_SORT;
     else
        style &= ~LBS_SORT;

     if(c_LBSUseTabStops.GetCheck())
        style |= LBS_USETABSTOPS;
     else
        style &= ~LBS_USETABSTOPS;

     if(c_LBSWantKeyboardInput.GetCheck())
        style |= LBS_WANTKEYBOARDINPUT;
     else
        style &= ~LBS_WANTKEYBOARDINPUT;

     if(c_LBSDisableNoScroll.GetCheck())
        style |= LBS_DISABLENOSCROLL;
     else
        style &= ~LBS_DISABLENOSCROLL;

     int ctlid = box->GetDlgCtrlID();

     style |= WS_CHILD;

     // We create it without the visible style so we can play the
     // animation game later.
     
     newWindow.Create(style & ~WS_VISIBLE, rect, box->GetParent(), ctlid);

     // we've created it, log this effect
     CString t;
     t.LoadString(id);
     
     // Lie about the style by adding in the visible style, since the
     // omission is a hack for visual effect in the Explorer.

     CString s;

     s.Format(_T("0x%08x"), style | WS_VISIBLE); 

     c_Messages.addMessage(newWindow.m_hWnd, t, s);

     // Copy the strings from the old to the new

     for(int i = 0; i < box->GetCount(); i++)
        { /* copy across */
         CString s;
         DWORD itemdata;

         box->GetText(i, s);
         itemdata = box->GetItemData(i);

         int index = newWindow.AddString(s);
         newWindow.SetItemData(i, itemdata);
        } /* copy across */

     // Now that we've copied the contents, we can delete the old window

     HWND oldwnd = box->UnsubclassWindow();
     ::DestroyWindow(oldwnd);

     HWND wnd;
        wnd = newWindow.UnsubclassWindow();

     // SubclassWindow will set up all the right pointers.  We need to
     // do this because different types of edit windows subclass to different
     // handlers.
     box->SubclassWindow(wnd);

     if(font != NULL)
        box->SetFont(font);
     else
        box->SetFont(orgfont);

     if(font != NULL)
        orgfont = font;

     setDlgCode(box->m_hWnd, dlgcodes);

    }

/****************************************************************************
*                        CListBoxTest::recreateWindows
* Result: void
*       
* Effect: 
*       Re-creates all button windows
****************************************************************************/

void CListBoxTest::recreateWindows()
    {
     recreateWindow(&c_Single, IDS_SINGLE, c_Single.GetStyle());
     recreateWindow(&c_Multiple, IDS_MULTIPLE, c_Multiple.GetStyle());
     recreateWindow(&c_Extended, IDS_EXTENDED, c_Extended.GetStyle());
     recreateWindow(&c_None, IDS_NONE, c_None.GetStyle());
     
     if(c_NoAnimation.GetCheck())
        { /* just do it */
         c_Single.ShowWindow(SW_SHOW);
         c_Multiple.ShowWindow(SW_SHOW);
         c_Extended.ShowWindow(SW_SHOW);
         c_None.ShowWindow(SW_SHOW);
        } /* just do it */
     else
        { /* animate */
         c_Single.ShowWindow(SW_HIDE);
         c_Multiple.ShowWindow(SW_HIDE);
         c_Extended.ShowWindow(SW_HIDE);
         c_None.ShowWindow(SW_HIDE);

         // Cute effect for demonstration of reconstruction

         beginAnimation(&c_Single);
        } /* animate */
    }

void CListBoxTest::OnTimer(UINT nIDEvent) 
{
        endAnimation(&c_Single);

        c_Multiple.ShowWindow(SW_SHOW);
        c_Extended.ShowWindow(SW_SHOW);
        c_None.ShowWindow(SW_SHOW);

        CPropertyPage::OnTimer(nIDEvent);
}


/****************************************************************************
*                     CListBoxTest::showResult_vkey_LBERR
* Inputs:
*       int msg: ID of message
*       CListBox * lb:
*       CString CtlName: control name
*       TCHAR w: character value (LOWORD wParam)
*       int pos: caret pos (HIWORD wParam)
*       int result: result value of executing it
* Result: void
*       
* Effect: 
*       Displays a char,int=>int message
****************************************************************************/

void CListBoxTest::showResult_vkey_LBERR(int msg, CListBox * lb, 
                                                CString CtlName, TCHAR w, 
                                                int pos, int result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(" HEXCHAR "='%s', %d) => %d"), 
                        t, (TCHAR)w, charToStr(w), pos, result);
     c_Messages.addMessage(lb->m_hWnd, CtlName, s);
    }
