// ButtTest.cpp : implementation file
//

#include "stdafx.h"
#include "dlgcodes.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "mybutton.h"
#include "tinybutt.h"
#include "ButtTest.h"
#include "btnmsg.h"
#include "Is95.h"
#include "copy.h"
#include "xform.h"
#include "animate.h"
#include "dlgcode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL mfc_create = TRUE;  // change with debugger

/////////////////////////////////////////////////////////////////////////////
// CButtonTest property page

IMPLEMENT_DYNCREATE(CButtonTest, CPropertyPage)

CButtonTest::CButtonTest() : CPropertyPage(CButtonTest::IDD)
{
        //{{AFX_DATA_INIT(CButtonTest)
        //}}AFX_DATA_INIT
}

CButtonTest::~CButtonTest()
{
}

void CButtonTest::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CButtonTest)
        DDX_Control(pDX, IDC_GROUP1, c_Group1);
        DDX_Control(pDX, IDC_GROUP3, c_Group3);
        DDX_Control(pDX, IDC_GROUP2, c_Group2);
        DDX_Control(pDX, IDC_WORKING3, c_Working3);
        DDX_Control(pDX, IDC_WORKING2, c_Working2);
        DDX_Control(pDX, IDC_WORKING1, c_Working1);
        DDX_Control(pDX, IDC_NO_ANIMATION, c_NoAnimation);
        DDX_Control(pDX, IDC_WS_BORDER, c_WSBorder);
        DDX_Control(pDX, IDC_EDIT, c_Edit);
        DDX_Control(pDX, IDC_STATIC_CONTROL, c_Static);
        DDX_Control(pDX, IDC_MESSAGES, c_Messages);
        DDX_Control(pDX, IDC_BS_FLAT, c_BSFlat);
        DDX_Control(pDX, IDC_ENABLED, c_Enable);
        DDX_Control(pDX, IDC_BUTTON3, c_Button3);
        DDX_Control(pDX, IDC_BUTTON2, c_Button2);
        DDX_Control(pDX, IDC_BUTTON1, c_Button1);
        DDX_Control(pDX, IDC_BS_VCENTER, c_BSVCenter);
        DDX_Control(pDX, IDC_BS_TOP, c_BSTop);
        DDX_Control(pDX, IDC_BS_TEXT, c_BSText);
        DDX_Control(pDX, IDC_BS_RIGHTBUTTON, c_BSRightButton);
        DDX_Control(pDX, IDC_BS_RIGHT, c_BSRight);
        DDX_Control(pDX, IDC_BS_RADIOBUTTON, c_BSRadioButton);
        DDX_Control(pDX, IDC_BS_PUSHLIKE, c_BSPushLike);
        DDX_Control(pDX, IDC_BS_PUSHBUTTON, c_BSPushButton);
        DDX_Control(pDX, IDC_BS_OWNERDRAW, c_BSOwnerDraw);
        DDX_Control(pDX, IDC_BS_NOTIFY, c_BSNotify);
        DDX_Control(pDX, IDC_BS_MULTILINE, c_BSMultiLine);
        DDX_Control(pDX, IDC_BS_LEFTTEXT, c_BSLeftText);
        DDX_Control(pDX, IDC_BS_LEFT, c_BSLeft);
        DDX_Control(pDX, IDC_BS_ICON, c_BSIcon);
        DDX_Control(pDX, IDC_BS_GROUPBOX, c_BSGroupBox);
        DDX_Control(pDX, IDC_BS_DEFPUSHBUTTON, c_BSDefPushButton);
        DDX_Control(pDX, IDC_BS_CHECKBOX, c_BSCheckBox);
        DDX_Control(pDX, IDC_BS_CENTER, c_BSCenter);
        DDX_Control(pDX, IDC_BS_BOTTOM, c_BSBottom);
        DDX_Control(pDX, IDC_BS_BITMAP, c_BSBitmap);
        DDX_Control(pDX, IDC_BS_AUTORADIOBUTTON, c_BSAutoRadioButton);
        DDX_Control(pDX, IDC_BS_AUTOCHECKBOX, c_BSAutoCheckBox);
        DDX_Control(pDX, IDC_BS_AUTO3STATE, c_BSAuto3State);
        DDX_Control(pDX, IDC_BS_3STATE, c_BS3State);
        DDX_Control(pDX, IDC_ALIGNMENT, c_Alignment);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CButtonTest, CPropertyPage)
        //{{AFX_MSG_MAP(CButtonTest)
        ON_WM_TIMER()
        ON_BN_CLICKED(IDC_BS_3STATE, OnBs3state)
        ON_BN_CLICKED(IDC_BS_AUTO3STATE, OnBsAuto3state)
        ON_BN_CLICKED(IDC_BS_AUTOCHECKBOX, OnBsAutocheckbox)
        ON_BN_CLICKED(IDC_BS_AUTORADIOBUTTON, OnBsAutoradiobutton)
        ON_BN_CLICKED(IDC_BS_BITMAP, OnBsBitmap)
        ON_BN_CLICKED(IDC_BS_BOTTOM, OnBsBottom)
        ON_BN_CLICKED(IDC_BS_CENTER, OnBsCenter)
        ON_BN_CLICKED(IDC_BS_CHECKBOX, OnBsCheckbox)
        ON_BN_CLICKED(IDC_BS_DEFPUSHBUTTON, OnBsDefpushbutton)
        ON_BN_CLICKED(IDC_BS_GROUPBOX, OnBsGroupbox)
        ON_BN_CLICKED(IDC_BS_ICON, OnBsIcon)
        ON_BN_CLICKED(IDC_BS_LEFT, OnBsLeft)
        ON_BN_CLICKED(IDC_BS_LEFTTEXT, OnBsLefttext)
        ON_BN_CLICKED(IDC_BS_MULTILINE, OnBsMultiline)
        ON_BN_CLICKED(IDC_BS_NOTIFY, OnBsNotify)
        ON_BN_CLICKED(IDC_BS_OWNERDRAW, OnBsOwnerdraw)
        ON_BN_CLICKED(IDC_BS_PUSHBUTTON, OnBsPushbutton)
        ON_BN_CLICKED(IDC_BS_PUSHLIKE, OnBsPushlike)
        ON_BN_CLICKED(IDC_BS_RADIOBUTTON, OnBsRadiobutton)
        ON_BN_CLICKED(IDC_BS_RIGHT, OnBsRight)
        ON_BN_CLICKED(IDC_BS_RIGHTBUTTON, OnBsRightbutton)
        ON_BN_CLICKED(IDC_BS_TEXT, OnBsText)
        ON_BN_CLICKED(IDC_BS_TOP, OnBsTop)
        ON_BN_CLICKED(IDC_BS_VCENTER, OnBsVcenter)
        ON_BN_CLICKED(IDC_BUTTON, OnButton)
        ON_BN_CLICKED(IDC_DLGCODE, OnDlgcode)
        ON_BN_CLICKED(IDC_ENABLED, OnEnabled)
        ON_BN_CLICKED(IDC_MESSAGES, OnMessages)
        ON_BN_CLICKED(IDC_NO_ANIMATION, OnNoAnimation)
        ON_BN_CLICKED(IDC_SENDMESSAGES, OnSendmessages)
        ON_BN_CLICKED(IDC_SETTEXT1, OnSettext1)
        ON_BN_CLICKED(IDC_SETTEXT2, OnSettext2)
        ON_BN_CLICKED(IDC_SETTEXT3, OnSettext3)
        ON_BN_CLICKED(IDC_WS_BORDER, OnWsBorder)
        ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
        ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
        ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
        ON_BN_CLICKED(IDC_BS_FLAT, OnBsFlat)
        ON_BN_DOUBLECLICKED(IDC_BUTTON1, OnDoubleclickedButton1)
        ON_BN_DOUBLECLICKED(IDC_BUTTON2, OnDoubleclickedButton2)
        ON_BN_DOUBLECLICKED(IDC_BUTTON3, OnDoubleclickedButton3)
        ON_BN_SETFOCUS(IDC_BUTTON1, OnSetFocusButton1)
        ON_BN_SETFOCUS(IDC_BUTTON2, OnSetFocusButton2)
        ON_BN_SETFOCUS(IDC_BUTTON3, OnSetFocusButton3)
        ON_BN_KILLFOCUS(IDC_BUTTON1, OnKillFocusButton1)
        ON_BN_KILLFOCUS(IDC_BUTTON2, OnKillFocusButton2)
        ON_BN_KILLFOCUS(IDC_BUTTON3, OnKillFocusButton3)
        ON_BN_CLICKED(IDC_HDEFAULT, OnHdefault)
        ON_BN_CLICKED(IDC_VDEFAULT, OnVdefault)
        ON_BN_CLICKED(IDC_GROUP1, OnGroup1)
        ON_BN_CLICKED(IDC_GROUP2, OnGroup2)
        ON_BN_CLICKED(IDC_GROUP3, OnGroup3)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        ON_MESSAGE(UWM_DRAWITEM, OnLogDrawItem)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonTest message handlers

