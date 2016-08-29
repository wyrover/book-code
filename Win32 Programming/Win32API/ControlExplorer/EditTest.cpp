// EditTest.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "IntEdit.h"
#include "recedit.h"
#include "dlgedit.h"
#include "TinyButt.h"
#include "hexdisp.h"
#include "NumList.h"
#include "dlgcodes.h"
#include "ixable.h"
#include "loggable.h" 

#include "Indexpg.h"
#include "EditTest.h"
#include "editops.h"
#include "EditNdx.h"
#include "EditMap.h"
#include "EditCont.h"
#include "EditStat.h"
#include "EditScrl.h"
#include "EditRect.h"
#include "EditTabs.h"
#include "msgsheet.h"
#include "dlgcode.h"
#include "animate.h"

#include "is95.h"

#define DIM(x) (sizeof(x) / sizeof((x)[0]))

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditTest property page

IMPLEMENT_DYNCREATE(CEditTest, CPropertyPage)

CEditTest::CEditTest() : CPropertyPage(CEditTest::IDD)
{
        //{{AFX_DATA_INIT(CEditTest)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CEditTest::~CEditTest()
{
}

void CEditTest::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditTest)
        DDX_Control(pDX, IDC_OEM_SAMPLE, c_OEMSample);
        DDX_Control(pDX, IDC_NO_ANIMATION, c_NoAnimation);
        DDX_Control(pDX, IDC_LEFT, c_Left);
        DDX_Control(pDX, IDC_CENTER, c_Center);
        DDX_Control(pDX, IDC_RIGHT, c_Right);
        DDX_Control(pDX, IDC_PASSWORD, c_Password);
        DDX_Control(pDX, IDC_OEMCONVERT, c_OEMConvert);
        DDX_Control(pDX, IDC_USELIMIT, c_UseLimit);
        DDX_Control(pDX, IDC_SPINLIMIT, c_SpinLimit);
        DDX_Control(pDX, IDC_LIMITVAL, c_LimitVal);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_WANTRETURN, c_WantReturn);
        DDX_Control(pDX, IDC_VSCROLL, c_VScroll);
        DDX_Control(pDX, IDC_UPPERCASE, c_Uppercase);
        DDX_Control(pDX, IDC_AUTOVSCROLL, c_AutoVScroll);
        DDX_Control(pDX, IDC_AUTOHSCROLL, c_AutoHScroll);
        DDX_Control(pDX, IDC_BORDER, c_Border);
        DDX_Control(pDX, IDC_READONLY, c_ReadOnly);
        DDX_Control(pDX, IDC_NUMBER, c_Number);
        DDX_Control(pDX, IDC_NOHIDESEL, c_NoHideSel);
        DDX_Control(pDX, IDC_MULTILINE, c_Multiline);
        DDX_Control(pDX, IDC_LOWERCASE, c_Lowercase);
        DDX_Control(pDX, IDC_HSCROLL, c_HScroll);
        DDX_Control(pDX, IDC_ENABLE, c_Enable);
        DDX_Control(pDX, IDC_EDIT, c_Edit);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditTest, CPropertyPage)
        //{{AFX_MSG_MAP(CEditTest)
        ON_BN_CLICKED(IDC_SENDMESSAGES, OnSendMessages)
        ON_BN_CLICKED(IDC_AUTOHSCROLL, OnAutohscroll)
        ON_BN_CLICKED(IDC_AUTOVSCROLL, OnAutovscroll)
        ON_BN_CLICKED(IDC_BORDER, OnBorder)
        ON_BN_CLICKED(IDC_ENABLE, OnEnable)
        ON_BN_CLICKED(IDC_HSCROLL, OnHscroll)
        ON_BN_CLICKED(IDC_LOWERCASE, OnLowercase)
        ON_BN_CLICKED(IDC_MULTILINE, OnMultiline)
        ON_BN_CLICKED(IDC_NOHIDESEL, OnNohidesel)
        ON_BN_CLICKED(IDC_NUMBER, OnNumber)
        ON_BN_CLICKED(IDC_READONLY, OnReadonly)
        ON_BN_CLICKED(IDC_UPPERCASE, OnUppercase)
        ON_BN_CLICKED(IDC_VSCROLL, OnVscroll)
        ON_BN_CLICKED(IDC_WANTRETURN, OnWantreturn)
        ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
        ON_EN_CHANGE(IDC_EDIT, OnChangeEdit)
        ON_EN_ERRSPACE(IDC_EDIT, OnErrspaceEdit)
        ON_EN_HSCROLL(IDC_EDIT, OnHscrollEdit)
        ON_EN_KILLFOCUS(IDC_EDIT, OnKillfocusEdit)
        ON_EN_MAXTEXT(IDC_EDIT, OnMaxtextEdit)
        ON_EN_SETFOCUS(IDC_EDIT, OnSetfocusEdit)
        ON_EN_UPDATE(IDC_EDIT, OnUpdateEdit)
        ON_EN_VSCROLL(IDC_EDIT, OnVscrollEdit)
        ON_WM_TIMER()
        ON_BN_CLICKED(IDC_USELIMIT, OnUselimit)
        ON_BN_CLICKED(IDC_CENTER, OnCenter)
        ON_BN_CLICKED(IDC_LEFT, OnLeft)
        ON_EN_CHANGE(IDC_LIMITVAL, OnChangeLimitval)
        ON_BN_CLICKED(IDC_OEMCONVERT, OnOemconvert)
        ON_BN_CLICKED(IDC_PASSWORD, OnPassword)
        ON_BN_CLICKED(IDC_RIGHT, OnRight)
        ON_BN_CLICKED(IDC_DLGCODE, OnDlgcode)
        ON_BN_CLICKED(IDC_OEM_SAMPLE, OnOemSample)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTest message handlers

