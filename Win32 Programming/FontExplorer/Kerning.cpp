// Kerning.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "fontdesc.h"
#include "legendi.h"
#include "legend.h"
#include "poplist.h"
#include "dispopts.h"
#include "fontdisp.h"
#include "Kerning.h"
#include "color.h"
#include "kernopt.h"
#include "toclip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKerning dialog


CKerning::CKerning(CWnd* pParent /*=NULL*/)
        : CDialog(CKerning::IDD, pParent)
{
 c_Kerned.color = ::GetSysColor(COLOR_WINDOWTEXT);
 c_Unkerned.color = ::GetSysColor(COLOR_WINDOWTEXT);

 kernedonly = FALSE;
        //{{AFX_DATA_INIT(CKerning)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CKerning::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CKerning)
        DDX_Control(pDX, IDC_TRACKVAL, c_TrackVal);
        DDX_Control(pDX, IDC_c_TRACK, c_c_Track);
        DDX_Control(pDX, IDC_TRACK, c_Track);
        DDX_Control(pDX, IDC_SAMPLE, c_Sample);
        DDX_Control(pDX, IDC_KERNED, c_Kerned);
        DDX_Control(pDX, IDC_UNKERNED, c_Unkerned);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKerning, CDialog)
        //{{AFX_MSG_MAP(CKerning)
        ON_EN_CHANGE(IDC_SAMPLE, OnChangeSample)
        ON_WM_SIZE()
        ON_WM_GETMINMAXINFO()
        ON_BN_CLICKED(IDC_FONT, OnFont)
        ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
        ON_WM_VSCROLL()
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        //}}AFX_MSG_MAP
        ON_MESSAGE(WM_CHOOSEFONT_GETLOGFONT, OnGetLogFont)
        ON_MESSAGE(UWM_GETCOLOR, OnGetColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKerning message handlers

void CKerning::OnChangeSample() 
{
 CString s;
 c_Sample.GetWindowText(s);

 c_Unkerned.SetWindowText(s);
 c_Kerned.SetWindowText(s);

 c_Unkerned.InvalidateRect(NULL);
 c_Kerned.InvalidateRect(NULL);
        
}

/****************************************************************************
*                              CKerning::Resize
* Result: void
*       
* Effect: 
*       Recomputes the sizes of the windows based on the dialog size and
*       the display options.
****************************************************************************/

void CKerning::Resize()
    {
     CRect unkerned;
     CRect kerned;

     // Handle the startup transient, where we get the initial WM_SIZE
     // long before the DDX has initialized the c_Kerned/c_Unkerned
     // variables...

     if(c_Kerned.m_hWnd == NULL)
        return;
     if(c_Unkerned.m_hWnd == NULL)
        return;

     c_Kerned.GetWindowRect(&kerned);
     c_Unkerned.GetWindowRect(&unkerned);

     ScreenToClient(&unkerned);
     ScreenToClient(&kerned);

     // The top of the unkerned rectangle remains constant; the two
     // rectangles split the remainging client space

     CRect client;
     GetClientRect(&client);

     CRect newKerned;
     CRect newUnkerned;

     /*
        client.top                                              
client.left+----------------------------------------------------+
        |                                                    |
        |unkerned.top                                        |
        +----------------------------------------------------+
        |                                                    |
        |                                                    |
        |                                                    |
        |                                                    |
        +----------------------------------------------------+
        |kerned.top                           unkerned.bottom|
        |                                                    |
        |                                                    |
        |                                                    |
        +----------------------------------------------------+ client.right
                                                  client.bottm
     */
     newUnkerned.top = unkerned.top;
     newUnkerned.left = client.left;
     newUnkerned.right = client.right;
     newUnkerned.bottom = unkerned.top + (client.bottom - unkerned.top) / 2;

     if(kernedonly)
        { /* kerned only */
         newKerned.top = unkerned.top;
         newKerned.left = client.left;
         newKerned.right = client.right;
         newKerned.bottom = client.bottom;
        } /* kerned only */
     else
        { /* both displays */
         newKerned.top = newUnkerned.bottom + 1;
         newKerned.left = client.left;
         newKerned.right = client.right;
         newKerned.bottom = client.bottom;
        } /* both displays */

     c_Unkerned.MoveWindow(&newUnkerned);
     c_Kerned.MoveWindow(&newKerned);
     
    }

void CKerning::OnSize(UINT nType, int cx, int cy) 
{
        CDialog::OnSize(nType, cx, cy);

        Resize();
}

void CKerning::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
        // TODO: Add your message handler code here and/or call default
        
        CDialog::OnGetMinMaxInfo(lpMMI);
}