/****************************************************************************
*                         CButtonTest::getButtonType
* Inputs:
*       DWORD style: Style flags
* Result: DWORD
*       The bits that determine the button style
****************************************************************************/

DWORD CButtonTest::getButtonType(DWORD style)
    {
     return style & (BS_PUSHBUTTON | BS_DEFPUSHBUTTON | BS_CHECKBOX |
                     BS_AUTOCHECKBOX | BS_RADIOBUTTON | BS_3STATE |
                     BS_AUTO3STATE | BS_GROUPBOX | BS_USERBUTTON |
                     BS_AUTORADIOBUTTON | BS_PUSHLIKE);
    }

/****************************************************************************
*                       CButtonTest::removeDisplayType
* Inputs:
*       DWORD style: Style flags
* Result: DWORD
*       
* Effect: 
*       Style flags with all display types removed
****************************************************************************/

DWORD CButtonTest::removeDisplayType(DWORD style)
    {
     return style & ~(BS_BITMAP | BS_ICON | BS_TEXT);
    }

/****************************************************************************
*                         CButtonTest::getDisplayType
* Result: DWORD
*       The display flags based on the controls
****************************************************************************/

DWORD CButtonTest::getDisplayType()
    {
     if(c_BSText.GetCheck())
        return BS_TEXT;
     else
     if(c_BSBitmap.GetCheck())
        return BS_BITMAP;
     else
     if(c_BSIcon.GetCheck())
        return BS_ICON;
     else
        return 0;
    }

