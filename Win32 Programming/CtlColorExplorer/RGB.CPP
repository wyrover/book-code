// RGB.cpp : implementation file
//

#include "stdafx.h"
#include "CtlColorExplorer.h"
#include "RGB.h"
#include "colors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRGB

CRGB::CRGB()
{
}

CRGB::~CRGB()
{
}


BEGIN_MESSAGE_MAP(CRGB, CStatic)
        //{{AFX_MSG_MAP(CRGB)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRGB message handlers

/****************************************************************************
*                             CRGB::SetWindowText
* Inputs:
*       COLORREF rgb: Color triple
* Result: void
*       
* Effect: 
*       Sets the RGB components
****************************************************************************/

void CRGB::SetWindowText(COLORREF rgb)
    {
     char buf[50];
     wsprintf(buf, _T("%d, %d, %d"), GetRValue(rgb), GetGValue(rgb),
                                     GetBValue(rgb));
     if(rgb == COLOR_NONE)
        CStatic::SetWindowText(_T(""));
     else
        CStatic::SetWindowText(buf);
    }
