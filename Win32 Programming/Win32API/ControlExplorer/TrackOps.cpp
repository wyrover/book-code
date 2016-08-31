#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "trackOps.h"

IMPLEMENT_DYNCREATE(CTrackOps, CLoggingPage)

CTrackOps::CTrackOps()
{
 ASSERT(FALSE); // Must never call in context where default constructor used
}

CTrackOps::CTrackOps(int idd) : CLoggingPage(idd)
{
}

CTrackOps::~CTrackOps()
{
}


/****************************************************************************
*                          CTrackOps::addMessage
* Inputs:
*       CString s: String to add
* Result: void
*       
* Effect: 
*       Logs the formatted string
****************************************************************************/

void CTrackOps::addMessage(CString s)
    {
     CString ctlname;
     ctlname.LoadString(IDS_TRACKBAR);
     c_Messages->addMessage(c_Slider->m_hWnd, ctlname, s);
    }
