// StatTest.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "dlgcodes.h"
#include "MyStatic.h"
#include "StatTest.h"
#include "Is95.h"
#include "animate.h"
#include "dlgcode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticTest property page

IMPLEMENT_DYNCREATE(CStaticTest, CPropertyPage)

CStaticTest::CStaticTest() : CPropertyPage(CStaticTest::IDD)
{
        //{{AFX_DATA_INIT(CStaticTest)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CStaticTest::~CStaticTest()
{
}

void CStaticTest::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CStaticTest)
        DDX_Control(pDX, IDC_SS_NOPREFIX, c_SSNoPrefix);
        DDX_Control(pDX, IDC_WS_BORDER, c_WSBorder);
        DDX_Control(pDX, IDC_SS_BITMAP, c_SSBitmap);
        DDX_Control(pDX, IDC_SS_WHITERECT, c_SSWhiteRect);
        DDX_Control(pDX, IDC_SS_WHITEFRAME, c_SSWhiteFrame);
        DDX_Control(pDX, IDC_SS_USERITEM, c_SSUserItem);
        DDX_Control(pDX, IDC_SS_SIMPLE, c_SSSimple);
        DDX_Control(pDX, IDC_SS_RIGHT, c_SSRight);
        DDX_Control(pDX, IDC_SS_OWNERDRAW, c_SSOwnerDraw);
        DDX_Control(pDX, IDC_SS_BLACKFRAME, c_SSBlackFrame);
        DDX_Control(pDX, IDC_SS_BLACKRECT, c_SSBlackRect);
        DDX_Control(pDX, IDC_SS_CENTER, c_SSCenter);
        DDX_Control(pDX, IDC_SS_CENTERIMAGE, c_SSCenterImage);
        DDX_Control(pDX, IDC_SS_ENHMETAFILE, c_SSEnhMetaFile);
        DDX_Control(pDX, IDC_SS_ETCHEDFRAME, c_SSEtchedFrame);
        DDX_Control(pDX, IDC_SS_ETCHEDHORZ, c_SSEtchedHorz);
        DDX_Control(pDX, IDC_SS_ETCHEDVERT, c_SSEtchedVert);
        DDX_Control(pDX, IDC_SS_GRAYFRAME, c_SSGrayFrame);
        DDX_Control(pDX, IDC_SS_GRAYRECT, c_SSGrayRect);
        DDX_Control(pDX, IDC_SS_ICON, c_SSIcon);
        DDX_Control(pDX, IDC_SS_LEFT, c_SSLeft);
        DDX_Control(pDX, IDC_SS_LEFTNOWORDWRAP, c_SSLeftNoWordWrap);
        DDX_Control(pDX, IDC_ENABLED, c_Enable);
        DDX_Control(pDX, IDC_EDIT, c_Edit);
        DDX_Control(pDX, IDC_STATIC_CONTROL, c_Static);
        DDX_Control(pDX, IDC_SS_SUNKEN, c_SSSunken);
        DDX_Control(pDX, IDC_SS_RIGHTJUST, c_SSRightJust);
        DDX_Control(pDX, IDC_SS_REALSIZEIMAGE, c_SSRealSize);
        DDX_Control(pDX, IDC_SS_NOTIFY, c_SSNotify);
        DDX_Control(pDX, IDC_SETTEXT, c_SetText);
        DDX_Control(pDX, IDC_NO_ANIMATION, c_NoAnimation);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStaticTest, CPropertyPage)
        //{{AFX_MSG_MAP(CStaticTest)
        ON_WM_TIMER()
        ON_STN_CLICKED(IDC_STATIC_CONTROL, OnStaticClicked)
        ON_STN_DBLCLK(IDC_STATIC_CONTROL, OnStaticDblClicked)
        ON_STN_ENABLE(IDC_STATIC_CONTROL, OnStaticEnable)
        ON_STN_DISABLE(IDC_STATIC_CONTROL, OnStaticDisable)
        ON_BN_CLICKED(IDC_DLGCODE, OnDlgcode)
        ON_BN_CLICKED(IDC_SETTEXT, OnSettext)
        ON_BN_CLICKED(IDC_SS_BITMAP, OnSSBitmap)
        ON_BN_CLICKED(IDC_SS_BLACKFRAME, OnSSBlackframe)
        ON_BN_CLICKED(IDC_SS_BLACKRECT, OnSSBlackrect)
        ON_BN_CLICKED(IDC_SS_CENTER, OnSSCenter)
        ON_BN_CLICKED(IDC_SS_CENTERIMAGE, OnSSCenterimage)
        ON_BN_CLICKED(IDC_SS_ENHMETAFILE, OnSSEnhmetafile)
        ON_BN_CLICKED(IDC_SS_ETCHEDFRAME, OnSSEtchedframe)
        ON_BN_CLICKED(IDC_SS_ETCHEDHORZ, OnSSEtchedhorz)
        ON_BN_CLICKED(IDC_SS_ETCHEDVERT, OnSSEtchedvert)
        ON_BN_CLICKED(IDC_SS_GRAYFRAME, OnSSGrayframe)
        ON_BN_CLICKED(IDC_SS_GRAYRECT, OnSSGrayrect)
        ON_BN_CLICKED(IDC_SS_ICON, OnSSIcon)
        ON_BN_CLICKED(IDC_SS_LEFT, OnSSLeft)
        ON_BN_CLICKED(IDC_SS_LEFTNOWORDWRAP, OnSSLeftnowordwrap)
        ON_BN_CLICKED(IDC_SS_NOTIFY, OnSSNotify)
        ON_BN_CLICKED(IDC_SS_OWNERDRAW, OnSSOwnerdraw)
        ON_BN_CLICKED(IDC_SS_REALSIZEIMAGE, OnSSRealsizeimage)
        ON_BN_CLICKED(IDC_SS_RIGHT, OnSSRight)
        ON_BN_CLICKED(IDC_SS_RIGHTJUST, OnSSRightjust)
        ON_BN_CLICKED(IDC_SS_SIMPLE, OnSSSimple)
        ON_BN_CLICKED(IDC_SS_SUNKEN, OnSSSunken)
        ON_BN_CLICKED(IDC_SS_USERITEM, OnSSUseritem)
        ON_BN_CLICKED(IDC_SS_WHITEFRAME, OnSSWhiteframe)
        ON_BN_CLICKED(IDC_SS_WHITERECT, OnSSWhiterect)
        ON_BN_CLICKED(IDC_ENABLED, OnEnabled)
        ON_BN_CLICKED(IDC_WS_BORDER, OnWsBorder)
        ON_BN_CLICKED(IDC_SS_NOPREFIX, OnSsNoprefix)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticTest message handlers

