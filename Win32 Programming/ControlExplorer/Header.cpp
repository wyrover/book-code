// Header.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "indexpg.h"
#include "NumericEdit.h"
#include "TinyButt.h"
#include "msgsheet.h"
#include "Header.h"
#include "ixable.h"
#include "loggable.h"
#include "BitCombo.h"
#include "image.h"
#include "bmselect.h"
#include "HdrMsg.h"
#include "HdrLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_HEADER 4095 // random large value for header control ID

/////////////////////////////////////////////////////////////////////////////
// CHeaderTest property page

IMPLEMENT_DYNCREATE(CHeaderTest, CPropertyPage)

CHeaderTest::CHeaderTest() : CPropertyPage(CHeaderTest::IDD)
{
        //{{AFX_DATA_INIT(CHeaderTest)
        //}}AFX_DATA_INIT
}

CHeaderTest::~CHeaderTest()
{
 // We need to do this explicitly because otherwise MFC becomes very
 // confused and gives all sorts of ASSERT failures
 HWND hdr = c_Header.Detach();
 if(::IsWindow(hdr))
    ::DestroyWindow(hdr);
}

void CHeaderTest::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CHeaderTest)
        DDX_Control(pDX, IDC_WIDTH, c_Width);
        DDX_Control(pDX, IDC_ITEM, c_Item);
        DDX_Control(pDX, IDC_NO_ANIMATION, c_NoAnimation);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_HEADING_SPACE, c_HeadingSpace);
        DDX_Control(pDX, IDC_HDS_HORZ, c_HDS_Horz);
        DDX_Control(pDX, IDC_HDS_HIDDEN, c_HDS_Hidden);
        DDX_Control(pDX, IDC_HDS_BUTTONS, c_HDS_Buttons);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeaderTest, CPropertyPage)
        //{{AFX_MSG_MAP(CHeaderTest)
        ON_BN_CLICKED(IDC_HDS_BUTTONS, OnHdsButtons)
        ON_BN_CLICKED(IDC_HDS_HIDDEN, OnHdsHidden)
        ON_BN_CLICKED(IDC_HDS_HORZ, OnHdsHorz)
        ON_BN_CLICKED(IDC_SENDMESSAGES, OnSendmessages)
        ON_NOTIFY(HDN_BEGINTRACK, IDC_HEADER, OnHeaderBeginTrack)
        ON_NOTIFY(HDN_DIVIDERDBLCLICK, IDC_HEADER, OnHeaderDividerDblClick)
        ON_NOTIFY(HDN_ENDTRACK, IDC_HEADER, OnHeaderEndTrack)
        ON_NOTIFY(HDN_ITEMCHANGED, IDC_HEADER, OnHeaderItemChanged)
        ON_NOTIFY(HDN_ITEMCHANGING, IDC_HEADER, OnHeaderItemChanging)
        ON_NOTIFY(HDN_ITEMCLICK, IDC_HEADER, OnHeaderClick)
        ON_NOTIFY(HDN_ITEMDBLCLICK, IDC_HEADER, OnHeaderDblClick)
        ON_NOTIFY(HDN_TRACK, IDC_HEADER, OnHeaderTrack)
        ON_WM_CLOSE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderTest message handlers

/****************************************************************************
*                            CHeaderTest::logTrack
* Inputs:
*       NMHDR * hdr: (HD_NOTIFY *)
* Result: void
*       
* Effect: 
*       Logs the tracking data to the tracking box
****************************************************************************/

void CHeaderTest::logTrack(NMHDR * hdr)
    {
     HD_NOTIFY * nhdr = (HD_NOTIFY *)hdr;

     c_Item.SetWindowText(nhdr->iItem);
     c_Width.SetWindowText(nhdr->pitem->cxy);
    }

/****************************************************************************
*                            CHeaderTest::addStyle
* Inputs:
*       CString & flags: String to add to
*       BOOL hasit: non-zero if style present
*       int styleid: ID of style
* Result: void
*       
* Effect: 
*       If the style flag is present, adds its name
****************************************************************************/

