// Sampler.cpp : implementation file
//

#include "stdafx.h"
#include "CtlColorExplorer.h"
#include "Sampler.h"
#include "sibling.h"
#include "cbtmsg.h"
#include "error.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampler property page

IMPLEMENT_DYNCREATE(CSampler, CPropertyPage)

CSampler::CSampler() : CPropertyPage(CSampler::IDD)
{
        //{{AFX_DATA_INIT(CSampler)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CSampler::~CSampler()
{
}

void CSampler::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CSampler)
        DDX_Control(pDX, IDC_MULTILINE, c_MultiLine);
        DDX_Control(pDX, IDC_DISABLED_CAPTION, c_DisabledCaption);
        DDX_Control(pDX, IDC_READONLY_CAPTION, c_ReadOnlyCaption);
        DDX_Control(pDX, IDC_NORMAL_CAPTION, c_NormalCaption);
        DDX_Control(pDX, IDC_RADIO3, c_Radio3);
        DDX_Control(pDX, IDC_RADIO2, c_Radio2);
        DDX_Control(pDX, IDC_RADIO1, c_Radio1);
        DDX_Control(pDX, IDC_SLIDER1, c_Slider1);
        DDX_Control(pDX, IDC_SCROLLBAR1, c_Scrollbar1);
        DDX_Control(pDX, IDC_PROGRESS1, c_Progress1);
        DDX_Control(pDX, IDC_LIST1, c_List1);
        DDX_Control(pDX, IDC_BUTTON, c_Button);
        DDX_Control(pDX, IDC_CHECK1, c_Check);
        DDX_Control(pDX, IDC_EDIT_READONLY, c_Edit_ReadOnly);
        DDX_Control(pDX, IDC_EDIT_DISABLED, c_Edit_Disabled);
        DDX_Control(pDX, IDC_GROUP, c_Group);
        DDX_Control(pDX, IDC_EDIT_NORMAL, c_Edit_Normal);
        DDX_Control(pDX, IDC_COMBO1, c_Combo1);
        DDX_Control(pDX, IDC_COMBO2, c_Combo2);
        DDX_Control(pDX, IDC_COMBO3, c_Combo3);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSampler, CPropertyPage)
        //{{AFX_MSG_MAP(CSampler)
        ON_BN_CLICKED(IDC_BUTTON, OnButton)
        ON_WM_CTLCOLOR()
        ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
        ON_WM_DESTROY()
        ON_WM_HSCROLL()
        ON_WM_VSCROLL()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampler message handlers

/****************************************************************************
*                                 HookProcCBT
                                NOT THREAD-SAFE
* Inputs:
*       int code:
*       WPARAM wParam:
*       LPARAM lParam:
* Result: LRESULT
*       As per CBT Hook specs
* Effect: 
*       Allows the message box to be colored using the selected colors
*       for the WM_CTLCOLORMSGBOX selection.
* Notes:
*       Note the use of static variables.  This hook function can be
*       associated with at most one thread because of this.  To be totally
*       thread-safe you must use Thread Local Storage (TlsAlloc, TlsFree,
*       TlsSetValue, TlsGetValue).  For simplicity, and because this is indeed
*       used only by one thread in this example, we forego the use of TLS.
*       However, when you do this you should VERY CAREFULLY leave a comment
*       like this and the one following the function name above so it is
*       well-understood that the function is NOT thread-safe.
****************************************************************************/

//===========================================
// STATIC variable!  Allocate using 'new' on
// per-thread basis for a thread-safe
// version!
//===========================================
static CCBTMsgBox msgbox;