/****************************************************************************
*                           CStaticTest::addMessage
* Inputs:
*       CString s: Message to display
* Result: void
*       
* Effect: 
*       adds the message
****************************************************************************/

void CStaticTest::addMessage(CString s)
     {
      CString CtlName;
      CtlName.LoadString(IDS_STATIC);

      c_Messages.addMessage(c_Static.m_hWnd, CtlName, s);
     }
         


/****************************************************************************
*                        CStaticTest::logSetWindowLong
* Inputs:
*       DWORD style: Style bits to set
* Result: void
*       
* Effect: 
*       Logs the style bits for the SetWindowLong
****************************************************************************/

void CStaticTest::logSetWindowLong(DWORD style)
    {
     CString fmt;
     fmt.LoadString(IDS_SETWINDOWLONG);

     CString s;
     s.Format(fmt, style, styleToString(style));
     addMessage(s);
    }

void CStaticTest::OnDlgcode() 
{
 CGetDlgCode dlg;

 dlg.ctl = &c_Static;

 dlg.DoModal();        
}

void CStaticTest::OnSettext() 
{
 CString s;
 c_Edit.GetWindowText(s);
 c_Static.SetWindowText(s);
 
 CString t;
 t.LoadString(IDS_WM_SETTEXT);
 
 s.Format(_T("%s(0, \"...\")"), t);
 addMessage(s);
}

void CStaticTest::OnSSBitmap() 
{
 recreateWindow(SS_BITMAP);
 c_Static.SendMessage(STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)(HBITMAP)bm);
}

void CStaticTest::OnSSBlackframe() 
{
 recreateWindow(SS_BLACKFRAME);
        
}

void CStaticTest::OnSSBlackrect() 
{
 recreateWindow(SS_BLACKRECT);
        
}

void CStaticTest::OnSSCenter() 
{
 recreateWindow(SS_CENTER);
}

void CStaticTest::OnSSCenterimage() 
{
 recreateWindow(SS_CENTERIMAGE);
}

void CStaticTest::OnSSEnhmetafile() 
{
 // TODO: Write this...
}

void CStaticTest::OnSSEtchedframe() 
{
 recreateWindow(SS_ETCHEDFRAME);
}

