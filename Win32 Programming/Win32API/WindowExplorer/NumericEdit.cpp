// NumericEdit.cpp : implementation file
//

#include "stdafx.h"
#include "stdlib.h"
#include "NumericEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NumericEdit

CNumericEdit::CNumericEdit()
{
 deffmt = NULL;
}

CNumericEdit::CNumericEdit(LPCTSTR fmt)
   {
    deffmt = fmt;
   }
	    

CNumericEdit::~CNumericEdit()
{
}



BEGIN_MESSAGE_MAP(CNumericEdit, CEdit)
	//{{AFX_MSG_MAP(CNumericEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NumericEdit message handlers

/****************************************************************************
*                          CNumericEdit.SetWindowText
* Inputs:
*       int val: Value to set
*	LPCTSTR fmt: Format value to use
* Result: void
*       
* Effect: 
*       Sets a representation of the numeric value to the window
****************************************************************************/

void CNumericEdit::SetWindowText(int val, LPCTSTR fmt)
    {
     if(fmt == NULL)
        { /* no explict format */
	 if(deffmt == NULL)
	   fmt = _T("%d");
        else
	   fmt = deffmt;
	} /* no explict format */

     CString s;
     s.Format(fmt, val);

     // Reduce flicker: see if we have same string; don't set text if
     // strings are identical.
     CString oldstr;
     CEdit::GetWindowText(oldstr);
     if(lstrcmp(oldstr, s) != 0)
	CEdit::SetWindowText(s);
    }

/****************************************************************************
*                         CNumericEdit::GetWindowInt
* Result: int
*       Integer value stored in edit control
****************************************************************************/

int CNumericEdit::GetWindowInt()
    {
     CString s;
     GetWindowText(s);
     int val;

     // TODO: Write Hex recognition code here...

#ifdef _UNICODE
     val = _wtoi(s);
#else
     val = atoi(s);
#endif
     return val;
    }

/****************************************************************************
*                             CNumericEdit::Blank
* Result: void
*       
* Effect: 
*       Blanks the contents of the control
****************************************************************************/

void CNumericEdit::Blank()
    {
     CEdit::SetWindowText(_T(""));
    }