/****************************************************************************
*                         CButtonTest::enableControls
* Result: void
*       
* Effect: 
*       Enables controls depending upon the platform
****************************************************************************/

void CButtonTest::enableControls()
    {
     c_BSIcon.EnableWindow(!Is3());  // not available on 3.51
     c_BSBitmap.EnableWindow(!Is3());  // not available on 3.51
     c_BSNotify.EnableWindow(!Is3());  // not available on 3.51
     c_BSFlat.EnableWindow(!Is3());  // not available on 3.51
     c_BSPushLike.EnableWindow(!Is3());  // not available on 3.51

     // We disable the alignment unless it is an owner-draw button
     // we base the decision on the Button2 style because we sometimes
     // set button1 to be a groupbox while leaving the other buttons alone

     DWORD type = getButtonType(c_Button2.GetStyle());
     BOOL enable = (type == BS_OWNERDRAW || !Is3());
     c_Alignment.EnableWindow(enable);

     c_BSBottom.EnableWindow(enable);
     c_BSCenter.EnableWindow(enable);
     c_BSLeft.EnableWindow(enable);
     c_BSRight.EnableWindow(enable);
     c_BSTop.EnableWindow(enable);
     c_BSVCenter.EnableWindow(enable);
     c_BSMultiLine.EnableWindow(enable);  


     switch(type)
        { /* lefttext */
         case BS_AUTOCHECKBOX:
         case BS_CHECKBOX:
         case BS_RADIOBUTTON:
         case BS_AUTORADIOBUTTON:
         case BS_3STATE:
         case BS_AUTO3STATE:
                 enable = TRUE;
                 break;
         default:
                 enable = FALSE;
                 break;
        } /* lefttext */

     c_BSRightButton.EnableWindow(enable);
     c_BSLeftText.EnableWindow(enable);

     switch(type)
        { /* groupable? */
         case BS_RADIOBUTTON:
         case BS_AUTORADIOBUTTON:
                 enable = TRUE;
                 break;
         default:
                 enable = FALSE;
                 break;
        } /* groupable? */

     c_Group1.EnableWindow(enable);
     c_Group2.EnableWindow(enable);
     c_Group3.EnableWindow(enable);

     if(enable)
        { /* set values */
         c_Group1.SetCheck(c_Button1.GetStyle() & WS_GROUP);
         c_Group2.SetCheck(c_Button2.GetStyle() & WS_GROUP);
         c_Group3.SetCheck(c_Button3.GetStyle() & WS_GROUP);
        } /* set values */

     c_Group1.ShowWindow(enable ? SW_SHOW : SW_HIDE);
     c_Group2.ShowWindow(enable ? SW_SHOW : SW_HIDE);
     c_Group3.ShowWindow(enable ? SW_SHOW : SW_HIDE);
    }

/****************************************************************************
*                            CButtonTest::setIcon
* Inputs:
*       CButton * button: Button to set image in
*       int id: Resource id of image
* Result: void
*       
* Effect: 
*       Stores the image in the button
****************************************************************************/

void CButtonTest::setIcon(CButton * button, int id)
    {
     HICON icon;

     icon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(id),
                         IMAGE_ICON, 
                         ::GetSystemMetrics(SM_CXSMICON),
                         ::GetSystemMetrics(SM_CYSMICON),
                         LR_DEFAULTCOLOR);
     button->SetIcon(icon);
    }

BOOL CButtonTest::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        // Save the parent window handle because in a CPropertyPage, the
        // 'this' pointer references not the current dialog, but the
        // parent CPropertySheet.  We find this undocumented behavior
        // rather surprising.

        // We can choose any button in the set.

        ControlParent = c_Button1.GetParent();

        CString s;
        s.LoadString(IDS_BUTTON1);
        c_Messages.initialize(c_Messages.GetStringWidth(s));
        c_Messages.setWrap(TRUE);
        
        // Because we may have been subclassed by CTL3D, we probably have
        // other than the standard font selected.  Save this in case we
        // need a font again.
        orgfont = c_Button1.GetFont();

        setIcon(&c_Button1, IDI_ICON1);
        setIcon(&c_Button2, IDI_ICON2);
        setIcon(&c_Button3, IDI_ICON3);

        // Save the size of the bounding box, then hide it because
        // its sole purpose is to mark the bounding area

        c_Static.GetWindowRect(&orgsize);
        ScreenToClient(&orgsize);
        c_Static.ShowWindow(SW_HIDE);

        // Save the sizes of the three placeholder buttons.

        c_Button1.GetWindowRect(&size1);
        ScreenToClient(&size1);

        c_Button2.GetWindowRect(&size2);
        ScreenToClient(&size2);

        c_Button3.GetWindowRect(&size3);
        ScreenToClient(&size3);

        CheckRadioButton(IDC_BS_AUTOCHECKBOX, IDC_BS_3STATE, IDC_BS_PUSHBUTTON);
        CheckRadioButton(IDC_BS_LEFT, IDC_HDEFAULT, IDC_HDEFAULT);
        CheckRadioButton(IDC_BS_TOP, IDC_VDEFAULT, IDC_VDEFAULT);

        CheckRadioButton(IDC_BS_BITMAP, IDC_BS_TEXT, IDC_BS_TEXT);
        c_Enable.SetCheck(TRUE);

        enableControls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                                  addStyle