void CHeaderTest::addStyle(CString & flags, BOOL hasit, int styleid)
    {
     CString s;

     if(hasit)
        { /* has style */
         s.LoadString(styleid);
         if(flags.GetLength() > 0)
            flags += " | ";
         flags += s;
        } /* HDS_BUTTONS */
    }

/****************************************************************************
*                        CHeaderTest::logSetWindowLong
* Inputs:
*       DWORD style: Style flags
* Result: void
*       
* Effect: 
*       Logs the SetWindowLong event
****************************************************************************/

void CHeaderTest::logSetWindowLong(DWORD style)
    {
     // SetWindowLong(...,GWL_STYLE, %08x=%s)

     CString fmt;
     CString flags;
     CString s;

     fmt.LoadString(IDS_SETWINDOWLONG);
     
     addStyle(flags, style & WS_BORDER, IDS_WS_BORDER);
     addStyle(flags, style & WS_VISIBLE, IDS_WS_VISIBLE);
     addStyle(flags, style & WS_CHILD, IDS_WS_CHILD);
     addStyle(flags, style & HDS_BUTTONS, IDS_HDS_BUTTONS);
     addStyle(flags, style & HDS_HIDDEN, IDS_HDS_HIDDEN);
     // addStyle(flags, style & HDS_HORZ, IDS_HDS_HORZ);

     s.Format(fmt, style, flags);
     CString ctlName;
     ctlName.LoadString(IDS_HEADER);

     c_Messages.addMessage(c_Header.m_hWnd, ctlName, s);
    }

/****************************************************************************
*                           CHeaderTest::logNotify
* Inputs:
*       NMHDR hdr: Header
* Result: void
*       
* Effect: 
*       Logs the notification message
****************************************************************************/

void CHeaderTest::logNotify(NMHDR * hdr)
    {
     HD_NOTIFY * nhdr = (HD_NOTIFY *)hdr;

     CString fmt;
     fmt.LoadString(IDS_WM_NOTIFYFMT);

     CString scode;
     switch(hdr->code)
        { /* scode */
         case HDN_BEGINTRACK:
                 scode.LoadString(IDS_HDN_BEGINTRACK);
                 break;
         case HDN_DIVIDERDBLCLICK:
                 scode.LoadString(IDS_HDN_DIVIDERDBLCLICK);
                 break;
         case HDN_ENDTRACK:
                 scode.LoadString(IDS_HDN_ENDTRACK);
                 break;
         case HDN_ITEMCHANGED:
                 scode.LoadString(IDS_HDN_ITEMCHANGED);
                 break;
         case HDN_ITEMCHANGING:
                 scode.LoadString(IDS_HDN_ITEMCHANGING);
                 break;
         case HDN_ITEMCLICK:
                 scode.LoadString(IDS_HDN_ITEMCLICK);
                 break;
         case HDN_ITEMDBLCLICK:
                 scode.LoadString(IDS_HDN_ITEMDBLCLICK);
                 break;
         case HDN_TRACK:
                 scode.LoadString(IDS_HDN_TRACK);
                 break;
        } /* scode */

     // WM_NOTIFY(%s, {hwnd: %08x, id: %d, code: %d=%s, item: %d, button: %s}
     
     CString shdr;
     shdr.LoadString(IDS_IDC_HEADER);

     CString button;
     switch(nhdr->iButton)
        { /* iButton */
         case 0:
                 button.LoadString(IDS_LEFT_BUTTON_CODE);
                 break;
         case 1:
                 button.LoadString(IDS_RIGHT_BUTTON_CODE);
                 break;
         case 2:
                 button.LoadString(IDS_MIDDLE_BUTTON_CODE);
                 break;
        } /* iButton */

     CString s;
     s.Format(fmt, shdr, hdr->hwndFrom, hdr->idFrom, hdr->code, scode,
                                nhdr->iItem,
                                button);
     CString ctlName;
     ctlName.LoadString(IDS_HEADER);

     c_Messages.addMessage(c_Header.m_hWnd, ctlName, s);
     
    }