void CStaticTest::OnSSEtchedhorz() 
{
 recreateWindow(SS_ETCHEDHORZ);
        
}

void CStaticTest::OnSSEtchedvert() 
{
 recreateWindow(SS_ETCHEDVERT);
}

void CStaticTest::OnSSGrayframe() 
{
 recreateWindow(SS_GRAYFRAME);
}

void CStaticTest::OnSSGrayrect() 
{
 recreateWindow(SS_GRAYRECT);
}

void CStaticTest::OnSSIcon() 
{
 recreateWindow(SS_ICON);
 HICON icon = AfxGetApp()->LoadIcon(IDI_JMNCO);  // load my favorite image...
 c_Static.SetIcon(icon);
}

void CStaticTest::OnSSLeft() 
{
 recreateWindow(SS_LEFT);
}

void CStaticTest::OnSSLeftnowordwrap() 
{
 recreateWindow(SS_LEFTNOWORDWRAP);
}

void CStaticTest::OnSSNotify() 
{
 DWORD style = c_Static.GetStyle();

 if(c_SSNotify.GetCheck())
    style |= SS_NOTIFY;
 else
    style &= ~SS_NOTIFY;

 ::SetWindowLong(c_Static.m_hWnd, GWL_STYLE, style);
 logSetWindowLong(style);
}

void CStaticTest::OnSSOwnerdraw() 
{
 recreateWindow(SS_OWNERDRAW);
}

void CStaticTest::OnSSRealsizeimage() 
{
 recreateWindow(SS_REALSIZEIMAGE);
}

void CStaticTest::OnSSRight() 
{
 recreateWindow(SS_RIGHT);
}

void CStaticTest::OnSSRightjust() 
{
 recreateWindow(SS_RIGHTJUST);
}

void CStaticTest::OnSSSimple() 
{
 recreateWindow(SS_SIMPLE);
}

void CStaticTest::OnSSSunken() 
{
 recreateWindow(SS_SUNKEN);
}

void CStaticTest::OnSSUseritem() 
{
 recreateWindow(SS_USERITEM);
}

void CStaticTest::OnSSWhiteframe() 
{
 recreateWindow(SS_WHITEFRAME);
}

void CStaticTest::OnSSWhiterect() 
{
 recreateWindow(SS_WHITERECT);
}

/****************************************************************************
*                            CStaticTest::addStyle
* Inputs:
*       CString & s: String to add to
*       DWORD styles: Style flags
*       DWORD style: Style flag desired
*       int styleid: Style ID to add
* Result: void
*       
* Effect: 
*       If the style specified by the 'style' flag is present in 'styles',
*       appends " | " and style name to string
****************************************************************************/

void CStaticTest::addStyle(CString & stylestr, DWORD styles, DWORD style, int styleid)
    {
     if((styles & style) == 0)
        return;
     CString s;
     s.LoadString(styleid);
     stylestr += _T( " | ");
     stylestr += s;
    }

/****************************************************************************
*                         CStaticTest::styleToString
* Inputs:
*       DWORD style
* Result: CString
*       Style bits decoded to string
* Effect: 
*       Allocates a string representing the style bits
****************************************************************************/