void CKerning::OnFont() 
{
 CFont * oldFont;
 LOGFONT lf;

 memset(&lf, 0, sizeof(LOGFONT));
 oldFont = c_Kerned.GetFont();
 oldFont->GetObject(sizeof(LOGFONT), &lf);

 CFontDialog fd(&lf);
 if(fd.DoModal())
    { /* successful choice */
     CFont newFont;
     if(newFont.CreateFontIndirect(&lf))
        { /* successful creation */
         c_Kerned.SetFont(&newFont);
         c_Unkerned.SetFont(&newFont);
         c_Kerned.InvalidateRect(NULL);
         c_Unkerned.InvalidateRect(NULL);
         newFont.Detach();
         oldFont->DeleteObject();
         c_Kerned.color = fd.GetColor();
         c_Unkerned.color = fd.GetColor();
        } /* successful creation */
    } /* successful choice */
     
        
}

/****************************************************************************
*                           CKerning::OnGetLogFont
* Inputs:
*       WPARAM: ignored
*       LPARAM: (LPARAM)LOGFONT
* Result: LRESULT
*       0, always (logically void)
* Effect: 
*       Copies the contents of the logfont to the logfont of the lParam
****************************************************************************/

LRESULT CKerning::OnGetLogFont(WPARAM, LPARAM lParam)
    {
     LPLOGFONT lf = (LPLOGFONT)lParam;
     CFont * font = c_Unkerned.GetFont();
     if(font == NULL)
        { /* no font */
         memset(lf, 0, sizeof(LOGFONT));
        } /* no font */
     else
        { /* has font */
         font->GetObject(sizeof(LOGFONT), lf);
        } /* has font */
     return 0;
    }

/****************************************************************************
*                            CKerning::OnGetColor
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       (LRESULT)(COLORREF)
****************************************************************************/

LRESULT CKerning::OnGetColor(WPARAM, LPARAM)
    {
     return (LRESULT)c_Kerned.color;
    }


/****************************************************************************
*                          CKerning::enableTracking
* Inputs:
*       BOOL mode: enable/disable
* Result: void
*       
* Effect: 
*       Based on the mode, enable or disable the track kerning options
****************************************************************************/

void CKerning::enableTracking(BOOL mode)
    {
     c_Track.EnableWindow(mode);
     c_c_Track.EnableWindow(mode);
     c_TrackVal.EnableWindow(mode);
     
    }

/****************************************************************************
*                          CKerning::enableControls
* Result: void
*       
* Effect: 
*       Enables or disables appropriate controls
****************************************************************************/

void CKerning::enableControls()
    {
     enableTracking(c_Kerned.opts.track);
     c_Unkerned.ShowWindow(kernedonly ? SW_HIDE : SW_SHOW);
    }

BOOL CKerning::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        c_Kerned.opts.kerning = TRUE;
        c_Kerned.opts.track = TRUE;  // assume track kerning active
        c_Kerned.opts.trackval = 0;

        CString s;
        enableControls();

        c_Track.SetRange(0, 20);
        c_Track.SetPos(0);

        
        c_Kerned.GetWindowText(s);
        c_Sample.SetWindowText(s);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CKerning::OnOptions() 
{
 CKernOptions dlg;
 BOOL resize_needed;
 dlg.opts = c_Kerned.opts;
 dlg.m_KernedOnly = kernedonly;

 dlg.DoModal();

 c_Kerned.opts = dlg.opts;
 c_Unkerned.opts = dlg.opts;  // copies everything, reset fields for unkerned

 // Force the unkerned fields to be properly set:
 c_Unkerned.opts.kerning = FALSE;
 c_Unkerned.opts.track = FALSE;
 c_Unkerned.opts.basal = FALSE;
 resize_needed = (kernedonly != dlg.m_KernedOnly);
 kernedonly = dlg.m_KernedOnly;
        
 c_Kerned.InvalidateRect(NULL);
 c_Unkerned.InvalidateRect(NULL);

 enableControls();
 
 // If we changed the presence of the unkerned window, we need to force
 // a resizing because that is where we compute the sizes of the boxes.
 if(resize_needed)
    { /* force resize */
     Resize();
    } /* force resize */
}

void CKerning::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 if(pScrollBar->m_hWnd == c_Track.m_hWnd)
    { /* track kerning */
     c_Kerned.opts.trackval = ((CSpinButtonCtrl *)pScrollBar)->GetPos();
     c_Kerned.InvalidateRect(NULL);
     return;
    } /* track kerning */
        
        CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CKerning::OnCopy() 
{
 toClipboard(this, TRUE);
        
}
