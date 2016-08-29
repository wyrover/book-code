/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
* 12-Feb-96 | Created log
* 12-Feb-96 | GetBaseLogFont returns TRUE and zero-initialized LOGFONT object
*           | instead of FALSE 
*****************************************************************************/
// fontsel.cpp : implementation file
//
#define STRICT
#include "stdafx.h"
#include <afxdlgs.h>
#include "errors.h"
#include <windowsx.h>  // required only for ChooseFont hook procedure
#include <dlgs.h>      // required for ChooseFont hook procedure

#include "resource.h"
#include "fontobj.h"
#include "fontsel.h"
#include "fontopt.h"
#include "fontstr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/****************************************************************************
*                                GetNewLogFont
* Inputs:
*       CFontObj * fd: Font descriptor
*       LPLOGFONT lf: Place to put logical font info
*       CFont ** f: Place to put CFont *, or NULL if CFont not needed
* Result: BOOL
*       TRUE if there was a new font and it has been put into the lf data
*       FALSE if there was no new font
* Effect: 
*       If there is a "new" font, does a GetObject call on the "New" font.
*       If 'f' is  non-NULL, returns a CFont * via the pointer f
****************************************************************************/

static BOOL GetNewLogFont(CFontObj * fd, LPLOGFONT lf, CFont ** f)
    {
     CFont * newfont = fd->GetNewFont();
     if(newfont != NULL)
        { /* valid font */
         if(f != NULL)
            *f = newfont;
         newfont->GetObject(sizeof(*lf), (LPVOID)lf);
         return TRUE;
        } /* valid font */
     else
        return FALSE;
    }

/****************************************************************************
*                               GetBaseLogFont
* Inputs:
*       CFontObj * fd: Font descriptor
*       LPLOGFONT lf: LOGFONT structure to be filled in
*       CFont ** f: Place to put CFont *, or NULL if CFont not needed
* Result: BOOL
*       TRUE if there was a valid base font and its data has been put into the
*            LOGFONT structure
*       If there is no valid base font, sets LOGFONT to zero values.
* Effect: 
*       If there is a "base" font, does a GetObject call on the "base" font.
*       If 'f' is  non-NULL, returns a CFont * via the pointer f.
****************************************************************************/

static BOOL GetBaseLogFont(CFontObj * fd, LPLOGFONT lf, CFont ** f)
    {
     CFont * font = fd->GetBaseFont();
     if(font == NULL)
        { /* default system font */
         memset(lf, 0, sizeof(LOGFONT));
        } /* default system font */
     else
        { /* specific font */
         font->GetObject(sizeof(*lf), (LPVOID)lf);
        } /* specific font */

     if(f != NULL)
        *f = font;
     return TRUE;
    }

/****************************************************************************
*                                 SetNewLogFont
* Inputs:
*       CFontObj * fd: Font descriptor
*       LPLOGFONT lf: Descriptor of new font
* Result: BOOL
*       TRUE if we could create the font and set it as the new font
*       FALSE if there was a failure
* Effect: 
*       Creates a new font and sets it as the new font.  If there was
*       already a new font, deletes it before storing the new one.
****************************************************************************/

static BOOL SetNewLogFont(CFontObj * fd, LPLOGFONT lf)
    {
     CFont * f = new CFont();
     if(!f->CreateFontIndirect(lf))
        { /* failed */
         delete f;
         return FALSE;
        } /* failed */

     // Font created successfully

     fd->SetNewFont(f);

     return TRUE;
    }

/****************************************************************************
*                          GetSelectedFontDescriptor
* Result: CFontObj *
*       Font descriptor for currently selected font, or NULL if none selected
****************************************************************************/

CFontObj * CFontSelect::GetSelectedFontDescriptor() 
    {
     int n = c_FontList.GetCurSel();
     if(n == CB_ERR)
        return NULL;
     CFontObj * fd = (CFontObj *)c_FontList.GetItemData(n);
     return fd;
    }
   
