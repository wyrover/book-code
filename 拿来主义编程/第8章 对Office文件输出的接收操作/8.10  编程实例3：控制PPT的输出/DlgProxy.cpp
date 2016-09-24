// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "VcPPT.h"
#include "DlgProxy.h"
#include "VcPPTDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVcPPTDlgAutoProxy

IMPLEMENT_DYNCREATE(CVcPPTDlgAutoProxy, CCmdTarget)

CVcPPTDlgAutoProxy::CVcPPTDlgAutoProxy()
{
    EnableAutomation();
    // To keep the application running as long as an automation
    //  object is active, the constructor calls AfxOleLockApp.
    AfxOleLockApp();
    // Get access to the dialog through the application's
    //  main window pointer.  Set the proxy's internal pointer
    //  to point to the dialog, and set the dialog's back pointer to
    //  this proxy.
    ASSERT(AfxGetApp()->m_pMainWnd != NULL);
    ASSERT_VALID(AfxGetApp()->m_pMainWnd);
    ASSERT_KINDOF(CVcPPTDlg, AfxGetApp()->m_pMainWnd);
    m_pDialog = (CVcPPTDlg*) AfxGetApp()->m_pMainWnd;
    m_pDialog->m_pAutoProxy = this;
}

CVcPPTDlgAutoProxy::~CVcPPTDlgAutoProxy()
{
    // To terminate the application when all objects created with
    //  with automation, the destructor calls AfxOleUnlockApp.
    //  Among other things, this will destroy the main dialog
    if (m_pDialog != NULL)
        m_pDialog->m_pAutoProxy = NULL;

    AfxOleUnlockApp();
}

void CVcPPTDlgAutoProxy::OnFinalRelease()
{
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.
    CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CVcPPTDlgAutoProxy, CCmdTarget)
    //{{AFX_MSG_MAP(CVcPPTDlgAutoProxy)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CVcPPTDlgAutoProxy, CCmdTarget)
    //{{AFX_DISPATCH_MAP(CVcPPTDlgAutoProxy)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IVcPPT to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {00C190A1-BAD8-465A-8903-0E32A8210A99}
static const IID IID_IVcPPT =
{ 0xc190a1, 0xbad8, 0x465a, { 0x89, 0x3, 0xe, 0x32, 0xa8, 0x21, 0xa, 0x99 } };

BEGIN_INTERFACE_MAP(CVcPPTDlgAutoProxy, CCmdTarget)
INTERFACE_PART(CVcPPTDlgAutoProxy, IID_IVcPPT, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {786CC47F-8DD0-40ED-8AF3-C5B04E270387}
IMPLEMENT_OLECREATE2(CVcPPTDlgAutoProxy, "VcPPT.Application", 0x786cc47f, 0x8dd0, 0x40ed, 0x8a, 0xf3, 0xc5, 0xb0, 0x4e, 0x27, 0x3, 0x87)

/////////////////////////////////////////////////////////////////////////////
// CVcPPTDlgAutoProxy message handlers
