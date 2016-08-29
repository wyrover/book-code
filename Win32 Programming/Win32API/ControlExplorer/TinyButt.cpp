// TinyButt.cpp : implementation file
//

#include "stdafx.h"
#include "TinyButt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTinyButton

CTinyButton::CTinyButton()
{
}

CTinyButton::~CTinyButton()
{
}


BEGIN_MESSAGE_MAP(CTinyButton, CButton)
	//{{AFX_MSG_MAP(CTinyButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTinyButton message handlers

/****************************************************************************
*			   CTinyButton::findCaption
* Result: CStatic *
*       Caption of the control, or NULL if problem
* Notes: 
*       All controls are arranged with their tab order established so
*	that the static control logically PRECEDES the button (even if
*	the static control is on the right), e.g.,
*
*	1-caption   2-[]         []-8  caption-7
*	3-caption   4-[]         []-10 caption-9
*	5-caption   6-[]         []-12 caption-11
*
*	To locate the caption of a control, we just take its predecessor
*	sibling
****************************************************************************/

CStatic * CTinyButton::findCaption()
    {
     return (CStatic *)GetWindow(GW_HWNDPREV);
    }

/****************************************************************************
*                         CTinyButton::GetWindowText
* Inputs:
*       CString &s: String to use
* Result: void
*       
* Effect: 
*       If the caption is found, return its value
****************************************************************************/

void CTinyButton::GetWindowText(CString & s)
    {
     CStatic * caption = findCaption();

     if(caption == NULL)
        { /* nothing */
	 s = _T("");
	 return;
	} /* nothing */
     caption->GetWindowText(s);
    }

/****************************************************************************
*                          CTinyButton::EnableWindow
* Inputs:
*       BOOL mode: TRUE or FALSE
* Result: BOOL
*       Previous enable value
* Effect: 
*       Enables or disables the window and its accompanying caption window
****************************************************************************/

BOOL CTinyButton::EnableWindow(BOOL newval)
    {
     BOOL old = IsWindowEnabled();

     CStatic * caption = findCaption();

     if(caption != NULL)
	caption->EnableWindow(newval);

     CButton::EnableWindow(newval);
     return old;
    }
