// CtlColor.cpp : implementation file
//

#include "stdafx.h"
#include "CtlColorExplorer.h"
#include "Color.h"
#include "rgb.h"
#include "CtlColor.h"
#include "colors.h"
#include "sibling.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtlColorParms property page

IMPLEMENT_DYNCREATE(CCtlColorParms, CPropertyPage)

CCtlColorParms::CCtlColorParms(UINT caption, UINT msg) : CPropertyPage(CCtlColorParms::IDD, caption)
{
        //{{AFX_DATA_INIT(CCtlColorParms)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT

        msgid = msg;
}

CCtlColorParms::~CCtlColorParms()
{
}

void CCtlColorParms::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CCtlColorParms)
        DDX_Control(pDX, IDC_TEXTCOLOR_CAPTION, c_TextColorCaption);
        DDX_Control(pDX, IDC_BKCOLOR_CAPTION, c_BkColorCaption);
        DDX_Control(pDX, IDC_BKBRUSH_CAPTION, c_BkBrushCaption);
        DDX_Control(pDX, IDC_FONTGROUP, c_FontGroup);
        DDX_Control(pDX, IDC_CHANGEFONT, c_ChangeFont);
        DDX_Control(pDX, IDC_SAMPLE, c_Sample);
        DDX_Control(pDX, IDC_CLEARFONT, c_ClearFont);
        DDX_Control(pDX, IDC_NOSETFONT, c_NoSetFont);
        DDX_Control(pDX, IDC_FACENAME, c_FaceName);
        DDX_Control(pDX, IDC_RGBTEXT, c_RGBText);
        DDX_Control(pDX, IDC_RGBBR, c_RGBBrush);
        DDX_Control(pDX, IDC_RGBBKG, c_RGBBkg);
        DDX_Control(pDX, IDC_BKBRUSH, c_BkBrush);
        DDX_Control(pDX, IDC_TEXTCOLOR, c_TextColor);
        DDX_Control(pDX, IDC_BKCOLOR, c_BkColor);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCtlColorParms, CPropertyPage)
        //{{AFX_MSG_MAP(CCtlColorParms)
        ON_CBN_SELENDOK(IDC_BKBRUSH, OnSelendokBkbrush)
        ON_CBN_SELENDOK(IDC_BKCOLOR, OnSelendokBkcolor)
        ON_CBN_SELENDOK(IDC_TEXTCOLOR, OnSelendokTextcolor)
        ON_BN_CLICKED(IDC_CHANGEFONT, OnChangefont)
        ON_BN_CLICKED(IDC_NOSETFONT, OnNosetfont)
        ON_BN_CLICKED(IDC_CLEARFONT, OnClearfont)
        ON_WM_DESTROY()
        ON_BN_CLICKED(IDC_TRANSPARENT, OnTransparent)
        ON_BN_CLICKED(IDC_OPAQUE, OnOpaque)
        //}}AFX_MSG_MAP
        ON_MESSAGE(PSM_QUERYSIBLINGS, OnQuerySiblings)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtlColorParms message handlers

/****************************************************************************
*                      CCtlColorParms::enableFontOptions
* Result: void
*       
* Effect: 
*       Enables or disables the font options
****************************************************************************/

void CCtlColorParms::enableFontOptions()
    {
     // note that we can't change the font for a MessageBox, or its
     // background brush
     if(msgid == CTLCOLOR_MSGBOX)
        { /* disable unused controls */
         c_ChangeFont.EnableWindow(FALSE);
         c_BkColor.EnableWindow(FALSE);
         c_BkColorCaption.EnableWindow(FALSE);
        } /* disable unused controls */
        
     BOOL enable = c_ChangeFont.IsWindowEnabled();

     c_NoSetFont.EnableWindow(enable && font.m_hObject != NULL);
     c_ClearFont.EnableWindow(enable && font.m_hObject != NULL);
     c_FontGroup.EnableWindow(enable);
     if(enable)
        c_Sample.SetFont(&font);
     else
        c_Sample.ShowWindow(SW_HIDE);

    }

void CCtlColorParms::enableColorOptions()
    {
     BOOL opaque = IsDlgButtonChecked(IDC_OPAQUE);
     c_BkColor.EnableWindow(opaque);
     c_BkColorCaption.EnableWindow(opaque);
     c_RGBBkg.ShowWindow(opaque ? SW_SHOW : SW_HIDE);

     if(msgid == CTLCOLOR_SCROLLBAR)
        { /* no text in scrollbar */
         c_TextColor.EnableWindow(FALSE);
         c_TextColorCaption.EnableWindow(FALSE);
         c_RGBText.ShowWindow(SW_HIDE);
        } /* no text in scrollbar */
    }