LRESULT CALLBACK HookProcCBT(int code, WPARAM wParam, LPARAM lParam)
    {
     if(code < 0)
        { /* call next */
         return CallNextHookEx(msgbox.hhook, code, wParam, lParam);
        } /* call next */

     // This was not a request to chain the call.
     if(code == HCBT_CREATEWND)
        { /* creating MessageBox (maybe) */
         // The class of the messagebox is specified by the 
         // special value WC_DIALOG.  Other windows may be
         // created and they will also be trapped by this hook
         // function, but they are child controls of the 
         // MessageBox, such as its static text component or its
         // OK, Retry, Cancel, or other pushbuttons.

         // Get a pointer to the creation parameters
         LPCBT_CREATEWND cbtcreate = (LPCBT_CREATEWND) lParam;
         
         if(cbtcreate->lpcs->lpszClass == WC_DIALOG)
            { /* subclassing */
             // If this is a creation, the wParam is the handle of the
             // window being created
             msgbox.hwnd = (HWND)wParam;

             // Force subclassing to occur on the next call:
             msgbox.subclass = TRUE; // note that we need to subclass this one
            } /* subclassing */
        } /* creating MessageBox (maybe) */
     else
     if(msgbox.subclass) // note use of static variable between calls!
        { /* subclass the MessageBox window */
         msgbox.SubclassWindow(msgbox.hwnd);
         msgbox.subclass = FALSE; // record that subclassing has occurred already
        } /* subclass the MessageBox window */
     else
     if(code == HCBT_DESTROYWND && (HWND)wParam == msgbox.hwnd)
        { /* reset the handler */
         msgbox.UnsubclassWindow();
         msgbox.hwnd = NULL; // clear the window handle
        } /* reset the handler */

     return 0;
    }

void CSampler::OnButton() 
{
 CString caption;
 CString contents;

 contents.LoadString(IDS_MESSAGEBOX_CONTENTS);
 caption.LoadString(IDS_MESSAGEBOX_CAPTION);

 // Note that for a thread-safe version we would use 'new' here to
 // allocate a new instance and TlsAlloc to get some thread-local
 // storage to hold the pointer.

 msgbox.hhook = ::SetWindowsHookEx(WH_CBT, HookProcCBT, 
                                 AfxGetInstanceHandle(), 
                                 (DWORD)::GetCurrentThreadId());
 if(msgbox.hhook == NULL)
    { /* failed */
     reportError(GetLastError());
    } /* failed */

 // This is a bit of a hack; the QuerySibling function wants a DC to set
 // so we give it one...

 { /* query siblings */
  CClientDC tempdc(this);

  msgbox.hbrush = (HBRUSH)QuerySiblings(MAKELONG(CTLCOLOR_MSGBOX, SIBLING_SETDC), 
                                    (LPARAM)&tempdc);

  // We use the text color and background color set in the DC.  
  msgbox.bkcolor = tempdc.GetBkColor();
  msgbox.textcolor = tempdc.GetTextColor();
  msgbox.bkmode    = tempdc.GetBkMode();
  msgbox.hfont     = (HFONT)QuerySiblings(MAKELONG(CTLCOLOR_MSGBOX, SIBLING_GETFONT),
                                          NULL);

  // Note the tempdc will be released as we leave scope
 } /* query siblings */

 MessageBox(contents, caption);

 if(msgbox.hhook != NULL)
    UnhookWindowsHookEx(msgbox.hhook);
        
}

BOOL CSampler::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        CString s;

        s.LoadString(IDS_READONLY);
        c_Edit_ReadOnly.SetWindowText(s);

        s.LoadString(IDS_DISABLED);
        c_Edit_Disabled.SetWindowText(s);

        s.LoadString(IDS_ENABLED);
        c_Edit_Normal.SetWindowText(s);

        s.LoadString(IDS_MULTILINE);
        c_MultiLine.SetWindowText(s);

        for(int i = IDS_LISTBOX0+1; i < IDS_LISTBOX_END; i++)
           { /* load listbox */
            s.LoadString(i);
            c_List1.AddString(s);
            c_Combo1.AddString(s);
            c_Combo2.AddString(s);
            c_Combo3.AddString(s);
           } /* load listbox */
        
        c_List1.SetCurSel(0);
        c_Combo1.SetCurSel(0);
        c_Combo2.SetCurSel(0);  
        c_Combo3.SetCurSel(0);  

        CFont * cfont = GetFont();
        LOGFONT lf;
        if(cfont != NULL)
           { /* make bold italic font */
            cfont->GetObject(sizeof(lf), &lf);
            lf.lfWeight = FW_BOLD;
            lf.lfItalic = TRUE;
            if(font.CreateFontIndirect(&lf))
               SetFont(&font);
           } /* make bold italic font */

        c_Scrollbar1.SetScrollRange(0,99);
        c_Slider1.SetRange(0, 99);
        c_Slider1.SetTicFreq(10);
        c_Progress1.SetRange(0, 99);
        c_Slider1.SetPageSize(10);


        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSampler::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
        HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
        HBRUSH nbr;
        
        nbr = (HBRUSH)QuerySiblings(MAKELONG(nCtlColor, SIBLING_SETDC), 
                                    (LPARAM)pDC);
        QuerySiblings(MAKELONG(nCtlColor, SIBLING_SETFONT),
                      (LPARAM)pWnd);
        
        if(nbr != NULL)
           return nbr;

        return hbr;
}