/////////////////////////////////////////////////////////////////////////////
// CFontSelect dialog

/*
   Thoughts: Make the font list be an owner-draw dropdown list
             If a font handle is NULL, gray out the option name
*/

/****************************************************************************
*                          CFontSelect::CFontSelect
* Inputs:
*       CWnd * pParent: default NULL, the parent of this dialog
* Effect: 
*       Initializes the member variables
****************************************************************************/

CFontSelect::CFontSelect(CWnd* pParent /*=NULL*/)
        : CDialog(CFontSelect::IDD, pParent)
{
        //{{AFX_DATA_INIT(CFontSelect)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
   AnyFontChanged = FALSE;
}

/****************************************************************************
*                         CFontSelect::DoDataExchange
* Inputs:
*       CDataExchange * pDX:
* Result: void
*       
* Effect: 
*       ²
****************************************************************************/

void CFontSelect::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CFontSelect)
        DDX_Control(pDX, IDC_FONTSAMPLE, c_FontSample);
        DDX_Control(pDX, IDC_FONTNAME, c_FontName);
        DDX_Control(pDX, IDC_FONTLIST, c_FontList);
        //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFontSelect, CDialog)
        //{{AFX_MSG_MAP(CFontSelect)
        ON_BN_CLICKED(IDC_CHANGEFONT, OnChangefont)
        ON_CBN_DBLCLK(IDC_FONTLIST, OnDblclkFontlist)
        ON_CBN_SELCHANGE(IDC_FONTLIST, OnSelchangeFontlist)
        ON_WM_DESTROY()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFontSelect message handlers

/****************************************************************************
*                             FontHook_OnCommand
* Inputs:
*       HWND hWnd: Window reference, ignored
*       int id: ID of control.  Deal with this only for psh3, the Apply button
*       HWND hWndCtl: ignored
*       UINT codeNotify: ignored
* Result: void
*       
* Effect: 
*       Because we are in C here, and don't have a good way to send the
*       message to the views, we send a UWM_FONTAPPLY message to the
*       main frame, which routes it to where it should go.
****************************************************************************/

static void FontHook_OnCommand(HWND hWnd, int id, HWND ctl, UINT codeNotify)
    {
     switch(id)
        { /* which id */
         case psh3: // "Apply"
                 // We don't currently support 'Apply' button
                 // app_FontApply();
                 break;
        } /* which id */
    }

/****************************************************************************
*                            FontHook_OnInitDialog
* Inputs:
*       HWND hWnd: Window reference
*       HWND hWndFocus: ignored
*       LPARAM lParam: (LPARAM)CHOOSEFONT *
* Result: BOOL
*       TRUE, always
* Effect: 
*       Sets the text as the sample text and the window title
****************************************************************************/

BOOL FontHook_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam)
    {
     CHOOSEFONT * cf = (CHOOSEFONT *)lParam;
     CFontObj * fd = (CFontObj *)(cf->lCustData);
     char title[256];

     GetWindowText(hWnd, title, sizeof(title));
     lstrcat(title, " - ");
     lstrcat(title, fd->GetDescription()->GetBuffer(0));
     SetWindowText(hWnd, title);
     
     CString * s = fd->GetSample();
     if(s != NULL && s->GetLength() > 0)
        SetDlgItemText(hWnd, stc5, *s);
     return TRUE;
    }

/****************************************************************************
*                               FontHandlerProc
* Inputs:
*       HWND hWnd: Window handle
*       UINT msg: Message
*       WPARAM wParam: 
*       LPARAM lParam:
* Result: BOOL
*       TRUE if we handled the message and FALSE if we did not
* Effect: 
*       Handles the Apply button ("psh3") 
* Notes:
*       There should be a better way to do this; MFC documentation is REALLY
*       bad in this regard
****************************************************************************/

LRESULT CALLBACK FontHandlerProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
     switch(msg)
        { /* decode message */
         HANDLE_MSG(hWnd, WM_COMMAND, FontHook_OnCommand);
         HANDLE_MSG(hWnd, WM_INITDIALOG, FontHook_OnInitDialog);
        } /* decode message */
     return FALSE;
    }

