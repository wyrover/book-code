// EnumEdit.cpp : implementation file
//

#include "stdafx.h"
#include "stdlib.h"
#include "EnumEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EnumEdit

CEnumEdit::CEnumEdit()
{
 deffmt = NULL;
}

CEnumEdit::CEnumEdit(LPENUMEDIT fmt)
   {
    deffmt = fmt;
   }
	    

CEnumEdit::~CEnumEdit()
{
}



BEGIN_MESSAGE_MAP(CEnumEdit, CEdit)
	//{{AFX_MSG_MAP(CEnumEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EnumEdit message handlers

/****************************************************************************
*                          CEnumEdit.SetWindowText
* Inputs:
*       int val: Value to set
*	LPENUMEDIT fmt: Table to use
* Result: void
*       
* Effect: 
*       Sets a representation of the numeric value to the window as a
*	string.
* Notes:
*	The fmt points to a table of ENUMEDIT values which terminate with
*	one which has a 0 id.
****************************************************************************/

void CEnumEdit::SetWindowText(int val, LPENUMEDIT fmt)
    {
     if(fmt == NULL)
	fmt = deffmt;

     CString s;
     s.Format(_T("%d"), val);	// assume decimal result
     if(fmt != NULL)
        { /* has format */
	 int i;
	 for(i = 0; fmt[i].id != 0; i++)
	    { /* scan table */
	     if(val == fmt[i].val)
	        { /* found it */
		 if(s.LoadString(fmt[i].id) == 0)
		    s.Format(_T("%d"), val);	// assume decimal result
		 break;
		} /* found it */
	    } /* scan table */
	} /* has format */
     CEdit::SetWindowText(s);
    }

/****************************************************************************
*                         CEnumEdit::GetWindowInt
* Result: int
*       Integer value stored in edit control, 0 if cannot decode it
****************************************************************************/

int CEnumEdit::GetWindowInt(LPENUMEDIT fmt)
    {
     if(fmt == NULL)
	fmt = deffmt;

     CString s;
     GetWindowText(s);

     int val;

     // TODO: Write Hex recognition code here...

#ifdef _UNICODE
     val = _wtoi(s);
#else
     val = atoi(s);
#endif
     if(fmt != NULL && val == 0)
        { /* might be decodable */
	 int i;
	 for(i = 0; fmt[i].id != 0; i++)
	    { /* scan table */
	     CString v;
	     if(v.LoadString(fmt[i].id) != 0)
	        { /* compare it */
		 if(lstrcmp(s, v) == 0)
		    return fmt[i].val;
		} /* compare it */
	    } /* scan table */
	} /* might be decodable */
	 
     return val;
    }

/****************************************************************************
*                              CEnumEdit::Blank
* Result: void
*       
* Effect: 
*       Blanks out contents
****************************************************************************/

void CEnumEdit::Blank()
    {
     CEdit::SetWindowText(_T(""));
    }
