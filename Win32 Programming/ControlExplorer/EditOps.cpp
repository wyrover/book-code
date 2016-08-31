// EditOps.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "IntEdit.h"
#include "TinyButt.h"
#include "HexDisp.h"
#include "ixable.h"
#include "loggable.h"
#include "EditOps.h"

#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditOps dialog

IMPLEMENT_DYNCREATE(CEditOps, CLoggingPage)

CEditOps::CEditOps()
{
}

CEditOps::CEditOps(int idd) : CLoggingPage(idd)
{
        //{{AFX_DATA_INIT(CEditOps)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CEditOps::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditOps)
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditOps, CDialog)
        //{{AFX_MSG_MAP(CEditOps)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditOps message handlers
void CEditOps::addMessage(CString s)
{
 CString msg;
 msg.LoadString(IDS_MSG);
 c_Messages->addMessage(edit->m_hWnd, msg, s);
}

void CEditOps::OnGetlinecount() 
{
 DWORD result = edit->SendMessage(EM_GETLINECOUNT);
 showResult_N_N_d(IDS_EM_GETLINECOUNT, result);
}

void CEditOps::OnCanundo() 
{
 DWORD result = edit->SendMessage(EM_CANUNDO);
 showResult_N_N_d(IDS_EM_CANUNDO, result);
}


void CEditOps::OnEmptyundobuffer() 
{
 edit->SendMessage(EM_EMPTYUNDOBUFFER);
 showResult_N_N_N(IDS_EM_EMPTYUNDOBUFFER);
 enableControls();
}

void CEditOps::OnFmtlines() 
{
 BOOL result = edit->FmtLines(c_EOLFlag.GetCheck());
 showResult_d_N_d(IDS_EM_FMTLINES, c_EOLFlag.GetCheck(), result);
 enableControls();
}


/****************************************************************************
*                        CEditOps::getFirstVisibleLine
* Result: DWORD
*       
* Effect: 
*       ²
****************************************************************************/

DWORD CEditOps::getFirstVisibleLine()
    {
     DWORD result = edit->SendMessage(EM_GETFIRSTVISIBLELINE);
     showResult_N_N_d(IDS_EM_GETFIRSTVISIBLELINE, result);

     return result;
     
    }


void CEditOps::OnGetfirstvisibleline() 
{
 getFirstVisibleLine();
}


void CEditOps::OnSetlimittext()
{
 int val = c_LimitVal.GetVal();
 edit->SendMessage(EM_SETLIMITTEXT, val);
 showResult_d_N_N(IDS_EM_SETLIMITTEXT, val);
}

void CEditOps::OnGetlimittext() 
{
 DWORD result = edit->SendMessage(EM_GETLIMITTEXT);
 showResult_N_N_d(IDS_EM_GETLIMITTEXT, result);
}

/****************************************************************************
*                             CEditOps::OnGetline
*
*                         len = EM_GETLINE(line, 0);
* Result: void
*       
* Effect: 
*       Retrieves a line from the edit control
****************************************************************************/

void CEditOps::OnGetline() 
{
 int line = c_GetlineVal.GetVal();

 // We don't actually report that we've sent this message, since this
 // is part of the internals of the Explorer.
 
 // We have the line number, but EM_LINELENGTH wants a character offset 
 // 

 int pos = edit->SendMessage(EM_LINEINDEX, line);

 int len = edit->LineLength(pos);

 // We need at least a word's worth of space at the start for a length word.
 // So make sure our initial size is at least this large.

 // Note that we have to allow enough space to hold the potential NUL byte
 // even though one is not actually inserted!
 if(len < sizeof(WORD))
    len = sizeof(WORD);

 len++;  // for NUL byte

 TCHAR * b;
 b = new TCHAR[len];

 *((WORD*)b) = len;

 int newlen = edit->SendMessage(EM_GETLINE, line, (LPARAM)b);
 
 b[newlen] = _T('\0');

 c_Edit.SetWindowText(b);

 CString t;
 t.LoadString(IDS_EM_GETLINE);
 
 CString s;
 s.Format(_T("%s (%d, b) [%d]=>%d"), t, line, len, newlen);
 addMessage(s);

 delete b;
        
}