/****************************************************************************
*                          CFontSelect::OnChangeFont
* Result: void
*       
* Effect: 
*       When the Change Font... button is pressed, brings up a CFontDialog
*       on the current font
****************************************************************************/

void CFontSelect::OnChangefont()
    {
     CFontDialog dlg;
     LOGFONT lf;

     CFontObj * fd = GetSelectedFontDescriptor();

     if(!GetNewLogFont(fd, &lf, NULL))
        if(!GetBaseLogFont(fd, &lf, NULL))
           return;

     memset(&dlg.m_cf, 0, sizeof(dlg.m_cf));
     dlg.m_cf.lStructSize = sizeof(dlg.m_cf);
     dlg.m_cf.hwndOwner = m_hWnd;
     dlg.m_cf.lpLogFont = &lf;
              // Add CF_APPLY to the list below if we implement Apply button
     dlg.m_cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_LIMITSIZE
                                     | CF_ENABLEHOOK ;
     dlg.m_cf.lpfnHook = (UINT (CALLBACK *) (HWND, UINT, WPARAM, LPARAM))FontHandlerProc;
     dlg.m_cf.nSizeMin = 1;
     dlg.m_cf.nSizeMax = 40;
     dlg.m_cf.lCustData = (LPARAM)fd;

     fd->SetFontDialog(&dlg);

     BOOL changed = (dlg.DoModal() == IDOK);
     // Remove the dialog from the description now that it is finished
     fd->SetFontDialog(NULL);

     if(changed)
        { /* new font */
         SetNewLogFont(fd, &lf);  // make the result font the new font
         AnyFontChanged = TRUE;
         // Notify parent of font change, if applicable
         // app_NotifyFontChange();
         OnSelchangeFontlist();  // force sample and description to change
        } /* new font */
     else
        { /* no new font */
         // The new font can be set by the "Apply" button
         fd->SetNewFont(NULL);
         // Recompute fonts because "Apply" button may have drawn them
         // Notify parent of font change, if applicable
         // app_NotifyFontChange();
        } /* no new font */

    }

/****************************************************************************
*                        CFontSelect::OnDblclkFontlist
* Result: void
*       
* Effect: 
*       When a font is double-clicked, cause it to be selected and change
****************************************************************************/

void CFontSelect::OnDblclkFontlist()
    {
     OnChangefont();
    }

/****************************************************************************
*                      CFontSelect::OnSelchangeFontlist
* Result: void
*       
* Effect: 
*       When the selection changes, change the sample to match the font
****************************************************************************/

void CFontSelect::OnSelchangeFontlist()
    {
     CFontObj * fd;
     fd = GetSelectedFontDescriptor();
     if(fd != NULL)
        { /* has selection */
         LOGFONT lf;
         CFont * f;
         if(!GetNewLogFont(fd, &lf, &f))
            if(!GetBaseLogFont(fd, &lf, &f))
               return;

         char fontname[256];

         CDC * dc = GetDC();
         c_FontSample.SetFont(f);
         FontToString(fontname, &lf, dc, FALSE);
         c_FontName.SetWindowText(fontname);
         if(!c_FontSample.IsWindowVisible())
            c_FontSample.ShowWindow(SW_SHOW);
         ReleaseDC(dc);
        } /* has selection */
     else
        { /* no selection */
         c_FontSample.ShowWindow(SW_HIDE);
         c_FontName.SetWindowText("");
        } /* no selection */
        
    }

/****************************************************************************
*                       CFontSelect::EstablishNewFonts
* Result: void
*       
* Effect: 
*       For each descriptor which has a new font, make the base font be
*       the new font.
****************************************************************************/

void CFontSelect::EstablishNewFonts()
    {
     int i;
     int count = c_FontList.GetCount();
     for(i = 0; i < count; i++)
        { /* handle each */
         CFontObj * fd = (CFontObj *)c_FontList.GetItemData(i);
         fd->ReplaceBaseFontWithNewFont();
        } /* handle each */
    }

