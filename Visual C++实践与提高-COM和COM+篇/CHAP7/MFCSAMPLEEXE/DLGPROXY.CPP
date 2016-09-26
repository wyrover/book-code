// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSampleExe.h"
#include "DlgProxy.h"
#include "MFCSampleExeDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeDlgAutoProxy

IMPLEMENT_DYNCREATE(CMFCSampleExeDlgAutoProxy, CCmdTarget)

CMFCSampleExeDlgAutoProxy::CMFCSampleExeDlgAutoProxy()
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
    ASSERT_KINDOF(CMFCSampleExeDlg, AfxGetApp()->m_pMainWnd);
    m_pDialog = (CMFCSampleExeDlg*) AfxGetApp()->m_pMainWnd;
    m_pDialog->m_pAutoProxy = this;
}

CMFCSampleExeDlgAutoProxy::~CMFCSampleExeDlgAutoProxy()
{
    // To terminate the application when all objects created with
    //  with automation, the destructor calls AfxOleUnlockApp.
    //  Among other things, this will destroy the main dialog
    if (m_pDialog != NULL)
        m_pDialog->m_pAutoProxy = NULL;

    AfxOleUnlockApp();
}

void CMFCSampleExeDlgAutoProxy::OnFinalRelease()
{
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.
    CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CMFCSampleExeDlgAutoProxy, CCmdTarget)
    //{{AFX_MSG_MAP(CMFCSampleExeDlgAutoProxy)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMFCSampleExeDlgAutoProxy, CCmdTarget)
    //{{AFX_DISPATCH_MAP(CMFCSampleExeDlgAutoProxy)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMFCSampleExe to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {CDB6A4DB-819B-11D4-A23D-02608CACEDBB}
static const IID IID_IMFCSampleExe =
{ 0xcdb6a4db, 0x819b, 0x11d4, { 0xa2, 0x3d, 0x2, 0x60, 0x8c, 0xac, 0xed, 0xbb } };

BEGIN_INTERFACE_MAP(CMFCSampleExeDlgAutoProxy, CCmdTarget)
INTERFACE_PART(CMFCSampleExeDlgAutoProxy, IID_IMFCSampleExe, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {CDB6A4D9-819B-11D4-A23D-02608CACEDBB}
IMPLEMENT_OLECREATE2(CMFCSampleExeDlgAutoProxy, "MFCSampleExe.Application", 0xcdb6a4d9, 0x819b, 0x11d4, 0xa2, 0x3d, 0x2, 0x60, 0x8c, 0xac, 0xed, 0xbb)

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeDlgAutoProxy message handlers