void CEditOps::OnGetmargins() 
{
 ASSERT(FALSE); // implement in lower level
}

void CEditOps::OnGetmodify() 
{
 DWORD result = edit->SendMessage(EM_GETMODIFY);
 showResult_N_N_d(IDS_EM_GETMODIFY, result);
}

void CEditOps::OnGetpasswordchar() 
{
 TCHAR ch = (TCHAR)edit->SendMessage(EM_GETPASSWORDCHAR);
 showResult_N_N_c(IDS_EM_GETPASSWORDCHAR, ch);
}

void CEditOps::OnGetrect() 
{
 ASSERT(FALSE);  // implement at lower level    
}

/****************************************************************************
*                              CEditOps::getSel
* Result: DWORD
*       selection value returned
* Effect: 
*       Gets the selection value and updates the controls.
****************************************************************************/

DWORD CEditOps::getSel()
{
 DWORD sel = edit->SendMessage(EM_GETSEL);

 if(c_LowSel.m_hWnd != NULL)
    c_LowSel.SetVal(LOWORD(sel));

 if(c_HighSel.m_hWnd != NULL)
    c_HighSel.SetVal(HIWORD(sel));

 return sel;
}

void CEditOps::OnGetsel() 
{
 DWORD sel = getSel();
 showResult_N_N_DW(IDS_EM_GETSEL, sel);
}

// Note: This applies only to rich edit controls

void CEditOps::OnGetseltext() 
{
 DWORD sel = edit->SendMessage(EM_GETSEL);
 int len = HIWORD(sel) - LOWORD(sel);

 TCHAR * text = new TCHAR[len + 1];

 edit->SendMessage(EM_GETSELTEXT, 0, (LPARAM)text);

 c_Edit.SetWindowText(text);

 delete text;
        
}

void CEditOps::OnGetthumb() 
{
 DWORD result = edit->SendMessage(EM_GETTHUMB);
 showResult_N_N_d(IDS_EM_GETTHUMB, result);
}

void CEditOps::OnWmUndo() 
{
 int result = edit->SendMessage(WM_UNDO);
 showResult_N_N_d(IDS_WM_UNDO, result);
}

void CEditOps::OnEmUndo() 
{
 int result = edit->SendMessage(EM_UNDO);
 showResult_N_N_d(IDS_EM_UNDO, result);
}

void CEditOps::OnWmSettext() 
{
 CString s;
 c_Edit.GetWindowText(s);
 edit->SetWindowText(s);
 enableControls();

 CString t;
 t.LoadString(IDS_WM_SETTEXT);
 
 s.Format(_T("%s(0, \"...\")"), t);
 addMessage(s);
}

void CEditOps::OnWmPaste() 
{
 int result = edit->SendMessage(WM_PASTE);
 showResult_N_N_N(IDS_WM_PASTE);
 enableControls();
}

void CEditOps::OnWmGettextlength() 
{
 DWORD result = edit->SendMessage(WM_GETTEXTLENGTH);
 showResult_N_N_d(IDS_WM_GETTEXTLENGTH, result);
}
 
void CEditOps::OnWmGettext() 
{
 CString s;

 int len = edit->SendMessage(WM_GETTEXTLENGTH);

 TCHAR * text = new TCHAR[len + 1];

 int result = edit->SendMessage(WM_GETTEXT, len + 1, (LPARAM)text);

 
 if(c_Hex.GetCheck())
    { /* use hex display */
     c_Edit.SetWindowTextHex(text);
    } /* use hex display */
 else
    { /* use normal display */
     c_Edit.SetWindowTextAscii(text);
    } /* use normal display */

 CString t;
 t.LoadString(IDS_WM_GETTEXT);
 
 s.Format(_T("%s(%d, &b) => %d"), t, len + 1, result);

 addMessage(s);

 delete text;
}

void CEditOps::OnWmCut() 
{
 edit->SendMessage(WM_CUT);
 showResult_N_N_N(IDS_WM_CUT);
 enableControls();
}

void CEditOps::OnWmCopy() 
{
 edit->SendMessage(WM_COPY);
 showResult_N_N_N(IDS_WM_COPY);
 enableControls();
}