/****************************************************************************
*                         CFontSelect::DeleteNewFonts
* Result: void
*       
* Effect: 
*       Deletes the CFont objects in the newfont field of the font descriptors.
*       If a CFont object has an associated GDI object, that GDI object is
*       deleted.  Note that we may have already detached the GDI object and
*       assigned it to the base font.
****************************************************************************/

void CFontSelect::DeleteNewFonts()
    {
     int i;
     int count = c_FontList.GetCount();
     for(i = 0; i < count; i++)
        { /* handle each */
         CFontObj * fd = (CFontObj *)c_FontList.GetItemData(i);
         fd->SetNewFont(NULL);
        } /* handle each */
    }

/****************************************************************************
*                            CFontSelect::OnCancel
* Result: void
*       
* Effect: 
*       Kills off the dialog.  
****************************************************************************/

void CFontSelect::OnCancel()
    {
     DeleteNewFonts();
        
     CDialog::OnCancel();
    }

/****************************************************************************
*                              CFontSelect::OnOK
* Result: void
*       
* Effect: 
*       Makes all changes be permanent.  It does this by calling
*       EstablishNewFonts, which replaces the base font description by the new
*       font description for all entries which have a valid newfont field.
*       This procedure then deletes all the (now unattached-to-anything)
*       CFont objects by calling DeleteNewFonts.
****************************************************************************/

void CFontSelect::OnOK()
    {
     EstablishNewFonts();
     DeleteNewFonts();

     // Now that we have set up the fonts, notify all the windows that there
     // has been a font change
     if(AnyFontChanged)
        { /* there has been a change */
         // Parent notifications here, if needed
         // app_NotifySettingsChange(TRUE);
         // app_NotifyFontChange();
        } /* there has been a change */

     CDialog::OnOK();
    }

/****************************************************************************
*                           CFontSelect::LoadFonts
* Result: void
*       
* Effect: 
*       Adds each of the fonts in the list to the combobox
****************************************************************************/

void CFontSelect::LoadFonts()
    {
     int i;
     for(i = 0; i < CFontObj::GetCount(); i++)
        { /* load each font */
         int n;
         CFontObj * fd = CFontObj::GetFontObj(i);
         n = c_FontList.AddString(fd->GetDescription()->GetBuffer(0));
         if(n != CB_ERR)
            { /* added OK */
             c_FontList.SetItemData(n, (LPARAM)fd);
            } /* added OK */
        } /* load each font */
    }

/****************************************************************************
*                          CFontSelect::OnInitDialog
* Result: BOOL
*       TRUE, always (leaves focus alone)
* Effect: 
*       Initializes the font list by calling the font insertion routine
*       for each of the known usages.
****************************************************************************/

BOOL CFontSelect::OnInitDialog()
    {
     CDialog::OnInitDialog();
        
     LoadFonts();
        
     // If the list is nonempty then select the first one in the list

     if(c_FontList.GetCount() > 0)
        { /* set and show */
         c_FontList.SetCurSel(0);
         OnSelchangeFontlist();
        } /* set and show */

     return TRUE;  // return TRUE  unless you set the focus to a control
    }

/****************************************************************************
*                           CFontSelect::OnDestroy
* Result: void
*       
* Effect: 
*       If any cleanup is required we will do it here
****************************************************************************/

void CFontSelect::OnDestroy()
{
        CDialog::OnDestroy();
        
        // TODO: Add your message handler code here
        
}

/****************************************************************************
*                                 FontOptions
* Inputs:
*       CWnd * parent: Parent window
* Result: void
*       
* Effect: 
*       Handles the font options dialog instantiation.  This pops up the
*       font dialog that will allow the user to select the font configuration
*       for the various pages
****************************************************************************/

void FontOptions(CWnd * parent)
    {
     CFontSelect dlg;
     dlg.DoModal();
    }