void setStyle(CWnd * wnd, DWORD style)
    {
     ::SetWindowLong(wnd->m_hWnd, GWL_STYLE, style);
    }

/****************************************************************************
*                           CEditTest::setStyleBit
* Inputs:
*       CButton * ctl: Control which establishes bit setting
*       DWORD newstyle: style bit which it will add or delete
* Result: void
*       
* Effect: 
*       If the control is checked, adds in the style bit, else
*       removes it.
* Notes:
*       This does not handle special cases such as dealing with the 
*       ES_CENTER, ES_LEFT, or ES_RIGHT bits not being relevant for
*       non-ES_MULTILINE.  We could handle this differently, but since the
*       bits are ignored we don't special-case this.
****************************************************************************/

void CEditTest::setStyleBit(CButton * ctl, DWORD newstyle)
    {
     DWORD oldstyle = style;

     if(ctl->GetCheck())
        style |= newstyle;
     else
        style &= ~newstyle;
     if(style != oldstyle)
        { /* set it */
         styleToControls();
         setStyle(&c_Edit, style);
        } /* set it */
    }

void CEditTest::clearStyleBit(CButton * ctl, DWORD newstyle)
    {
     DWORD oldstyle = style;

     if(!ctl->GetCheck())
        style |= newstyle;
     else
        style &= ~newstyle;
     if(style != oldstyle)
        { /* set it */
         styleToControls();
         setStyle(&c_Edit, style);
        } /* set it */

    }

void CEditTest::OnAutohscroll() 
{
 setStyleBit(&c_AutoHScroll, ES_AUTOHSCROLL);
 recreateWindow();
}

void CEditTest::OnAutovscroll() 
{
 setStyleBit(&c_AutoVScroll, ES_AUTOVSCROLL);
 recreateWindow();
}

void CEditTest::OnBorder() 
{
 setStyleBit(&c_Border, WS_BORDER);
 recreateWindow();
}

void CEditTest::OnEnable() 
{
 // We can't change the enable state simply by setting the bit.  We have
 // to call the EnableWindow function.

 c_Edit.EnableWindow(!c_Edit.IsWindowEnabled());
 if(c_Edit.IsWindowEnabled())
    style &= ~WS_DISABLED;
 else
    style |= WS_DISABLED;
}

void CEditTest::OnHscroll() 
{
 setStyleBit(&c_HScroll, WS_HSCROLL);
 recreateWindow();
}

