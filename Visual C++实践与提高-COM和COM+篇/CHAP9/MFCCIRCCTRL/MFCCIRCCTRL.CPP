// MfcCircCtrl.cpp : Implementation of CMfcCircCtrlApp and DLL registration.

#include "stdafx.h"
#include "MfcCircCtrl.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


CMfcCircCtrlApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
{ 0x4c0abe66, 0x8c14, 0x4191, { 0xa9, 0xd0, 0x42, 0xa7, 0xe, 0xf6, 0x34, 0x42 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CMfcCircCtrlApp::InitInstance - DLL initialization

BOOL CMfcCircCtrlApp::InitInstance()
{
    BOOL bInit = COleControlModule::InitInstance();

    if (bInit) {
        // TODO: Add your own module initialization code here.
    }

    return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CMfcCircCtrlApp::ExitInstance - DLL termination

int CMfcCircCtrlApp::ExitInstance()
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
