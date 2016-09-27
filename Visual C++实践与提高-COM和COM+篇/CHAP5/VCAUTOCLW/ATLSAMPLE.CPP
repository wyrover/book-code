// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "atlsample.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IAccount properties

/////////////////////////////////////////////////////////////////////////////
// IAccount operations

long IAccount::GetBalance()
{
    long result;
    InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
}

void IAccount::SetBalance(long nNewValue)
{
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
                 nNewValue);
}

CString IAccount::Post(long lAmount)
{
    CString result;
    static BYTE parms[] =
        VTS_I4;
    InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
                 lAmount);
    return result;
}
