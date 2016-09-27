// Account.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSample.h"
#include "Account.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccount

IMPLEMENT_DYNCREATE(CAccount, CCmdTarget)

CAccount::CAccount()
{
    EnableAutomation();
    // To keep the application running as long as an OLE automation
    //  object is active, the constructor calls AfxOleLockApp.
    m_balance = 1000;
    AfxOleLockApp();
}

CAccount::~CAccount()
{
    // To terminate the application when all objects created with
    //  with OLE automation, the destructor calls AfxOleUnlockApp.
    AfxOleUnlockApp();
}


void CAccount::OnFinalRelease()
{
    // When the last reference for an automation object is released
    // OnFinalRelease is called.  The base class will automatically
    // deletes the object.  Add additional cleanup required for your
    // object before calling the base class.
    CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAccount, CCmdTarget)
    //{{AFX_MSG_MAP(CAccount)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAccount, CCmdTarget)
    //{{AFX_DISPATCH_MAP(CAccount)
    DISP_PROPERTY_EX(CAccount, "Balance", GetBalance, SetBalance, VT_I4)
    DISP_FUNCTION(CAccount, "Post", Post, VT_BSTR, VTS_I4)
    //}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAccount to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {657F06D0-80D5-11D4-A23D-02608CACEDBB}
static const IID IID_IAccount =
{ 0x657f06d0, 0x80d5, 0x11d4, { 0xa2, 0x3d, 0x2, 0x60, 0x8c, 0xac, 0xed, 0xbb } };

BEGIN_INTERFACE_MAP(CAccount, CCmdTarget)
INTERFACE_PART(CAccount, IID_IAccount, Dispatch)
END_INTERFACE_MAP()

// {657F06D1-80D5-11D4-A23D-02608CACEDBB}
IMPLEMENT_OLECREATE(CAccount, "MFCSample.Account", 0x657f06d1, 0x80d5, 0x11d4, 0xa2, 0x3d, 0x2, 0x60, 0x8c, 0xac, 0xed, 0xbb)

/////////////////////////////////////////////////////////////////////////////
// CAccount message handlers

long CAccount::GetBalance()
{
    return m_balance;
}

void CAccount::SetBalance(long nNewValue)
{
    m_balance = nNewValue;
}

BSTR CAccount::Post(long lAmount)
{
    CString strResult;

    if ((m_balance + lAmount) < 0) { //͸֧
        strResult.Format("Error. Balance(%ld) can't be overdrawn by %ld.", m_balance, -lAmount);
    } else {
        m_balance += lAmount;
        strResult.Format("you'v just %s %ld, balance is %ld now!",
                         ((lAmount >= 0) ? _T("Credit") : _T("Debit")), lAmount, m_balance);
    }

    return strResult.AllocSysString();
}
