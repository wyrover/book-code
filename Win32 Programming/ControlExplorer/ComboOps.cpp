// comboops.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "comboops.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboOps property page

IMPLEMENT_DYNCREATE(CComboOps, CLoggingPage)

CComboOps::CComboOps(int idd) : CLoggingPage(idd)
{
}

CComboOps::CComboOps() : CLoggingPage((int)0)  // should never call default constructor!
{
        //{{AFX_DATA_INIT(CComboOps)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CComboOps::~CComboOps()
{
}

void CComboOps::DoDataExchange(CDataExchange * pDX)
{
        CLoggingPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CComboOps)
        //}}AFX_DATA_MAP
}
            
BEGIN_MESSAGE_MAP(CComboOps, CLoggingPage)
        //{{AFX_MSG_MAP(CComboOps)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CComboOps::addMessage(CString s)
    {
     CString CtlName;
     CtlName.LoadString(IDS_COMBO);

     c_Messages->addMessage(combo->m_hWnd, CtlName, s);
    }
