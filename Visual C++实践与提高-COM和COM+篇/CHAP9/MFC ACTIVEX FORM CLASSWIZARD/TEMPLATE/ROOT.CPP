// $$root$$.cpp : Implementation of C$$Safe_root$$App and DLL registration.

#include "stdafx.h"
#include "$$root$$.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


C$$Safe_root$$App NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
{ 0xafdc1584, 0xf993, 0x11d1, { 0x96, 0xed, 0xf0, 0x5f, 0x3, 0xc1, 0, 0 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// C$$Safe_root$$App::InitInstance - DLL initialization

BOOL C$$Safe_root$$App::InitInstance()
{
    AfxEnableControlContainer();
    BOOL bInit = COleControlModule::InitInstance();

    if (bInit) {
        // TODO: Add your own module initialization code here.
    }

    return bInit;
}


////////////////////////////////////////////////////////////////////////////
// C$$Safe_root$$App::ExitInstance - DLL termination

int C$$Safe_root$$App::ExitInstance()
{
    // TODO: Add your own module termination code here.
    return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    AFX_MANAGE_STATE(_afxModuleAddrThis);

    if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
        return ResultFromScode(SELFREG_E_TYPELIB);

    if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
        return ResultFromScode(SELFREG_E_CLASS);

    return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    AFX_MANAGE_STATE(_afxModuleAddrThis);

    if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
        return ResultFromScode(SELFREG_E_TYPELIB);

    if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
        return ResultFromScode(SELFREG_E_CLASS);

    return NOERROR;
}