void CEditTest::OnLowercase() 
{
 DWORD oldstyle = style;

 if(c_Lowercase.GetCheck())
    { /* checked */
     style |= ES_LOWERCASE;
     style &= ~ES_UPPERCASE;
    } /* checked */
 else
    { /* unchecked */
     style &= ~ES_LOWERCASE;
    } /* unchecked */

 if(oldstyle != style)
    { /* changed */
     styleToControls();
     setStyle(&c_Edit, style);
    } /* changed */
        
}

/****************************************************************************
*                         CEditTest::enableMultiLine
* Inputs:
*       BOOL multi: TRUE if multiline, FALSE if not multiline
* Result: void
*       
* Effect: 
*       Enables/disables the multiline controls
****************************************************************************/

void CEditTest::enableMultiLine(BOOL multi)
    {
     c_Left.EnableWindow(multi);
     c_Center.EnableWindow(multi);
     c_Right.EnableWindow(multi);
     c_Password.EnableWindow(!multi);
    }

/****************************************************************************
*                          CEditTest::enableControls
* Result: void
*       
* Effect: 
*       Enables the controls based on other settings
****************************************************************************/

void CEditTest::enableControls()
    {
     enableMultiLine(c_Multiline.GetCheck());
    }


void CEditTest::OnMultiline() 
{
 setStyleBit(&c_Multiline, ES_MULTILINE);
 enableControls();
 recreateWindow();
}

void CEditTest::OnNohidesel() 
{
 setStyleBit(&c_NoHideSel, ES_NOHIDESEL);
 recreateWindow();
}

void CEditTest::OnNumber() 
{
 setStyleBit(&c_Number, ES_NUMBER);
}

void CEditTest::OnReadonly() 
{
 if(c_ReadOnly.GetCheck())
    style |= ES_READONLY;
 else
    style &= ~ES_READONLY;

 // We can't change the read-only property just by setting the style bit
 // we keep the style bit in the 'style' variable so it is there for future
 // window creations and so it is reflected in the controls, but the real
 // way to change the read-onliness of a window is to send it the 
 // EM_SETREADONLY message:

 c_Edit.SetReadOnly(style & ES_READONLY ? TRUE : FALSE);
}

void CEditTest::OnUppercase() 
{
 DWORD oldstyle = style;

 if(c_Uppercase.GetCheck())
    { /* checked */
     style |= ES_UPPERCASE;
     style &= ~ES_LOWERCASE;
    } /* checked */
 else
    { /* unchecked */
     style &= ~ES_UPPERCASE;
    } /* unchecked */

 if(oldstyle != style)
    { /* changed */
     styleToControls();
     setStyle(&c_Edit, style);
    } /* changed */
        
}

void CEditTest::OnVscroll() 
{
 setStyleBit(&c_VScroll, WS_VSCROLL);
 recreateWindow();
}

void CEditTest::OnWantreturn() 
{
 setStyleBit(&c_WantReturn, ES_WANTRETURN);
}


/****************************************************************************
*                         CEditTest::styleToControls
* Result: void
*       
* Effect: 
*       Given the 'style' flag, sets the controls to reflect its state
****************************************************************************/

void CEditTest::styleToControls()
    {
     c_Left.SetCheck((style & (ES_CENTER | ES_RIGHT)) == 0);
     c_Center.SetCheck(style & ES_CENTER);
     c_Right.SetCheck(style & ES_RIGHT);
     c_Multiline.SetCheck(style & ES_MULTILINE);
     c_Uppercase.SetCheck(style & ES_UPPERCASE);
     c_Lowercase.SetCheck(style & ES_LOWERCASE);
     c_Password.SetCheck(style & ES_PASSWORD);
     c_AutoVScroll.SetCheck(style & ES_AUTOVSCROLL);
     c_AutoHScroll.SetCheck(style & ES_AUTOHSCROLL);
     c_NoHideSel.SetCheck(style & ES_NOHIDESEL);
     c_OEMConvert.SetCheck(style & ES_OEMCONVERT);
     c_ReadOnly.SetCheck(style & ES_READONLY);
     c_WantReturn.SetCheck(style & ES_WANTRETURN);
     c_Number.SetCheck(style & ES_NUMBER);

     c_VScroll.SetCheck(style & WS_VSCROLL);
     c_Border.SetCheck(style & WS_BORDER);
     c_HScroll.SetCheck(style & WS_HSCROLL);
     c_Enable.SetCheck(!(style & WS_DISABLED));
    }

