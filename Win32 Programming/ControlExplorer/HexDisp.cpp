// HexDisp.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "HexDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexDisplay

CHexDisplay::CHexDisplay()
{
}

CHexDisplay::~CHexDisplay()
{
}


BEGIN_MESSAGE_MAP(CHexDisplay, CEdit)
        //{{AFX_MSG_MAP(CHexDisplay)
        ON_WM_DESTROY()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexDisplay message handlers

/****************************************************************************
*                           CHexDisp::SetWindowText
* Inputs:
*       LPCTSTR s: text to store
* Result: void
*       
* Effect: 
*       Stores the text in the window.  If the GWL_USERDATA field is 0,
*       stores it as ASCII text.  If the GWL_USERDATA field is nonzero,
*       stores the text in the GWL_USERDATA field and stores a hex
*       representation in the window
****************************************************************************/

void CHexDisplay::SetWindowText(LPCTSTR s)
    {
     CString * ascii = (CString *)::GetWindowLong(m_hWnd, GWL_USERDATA);
     if(ascii == NULL)
        { /* store ascii */
         CEdit::SetWindowText(s);
         return;
        } /* store ascii */

     delete ascii;

     ascii = new CString(s);
     ::SetWindowLong(m_hWnd, GWL_USERDATA, (LPARAM)ascii);

     CString text = makeHex(s);

     CEdit::SetWindowText(text);
    }


void CHexDisplay::OnDestroy() 
{
        CEdit::OnDestroy();
        
 CString * ascii = (CString *)::GetWindowLong(m_hWnd, GWL_USERDATA);
 if(ascii != NULL)
    delete ascii;
 ::SetWindowLong(m_hWnd, GWL_USERDATA, 0);
        
}

/****************************************************************************
*                            CHexDisplay::makeHex
* Inputs:
*       LPCTSTR s: input string
* Result: CString
*       String converted to hex
* Effect: 
*       Allocates a CString
****************************************************************************/

CString CHexDisplay::makeHex(LPCTSTR s)
    {
     CString text;
     TCHAR hex[10];

     while(*s != _T('\0'))
        { /* scan each */
         wsprintf(hex, _T("%02x "), (BYTE)*s);
         text += hex;
         s++;
        } /* scan each */

     return text;
    }

/****************************************************************************
*                             CHexDisplay::toHex
* Result: BOOL
*       TRUE if the display was in ascii mode and now is in hex
*       FALSE if the display was already in hex mode
* Effect: 
*       Converts the ascii display to a hex display
****************************************************************************/

BOOL CHexDisplay::toHex()
    {
     CString * ascii = (CString *)::GetWindowLong(m_hWnd, GWL_USERDATA);
     if(ascii != NULL)
        return FALSE; // already in hex mode

     CString s;
     CEdit::GetWindowText(s);
     
     ascii = new CString(s);
     ::SetWindowLong(m_hWnd, GWL_USERDATA, (LPARAM)ascii);

     CEdit::SetWindowText(makeHex(s));

     return TRUE;
    }

/****************************************************************************
*                            CHexDisplay::toAscii
* Result: BOOL
*       TRUE if hex value is replaced by ASCII value
*       FALSE if already in ASCII
* Effect: 
*       If in hex, converts the display to ASCII
****************************************************************************/

BOOL CHexDisplay::toAscii()
    {
     CString * ascii = (CString *)::GetWindowLong(m_hWnd, GWL_USERDATA);
     if(ascii == NULL)
        return FALSE; // already in ASCII mode
     
     CEdit::SetWindowText(*ascii);

     delete ascii;
     ::SetWindowLong(m_hWnd, GWL_USERDATA, 0);
     return TRUE;
    }

/****************************************************************************
*                             CHexDisplay::IsHex
* Result: BOOL
*       TRUE if display is in hex
*       FALSE if display is in ASCII
****************************************************************************/

BOOL CHexDisplay::IsHex()
    {
     return (::GetWindowLong(m_hWnd, GWL_USERDATA) != 0);
    }

/****************************************************************************
*                       CHexDisplay::SetWindowTextAscii
* Inputs:
*       LPCTSTR s: String to set
* Result: void
*       
* Effect: 
*       Sets the text unconditionally in ASCII.
****************************************************************************/

void CHexDisplay::SetWindowTextAscii(LPCTSTR s)
    {
     CString * ascii = (CString *)::GetWindowLong(m_hWnd, GWL_USERDATA);
     if(ascii != NULL)
        delete ascii;

     ::SetWindowLong(m_hWnd, GWL_USERDATA, 0);
     CEdit::SetWindowText(s);
    }

/****************************************************************************
*                        CHexDisplay::SetWindowTextHex
* Inputs:
*       LPCTSTR s: String to be stored as hex
* Result: void
*       
* Effect: 
*       Stores the string as both hidden ASCII and visible hex data
****************************************************************************/

void CHexDisplay::SetWindowTextHex(LPCTSTR s)
    {
     CString * ascii = (CString *)::GetWindowLong(m_hWnd, GWL_USERDATA);
     if(ascii != NULL)
        delete ascii;
     ascii = new CString(s);
     ::SetWindowLong(m_hWnd, GWL_USERDATA, (LPARAM)ascii);

     CEdit::SetWindowText(makeHex(s));
    }

/****************************************************************************
*                         CHexDisplay::GetWindowText
* Inputs:
*       LPTSTR s: String location to store data
*       int count: Maximum count
* Result: int
*       Number of characters stored
* Effect: 
*       Copies the ascii data to the string
****************************************************************************/

int CHexDisplay::GetWindowText(LPTSTR str, int count)
    {
     CString * ascii = (CString *)::GetWindowLong(m_hWnd, GWL_USERDATA);
     if(ascii == NULL)
        return CEdit::GetWindowText(str, count);

     LPTSTR d = str;
     LPCTSTR s = (LPCTSTR)ascii;
     int c = count;

     while(c-- > 1 && *s != _T('\0'))
        *d++ = *s++;
     *d = _T('\0');
     
     return count - c;
    }

/****************************************************************************
*                         CHexDisplay::GetWindowText
* Inputs:
*       CString & str: Place to put result
* Result: void
*       
* Effect: 
*       Copies the ASCII data from the window to the string
****************************************************************************/

void CHexDisplay::GetWindowText(CString &str)
    {
     CString * ascii = (CString *)::GetWindowLong(m_hWnd, GWL_USERDATA);
     if(ascii == NULL)
        { /* ascii */
         CEdit::GetWindowText(str);
         return;
        } /* ascii */
     str = *ascii;
    }