* Inputs:
*       CButton * button: Button whose style is to be modified
*       DWORD style: Style flag to add
*       BOOL add: TRUE to add, FALSE to remove
* Result: void
*       
* Effect: 
*       Adds or removes the style flag
****************************************************************************/

void CButtonTest::addStyle(CButton * button, DWORD style, BOOL add)
    {
     DWORD newstyle = button->GetStyle();

     if(add)
        newstyle |= style;
     else
        newstyle &= ~style;

     ::SetWindowLong(button->m_hWnd, GWL_STYLE, newstyle);
    }

void CButtonTest::OnBs3state() 
{
 recreateWindows(BS_3STATE);
        
}

void CButtonTest::OnBsAuto3state() 
{
 recreateWindows(BS_AUTO3STATE);
        
}

void CButtonTest::OnBsAutocheckbox() 
{
 recreateWindows(BS_AUTOCHECKBOX);
        
}

void CButtonTest::OnBsAutoradiobutton() 
{       
 recreateWindows(BS_AUTORADIOBUTTON);
}


/****************************************************************************
*                        CButtonTest::removeHAlignment
* Inputs:
*       DWORD style: style we already have
* Result: DWORD
*       style with all alignment bits removed
****************************************************************************/

DWORD CButtonTest::removeHAlignment(DWORD style)
    {
     return style & ~(BS_LEFT | BS_RIGHT | BS_CENTER );
    }

/****************************************************************************
*                        CButtonTest::removeVAlignment
* Inputs:
*       DWORD style: style we already have
* Result: DWORD
*       style with all vertical alignment bits removed
****************************************************************************/

DWORD CButtonTest::removeVAlignment(DWORD style)
    {
     return style & ~(BS_TOP | BS_BOTTOM | BS_VCENTER);
     
    }

/****************************************************************************
*                          CButtonTest::setHAlignment
* Inputs:
*       CButton * button: button whose alignment is to be changed
*       DWORD alignment: New alignment
* Result: void
*       
* Effect: 
*       Changes the horizontal alignment of a button
****************************************************************************/

void CButtonTest::setHAlignment(CButton * button, DWORD style)
    {
     DWORD newstyle = button->GetStyle();

     newstyle = removeHAlignment(newstyle);

     newstyle |= style;
     ::SetWindowLong(button->m_hWnd, GWL_STYLE, newstyle);
     button->InvalidateRect(NULL);

     // We also need to invalidate the area of our window so that the
     // old button image is erased
     CRect w;
     button->GetWindowRect(&w);
     ScreenToClient(&w);
     InvalidateRect(&w);
    }

/****************************************************************************
*                          CButtonTest::setVAlignment
* Inputs:
*       CButton * button: button whose alignment is to be changed
*       DWORD alignment: New alignment
* Result: void
*       
* Effect: 
*       Changes the vertical alignment of a button
****************************************************************************/

void CButtonTest::setVAlignment(CButton * button, DWORD style)
    {
     DWORD newstyle = button->GetStyle();

     newstyle = removeVAlignment(newstyle);

     newstyle |= style;
     ::SetWindowLong(button->m_hWnd, GWL_STYLE, newstyle);
     button->InvalidateRect(NULL);

     // We also need to invalidate the area of our window so that the
     // old button image is erased
     CRect w;
     button->GetWindowRect(&w);
     ScreenToClient(&w);
     InvalidateRect(&w);
    }

/****************************************************************************
*                         CButtonTest::setHAlignments
* Inputs:
*       DWORD style: New style flag
* Result: void
*       
* Effect: 
*       Changes the horizontal alignment for each of the buttons
****************************************************************************/

void CButtonTest::setHAlignments(DWORD style)
    {
     setHAlignment(&c_Button1, style);
     setHAlignment(&c_Button2, style);
     setHAlignment(&c_Button3, style);
    }

/****************************************************************************
*                         CButtonTest::setVAlignments
* Inputs:
*       DWORD style: New style flag
* Result: void
*       
* Effect: 
*       Changes the vertical alignment for each of the buttons
****************************************************************************/

void CButtonTest::setVAlignments(DWORD style)
    {
     setVAlignment(&c_Button1, style);
     setVAlignment(&c_Button2, style);
     setVAlignment(&c_Button3, style);
    }


void CButtonTest::OnBsCheckbox() 
{
 recreateWindows(BS_CHECKBOX);
        
}

void CButtonTest::OnBsDefpushbutton() 
{
 recreateWindows(BS_DEFPUSHBUTTON);
        
}

void CButtonTest::OnBsFlat() 
{
 recreateWindows(BS_FLAT);
        
}

void CButtonTest::OnBsGroupbox() 
{
 recreateWindows(BS_GROUPBOX);
}


void CButtonTest::OnBsLefttext() 
{
 c_BSRightButton.SetCheck(c_BSLeftText.GetCheck());
 recreateWindows(getButtonType(c_Button2.GetStyle()));
 enableControls();
}

