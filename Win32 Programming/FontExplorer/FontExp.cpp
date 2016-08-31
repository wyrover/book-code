// FontExp.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "dlgs.h"
#include "fontdesc.h"
#include "legendi.h"
#include "legend.h"
#include "dispopts.h"
#include "poplist.h"
#include "fontmet.h"
#include "fontdisp.h"
#include "fontexp.h"
#include "logfont.h"
#include "options.h"
#include "tm.h"
#include "kp.h"
#include "otm.h"
#include "chformat.h"
#include "abc.h"
#include "toclip.h"
#include "subclass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontExplorer

CFontExplorer * CFontExplorer::self;

IMPLEMENT_DYNAMIC(CFontExplorer, CFontDialog)


CFontExplorer::CFontExplorer(LPLOGFONT lplfInitial, DWORD dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) : 
        CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd)
{
 self = this;
 hook = NULL;
 debug = FALSE;
        //{{AFX_DATA_INIT(CFontExplorer)
        //}}AFX_DATA_INIT
}

void CFontExplorer::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CFontExplorer)
        DDX_Control(pDX, IDC_TREEMETRICS, c_TreeMetrics);
        DDX_Control(pDX, IDC_c_LEGEND, c_c_Legend);
        DDX_Control(pDX, IDC_LEGEND, c_Legend);
        DDX_Control(pDX, stc5, c_SmallSample);
        DDX_Control(pDX, IDC_CHARDISPLAY, c_CharDisplay);
        DDX_Control(pDX, IDC_CHAR, c_Char);
        DDX_Control(pDX, IDC_INFO, c_Info);
        //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFontExplorer, CFontDialog)
        //{{AFX_MSG_MAP(CFontExplorer)
        ON_BN_CLICKED(IDC_INFO, OnInfo)
        ON_BN_CLICKED(IDC_CHAR, OnChar)
        ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
        ON_BN_CLICKED(IDC_COPY, OnCopy)
        ON_WM_DESTROY()
        //}}AFX_MSG_MAP
        ON_MESSAGE(UWM_UPDATE_FONTS, OnUpdateFonts)
END_MESSAGE_MAP()

// Ideally, we would like to have these handlers enabled.  But because
// Microsoft cannot read their own documentation, they carefully designed
// MFC so that it cannot be used to subclass common dialogs effectively.
// If we process these messages, then the common dialog handler will NOT do
// its proper handling, so we can't actually "subclass" the common dialog
// as we want to! 
//
// Take as given that we are *really* trying to implement the following
// message map entries in spite of Microsoft's inability to get their design
// right! (Caused, apparently, by a failure to read their own documentation
// and realize that the return value MUST be specifiable by the programmer!)

// Unfortunately, the common dialogs in Win95 and WinNT4.0 do not actually
// send the undocumented message that WinNT3.51 versions sent, indicating
// that the font selection had changed.  This design error, failing to provide
// the subclass with such a notification, is a serious design oversight on
// the part of Microsoft and necessitates all this ugly hackery.

// See the file subclass.cpp in which we subclass the window in a fairly
// primitive way.

        // ON_CBN_SELCHANGE(cmb1, OnSelchangecmb1)
        // ON_CBN_SELCHANGE(cmb2, OnSelchangecmb2)
        // ON_CBN_SELCHANGE(cmb3, OnSelchangecmb3)
        // ON_CBN_SELCHANGE(cmb4, OnSelchangecmb4)
        // ON_CBN_SELCHANGE(cmb5, OnSelchangecmb5)
        // ON_BN_CLICKED(chx1, Onchx1)
        // ON_BN_CLICKED(chx2, Onchx2)

/****************************************************************************
*                        CFontExplorer::loadWidths
* Result: void
*       
* Effect: 
*       Adds the ABCWidths info to the dropdown list
****************************************************************************/

void CFontExplorer::loadWidths(CFontDescriptor * desc)
    {
     CString s;
     CString smp;
     int i;

     c_CharDisplay.GetWindowText(smp);
     s.LoadString(IDS_WIDTHS);

     HTREEITEM wd = c_TreeMetrics.InsertItem(s);

         s.Format(_T("GetTextExtentPoint32"));
         HTREEITEM te = c_TreeMetrics.InsertItem(s, wd);

             SIZE sz = desc->dc->GetTextExtent(smp);
             s.Format(_T("cx %d"), sz.cx);
             c_TreeMetrics.InsertItem(s, te);
             s.Format(_T("cy %d"), sz.cy);
             c_TreeMetrics.InsertItem(s, te);

             for(i = 0; i < smp.GetLength(); i++)
                { /* add each */
                 charFormat(s, smp[i]);
                 HTREEITEM charwd = c_TreeMetrics.InsertItem(s, te);

                 // Note the following clumsy construction because &smp[i]
                 // gets a compilation error:
                 TCHAR c[2];
                 c[0] = smp[i];
                 c[1] = _T('\0');
                 sz = desc->dc->GetTextExtent(c, 1);

                 s.Format(_T("cx %d"), sz.cx);
                 c_TreeMetrics.InsertItem(s, charwd);
                 s.Format(_T("cy %d"), sz.cy);
                 c_TreeMetrics.InsertItem(s, charwd);
                } /* add each */

         loadABCWidths(desc->dc, smp, &c_TreeMetrics, wd);
    }