/****************************************************************************
*                            CHeaderTest::setStyle
* Inputs:
*       BOOL checked: TRUE if checked, FALSE if unchecked
*       DWORD newstyle: Style flag
* Result: void
*       
* Effect: 
*       Sets or clears the style flag
****************************************************************************/

void CHeaderTest::setStyle(BOOL checked, DWORD newstyle)
    {
     DWORD style = ::GetWindowLong(c_Header.m_hWnd, GWL_STYLE);
     if(checked)
        style |= newstyle;
     else
        style &= ~newstyle;

     ::SetWindowLong(c_Header.m_hWnd, GWL_STYLE, style);
     logSetWindowLong(style);
    }

void CHeaderTest::OnHdsButtons() 
{
 setStyle(c_HDS_Buttons.GetCheck(), HDS_BUTTONS);
}

void CHeaderTest::OnHdsHidden() 
{
 setStyle(c_HDS_Hidden.GetCheck(), HDS_HIDDEN);
}

void CHeaderTest::OnHdsHorz() 
{
 // 
 // HDS_HORZ is always set, and has a 0 value
 //
}

void CHeaderTest::OnSendmessages() 
{
 CMessageSheet sheet(IDS_SEND_HEADER_MESSAGES);
 CHdrLayout layout;
 CHeaderMsg msgs;

 layout.c_Messages = &c_Messages;
 layout.c_Header = &c_Header;
 sheet.AddPage(&layout);

 msgs.c_Messages = &c_Messages;
 msgs.c_Header = &c_Header;
 sheet.AddPage(&msgs);

 // Move us down to the bottom left corner
 CRect r;
 GetParent()->GetWindowRect(&r);
 GetParent()->SetWindowPos(NULL, 0, 
                        ::GetSystemMetrics(SM_CYSCREEN) - r.Height(), 0, 0, 
                        SWP_NOSIZE | SWP_NOZORDER);

 sheet.DoModal();
}

BOOL CHeaderTest::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        CString s;
        s.LoadString(IDS_HEADER);

        // HDS_HORZ is 0, and is always set
        c_HDS_Horz.SetCheck(BST_CHECKED);
        c_HDS_Horz.EnableWindow(FALSE);


        c_Messages.initialize(c_Messages.GetStringWidth(s));
        c_Messages.setWrap(TRUE);

        createHeaderControl();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                      CHeaderTest::createHeaderControl
* Result: void
*       
* Effect: 
*       Creates a header control
****************************************************************************/

void CHeaderTest::createHeaderControl()
    {
     CRect rect;
     c_HeadingSpace.GetWindowRect(&rect);
     ScreenToClient(&rect);

     HWND hdr = ::CreateWindowEx(0, WC_HEADER, (LPCTSTR)NULL,
                                  WS_CHILD | WS_BORDER | WS_VISIBLE |
                                  (c_HDS_Horz.GetCheck() ? HDS_HORZ : 0) |
                                  (c_HDS_Hidden.GetCheck() ? HDS_HIDDEN : 0) |
                                  (c_HDS_Buttons.GetCheck() ? HDS_BUTTONS : 0),
                                  rect.left,
                                  rect.top,
                                  rect.Width(),
                                  rect.Height(),
                                  m_hWnd,
                                  (HMENU)IDC_HEADER,
                                  AfxGetInstanceHandle(),
                                  (LPVOID)NULL);
     if(hdr == NULL)
        return;

     if(IsWindow(c_Header.m_hWnd))
        { /* already exists */
         HWND old = c_Header.Detach();
         ::DestroyWindow(old);
        } /* already exists */
     
     c_Header.Attach(hdr);
    }

