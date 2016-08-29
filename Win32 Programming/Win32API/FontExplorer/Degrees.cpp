// Degrees.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "Degrees.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDegrees

CDegrees::CDegrees()
{
}

CDegrees::~CDegrees()
{
}


BEGIN_MESSAGE_MAP(CDegrees, CEdit)
        //{{AFX_MSG_MAP(CDegrees)
        ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
        ON_WM_KILLFOCUS()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDegrees message handlers

void CDegrees::OnChange() 
{
        // TODO: Add your control notification handler code here
        
}

/****************************************************************************
*                            CDegrees::OnKillFocus
* Inputs:
*       CWnd * pNewWnd: ignored except as passed to superclass
* Result: void
*       
* Effect: 
*       Reads the integer value from the control, which may not be in 
*       canonical format, then sets it back, which canonicalizes it
****************************************************************************/

void CDegrees::OnKillFocus(CWnd* pNewWnd) 
{
        CEdit::OnKillFocus(pNewWnd);
        

        int pos = GetWindowInt();
        SetWindowInt(pos);
        
}

/****************************************************************************
*                           CDegrees::SetWindowInt
* Inputs:
*       int n: Rotation, expressed in tenths of a degree
* Result: void
*       
* Effect: 
*       Formats the text and sets it in the control
****************************************************************************/

void CDegrees::SetWindowInt(int n)
    {
     CString s;
     s.Format(_T("%s%d.%d°"), (-10 < n && n < 0 ? "-" : ""), 
                                        n / 10, abs(n) % 10);
     CEdit::SetWindowText(s);
    }

/****************************************************************************
*                           CDegrees::GetWindowInt
* Result: int
*       
* Effect: 
*       Parses the contents of the control
* Notes:
*       Parsing is very crude.
****************************************************************************/

int CDegrees::GetWindowInt()
    {
     CString s;
     GetWindowText(s);
     int val = 0;
     BOOL neg = FALSE;
     BOOL dec = FALSE;

     LPCTSTR p = (LPCTSTR)s;
     while(*p)
        { /* parse */
         switch(*p)
            { /* *p */
             case _T(' '):
             case _T('\t'):
                     break; // ignore whitespace (even embedded)
             case _T('+'):
                     if(neg || dec || val > 0)
                        goto done;  // syntax error: embedded +
                     break;
             case _T('-'):
                     if(neg)
                        goto done;  // syntax error: two --
                     neg = TRUE;
                     break;
             case _T('.'):
                     dec = TRUE; 
                     break;
             case _T('0'):
             case _T('1'):
             case _T('2'):
             case _T('3'):
             case _T('4'):
             case _T('5'):
             case _T('6'):
             case _T('7'):
             case _T('8'):
             case _T('9'):
                     val = val * 10 + (*p - _T('0'));
                     if(dec)
                        goto done; // only one decimal digit allowed
                     break;
              default:
                     goto done;  // syntax error
            } /* *p */
         p++;
        } /* parse */
done:
     if(!dec)
        val *= 10;  // if no decimal point, scale up to integer from tenths
     return (neg ? -val : val);
     
    }