/****************************************************************************
*                        CFontExplorer::updateFontInfo
* Result: void
*       
* Effect: 
*       Updates all the font info
****************************************************************************/

void CFontExplorer::updateFontInfo()
    {
     CFontDescriptor desc(this, this);
     c_TreeMetrics.DeleteAllItems();

     // We can't subclass the control because it must be handled
     // as an owner-draw control by the common dialog.
     int sel = GetDlgItem(cmb4)->SendMessage(CB_GETCURSEL);
     COLORREF c = GetDlgItem(cmb4)->SendMessage(CB_GETITEMDATA, sel);
     c_CharDisplay.color = c;
     
     loadTextMetrics(&desc.tm, &c_TreeMetrics);
     if(desc.otm != NULL)
        loadOutlineTextMetrics(desc.otm, &c_TreeMetrics);
     loadLogFont(&desc.lf, &c_TreeMetrics, FALSE);
     loadWidths(&desc);
     loadKerningPairs(*desc.dc, &c_TreeMetrics);
     c_CharDisplay.InvalidateRect(NULL, TRUE);

    }

/****************************************************************************
*                        CFontExplorer::OnFontChanged
* Inputs:
*       WPARAM wParam: ignored but sent to parent
*       LPARAM lParam: ignored but sent to parent
* Result: LRESULT
*       FALSE, always, indicating that the parent should process this
* Effect: 
*       Allows us to intercept all font changes and change the display
****************************************************************************/

//LRESULT CFontExplorer::OnFontChanged(WPARAM wParam, LPARAM lParam)
//    {
//     updateFontInfo();
//     return FALSE;
//    }

/****************************************************************************
*                        CFontExplorer.enableControls
* Result: void
*       
* Effect: 
*       ²
****************************************************************************/

void CFontExplorer::enableControls()
    {
     if(c_Info.GetCheck())
        { /* info display */
         c_TreeMetrics.ShowWindow(SW_SHOW);
         c_CharDisplay.ShowWindow(SW_HIDE);
         c_Legend.ShowWindow(SW_HIDE);
         c_c_Legend.ShowWindow(SW_HIDE);
        } /* info display */
     else
        { /* char display */
         c_TreeMetrics.ShowWindow(SW_HIDE);
         c_CharDisplay.ShowWindow(SW_SHOW);
         c_Legend.ShowWindow((c_CharDisplay.opts.colors ? SW_SHOW : SW_HIDE));
         c_c_Legend.ShowWindow((c_CharDisplay.opts.colors ? SW_SHOW : SW_HIDE));
        } /* char display */
    }


/****************************************************************************
*                               simulateMsgMap
* Inputs:
*       int code: WC_ACTION, or value < 0
*       WPARAM wParam: Current-process flag (ignored)
*       LPARAM lParam: Pointer to CWPSTRUCT structure
* Result: LRESULT
*       Whatever is returned by CallNextHookEx
* Effect: 
*       In the case of several notification messages, we simulate the
*       message map entries by sending our own message.  Since in all
*       cases all we want to do is force the data to update by posting
*       a UWM_UPDATE_FONTS message, this is actually quite easy.
* Notes:
*       It does seem odd to specify the values as WPARAM and LPARAM types
*       since it is obvious they are BOOL and CWPSTRUCT *, but Microsoft
*       requires this spec (they were never very good at understanding
*       data types)
*
*       This is also grotesque in that it forces us to use two static
*       global variables to hold the hook and the current window handle
*       because MFC can't actually deliver the correct "CFontExplorer *"
*       refernce even using CWnd::FromHandle!
*
*       We can ignore the wParam because we set the hook to be a per-thread
*       hook and therefore we will never see messages other than the 
*       current thread.
****************************************************************************/

LRESULT CALLBACK simulateMsgMap(int code, WPARAM wParam, LPARAM lParam)
    {
#ifdef _DEBUG
     static int counter;
     counter++;  // see why we are so slow...
#endif

     CWPSTRUCT * cwp = (CWPSTRUCT *)lParam;

     if(code >= 0 && 
        cwp->hwnd == CFontExplorer::self->m_hWnd &&
        cwp->message == WM_COMMAND)
        { /* valid */
         // It is destined for our window.  We only care about WM_COMMAND
         // messages

         // It is a WM_COMMAND message.  If it is one of the controls that
         // would affect the information we are displaying we post a message
         // requesting an update

         // Note that this code critically depends on some information we
         // would be better off not having to code here, such as the fact
         // that the LOWORD of the wParam is the control ID
         // Such are the ugly hacks required by the MFC design problems

         switch(LOWORD(cwp->wParam))
            { /* which control */
             case cmb1:
             case cmb2:
             case cmb3:
             case cmb4:
             case cmb5:
             case chx1:
             case chx2:
                     CFontExplorer::self->PostMessage(UWM_UPDATE_FONTS);
                     break;
            } /* which control */
        } /* valid */

     // No matter what we saw, pass it on
     return CallNextHookEx(CFontExplorer::self->hook, code, wParam, lParam);
    }