/****************************************************************************
*                       CHeaderTest::OnHeaderBeginTrack
* Inputs:
*       NMHDR * hdr: Header containing useful informatio
*       LRESULT * result: Place to store result code
* Result: void
*       
* Effect: 
*       Logs the BeginTrack message
****************************************************************************/

afx_msg void CHeaderTest::OnHeaderBeginTrack(NMHDR * hdr, LRESULT * result)
    {
     logNotify(hdr);     
     *result = 0;  //
    }

/****************************************************************************
*                        CHeaderTest::OnHeaderEndTrack
* Inputs:
*       NMHDR * hdr: Header containing useful informatio
*       LRESULT * result: Place to store result code
* Result: void
*       
* Effect: 
*       Logs the EndTrack message
****************************************************************************/

afx_msg void CHeaderTest::OnHeaderEndTrack(NMHDR * hdr, LRESULT * result)
    {
     logNotify(hdr);
     
     *result = 0;  //
    }

/****************************************************************************
*                    CHeaderTest::OnHeaderDividerDblClick
* Inputs:
*       NMHDR * hdr: Header containing useful informatio
*       LRESULT * result: Place to store result code
* Result: void
*       
* Effect: 
*       Logs the DividerDblClick message
****************************************************************************/

afx_msg void CHeaderTest::OnHeaderDividerDblClick(NMHDR * hdr, LRESULT * result)
    {
     logNotify(hdr);     
     *result = 0;  //
    }

/****************************************************************************
*                      CHeaderTest::OnHeaderItemChanged
* Inputs:
*       NMHDR * hdr: Header containing useful informatio
*       LRESULT * result: Place to store result code
* Result: void
*       
* Effect: 
*       Logs the ItemChanged message
****************************************************************************/

afx_msg void CHeaderTest::OnHeaderItemChanged(NMHDR * hdr, LRESULT * result)
    {
     logNotify(hdr);     
     *result = 0;  //
    }

/****************************************************************************
*                      CHeaderTest::OnHeaderItemChanging
* Inputs:
*       NMHDR * hdr: Header containing useful informatio
*       LRESULT * result: Place to store result code
* Result: void
*       
* Effect: 
*       Logs the ItemChanging message
****************************************************************************/

afx_msg void CHeaderTest::OnHeaderItemChanging(NMHDR * hdr, LRESULT * result)
    {
     logNotify(hdr);     
     *result = 0;  //
    }

/****************************************************************************
*                         CHeaderTest::OnHeaderClick
* Inputs:
*       NMHDR * hdr: Header containing useful informatio
*       LRESULT * result: Place to store result code
* Result: void
*       
* Effect: 
*       Logs the Click message
****************************************************************************/

afx_msg void CHeaderTest::OnHeaderClick(NMHDR * hdr, LRESULT * result)
    {
     logNotify(hdr);     
     *result = 0;  //
    }

/****************************************************************************
*                        CHeaderTest::OnHeaderDblClick
* Inputs:
*       NMHDR * hdr: Header containing useful informatio
*       LRESULT * result: Place to store result code
* Result: void
*       
* Effect: 
*       Logs the DblClick message
****************************************************************************/

afx_msg void CHeaderTest::OnHeaderDblClick(NMHDR * hdr, LRESULT * result)
    {
     logNotify(hdr);     
     *result = 0;  //
    }

/****************************************************************************
*                         CHeaderTest::OnHeaderTrack
* Inputs:
*       NMHDR * hdr: Header containing useful informatio
*       LRESULT * result: Place to store result code
* Result: void
*       
* Effect: 
*       Logs the Track message
****************************************************************************/

afx_msg void CHeaderTest::OnHeaderTrack(NMHDR * hdr, LRESULT * result)
    {
     logNotify(hdr);     
     logTrack(hdr);
     *result = 0;  //
    }

void CHeaderTest::OnClose() 
{
        CPropertyPage::OnClose();
}