BOOL CCtlColorParms::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        c_BkColor.LoadColors();
        if(msgid == CTLCOLOR_MSGBOX)
           c_BkColor.SetColor(GetSysColor(COLOR_3DFACE));
        else
           c_BkColor.SetColor(GetSysColor(COLOR_WINDOW));
        c_RGBBkg.SetWindowText(c_BkColor.GetColor());

        c_TextColor.LoadColors();
        c_TextColor.SetColor(GetSysColor(COLOR_WINDOWTEXT));
        c_RGBText.SetWindowText(c_TextColor.GetColor());

        c_BkBrush.LoadColors();
        if(msgid == CTLCOLOR_DLG || msgid == CTLCOLOR_BTN 
           || msgid == CTLCOLOR_MSGBOX)
           c_BkBrush.SetColor(GetSysColor(COLOR_3DFACE));
        else
           c_BkBrush.SetColor(GetSysColor(COLOR_WINDOW));

        c_RGBBrush.SetWindowText(c_BkBrush.GetColor());

        CheckRadioButton(IDC_TRANSPARENT, IDC_OPAQUE, IDC_OPAQUE);

        CString s;
        s.LoadString(IDS_DEFAULTFONT);
        c_FaceName.SetWindowText(s);

        CFont * cfont = GetFont();
        if(cfont != NULL)
           { /* has dialog font */
            LOGFONT lf;
            cfont->GetObject(sizeof(lf), &lf);
            
            font.CreateFontIndirect(&lf);
           } /* has dialog font */

        enableFontOptions();
        enableColorOptions();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                 CCtlColorParms::SetDC(UINT type, CDC * dc)
* Inputs:
*       UINT type: CTLCOLOR_ type
*       CDC * dc: Display context to modify
* Result: LRESULT (HBRUSH)
*       Brush to use for painting the background of the control
* Effect: 
*       Modifies the display context:
*               SetBkColor of the selected background color
*               SetBkMode  of the selected background mode
*               SetTextColor of the selected text color
****************************************************************************/

LRESULT CCtlColorParms::SetDC(UINT type, CDC * dc)
    {
     if(type != msgid)
        return 0;  // doesn't match us

     // The type is correct

     // Select the text color

     COLORREF color = c_TextColor.GetColor();
     if(color != COLOR_NONE)
        { /* has text color */
         dc->SetTextColor(color);
        } /* has text color */

     // Set the text background color
     color = c_BkColor.GetColor();
     if(color != COLOR_NONE)
        { /* has text background */
         dc->SetBkColor(color);
        } /* has text background */

     // Select the background mode
     
     if(IsDlgButtonChecked(IDC_TRANSPARENT))
        dc->SetBkMode(TRANSPARENT);
     else
        dc->SetBkMode(OPAQUE);

#if 0
     // If the font is defined, select it into the DC

     if(font.m_hObject != NULL && !c_NoSelectObject.GetCheck())
        { /* select the font */
         // This should really be written as
         // dc->SelectObject(font);
         // But due to a bug in MFC 4.1 (and perhaps other versions)
         // that code will generate an ASSERT failure due to failure to
         // initialize the m_hAttribDC member when the DC is "faked up"
         // for the WM_CTLCOLOR message.  This calls the low-level API
         // function.  Ugly, but it works
         ::SelectObject(dc->m_hDC, font.m_hObject);
        } /* select the font */
#endif

     // Select the background brush and return it

     color = (COLORREF)c_BkBrush.GetColor();
     if(color != COLOR_NONE)
        { /* has background brush */
         if(brush.m_hObject != NULL)
            brush.DeleteObject();

         brush.CreateSolidBrush(color);
         return (LRESULT)brush.m_hObject;
        } /* has background brush */

     return 0; // NULL brush handle
     
    }

/****************************************************************************
*                           CCtlColorParms::SetFont
* Inputs:
*       UINT type: Type code, CTLCOLOR_
*       CWND * wnd: Window handle to set it in 
* Result: LRESULT (HFONT)
*       Previous font handle
* Effect: 
*       Calls the SetFont method on the CWnd
****************************************************************************/

LRESULT CCtlColorParms::SetFont(UINT type, CWnd * wnd)
    {
     if(type != msgid)
        return 0;  // doesn't match

     CFont * cf = wnd->GetFont();
     HFONT hfont;

     if(cf == NULL)
        hfont = NULL;
     else
        hfont = (HFONT)(cf->m_hObject);

     if(hfont != font.m_hObject && !c_NoSetFont.GetCheck())
        wnd->SetFont(&font, FALSE);
     return (LRESULT)hfont;
    }


