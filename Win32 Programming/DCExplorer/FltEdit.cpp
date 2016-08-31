// FltEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "FltEdit.h"
#include "stdlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatEdit

CFloatEdit::CFloatEdit()
{
}

CFloatEdit::~CFloatEdit()
{
}


BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
	//{{AFX_MSG_MAP(CFloatEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatEdit message handlers

/****************************************************************************
*                           CFloatEdit::GetVal
* Result: FLOAT
*       
* Effect: 
*       The float value of the edit control
****************************************************************************/

FLOAT CFloatEdit::GetVal()
    {
     CString s;
     CEdit::GetWindowText(s);
     return (FLOAT)atof(s);
    }

/****************************************************************************
*                           CFloatEdit::SetVal
* Inputs:
*       FLOAT f: Float value
* Result: void
*       
* Effect: 
*       Sets the float value of the edit control
****************************************************************************/

void CFloatEdit::SetVal(FLOAT f)
    {
     CString s;
     s.Format(_T("%6.3g"), f);
     SetWindowText(s);
    }

/****************************************************************************
*                             CFloatEdit::IsBlank
* Result: BOOL
*       TRUE if control is completely blank
*	FALSE if control is nonblank
****************************************************************************/

BOOL CFloatEdit::IsBlank()
    {
     CString s;
     CEdit::GetWindowText(s);
     int len = s.GetLength();
     if(len == 0)
	return TRUE;
     for(int i = 0; i < len; i++)
	if(s[i] != _T(' ') && s[i] != _T('\t'))
	   return FALSE;

     return TRUE;
    }