/****************************************************************************
*                            CSampler::enableControls
* Inputs:
*       BOOL enable: TRUE to enable, FALSE to disable
* Result: void
*       
* Effect: 
*       Enables or disables all the list boxes
****************************************************************************/

void CSampler::enableControls(BOOL enable)
    {
     c_Combo1.EnableWindow(enable);
     c_Combo2.EnableWindow(enable);
     c_Combo3.EnableWindow(enable);
     c_Radio1.EnableWindow(enable);
     c_Radio2.EnableWindow(enable);
     c_Radio3.EnableWindow(enable);
     c_Button.EnableWindow(enable);
     c_Slider1.EnableWindow(enable);
     c_Scrollbar1.EnableWindow(enable);
     c_Progress1.EnableWindow(enable);
     c_Group.EnableWindow(enable);
     c_List1.EnableWindow(enable);
     c_NormalCaption.EnableWindow(enable);
     c_ReadOnlyCaption.EnableWindow(enable);
     c_DisabledCaption.EnableWindow(enable);
     c_MultiLine.EnableWindow(enable);
    }

void CSampler::OnCheck1() 
{
 enableControls(!c_Check.GetCheck());
}

void CSampler::OnDestroy() 
{
        CPropertyPage::OnDestroy();
        
        font.DeleteObject();
        
}

int CSampler::moveScrollBar(UINT nSBCode, UINT nPos, int bottom, int top, int pos, int delta) 
{
 switch(nSBCode)
    { /* sbcode */
     case SB_LINELEFT:
             if(pos > bottom)
                pos--;
             break;
     case SB_LINERIGHT:
             if(pos < top - 1)
                pos ++;
             break;
     case SB_PAGELEFT:
             if(pos > bottom)
                pos = max(bottom, pos - delta);
             break;
     case SB_PAGERIGHT:
             if(pos < top - 1)
                pos = min(top, pos + delta);
             break;
     case SB_THUMBTRACK:
             pos = nPos;
             break;
     case SB_THUMBPOSITION:
             pos = nPos;
    } /* sbcode */
        

 return pos;
}

void CSampler::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 int pos = pScrollBar->GetScrollPos();
 int bottom;
 int top;
 int delta;

 pScrollBar->GetScrollRange(&bottom, &top);

 delta = (top - bottom) / 10;

 pos = moveScrollBar(nSBCode, nPos, bottom, top, pos, delta);

 // We have only one horizontal scrolling control 

 c_Slider1.SetPos(pos);
 c_Progress1.SetPos(pos);
 c_Scrollbar1.SetScrollPos(pos);
}

void CSampler::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 int pos = ((CSliderCtrl *)pScrollBar)->GetPos();
 int bottom;
 int top;
 int delta;

 ((CSliderCtrl *)pScrollBar)->GetRange(bottom, top);

 // We only one vertical scrolling control

 delta = ((CSliderCtrl *)pScrollBar)->GetPageSize();

 pos = moveScrollBar(nSBCode, nPos, bottom, top, pos, delta);
        
 c_Slider1.SetPos(pos);
 c_Progress1.SetPos(pos);
 c_Scrollbar1.SetScrollPos(pos);
}