CString CStaticTest::styleToString(DWORD style)
    {
     CString stylestr;

     switch(style & SS_TYPEMASK)
        { /* typemask */
         case SS_LEFT:
                 stylestr.LoadString(IDS_SS_LEFT);
                 break;
         case SS_CENTER:
                 stylestr.LoadString(IDS_SS_CENTER);
                 break;
         case SS_RIGHT:
                 stylestr.LoadString(IDS_SS_RIGHT);
                 break;
         case SS_ICON:
                 stylestr.LoadString(IDS_SS_ICON);
                 break;
         case SS_BLACKRECT:
                 stylestr.LoadString(IDS_SS_BLACKRECT);
                 break;
         case SS_GRAYRECT:
                 stylestr.LoadString(IDS_SS_GRAYRECT);
                 break;
         case SS_WHITERECT:
                 stylestr.LoadString(IDS_SS_WHITERECT);
                 break;
         case SS_BLACKFRAME:
                 stylestr.LoadString(IDS_SS_BLACKFRAME);
                 break;
         case SS_GRAYFRAME:
                 stylestr.LoadString(IDS_SS_GRAYFRAME);
                 break;
         case SS_WHITEFRAME:
                 stylestr.LoadString(IDS_SS_WHITEFRAME);
                 break;
         case SS_USERITEM:
                 stylestr.LoadString(IDS_SS_USERITEM);
                 break;
         case SS_SIMPLE:
                 stylestr.LoadString(IDS_SS_SIMPLE);
                 break;
         case SS_LEFTNOWORDWRAP:
                 stylestr.LoadString(IDS_SS_LEFTNOWORDWRAP);
                 break;
         case SS_OWNERDRAW:
                 stylestr.LoadString(IDS_SS_OWNERDRAW);
                 break;
         case SS_BITMAP:
                 stylestr.LoadString(IDS_SS_BITMAP);
                 break;
         case SS_ENHMETAFILE:
                 stylestr.LoadString(IDS_SS_ENHMETAFILE);
                 break;
         case SS_ETCHEDHORZ:
                 stylestr.LoadString(IDS_SS_ETCHEDHORZ);
                 break;
         case SS_ETCHEDVERT:
                 stylestr.LoadString(IDS_SS_ETCHEDVERT);
                 break;
         case SS_ETCHEDFRAME:
                 stylestr.LoadString(IDS_SS_ETCHEDFRAME);
                 break;
        } /* typemask */

     addStyle(stylestr, style, SS_NOPREFIX, IDS_SS_NOPREFIX);
     addStyle(stylestr, style, SS_NOTIFY,   IDS_SS_NOTIFY);
     addStyle(stylestr, style, SS_CENTERIMAGE, IDS_SS_CENTERIMAGE);
     addStyle(stylestr, style, SS_RIGHTJUST,   IDS_SS_RIGHTJUST);
     addStyle(stylestr, style, SS_REALSIZEIMAGE, IDS_SS_REALSIZEIMAGE);
     addStyle(stylestr, style, SS_SUNKEN, IDS_SS_SUNKEN);

     addStyle(stylestr, style, WS_DISABLED, IDS_WS_DISABLED);
     addStyle(stylestr, style, WS_CLIPSIBLINGS, IDS_WS_CLIPSIBLINGS);
     addStyle(stylestr, style, WS_CHILD, IDS_WS_CHILD);
     addStyle(stylestr, style, WS_TABSTOP, IDS_WS_TABSTOP);
     addStyle(stylestr, style, WS_GROUP, IDS_WS_GROUP);
     addStyle(stylestr, style, WS_BORDER, IDS_WS_BORDER);
     addStyle(stylestr, style, WS_VISIBLE, IDS_WS_VISIBLE);
     
     return stylestr;
    }

/****************************************************************************
*                         CStaticTest::recreateWindow
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

void CStaticTest::recreateWindow(DWORD style)
    {
     CStatic newWindow;
     CRect rect;
     CFont * font;
     HICON icon;
     UINT dlgcodes;
     CString stylestr;

     rect = orgsize;

     dlgcodes = getDlgCode(c_Static.m_hWnd);

     font = c_Static.GetFont();
     icon = c_Static.GetIcon();

     // Add in the orthogonal style bits
     // WS_BORDER
     // WS_DISABLED

     // SS_NOPREFIX     
     // SS_CENTERIMAGE
     // SS_NOTIFY
     // SS_RIGHTJUST
     // SS_REALSIZEIMAGE
     // SS_SUNKEN

     if(c_WSBorder.GetCheck())
        style |= WS_BORDER;

     if(!c_Enable.GetCheck())
        style |= WS_DISABLED;

     if(c_SSCenterImage.GetCheck())
        style |= SS_CENTERIMAGE;

     if(c_SSNoPrefix.GetCheck())
        style |= SS_NOPREFIX;

     if(c_SSNotify.GetCheck())
        style |= SS_NOTIFY;

     if(c_SSRealSize.GetCheck())
        style |= SS_REALSIZEIMAGE;

     if(c_SSRightJust.GetCheck())
        style |= SS_RIGHTJUST;

     if(c_SSSunken.GetCheck())
        style |= SS_SUNKEN;

     CString s;
     c_Static.GetWindowText(s);

     // Now decode the style bits to a string

     stylestr = styleToString(style);

     BOOL result = newWindow.Create(s, style & ~WS_VISIBLE, rect, this, IDC_STATIC_CONTROL);

// CreateWindow("static", "%s", %s, %d, %d, %d, %d, %08x, %d, %08x, NULL) => %s

     CString fmt;
     fmt.LoadString(IDS_CREATE_STATIC);
     
     CString log;
     CString resultstr;
     if(result)
        resultstr.Format(_T("%08x"), newWindow.m_hWnd);
     else
        resultstr.LoadString(IDS_NULL);

     log.Format(fmt, s, stylestr, rect.left, rect.top, rect.Width(), 
                                rect.Height(), (int)this->m_hWnd, 
                                (int)IDC_STATIC_CONTROL,
                                (int)AfxGetInstanceHandle(),
                                resultstr);

     CString CtlName;
     CtlName.LoadString(IDS_STATIC);
     c_Messages.addMessage(newWindow.m_hWnd, CtlName, log);

     HWND oldwnd = c_Static.UnsubclassWindow();
     ::DestroyWindow(oldwnd);

     HWND wnd = newWindow.UnsubclassWindow();

     // SubclassWindow will set up all the right pointers.  We need to
     // do this because different types of windows subclass to different
     // handlers.
     c_Static.SubclassWindow(wnd);

     if(font != NULL)
        c_Static.SetFont(font);
     else
        c_Static.SetFont(orgfont);

     if(font != NULL)
        orgfont = font;
     if(icon != NULL)
        c_Static.SetIcon(icon);

     setDlgCode(c_Static.m_hWnd, dlgcodes);


     if(c_NoAnimation.GetCheck())
        { /* just do it */
         c_Static.ShowWindow(SW_SHOW);
        } /* just do it */
     else
        { /* animate */
         
         // Cute effect for demonstration of reconstruction

         beginAnimation(&c_Static);
        } /* animate */
    }