void CButtonTest::OnBsMultiline() 
{
 addStyle(&c_Button1, BS_MULTILINE, c_BSMultiLine.GetCheck());
 addStyle(&c_Button2, BS_MULTILINE, c_BSMultiLine.GetCheck());
 addStyle(&c_Button3, BS_MULTILINE, c_BSMultiLine.GetCheck());

 DWORD type = getButtonType(c_Button2.GetStyle());
 if(type == BS_OWNERDRAW)
    { /* redraw */
     c_Button1.InvalidateRect(NULL);
     c_Button2.InvalidateRect(NULL);
     c_Button3.InvalidateRect(NULL);
    } /* redraw */
     
}

void CButtonTest::OnBsRadiobutton() 
{
 recreateWindows(BS_RADIOBUTTON);
}

void CButtonTest::OnBsRightbutton() 
{
 c_BSLeftText.SetCheck(c_BSRightButton.GetCheck());
 recreateWindows(getButtonType(c_Button2.GetStyle()));
 enableControls();
}

void CButtonTest::OnBsOwnerdraw() 
{
 recreateWindows(BS_OWNERDRAW);
}

void CButtonTest::OnBsPushbutton() 
{
 recreateWindows(BS_PUSHBUTTON);
}


//=============================================================================
// Style modifications
void CButtonTest::OnBsNotify() 
{
 addStyle(&c_Button1, BS_NOTIFY, c_BSNotify.GetCheck());
 addStyle(&c_Button2, BS_NOTIFY, c_BSNotify.GetCheck());
 addStyle(&c_Button3, BS_NOTIFY, c_BSNotify.GetCheck());
}

void CButtonTest::OnBsPushlike() 
{
 addStyle(&c_Button1, BS_PUSHLIKE, c_BSPushLike.GetCheck());
 addStyle(&c_Button2, BS_PUSHLIKE, c_BSPushLike.GetCheck());
 addStyle(&c_Button3, BS_PUSHLIKE, c_BSPushLike.GetCheck());
 c_Button1.InvalidateRect(NULL);
 c_Button2.InvalidateRect(NULL);
 c_Button3.InvalidateRect(NULL);
 enableControls();
}


//=============================================================================
// Display style: icon, bitmap, or text

void CButtonTest::setDisplayType(CButton * button, DWORD style)
{
 DWORD newstyle = button->GetStyle();
 newstyle = removeDisplayType(newstyle) | style;
 ::SetWindowLong(button->m_hWnd, GWL_STYLE, newstyle);
}

void CButtonTest::setDisplayTypes(DWORD style)
{
 setDisplayType(&c_Button1, style);
 setDisplayType(&c_Button2, style);
 setDisplayType(&c_Button3, style);
}

void CButtonTest::OnBsBitmap() 
{
 setDisplayTypes(BS_BITMAP);
}

void CButtonTest::OnBsIcon() 
{
 setDisplayTypes(BS_ICON);
}

void CButtonTest::OnBsText() 
{
 setDisplayTypes(BS_TEXT);
}

//=============================================================================
// Vertical alignment: top, vcenter, bottom, default

void CButtonTest::OnBsBottom() 
{
 setVAlignments(BS_BOTTOM);
}

void CButtonTest::OnBsTop() 
{
 setVAlignments(BS_TOP);
}

void CButtonTest::OnBsVcenter() 
{
 setVAlignments(BS_VCENTER);
}

void CButtonTest::OnVdefault() 
{
 setVAlignments(0);
}


//=============================================================================
// Horizontal alignment: left, center, right, default

void CButtonTest::OnBsLeft() 
{
 setHAlignments(BS_LEFT);
}

void CButtonTest::OnBsCenter() 
{
 setHAlignments(BS_CENTER);
}

void CButtonTest::OnBsRight() 
{
 setHAlignments(BS_RIGHT);
}

void CButtonTest::OnHdefault() 
{
 setHAlignments(0);
}


void CButtonTest::OnButton() 
{
        // TODO: Add your control notification handler code here
        
}

void CButtonTest::OnDlgcode() 
{
 CGetDlgCode dlg;

 dlg.ctl = &c_Button1;

 dlg.DoModal();

 // Now set the same dlg codes for the other two buttons

 UINT dlgcodes;
 dlgcodes = getDlgCode(c_Button1.m_hWnd);
 setDlgCode(c_Button2.m_hWnd, dlgcodes);
 setDlgCode(c_Button3.m_hWnd, dlgcodes);
        
}

void CButtonTest::OnEnabled() 
{
 BOOL enable = c_Enable.GetCheck();
 c_Button1.EnableWindow(enable);
 c_Button2.EnableWindow(enable);
 c_Button3.EnableWindow(enable);
        
}

void CButtonTest::OnMessages() 
{
        // TODO: Add your control notification handler code here
        
}

void CButtonTest::OnNoAnimation() 
{
 // Nothing done at the moment; passive control 
}

