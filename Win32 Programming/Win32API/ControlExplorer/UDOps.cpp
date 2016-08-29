#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "UDOps.h"

IMPLEMENT_DYNCREATE(CUDOps, CLoggingPage)

CUDOps::CUDOps()
{
 ASSERT(FALSE); // Must never call in context where default constructor used
}

CUDOps::CUDOps(int idd) : CLoggingPage(idd)
{
}

CUDOps::~CUDOps()
{
}


/****************************************************************************
*                          CUDOps::addMessage
* Inputs:
*       CString s: String to add
* Result: void
*       
* Effect: 
*       Logs the formatted string
****************************************************************************/

void CUDOps::addMessage(CString s)
    {
     CString ctlname;
     ctlname.LoadString(IDS_UPDOWN);
     c_Messages->addMessage(c_UpDown->m_hWnd, ctlname, s);
    }