void CStaticTest::OnTimer(UINT nIDEvent) 
{
        endAnimation(&c_Static);

        CPropertyPage::OnTimer(nIDEvent);
}

void CStaticTest::OnEnabled() 
{
 c_Static.EnableWindow(c_Enable.GetCheck());
        
}

BOOL CStaticTest::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        CString s;
        s.LoadString(IDS_STATIC);
        c_Messages.initialize(c_Messages.GetStringWidth(s));
        
        c_Messages.setWrap(TRUE);

        bm.LoadBitmap(IDB_LEAF);

        // c_SSBitmap.EnableWindow(!Is3());
        c_SSEnhMetaFile.EnableWindow(!Is3());
        c_SSEtchedFrame.EnableWindow(!Is3());
        c_SSEtchedHorz.EnableWindow(!Is3());
        c_SSEtchedVert.EnableWindow(!Is3());
        c_SSRealSize.EnableWindow(!Is3());
        c_SSRightJust.EnableWindow(!Is3());
        c_SSSunken.EnableWindow(!Is3());
        c_SSNotify.EnableWindow(!Is3());
        
        c_SSEnhMetaFile.EnableWindow(FALSE); // TODO: implement this


        // Because we may have been subclassed by CTL3D, we probably have
        // other than the standard font selected.  Save this in case we
        // need a font again.
        orgfont = c_Static.GetFont();

        // Save the original size, because moving to an icon, for example,
        // changes the size to SM_CXICON/SM_CYICON

        c_Static.GetWindowRect(&orgsize);
        ScreenToClient(&orgsize);
        
        CheckRadioButton(IDC_SS_BITMAP, IDC_SS_WHITERECT, IDC_SS_LEFT);

        c_Enable.SetCheck(TRUE);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CStaticTest::OnWsBorder() 
{
 DWORD style = c_Static.GetStyle() & SS_TYPEMASK;

 recreateWindow(style);
        
}

void CStaticTest::OnSsNoprefix() 
{
 DWORD style = c_Static.GetStyle();

 if(c_SSNoPrefix.GetCheck())
    style |= SS_NOPREFIX;
 else
    style &= ~SS_NOPREFIX;

 ::SetWindowLong(c_Static.m_hWnd, GWL_STYLE, style);
 logSetWindowLong(style);
 c_Static.InvalidateRect(NULL);
 
        
}

void CStaticTest::OnStaticClicked()
{
 CString s;
 s.LoadString(IDS_STN_CLICKED);

 addMessage(s);
}


void CStaticTest::OnStaticEnable()
{
 CString s;
 s.LoadString(IDS_STN_ENABLE);

 addMessage(s);
}

void CStaticTest::OnStaticDisable()
{
 CString s;
 s.LoadString(IDS_STN_DISABLE);

 addMessage(s);
}

void CStaticTest::OnStaticDblClicked()
{
 CString s;
 s.LoadString(IDS_STN_DBLCLK);

 addMessage(s);
}