void CButtonTest::OnSendmessages() 
{
 CButtonMessages dlg;
 dlg.c_Messages = &c_Messages;
 dlg.c_Button1 = &c_Button1;
 dlg.c_Button2 = &c_Button2;
 dlg.c_Button3 = &c_Button3;

 CRect r;
 GetParent()->GetWindowRect(&r);
 GetParent()->SetWindowPos(NULL, 0, 
                        ::GetSystemMetrics(SM_CYSCREEN) - r.Height(), 0, 0, 
                        SWP_NOSIZE | SWP_NOZORDER);
                        
 dlg.DoModal();
}

/****************************************************************************
*                           CButtonTest::transform
* Inputs:
*       CString s: Input string, may contain escapes
* Result: CSTring
*       copy of string with escapes transformed
****************************************************************************/

CString CButtonTest::transform(CString s)
    {
     return ::transform(s);
    }

/****************************************************************************
*                         CButtonTest::setButtonText
* Inputs:
*       CButton * button:
*       int id: ID of button
* Result: void
*       
* Effect: 
*       Sets the button text for the button
****************************************************************************/

void CButtonTest::setButtonText(CButton * button, int id)
    {
     CString bname;
     bname.LoadString(id);

     CString text;
     c_Edit.GetWindowText(text);

     button->SetWindowText(transform(text));

     CString t;
     t.LoadString(IDS_WM_SETTEXT);

     CString s;
     s.Format(_T("%s(0, \"%s\")"), t, text);
     c_Messages.addMessage(button->m_hWnd, bname, s);
     
    }

void CButtonTest::OnSettext1() 
{
 setButtonText(&c_Button1, IDS_BUTTON1);
}

void CButtonTest::OnSettext2() 
{
 setButtonText(&c_Button2, IDS_BUTTON2);
}

void CButtonTest::OnSettext3() 
{
 setButtonText(&c_Button3, IDS_BUTTON3);
}

void CButtonTest::OnWsBorder() 
{
 DWORD style = c_Button1.GetStyle();
 recreateWindows(style);
        
}

/****************************************************************************
*                          CButtonTest::getAlignment
* Result: DWORD
*       A word containing the alignment style based on the control settings,
*       or 0 if alignment is not permitted for this control.
*       Contains both the horizontal and vertical alignment bits.
* Notes:
*       We return alignment only if there is a meaningful button checked.
*       If there is no button checked here, it is the "Default" selection
*       which is 0.
****************************************************************************/

DWORD CButtonTest::getAlignment()
    {
     // Check for enablement.  We can do this by checking the global
     // enablement window, the group box that surrounds all the buttons...

     if(!c_Alignment.IsWindowEnabled())
        return 0;

     DWORD align = 0;

     // read the vertical alignment styles
     if(c_BSTop.GetCheck())
        align |= BS_TOP;
     else
     if(c_BSBottom.GetCheck())
        align |= BS_BOTTOM;
     else
     if(c_BSVCenter.GetCheck())
        align |= BS_VCENTER;

     // Read the horizontal alignment styles
     if(c_BSLeft.GetCheck())
        align |= BS_LEFT;
     else
     if(c_BSCenter.GetCheck())
        align |= BS_CENTER;
     else
     if(c_BSRight.GetCheck())
        align |= BS_RIGHT;

     return align;
    }