/****************************************************************************
*                       CCtlColorParms::OnQuerySiblings
* Inputs:
*       WPARAM wParam: LOWORD: Control type ID
*                      HIWORD: operation to perform
*       LPARAM lParam: HIWORD(wParam) == SIBLING_SETDC: DC to modify
*                      HIWORD(wParam) == SIBLING_SETFONT: window handle
* Result: LRESULT 
*       SIBLING_SETDC: The brush to use to paint the background
*       SIBLING_SETFONT: The old font handle
* Effect: 
*       Modifies the DC if the control type ID matches
****************************************************************************/

LRESULT CCtlColorParms::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
    {
     switch(HIWORD(wParam))
        { /* op */
         case SIBLING_SETDC:
                 return SetDC(LOWORD(wParam), (CDC *)lParam);
         case SIBLING_SETFONT:
                 return SetFont(LOWORD(wParam), (CWnd *)lParam);
        } /* op */
     return 0;  // should never get here
    }

void CCtlColorParms::OnSelendokBkbrush() 
{
 c_RGBBrush.SetWindowText(c_BkBrush.GetColor());
        
}

void CCtlColorParms::OnSelendokBkcolor() 
{
 c_RGBBkg.SetWindowText(c_BkColor.GetColor());
        
}

void CCtlColorParms::OnSelendokTextcolor() 
{
 c_RGBText.SetWindowText(c_TextColor.GetColor());
        
}

/****************************************************************************
*                       CCtlColorParms::getWeightString
* Inputs:
*       int weight: Weight of font
* Result: CString
*       String which is the weight
* Effect: 
*       Loads the string and uses a temporary CString object to return the
*       value
****************************************************************************/

CString CCtlColorParms::getWeightString(int weight)
    {
     CString s;
     s = _T("");

     switch(weight)
        { /* decode */
         case FW_THIN:
                 s.LoadString(IDS_THIN);
                 break;
         case FW_EXTRALIGHT:
                 s.LoadString(IDS_EXTRALIGHT);
                 break;
         case FW_LIGHT:
                 s.LoadString(IDS_LIGHT);
                 break;
         case FW_NORMAL:
                 s.LoadString(IDS_NORMAL);
                 break;
         case FW_MEDIUM:
                 s.LoadString(IDS_MEDIUM);
                 break;
         case FW_SEMIBOLD:
                 s.LoadString(IDS_SEMIBOLD);
                 break;
         case FW_BOLD:
                 s.LoadString(IDS_BOLD);
                 break;
         case FW_EXTRABOLD:
                 s.LoadString(IDS_EXTRABOLD);
                 break;
         case FW_HEAVY:
                 s.LoadString(IDS_HEAVY);
                 break;
         default:
                 s.Format("(%d)", weight);
                 break;
        } /* decode */

     return s;
    }

void CCtlColorParms::OnChangefont() 
{
 LOGFONT lf;
 CFontDialog fd(&lf);

 if(font.m_hObject != NULL)
    { /* has font */
     font.GetObject(sizeof(lf), &lf);
     fd.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
    } /* has font */
 else
    { /* no font */
     memset(&lf, 0, sizeof(lf));
    } /* no font */

 fd.m_cf.rgbColors = c_TextColor.GetColor();

 if(fd.DoModal())
    { /* OK */
     c_TextColor.SetColor(fd.m_cf.rgbColors);
     // temporary until we get full set of colors defined
     c_RGBText.SetWindowText(fd.m_cf.rgbColors);
    } /* OK */

 font.DeleteObject();

 if(!font.CreateFontIndirect(&fd.m_lf))
    { /* failed */
     // NYI: MessageBox
     CString s;
     s.LoadString(IDS_DEFAULTFONT);
     c_FaceName.SetWindowText(s);
    } /* failed */
 else
    { /* success */
     CString s;
     s.Format("%s %d %s", fd.m_lf.lfFaceName,
                                abs(fd.m_lf.lfHeight),
                                getWeightString(fd.m_lf.lfWeight));
     c_FaceName.SetWindowText(s);
    } /* success */
 
 enableFontOptions();
}

void CCtlColorParms::OnNosetfont() 
{
        // TODO: Add your control notification handler code here
        
}

void CCtlColorParms::OnClearfont() 
{
 if(font.m_hObject != NULL)
    { /* clear font */
     font.DeleteObject();
     CString s;
     s.LoadString(IDS_DEFAULTFONT);
     c_FaceName.SetWindowText(s);
    } /* clear font */

 enableFontOptions();
}

void CCtlColorParms::OnDestroy() 
{
        CPropertyPage::OnDestroy();
        
        font.DeleteObject();
        
}

void CCtlColorParms::OnTransparent() 
{
 enableColorOptions();
        
}

void CCtlColorParms::OnOpaque() 
{
 enableColorOptions();
        
}