void CEditOps::OnWmClear() 
{
 edit->SendMessage(WM_CLEAR);
 showResult_N_N_N(IDS_WM_CLEAR);
 enableControls();
}

/****************************************************************************
*                             CEditOps::OnSetsel
* Result: void
*       
* Effect: 
*       Issues an EM_SETSEL message.  
****************************************************************************/

void CEditOps::OnSetsel() 
{
 int low = c_LowSel.GetVal();
 int high = c_HighSel.GetVal();
 BOOL scroll;

 if(c_ScrollSel.m_hWnd != NULL)
    scroll = c_ScrollSel.GetCheck();
 else
    scroll = FALSE;

 edit->SetSel(low, high, scroll);
 
 CString t;
 t.LoadString(IDS_EM_SETSEL);
 
 CString s;
 s.Format(_T("%s(%d, 0x%08x=%d,%d)"), t, scroll, 
                        MAKELONG(low, high), low, high);

 enableControls();  // enable or disable cut/copy commands
 addMessage(s);
        
}

void CEditOps::OnSetrectnp() 
{
 ASSERT(FALSE);  // implement at lower level
        
}

void CEditOps::OnSetrect() 
{
 ASSERT(FALSE); // implement at lower level
        
}

void CEditOps::OnSetreadonly() 
{
 edit->SendMessage(EM_SETREADONLY, c_ReadOnlyFlag.GetCheck());
 showResult_d_N_N(IDS_EM_SETREADONLY, c_ReadOnlyFlag.GetCheck());
 enableControls();
}

void CEditOps::OnSetpasswordchar() 
{
 CString s;
 c_PasswordChar.GetWindowText(s);

 // Note: if there is no text, this should clear the password char.
 // Therefore, we have to check that there is a character in the string
 // and set the password char to 0 if there isn't
 // Note that since this is a CString, so s[0] will cause an assert failure
 // since we are attempting to access off the end of the string.

 TCHAR ch;

 if(s.GetLength() > 0)
    ch = s[0];
 else
    ch = _T('\0');
 edit->SetPasswordChar(ch);
 showResult_c_N_N(IDS_EM_SETPASSWORDCHAR, ch);
        
}

void CEditOps::OnSetmodify() 
{
 edit->SendMessage(EM_SETMODIFY, c_ModifyFlag.GetCheck());
 showResult_d_N_N(IDS_EM_SETMODIFY, c_ModifyFlag.GetCheck());
}

void CEditOps::OnSetmargins() 
{
 ASSERT(FALSE);  // implement at lower level
        
}

void CEditOps::OnScrollcaret() 
{
 ASSERT(FALSE); // implement at lower level
}

void CEditOps::OnScroll() 
{
 ASSERT(FALSE); // implement at lower level     
}

/****************************************************************************
*                           CEditOps::OnReplacesel
*
*                 EM_REPLACESEL(undoable, MAKELONG(low, high))
* Result: void
*       
* Effect: 
*       Replaces the selection in the list box by the string which it takes
*       from the scratch area.
* Notes:
*       We must send the EM_REPLACESEL message directly, since the MFC
*       interface does not support the "undoable" flag in wParam.
****************************************************************************/

void CEditOps::OnReplacesel() 
{
 CString b;
 c_Edit.GetWindowText(b);
 edit->SendMessage(EM_REPLACESEL, c_Undoable.GetCheck(), (LPARAM)(LPCTSTR)b);

 CString t;
 t.LoadString(IDS_EM_REPLACESEL);
 
 BOOL undoable;
 if(Is3())
    undoable = 0;
 else
    undoable = c_Undoable.GetCheck();

 CString s;
 s.Format(_T("%s(%d, b)"), t, undoable);
 addMessage(s);

 enableControls();
}

/****************************************************************************
*                           CEditOps::OnPosfromchar
*
*                               EM_POSFROMCHAR
* Result: void
*       
* Effect: 
*       ²
****************************************************************************/

void CEditOps::OnPosfromchar() 
{
        
}

void CEditOps::OnLinescroll() 
{
 ASSERT(FALSE); // implement at lower level     
}

void CEditOps::OnLineindex() 
{
 ASSERT(FALSE);  // implement at lower level    
}