/****************************************************************************
*                           CEditTest::setTextLimit
* Result: void
*       
* Effect: 
*       If the limit-check checkbox is set, set the limit to the value
*       specified (providing it is > 0).  If the limit-check box is not
*       set, set the limit to 0, meaning unconstrained.
****************************************************************************/

void CEditTest::setTextLimit()
    {
     if(c_UseLimit.m_hWnd == NULL)
        return;  // not initialized yet

     if(c_UseLimit.GetCheck())
        { /* set limit */
         int limit = c_LimitVal.GetVal();
         if(limit >= 0)
            c_Edit.SetLimitText(limit);
        } /* set limit */
     else
        c_Edit.SetLimitText(0); // remove limit
    }

/****************************************************************************
*                          CEditTest::recreateWindow
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

void CEditTest::recreateWindow()
    {
     CEdit newWindow;
     CRect rect;
     DWORD sel;
     CFont * font;
     UINT dlgcodes;

     c_Edit.GetWindowRect(&rect);
     ScreenToClient(&rect);

     dlgcodes = getDlgCode(c_Edit.m_hWnd);

     sel = c_Edit.GetSel();
     font = c_Edit.GetFont();

     if(!c_Enable.GetCheck())
        style |= WS_DISABLED;

     newWindow.Create(style & ~WS_VISIBLE, rect, this, IDC_EDIT);

     CString s;
     c_Edit.GetWindowText(s);
     newWindow.SetWindowText(s);
     
     HWND oldwnd = c_Edit.UnsubclassWindow();
     ::DestroyWindow(oldwnd);

     HWND wnd = newWindow.UnsubclassWindow();
     // SubclassWindow will set up all the right pointers.  We need to
     // do this because different types of edit windows subclass to different
     // handlers.
     c_Edit.SubclassWindow(wnd);

     setTextLimit();
     c_Edit.SetSel(sel);
     c_Edit.SetFont(font);

     setDlgCode(c_Edit.m_hWnd, dlgcodes);


     if(c_NoAnimation.GetCheck())
        { /* just do it */
         c_Edit.ShowWindow(SW_SHOW);
        } /* just do it */
     else
        { /* animate */
         
         // Cute effect for demonstration of reconstruction

         beginAnimation(&c_Edit);
         // We drew these non-overlapping for ease of use of the dialog editor
        } /* animate */
    }


/****************************************************************************
*                           CEditTest::enableLimit
* Result: void
*       
* Effect: 
*       If the limit checkbox is on, enable the limit controls, otherwise
*       disable them
****************************************************************************/

void CEditTest::enableLimit()
    {
     c_SpinLimit.EnableWindow(c_UseLimit.GetCheck());
     c_LimitVal.EnableWindow(c_UseLimit.GetCheck());
    }

BOOL CEditTest::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        style = c_Edit.GetStyle();
        styleToControls();
        
        CString s;
        s.LoadString(IDS_EDIT);

        c_Messages.initialize(c_Messages.GetStringWidth(s));
        c_Messages.setWrap(TRUE);

        c_Number.EnableWindow(!Is3());

        c_SpinLimit.SetRange(1, 100);
        c_SpinLimit.SetPos(10);

	SendMessage(DM_SETDEFID, IDC_DEFAULT);  // force this because
						// we are in property page

        enableControls();
        enableLimit();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditTest::OnDefault() 
{
 CString s;
 s.LoadString(IDS_DEFAULT_BUTTON);
 AfxMessageBox(s);
}

void CEditTest::OnChangeEdit() 
{
 logEdit(IDS_EN_CHANGE);
}

void CEditTest::OnErrspaceEdit() 
{
 logEdit(IDS_EN_ERRSPACE);
}

void CEditTest::OnHscrollEdit() 
{
 logEdit(IDS_EN_HSCROLL);
}

void CEditTest::OnKillfocusEdit() 
{
 logEdit(IDS_EN_KILLFOCUS);
}