/****************************************************************************
*                         CButtonTest::recreateWindow
* Inputs:
*       CButton * button: Button to recreate
*       int id: ID to display in log
*       CRect rect: Bounding rectangle to use
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

void CButtonTest::recreateWindow(CButton * button, int id, CRect rect, DWORD style)
    {
     CButton newWindow;
     CFont * font;
     HICON icon;
     UINT dlgcodes;

     dlgcodes = getDlgCode(button->m_hWnd);

     CWnd * oldparent = button->GetParent();

     font = button->GetFont();
     icon = button->GetIcon();

     // Add in the orthogonal style bits
     // WS_BORDER
     // WS_DISABLED

     // BS_NOTIFY
     // BS_MULTILINE

     if(c_WSBorder.GetCheck())
        style |= WS_BORDER;
     else
        style &= ~WS_BORDER;

     if(!c_Enable.GetCheck())
        style |= WS_DISABLED;
     else
        style &= ~WS_DISABLED;

     if(c_BSNotify.IsWindowEnabled() && c_BSNotify.GetCheck())
        style |= BS_NOTIFY;
     else
        style &= ~BS_NOTIFY;

     if(c_BSMultiLine.IsWindowEnabled() && c_BSMultiLine.GetCheck())
        style |= BS_MULTILINE;
     else
        style &= ~BS_MULTILINE;

     if(c_BSPushLike.IsWindowEnabled() && c_BSPushLike.GetCheck())
        style |= BS_PUSHLIKE;
     else
        style &= ~BS_PUSHLIKE;

     // add in the display type

     style = removeDisplayType(style) | getDisplayType();

     // Because we keep lefttext and rightbutton in "lock-sync" we need only
     // need to check one of them.  Ignore the bit if the control is
     // not enabled.
     if(c_BSLeftText.IsWindowEnabled() && c_BSLeftText.GetCheck())
        style |= BS_LEFTTEXT;
     else
        style &= ~BS_LEFTTEXT;

     if(c_BSIcon.IsWindowEnabled() && c_BSIcon.GetCheck())
        style |= BS_ICON;
     else
        style &= ~BS_ICON;

     if(c_BSBitmap.IsWindowEnabled() && c_BSBitmap.GetCheck())
        style |= BS_BITMAP;
     else
        style &= ~BS_BITMAP;

     style = removeVAlignment(removeHAlignment(style)) | getAlignment();

     CString s;
     button->GetWindowText(s);

     int ctlid = button->GetDlgCtrlID();

     style |= WS_CHILD | WS_TABSTOP;

     // We create it without the visible style so we can play the
     // animation game later.
     
     HWND newwnd ;

     if(mfc_create)
         newWindow.Create(s, style & ~WS_VISIBLE, rect, ControlParent, ctlid);

     else
         newwnd = ::CreateWindow(_T("BUTTON"), s, style & ~WS_VISIBLE, 
                                  rect.left, rect.top, 
                                  rect.Width(), rect.Height(),
                                  this->m_hWnd,
                                  (HMENU)ctlid,
                                  AfxGetInstanceHandle(),
                                  NULL);

     // we've created it, log this effect
     CString t;
     t.LoadString(id);
     
     // Lie about the style by adding in the visible style, since the
     // omission is a hack for visual effect in the Explorer.

#ifdef _DEBUG
     if(mfc_create)
         s.Format(_T("0x%08x->0x%08x [0x%08x->0x%08x]"), style | WS_VISIBLE, 
                                newWindow.GetStyle(), oldparent->m_hWnd, 
                                newWindow.GetParent()->m_hWnd);
     else
         s.Format(_T("0x%08x->0x%08x [0x%08x->0x%08x]"), style | WS_VISIBLE, 
                                ::GetWindowLong(newwnd, GWL_STYLE), 
                                oldparent->m_hWnd, ::GetParent(newwnd));
#else
     s.Format(_T("0x%08x"), style | WS_VISIBLE); 
#endif
     c_Messages.addMessage(newWindow.m_hWnd, t, s);

     HWND oldwnd = button->UnsubclassWindow();
     ::DestroyWindow(oldwnd);

     HWND wnd;
     if(mfc_create)
        wnd = newWindow.UnsubclassWindow();
     else
        wnd = newwnd;

     // SubclassWindow will set up all the right pointers.  We need to
     // do this because different types of edit windows subclass to different
     // handlers.
     button->SubclassWindow(wnd);

     if(font != NULL)
        button->SetFont(font);
     else
        button->SetFont(orgfont);

     if(font != NULL)
        orgfont = font;
     if(icon != NULL)
        button->SetIcon(icon);

     setDlgCode(button->m_hWnd, dlgcodes);

    }

/****************************************************************************
*                        CButtonTest::recreateWindows
* Inputs:
*       DWORD style: Style flags to use
* Result: void
*       
* Effect: 
*       Re-creates all button windows
****************************************************************************/

void CButtonTest::recreateWindows(DWORD style)
    {
     CRect rect = orgsize;
     
     // If we are creating a group box, create only one of them;
     //
     switch(style)
        { /* styles */
         case BS_GROUPBOX:
             recreateWindow(&c_Button1, IDS_BUTTON1, orgsize, style);
             break;
         case BS_DEFPUSHBUTTON:
             recreateWindow(&c_Button1, IDS_BUTTON1, size1, BS_PUSHBUTTON);
             recreateWindow(&c_Button2, IDS_BUTTON2, size2, BS_DEFPUSHBUTTON);
             recreateWindow(&c_Button3, IDS_BUTTON1, size3, BS_PUSHBUTTON);
             // Because we are in the Control Explorer, we have to take
             // this extra step to ensure that the control really is the
             // default...
             SetDefID(c_Button2.GetDlgCtrlID());
             break;
         default:
             recreateWindow(&c_Button1, IDS_BUTTON1, size1, style | WS_GROUP);
             recreateWindow(&c_Button2, IDS_BUTTON2, size2, style);
             recreateWindow(&c_Button3, IDS_BUTTON3, size3, style);
             break;
        } /* styles */
     
     if(c_NoAnimation.GetCheck())
        { /* just do it */
         c_Button1.ShowWindow(SW_SHOW);
         c_Button2.ShowWindow(SW_SHOW);
         c_Button3.ShowWindow(SW_SHOW);
        
         enableControls();
        } /* just do it */
     else
        { /* animate */
         c_Group1.ShowWindow(SW_HIDE);
         c_Group2.ShowWindow(SW_HIDE);
         c_Group3.ShowWindow(SW_HIDE);

         // Cute effect for demonstration of reconstruction

         beginAnimation(&c_Button1);
        } /* animate */
    }

void CButtonTest::OnTimer(UINT nIDEvent) 
{
        endAnimation(&c_Button1);

        c_Button2.ShowWindow(SW_SHOW);
        c_Button3.ShowWindow(SW_SHOW);

        enableControls();

        CPropertyPage::OnTimer(nIDEvent);
}

/****************************************************************************
*                          CButtonTest::hasState
* Inputs:
*       CButton * button: Button to test
* Result: BOOL
*       TRUE if the button has state for .GetCheck()
*       FALSE if the button has no state
****************************************************************************/

