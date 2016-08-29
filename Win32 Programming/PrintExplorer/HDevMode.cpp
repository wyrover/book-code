// HDevMode.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "HandleEdit.h"
#include "HDevMode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHDevMode

CHDevMode::CHDevMode()
{
}

CHDevMode::~CHDevMode()
{
}


BEGIN_MESSAGE_MAP(CHDevMode, CTextHandleEdit)
        //{{AFX_MSG_MAP(CHDevMode)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHDevMode message handlers

/****************************************************************************
*                          CHDevMode::SetWindowText
* Inputs:
*       HANDLE hDevMode: Handle to DevMode structure
* Result: void
*       
* Effect: 
*       Sets the text to a string representation of the DevMode contents
****************************************************************************/

void CHDevMode::SetWindowText(HANDLE hDevMode)
    {
     CTextHandleEdit::SetWindowText(hDevMode);
    }