void CEditTest::OnMaxtextEdit() 
{
 logEdit(IDS_EN_MAXTEXT);
}

void CEditTest::OnSetfocusEdit() 
{
 logEdit(IDS_EN_SETFOCUS);
}

void CEditTest::OnUpdateEdit() 
{
 logEdit(IDS_EN_UPDATE);
}

void CEditTest::OnVscrollEdit() 
{
 logEdit(IDS_EN_VSCROLL);
}

/****************************************************************************
*                             CEditTest::logEdit
* Inputs:
*       int id: ID of string to load
* Result: void
*       
* Effect: 
*       Logs the message
****************************************************************************/

void CEditTest::logEdit(int id)
    {
     CString CtlName;
     CtlName.LoadString(IDS_EDIT);

     CString s;
     s.LoadString(id);

     c_Messages.addMessage(c_Edit.m_hWnd, CtlName, s);
    }

void CEditTest::OnTimer(UINT nIDEvent) 
{
        endAnimation(&c_Edit);
        CPropertyPage::OnTimer(nIDEvent);
}

void CEditTest::OnUselimit() 
{
 enableLimit();
 setTextLimit();
}

void CEditTest::OnCenter() 
{
 if(c_Center.GetCheck())
    { /* clear others */
     c_Left.SetCheck(FALSE);
     c_Right.SetCheck(FALSE);
    } /* clear others */

 setStyleBit(&c_Left, ES_LEFT);
 setStyleBit(&c_Center, ES_CENTER);
 setStyleBit(&c_Right, ES_RIGHT);
 recreateWindow();
}

void CEditTest::OnLeft() 
{
 if(c_Left.GetCheck())
    { /* clear others */
     c_Center.SetCheck(FALSE);
     c_Right.SetCheck(FALSE);
    } /* clear others */

 setStyleBit(&c_Left, ES_LEFT);
 setStyleBit(&c_Center, ES_CENTER);
 setStyleBit(&c_Right, ES_RIGHT);
 recreateWindow();
}

void CEditTest::OnChangeLimitval() 
{
 setTextLimit();
}

void CEditTest::OnOemconvert() 
{
 setStyleBit(&c_OEMConvert, ES_OEMCONVERT);
}

void CEditTest::OnPassword() 
{
 setStyleBit(&c_Password, ES_PASSWORD);
 recreateWindow();
}

void CEditTest::OnRight() 
{
 if(c_Right.GetCheck())
    { /* clear others */
     c_Left.SetCheck(FALSE);
     c_Center.SetCheck(FALSE);
    } /* clear others */

 setStyleBit(&c_Left, ES_LEFT);
 setStyleBit(&c_Center, ES_CENTER);
 setStyleBit(&c_Right, ES_RIGHT);
 recreateWindow();
}

void CEditTest::OnSendMessages()
{
 CMessageSheet sheet(IDS_SEND_EDIT_MESSAGES);
 CEditIndex index;
 CEditContents contents;
 CEditMapping mapping;
 CEditState state;
 CEditScroll scroll;
 CEditRect rect;
 CEditTabs tabs;

 contents.c_Messages = &c_Messages;
 contents.edit     = &c_Edit;

 mapping.c_Messages  = &c_Messages;
 mapping.edit      = &c_Edit;

 state.c_Messages    = &c_Messages;
 state.edit        = &c_Edit;

 scroll.c_Messages   = &c_Messages;
 scroll.edit       = &c_Edit;

 rect.c_Messages     = &c_Messages;
 rect.edit         = &c_Edit;

 tabs.c_Messages     = &c_Messages;
 tabs.edit         = &c_Edit;

 sheet.AddPage(&index);
 sheet.AddPage(&contents);
 sheet.AddPage(&mapping);
 sheet.AddPage(&state);
 sheet.AddPage(&scroll);
 sheet.AddPage(&rect);
 sheet.AddPage(&tabs);

 // Move us down to the bottom left corner
 CRect r;
 GetParent()->GetWindowRect(&r);
 GetParent()->SetWindowPos(NULL, 0, 
                        ::GetSystemMetrics(SM_CYSCREEN) - r.Height(), 0, 0, 
                        SWP_NOSIZE | SWP_NOZORDER);

 sheet.DoModal();

 // We may have changed the readonly style by using the messager page

 style = (style & ~ES_READONLY) | (c_Edit.GetStyle() & ES_READONLY);

 // We may also have changed the text limit

 if(!Is3())
    { /* fix up limit */
     int limit = c_Edit.GetLimitText();
     c_LimitVal.SetVal(limit);
     c_UseLimit.SetCheck(limit != 0);
     enableLimit();
    } /* fix up limit */

 styleToControls();
 enableControls();
}