/****************************************************************************
*                           CEditOps::getLineFromChar
* Inputs:
*       int pos: Character position
* Result: int
*       Line position, given character position
* Effect: 
*       Given the character position, sends a message requesting the line
*       number and displays the result in the message window.
****************************************************************************/

int CEditOps::getLineFromChar(int pos)
    {
     // returns the line number from the character position

     int result = edit->LineFromChar(pos);

     showResult_d_N_d(IDS_EM_LINEFROMCHAR, pos, result);
     
     return result;
    }

void CEditOps::OnLinefromchar() 
{
 getLineFromChar(c_LineFromCharVal.GetVal());

}

void CEditOps::OnLinelength() 
{
 int pos = c_LowSel.GetVal();

 int len = edit->SendMessage(EM_LINELENGTH, pos);

 showResult_d_N_d(IDS_EM_LINELENGTH, pos, len);
        
}

/****************************************************************************
*                          CEditOps::enableControls
* Result: void
*       
* Effect: 
*       Looks at all conditions and decides what controls should be enabled
****************************************************************************/

void CEditOps::enableControls()
    {
     DWORD sel;
     sel = edit->SendMessage(EM_GETSEL);
     
     BOOL hassel = (HIWORD(sel) != LOWORD(sel));

     if(c_WM_Paste.m_hWnd != NULL)
        c_WM_Paste.EnableWindow(IsClipboardFormatAvailable(CF_TEXT) &&
                                !(edit->GetStyle() & ES_READONLY));

     if(c_WM_Cut.m_hWnd != NULL)
        c_WM_Cut.EnableWindow(hassel);

     if(c_WM_Cut.m_hWnd != NULL)
        c_WM_COPY.EnableWindow(hassel);

     if(c_Hex.m_hWnd != NULL && c_Edit.m_hWnd != NULL)
        { /* hex display? */
         c_SetText.EnableWindow(!c_Hex.GetCheck());
         c_Edit.SetReadOnly(c_Hex.GetCheck());
        } /* hex display? */

     if(c_SetMargins.m_hWnd != NULL)
        c_SetMargins.EnableWindow(!Is3());
     if(c_GetMargins.m_hWnd != NULL)
        c_GetMargins.EnableWindow(!Is3());

     if(c_GetThumb.m_hWnd != NULL)
        c_GetThumb.EnableWindow(!Is3());
     if(c_EM_Undo.m_hWnd != NULL)
        c_EM_Undo.EnableWindow(edit->SendMessage(EM_CANUNDO));
     if(c_WM_Undo.m_hWnd != NULL)
        c_WM_Undo.EnableWindow(edit->SendMessage(EM_CANUNDO));
     if(c_EmptyUndoBuffer.m_hWnd != NULL)
        c_EmptyUndoBuffer.EnableWindow(edit->SendMessage(EM_CANUNDO));

     int len = edit->GetWindowTextLength();
     if(c_SpinLowSel.m_hWnd != NULL)
        c_SpinLowSel.SetRange(-1, len);
     if(c_SpinHighSel.m_hWnd != NULL)
        c_SpinHighSel.SetRange(-1, len);
        
     if(c_Undoable.m_hWnd != NULL)
        c_Undoable.EnableWindow(!Is3());
    }

BOOL CEditOps::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditOps::OnChangePasswordChar() 
{
 enableControls();
}

void CEditOps::OnChangeEdit() 
{

}

/****************************************************************************
*                            CEditOps::OnSetActive
* Result: BOOL
*       
* Effect: 
*       Forces the controls to be updated when the page becomes active.
*       Thus any changes made will become evident
****************************************************************************/

BOOL CEditOps::OnSetActive()
    {
     enableControls();

     return CIndexablePage::OnSetActive();
    }

/****************************************************************************
*                               CEditOps::OnHex
* Result: void
*       
* Effect: 
*       Converts the display to hex format or back depending on setting
****************************************************************************/

void CEditOps::OnHex()
    {
     if(c_Hex.GetCheck())
        { /* hex */
         c_Edit.toHex();
         c_Edit.SetReadOnly(TRUE);
        } /* hex */
     else
        { /* ascii */
         c_Edit.toAscii();
         c_Edit.SetReadOnly(FALSE);
        } /* ascii */

     enableControls();
    }