/****************************************************************************
*                         CFontExplorer::OnInitDialog
* Result: BOOL
*       TRUE, always (leave focus alone)
* Effect: 
*       Fairly straightforword with one major exception: We install a
*       "hook" procedure to simulate the effect of the message map.  This
*       is required because MFC deliberately and with malice aforethought
*       goes out of its way to improperly implement the handling of 
*       WM_COMMAND messages, always returning a value that says "message
*       completely handled", instead of giving the programmer the option.
*       This makes it impossible to subclass a common dialog properly.
****************************************************************************/

BOOL CFontExplorer::OnInitDialog() 
{
        CFontDialog::OnInitDialog();
        
        c_Legend.AddLegends(CFontDisplay::GetLegend());
        CheckRadioButton(IDC_INFO, IDC_CHAR, IDC_INFO);
        enableControls();

        subclassCommand(this, cmb1, CBN_SELCHANGE);
        subclassCommand(this, cmb2, CBN_SELCHANGE);
        subclassCommand(this, cmb3, CBN_SELCHANGE);
        subclassCommand(this, cmb4, CBN_SELCHANGE);
        subclassCommand(this, cmb5, CBN_SELCHANGE);
        subclassCommand(this, chx1, BN_CLICKED);
        subclassCommand(this, chx2, BN_CLICKED);

        //hook = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)simulateMsgMap,
        //                      AfxGetInstanceHandle(), GetCurrentThreadId());
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CFontExplorer::OnInfo() 
{
        enableControls();
        
}

void CFontExplorer::OnChar() 
{
        enableControls();
        
}

void CFontExplorer::OnOptions() 
{
 COptions dlg;

 c_CharDisplay.GetWindowText(dlg.m_Sample); 
 c_SmallSample.GetWindowText(dlg.m_SmallSample);

 dlg.opts = c_CharDisplay.opts;

 dlg.DoModal();

 CString smp;
 CString Smallsmp;

 c_CharDisplay.GetWindowText(smp);
 c_CharDisplay.SetWindowText(dlg.m_Sample);

 c_SmallSample.GetWindowText(Smallsmp);
 c_SmallSample.SetWindowText(dlg.m_SmallSample);

 c_SmallSample.InvalidateRect(NULL, TRUE);
 c_SmallSample.UpdateWindow();

 c_CharDisplay.opts = dlg.opts;

 c_CharDisplay.InvalidateRect(NULL, TRUE);
 c_CharDisplay.UpdateWindow();

 if(smp != dlg.m_Sample)
    { /* update large display */
     updateFontInfo();
    } /* update large display */

 enableControls();
}

// If we actually were able to use the message map, whose usage is defeated
// by a serious design flaw in MFC, we would use these handlers.

//void CFontExplorer::OnSelchangecmb1() 
//{
// PostMessage(UWM_UPDATE_FONTS);
//}

//void CFontExplorer::OnSelchangecmb2() 
//{
// PostMessage(UWM_UPDATE_FONTS);
//}

//void CFontExplorer::OnSelchangecmb3() 
//{
// PostMessage(UWM_UPDATE_FONTS);
//}

//void CFontExplorer::OnSelchangecmb4() 
//{
// PostMessage(UWM_UPDATE_FONTS);
//}

//void CFontExplorer::OnSelchangecmb5() 
//{
// PostMessage(UWM_UPDATE_FONTS);
//}

//void CFontExplorer::Onchx1() 
//{
// PostMessage(UWM_UPDATE_FONTS);
//}

//void CFontExplorer::Onchx2() 
//{
// PostMessage(UWM_UPDATE_FONTS);
//}

/****************************************************************************
*                        CFontExplorer::OnUpdateFonts
* Inputs:
*       ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Updates the font information.  This must be deferred.
****************************************************************************/

LRESULT CFontExplorer::OnUpdateFonts(WPARAM, LPARAM)
    {
     updateFontInfo();     
     return 0;
    }

void CFontExplorer::OnCopy() 
{
 toClipboard(&c_CharDisplay, TRUE);
}

void CFontExplorer::OnDestroy() 
{
        CFontDialog::OnDestroy();
        
        unsubclassCommand(this, cmb1);
        unsubclassCommand(this, cmb2);
        unsubclassCommand(this, cmb3);
        unsubclassCommand(this, cmb4);
        unsubclassCommand(this, cmb4);
        unsubclassCommand(this, chx1);
        unsubclassCommand(this, chx2);

        // if(hook != NULL)
        //   UnhookWindowsHookEx(hook);
        
}