void CEditTest::OnDlgcode() 
{
 CGetDlgCode dlg;

 dlg.ctl = &c_Edit;

 dlg.DoModal();
}

void CEditTest::OnOemSample() 
{
 // The character sample is designed to have characters that
 // are different in the OEM and ANSI character sets.

 c_Edit.SetWindowText(_T("\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\r\n"
                         "\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF\r\n"
                         "\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\r\n"
                         "\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF\r\n"
                         "\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\r\n"
                         "\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF\r\n"
                         "\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\r\n"
                         "\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF"));

        
}

/****************************************************************************
*                          CEditTest:PreTranslateMessage
* Inputs:
*       MSG * pMsg: MSG structure
* Result: BOOL
*       ²
* Effect: 
*       This must be overridden to allow the edit control 
*       to receive the Enter and Esc keys.
* Notes:
*       See KB article Q125645: FIX: Edit Control in Property Page Does
*       Not Get Return Keys.  Several bug fixes were made to that code
*       when it was installed here, not the least of which is the
*       incorrect use of sizeof.  The most serious bug fix is that the
*       correct value to return is TRUE, because otherwise the 
*       property *sheet* will next get the message and mess over the world.
*       But returning TRUE will terminate the processing of the message,
*       so we have to ::TranslateMessage and ::DispatchMessage right here!
****************************************************************************/

BOOL CEditTest::PreTranslateMessage(MSG* pMsg) 
{
 if(pMsg->message == WM_KEYDOWN &&
    (pMsg->wParam == VK_RETURN ||
     pMsg->wParam == VK_ESCAPE))
        { /* special key */
         static const TCHAR editclassname[] = _T("edit");
         // We add +1 not for the NUL, which is already accounted for
         // by the DIM size, but to guarantee that a class name like "editor"
         // is not truncated to "edit" and appear to be equal.
         TCHAR focusname[DIM(editclassname) + 1];
         HWND hFocus = ::GetFocus();

         ::GetClassName(hFocus, focusname, DIM(focusname));
         if(lstrcmpi(focusname, editclassname) == 0)
            { /* edit control */
             if(::GetWindowLong(hFocus, GWL_STYLE) & ES_WANTRETURN)
                { /* deal with it */
                 ::TranslateMessage(pMsg);
                 ::DispatchMessage(pMsg);
                 return TRUE; // don't pretranslate
                } /* deal with it */
	     else
	        { /* try for defid */
		 if(Is3())
		    { /* fake it */
		     // Really ugly faking of button click required
		     // because we're simulating the default while using
		     // a CPropertyPage.  No automatic packing of 
		     // parameters, so Win32-specific.
		     SendMessage(WM_COMMAND, 
				    MAKELONG(IDC_DEFAULT, BN_CLICKED),
				    (LPARAM)GetDlgItem(IDC_DEFAULT));
		    } /* fake it */
		 else
		    { /* simulate it */
		     // Much easier to fake button click at API level 4
		     // just force a BM_CLICK to the button and everything
		     // works.
		     ::SendMessage(GetDlgItem(IDC_DEFAULT)->m_hWnd, 
							 BM_CLICK, 0, 0);
		    } /* simulate it */
		} /* try for defid */
            } /* edit control */

         if(::SendMessage(hFocus, WM_GETDLGCODE, 0, 0) & 
             (DLGC_WANTALLKEYS | DLGC_WANTCHARS | DLGC_WANTMESSAGE))
                { /* deal with it here */
                 ::TranslateMessage(pMsg);
                 ::DispatchMessage(pMsg);
                 return TRUE;
                } /* deal with it here */
        } /* special key */

 return CPropertyPage::PreTranslateMessage(pMsg);
}