BOOL CButtonTest::hasState(CButton * button)
    {
     DWORD style = button->GetStyle();
     switch(getButtonType(style))
        { /* style */
         case BS_CHECKBOX:
         case BS_AUTOCHECKBOX:
         case BS_RADIOBUTTON:
         case BS_3STATE:
         case BS_AUTO3STATE:
         case BS_AUTORADIOBUTTON:
         case BS_PUSHLIKE:
                 return TRUE;
        } /* style */

     return FALSE;
    }

/****************************************************************************
*                          CButtonTest::buttonEvent
* Inputs:
*       CButton * button: Button which caused the event
*       int id: ID of button
*       int event: event id
* Result: void
*       
* Effect: 
*       Posts a message to the message box
****************************************************************************/

void CButtonTest::buttonEvent(CButton * button, int id, int event)
    {
     CString bname;
     CString ename;

     bname.LoadString(id);
     ename.LoadString(event);

     
     // If it is a button, tell its current state

     if(hasState(button))
        { /* has state */
         CString fmt;
         fmt.LoadString(IDS_PB_STATE);
         
         int checked = button->GetCheck();
         CString statestr;
         switch(checked)
            { /* state */
             case 0:
                     statestr.LoadString(IDS_UNCHECKED);
                     break;
             case 1:
                     statestr.LoadString(IDS_CHECKED);
                     break;
             case 2:
                     statestr.LoadString(IDS_3STATE);
                     break;
            } /* state */
         
         CString s;
         s.Format(fmt, bname, ename, checked, statestr);
         c_Messages.addMessage(button->m_hWnd, bname, s);
        } /* has state */
     else
        { /* no state */
         c_Messages.addMessage(button->m_hWnd, bname, ename);
        } /* no state */

    }


void CButtonTest::OnGroup1() 
{
 addStyle(&c_Button1, WS_GROUP, c_Group1.GetCheck());
}

void CButtonTest::OnGroup2() 
{
 addStyle(&c_Button2, WS_GROUP, c_Group2.GetCheck());
}

void CButtonTest::OnGroup3() 
{
 addStyle(&c_Button3, WS_GROUP, c_Group3.GetCheck());
}


void CButtonTest::OnButton1() 
{
 buttonEvent(&c_Button1, IDS_BUTTON1, IDS_BN_CLICKED);
}

void CButtonTest::OnButton2() 
{
 buttonEvent(&c_Button2, IDS_BUTTON2, IDS_BN_CLICKED);
}

void CButtonTest::OnButton3() 
{
 buttonEvent(&c_Button3, IDS_BUTTON3, IDS_BN_CLICKED);
}


void CButtonTest::OnDoubleclickedButton1() 
{
 buttonEvent(&c_Button1, IDS_BUTTON1, IDS_BN_DOUBLECLICKED);
}

void CButtonTest::OnDoubleclickedButton2() 
{
 buttonEvent(&c_Button2, IDS_BUTTON2, IDS_BN_DOUBLECLICKED);
}

void CButtonTest::OnDoubleclickedButton3() 
{
 buttonEvent(&c_Button3, IDS_BUTTON3, IDS_BN_DOUBLECLICKED);
}

void CButtonTest::OnSetFocusButton1() 
{
 buttonEvent(&c_Button1, IDS_BUTTON1, IDS_BN_SETFOCUS);
}

void CButtonTest::OnSetFocusButton2() 
{
 buttonEvent(&c_Button2, IDS_BUTTON2, IDS_BN_SETFOCUS);
}

void CButtonTest::OnSetFocusButton3() 
{
 buttonEvent(&c_Button3, IDS_BUTTON3, IDS_BN_SETFOCUS);
}

//---------------------BN_KILLFOCUS-----------------------------
void CButtonTest::OnKillFocusButton1() 
{
 buttonEvent(&c_Button1, IDS_BUTTON1, IDS_BN_KILLFOCUS);
}

void CButtonTest::OnKillFocusButton2() 
{
 buttonEvent(&c_Button2, IDS_BUTTON2, IDS_BN_KILLFOCUS);
}

void CButtonTest::OnKillFocusButton3() 
{
 buttonEvent(&c_Button3, IDS_BUTTON3, IDS_BN_KILLFOCUS);
}



void CButtonTest::OnCopy() 
{
 copyToClipboard(&c_Static);
}

/****************************************************************************
*                         CButtonTest::OnLogDrawItem
* Inputs:
*       WPARAM: window handle
*       LPARAM lParam: (LPARAM)(LPCTSTR)
* Result: LRESULT
*       0, always (logically void)
* Effect: 
*       Logs the message
* Notes:
*       The way of getting the button ID is a bit kludgy, but it will do
*       for purposes of the explorer
****************************************************************************/

LRESULT CButtonTest::OnLogDrawItem(WPARAM wParam, LPARAM lParam)
    {
     HWND hwnd = (HWND)wParam;
     LPCTSTR msg = (LPCTSTR) lParam;
     
     CString bname;

     if(hwnd == c_Button1.m_hWnd)
        bname.LoadString(IDS_BUTTON1);
     else
     if(hwnd == c_Button2.m_hWnd)
        bname.LoadString(IDS_BUTTON2);
     else
     if(hwnd == c_Button3.m_hWnd)
        bname.LoadString(IDS_BUTTON3);

     c_Messages.addMessage(hwnd, bname, msg);

     return 0;
    }
