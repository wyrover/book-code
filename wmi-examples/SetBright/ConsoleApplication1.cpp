//Additional dependency? wbemuuid.lib
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#ifdef DEBUG
    //Because I'm crazy!
    #pragma strict_gs_check(on)
#endif


#include "ConsoleApplication1.h"

#include <objbase.h>
#include <windows.h>
#include <stdio.h>
#include <wbemidl.h>
#include <comdef.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <HighLevelMonitorConfigurationAPI.h>
#include <LowLevelMonitorConfigurationAPI.h>
//#include <tchar.h>
//#include <dxva2api.h>
//#include <stdio.h>
//#include <iostream>
#include <strsafe.h>
#include <string>
#include <memory>


//define SCOPE_GUARD_DEBUGGING for enhanced scope guard debugging.

#include "ScopeGuard.h"




//On returning E_FAIL, call GetLastError for details. That's not my idea! //TODO: mark as only returning S_OK, E_FAIL
_Success_(SUCCEEDED(return)) HRESULT CStyle_GetLastErrorAsFormattedMessage(_Out_writes_z_(strSize) _Pre_writable_size_(strSize) PWSTR psz_formatted_error, _In_range_(128, 32767) const rsize_t strSize, _In_ const DWORD error)
{
    //const auto err = GetLastError( );
    const auto err = error;
    const auto ret = FormatMessageW(
                         (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS),
                         NULL,
                         err,
                         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                         psz_formatted_error,
                         static_cast<DWORD>(strSize),
                         NULL);

    if (ret != 0) {
        static_assert(SUCCEEDED(S_OK), "CStyle_GetLastErrorAsFormattedMessage doesn't return a valid success code!");
        return S_OK;
    }

    const DWORD error_err = GetLastError();
    const rsize_t err_msg_buff_size = 512;
    _Null_terminated_ char err_msg_buff[ err_msg_buff_size ] = { 0 };
    const HRESULT fmt_res = StringCchPrintfA(err_msg_buff, err_msg_buff_size, "WDS: FormatMessageW failed with error code: `%lu`!!\r\n", error_err);

    if (SUCCEEDED(fmt_res)) {
        OutputDebugStringA(err_msg_buff);
    } else {
        WDS_ASSERT_EXPECTED_STRING_FORMAT_FAILURE_HRESULT(fmt_res);
        OutputDebugStringA("WDS: FormatMessageW failed, and THEN formatting the error message for FormatMessageW failed!\r\n");
        std::terminate();
    }

    static_assert(!SUCCEEDED(E_FAIL), "CStyle_GetLastErrorAsFormattedMessage doesn't return a valid error code!");
    return E_FAIL;
}




//what the fuck?
void printError()
{
    const DWORD lastErr = GetLastError();
    const rsize_t strBufferSize = 512u;
    wchar_t errBuffer[ strBufferSize ] = { 0 };
    const HRESULT fmt_res = CStyle_GetLastErrorAsFormattedMessage(errBuffer, strBufferSize, lastErr);

    if (SUCCEEDED(fmt_res)) {
        //OutputDebugStringW( errBuffer );
        wprintf(L"failed with error: %u\r\n\ttext: %s\r\n\r\n", lastErr, errBuffer);
        return;
    }

    printf("printError - DOUBLE FAULT!\r\n");
    std::terminate();
}

void printCOMerror()
{
    IErrorInfo* errInfo = NULL;
    //GetErrorInfo:
    //https://msdn.microsoft.com/en-us/library/ms221032.aspx
    const HRESULT errInfoResult = GetErrorInfo(0, &errInfo);

    if (FAILED(errInfoResult)) {
        printf("FAILED to get COM error info!\r\n");

        if (errInfoResult == S_FALSE) {
            printf("There was no error object to return\r\n");
        }

        return;
    }

    if (errInfoResult == S_FALSE) {
        printf("There was no error object to return\r\n");
        return;
    }

    BSTR errDescription = NULL;
    const HRESULT errDescriptionResult = errInfo->GetDescription(&errDescription);

    if (FAILED(errDescriptionResult)) {
        printf("Failed to get COM error description!\r\n");
        return;
    }

    printf("COM error description: %S\r\n", errDescription);
    SysFreeString(errDescription);
    return;
}

bool doesMonitorSupportBrightnessConfigurationViaDDC(_In_ const HANDLE hPhysicalMonitor)
{
    DWORD capabilitiesFlags_temp = 0;
    DWORD colorTempsFlags_junk = 0;
    const BOOL capabilitiesResult = GetMonitorCapabilities(hPhysicalMonitor, &capabilitiesFlags_temp, &colorTempsFlags_junk);

    if (capabilitiesResult != TRUE) {
        printf("failed to get monitor capabilities!\r\n");
        printError();
        return false;
    }

    const DWORD capabilitiesFlags = capabilitiesFlags_temp;

    if ((capabilitiesFlags & MC_CAPS_BRIGHTNESS) == 0) {
        printf("monitor DOES NOT support GetMonitorBrightness/SetMonitorBrightness!\r\n");
        return false;
    }

    return true;
}


bool ddcGetBrightness()
{
    //HMONITOR hMonitor = NULL;
    DWORD pdwMinimumBrightness = 0;
    DWORD pdwCurrentBrightness = 0;
    DWORD pdwMaximumBrightness = 0;
    const HWND hwnd = FindWindowW(NULL, NULL);
    printf("Window handle: %p\r\n", hwnd);
    const HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
    printf("hMonitor: %p\r\n", hMonitor);
    DWORD physicalMonitors_temp = 0;
    const BOOL numberMonitorsResult = GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &physicalMonitors_temp);

    if (numberMonitorsResult != TRUE) {
        printf("GetNumberOfPhysicalMonitorsFromHMONITOR failed!\r\n");
        printError();
        return false;
    }

    const DWORD physicalMonitors = physicalMonitors_temp;

    if (physicalMonitors == 0) {
        printf("No monitors? What??\r\n");
        return false;
    }

    _Field_size_(physicalMonitors) std::unique_ptr<PHYSICAL_MONITOR[]> monitors = std::make_unique<PHYSICAL_MONITOR[ ]>(physicalMonitors);
    static_assert(std::is_pod<PHYSICAL_MONITOR>::value, "can't memset!");
    memset(monitors.get(), 0, (sizeof(PHYSICAL_MONITOR) * physicalMonitors));
    const BOOL getPhysicalMonitors = GetPhysicalMonitorsFromHMONITOR(hMonitor, physicalMonitors, monitors.get());

    if (getPhysicalMonitors != TRUE) {
        printf("GetPhysicalMonitorsFromHMONITOR failed!\r\n");
        printError();
        return false;
    }

    auto monitorsGuard = SCOPEGUARD_INSTANCE([ &] {
        const BOOL destroyRes = DestroyPhysicalMonitors(physicalMonitors, monitors.get());

        if (destroyRes != TRUE)
        {
            printf("DestroyPhysicalMonitors failed!\r\n");
            printError();
            std::terminate();
        }
    });
    printf("Got %u physical monitors\r\n", physicalMonitors);

    for (DWORD i = 0; i < physicalMonitors; ++i) {
        printf("Monitor #%u description: %S\r\n", i, monitors[ i ].szPhysicalMonitorDescription);
    }

    const bool doesSupport = doesMonitorSupportBrightnessConfigurationViaDDC(monitors[ 0 ].hPhysicalMonitor);

    if (!doesSupport) {
        return false;
    }

    //LPSTR pszASCIICapabilitiesString = NULL;
    DWORD cchStringLength = NULL;
    //cchStringLength = NULL;
    //C6102 warning BUG? http://www.beta.microsoft.com/VisualStudio/feedback/details/812312/incorrect-code-analysis-warning-c6102
    const BOOL capabilityStrLen = GetCapabilitiesStringLength(monitors[ 0 ].hPhysicalMonitor, &cchStringLength);

    if (capabilityStrLen != TRUE) {
        printf("GetCapabilitiesStringLength failed!!\r\n");
        printError();
        return false;
    }

    printf("GetCapabilitiesStringLength Succeeded!!\r\n");
    std::unique_ptr<_Null_terminated_ char[ ]> capabilitiesString = std::make_unique<char[ ]>(cchStringLength);
    const BOOL capabilitiesRequestAndCapabilitiesReplySucces = CapabilitiesRequestAndCapabilitiesReply(hwnd, capabilitiesString.get(), cchStringLength);

    if (capabilitiesRequestAndCapabilitiesReplySucces != TRUE) {
        printf("CapabilitiesRequestAndCapabilitiesReply failed!\r\n");
        printError();
        return false;
    }

    printf("szCapabilitiesString: %s\r\n", capabilitiesString.get());
    const BOOL getBSuccess = GetMonitorBrightness(hMonitor, &pdwMinimumBrightness, &pdwCurrentBrightness, &pdwMaximumBrightness);

    if (getBSuccess != TRUE) {
        printf("GetMonitorBrightness failed!\r\n");
        return false;
    }

    printf("pdwMinimumBrightness: %u\r\n", pdwMinimumBrightness);
    printf("pdwCurrentBrightness: %u\r\n", pdwCurrentBrightness);
    printf("pdwMaximumBrightness: %u\r\n", pdwMaximumBrightness);
    return true;
}

bool ddcSetBrightness(_In_ const DWORD dwNewBrightness)
{
    //HMONITOR hMonitor = NULL;
    //DWORD pdwMinimumBrightness = 0;
    //DWORD pdwCurrentBrightness = 0;
    //DWORD pdwMaximumBrightness = 0;
    const HWND hwnd = FindWindowW(NULL, NULL);
    printf("Window handle: %p\r\n", hwnd);
    const HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
    printf("hMonitor: %p\r\n", hMonitor);
    DWORD physicalMonitors_temp = 0;
    const BOOL numberMonitorsResult = GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &physicalMonitors_temp);

    if (numberMonitorsResult != TRUE) {
        printf("GetNumberOfPhysicalMonitorsFromHMONITOR failed!\r\n");
        printError();
        return false;
    }

    const DWORD physicalMonitors = physicalMonitors_temp;

    if (physicalMonitors == 0) {
        printf("No monitors? What??\r\n");
        return false;
    }

    _Field_size_(physicalMonitors) std::unique_ptr<PHYSICAL_MONITOR[]> monitors = std::make_unique<PHYSICAL_MONITOR[ ]>(physicalMonitors);
    static_assert(std::is_pod<PHYSICAL_MONITOR>::value, "can't memset!");
    memset(monitors.get(), 0, (sizeof(PHYSICAL_MONITOR) * physicalMonitors));
    const BOOL getPhysicalMonitors = GetPhysicalMonitorsFromHMONITOR(hMonitor, physicalMonitors, monitors.get());

    if (getPhysicalMonitors != TRUE) {
        printf("GetPhysicalMonitorsFromHMONITOR failed!\r\n");
        printError();
        return false;
    }

    auto monitorsGuard = SCOPEGUARD_INSTANCE([ &] {
        const BOOL destroyRes = DestroyPhysicalMonitors(physicalMonitors, monitors.get());

        if (destroyRes != TRUE)
        {
            printf("DestroyPhysicalMonitors failed!\r\n");
            printError();
            std::terminate();
        }
    });
    printf("Got %u physical monitors\r\n", physicalMonitors);

    for (DWORD i = 0; i < physicalMonitors; ++i) {
        printf("Monitor #%u description: %S\r\n", i, monitors[ i ].szPhysicalMonitorDescription);
    }

    const bool doesSupport = doesMonitorSupportBrightnessConfigurationViaDDC(monitors[ 0 ].hPhysicalMonitor);

    if (!doesSupport) {
        return false;
    }

    const BOOL setBSuccess = SetMonitorBrightness(monitors[ 0 ].hPhysicalMonitor, dwNewBrightness);

    if (setBSuccess == TRUE) {
        printf("SetMonitorBrightness %u succeeded!\r\n", dwNewBrightness);
        return true;
    }

    printf("SetMonitorBrightness %u failed!\r\n", dwNewBrightness);
    return false;
}

_Success_(return != -1)
int GetBrightness()
{
    BSTR path      = SysAllocString(L"root\\wmi");

    if (path == NULL) {
        printf("failed to allocate path BSTR!\r\n");
        return -1;
    }

    auto pathguard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(path); path = NULL; });
    BSTR ClassPath = SysAllocString(L"WmiMonitorBrightness");

    if (ClassPath == NULL) {
        printf("failed to allocate ClassPath BSTR!\r\n");
        return -1;
    }

    auto classPathguard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(ClassPath); ClassPath = NULL; });
    BSTR bstrQuery = SysAllocString(L"Select * from WmiMonitorBrightness");

    if (bstrQuery == NULL) {
        printf("failed to allocate bstrQuery BSTR!\r\n");
        return -1;
    }

    auto queryGuard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(bstrQuery); bstrQuery = NULL; });
    // Initialize COM and connect up to CIMOM
    const HRESULT initResult = CoInitialize(0);

    if (FAILED(initResult)) {
        printf("Failed to initialize COM!\r\n");

        if (initResult == S_FALSE) {
            CoUninitialize();
            return -1;
        }

        return -1;
    }

    //Somehow we're leaking here?
    auto comGuard = SCOPEGUARD_INSTANCE([] { CoUninitialize(); });
    //  NOTE:
    //  When using asynchronous WMI API's remotely in an environment where the "Local System" account has no network identity (such as non-Kerberos domains), the authentication level of RPC_C_AUTHN_LEVEL_NONE is needed. However, lowering the authentication level to RPC_C_AUTHN_LEVEL_NONE makes your application less secure. It is wise to use semi-synchronous API's for accessing WMI data and events instead of the asynchronous ones.
    const HRESULT initSecurityResult = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_SECURE_REFS, NULL);

    //change EOAC_SECURE_REFS to EOAC_NONE if you change dwAuthnLevel to RPC_C_AUTHN_LEVEL_NONE
    if (FAILED(initSecurityResult)) {
        printf("Failed to initialize security!\r\n");
        return -1;
    }

    PVOID pLocator_temp = NULL;
    const HRESULT createInstanceResult = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, &pLocator_temp);

    if (FAILED(createInstanceResult)) {
        printf("CoCreateInstance failed!\r\n");
        return -1;
    }

    IWbemLocator* pLocator = static_cast<IWbemLocator*>(pLocator_temp);
    auto locatorGuard = SCOPEGUARD_INSTANCE([ &] { pLocator->Release(); pLocator = NULL; });
    IWbemServices* pNamespace = NULL;
    const HRESULT connectServerResult = pLocator->ConnectServer(path, NULL, NULL, NULL, 0, NULL, NULL, &pNamespace);

    if (connectServerResult != WBEM_S_NO_ERROR) {
        printf("ConnectServer failed!\r\n");
        return -1;
    }

    auto namespaceGuard = SCOPEGUARD_INSTANCE([ &] { pNamespace->Release(); pNamespace = NULL; });
    const HRESULT setProxyBlanketResult = CoSetProxyBlanket(pNamespace, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    if (setProxyBlanketResult != WBEM_S_NO_ERROR) {
        printf("CoSetProxyBlanket failed!\r\n");
        return -1;
    }

    //Following comment kept for hilarity.
    //Yes, I'd really never seen that operator before :)
    /*
    See http://stackoverflow.com/a/1238643/625687 for info on usage of arrow ('->')
    ExecQuery:
        WQL:                            Query Language
        bstrQuery:                      Query to Execute
        WBEM_FLAG_RETURN_IMMEDIATELY:   Make a semi-synchronous call
        NULL:                           Context
        pEnum:                          Enumeration Interface
    */
    IEnumWbemClassObject* pEnum = NULL;
    //On [ExecQuery] failure, you can obtain any available information from the COM function GetErrorInfo.
    const HRESULT execQueryResult = pNamespace->ExecQuery(
                                        _bstr_t (L"WQL"),
                                        bstrQuery,
                                        WBEM_FLAG_RETURN_IMMEDIATELY,
                                        NULL,
                                        &pEnum
                                    );

    if (execQueryResult != WBEM_S_NO_ERROR) {
        printf("ExecQuery failed!!\r\n");
        printCOMerror();
        return -1;
    }

    auto enumGuard = SCOPEGUARD_INSTANCE([ &] { pEnum->Release(); pEnum = NULL; });
    //hr = WBEM_S_NO_ERROR;
    //https://code.google.com/p/stexbar/source/browse/trunk/Misc/AAClr/src/Utils.cpp?spec=svn926&r=926
    //while ( WBEM_S_NO_ERROR == hr ) {
    ULONG ulReturned = 0;
    IWbemClassObject* pObj = NULL;
    /*
    Next:
        Get the Next Object from the collection
            1:              Number of objects requested
    */
    //On [Next] failure, you can obtain any available information from the COM function GetErrorInfo.
    //TODO: leaking here somehow?
    const HRESULT nextResult = pEnum->Next(WBEM_INFINITE, 1, &pObj, &ulReturned);

    if (nextResult != WBEM_S_NO_ERROR) {
        printf("pEnum->Next failed!! (done?)\r\n");
        printCOMerror();
        return -1;
    }

    if (ulReturned != 1) {
        printf("pEnum->Next returned more IWbemClassObjects than expected! Actual#: %u\r\n", ulReturned);
        return -1;
    }

    auto pObjGuard = SCOPEGUARD_INSTANCE([ &] { pObj->Release(); pObj = NULL; });
    VARIANT var1;
    const HRESULT wmiBrightnessResult = pObj->Get(L"CurrentBrightness", 0, &var1, NULL, NULL);

    if (FAILED(wmiBrightnessResult)) {
        printf("pObj->Get( CurrentBrightness ) failed!\r\n");
        return -1;
    }

    const int ret = V_UI1(&var1);
    const HRESULT clearResult = VariantClear(&var1);

    if (FAILED(clearResult)) {
        printf("VariantClear failed!!\r\n");
        return -1;
    }

    printf("'ret' is valid: %i\r\n", ret);
    return ret;//?
    //}
    //return -1;
}

_Success_(return)
bool SetBrightness(const int val)
{
    printf("Attempting to set brightness: %i via WMI\r\n", val);
    //HRESULT hr = S_OK;
    BSTR path       = SysAllocString(L"root\\wmi");

    if (path == NULL) {
        printf("failed to allocate path BSTR!\r\n");
        return false;
    }

    auto pathguard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(path); path = NULL; });
    BSTR ClassPath  = SysAllocString(L"WmiMonitorBrightnessMethods");

    if (ClassPath == NULL) {
        printf("failed to allocate ClassPath BSTR!\r\n");
        return false;
    }

    auto classGuard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(ClassPath); ClassPath = NULL; });
    BSTR MethodName = SysAllocString(L"WmiSetBrightness");

    if (MethodName == NULL) {
        printf("failed to allocate MethodName BSTR!\r\n");
        return false;
    }

    auto methodGuard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(MethodName); MethodName = NULL; });
    BSTR ArgName0   = SysAllocString(L"Timeout");

    if (ArgName0 == NULL) {
        printf("failed to allocate ArgName0 BSTR!\r\n");
        return false;
    }

    auto argZeroGuard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(ArgName0); ArgName0 = NULL; });
    BSTR ArgName1   = SysAllocString(L"Brightness");

    if (ArgName1 == NULL) {
        printf("failed to allocate ArgName1 BSTR!\r\n");
        return false;
    }

    auto argOneGuard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(ArgName1); ArgName1 = NULL; });
    BSTR bstrQuery  = SysAllocString(L"Select * from WmiMonitorBrightnessMethods");

    if (bstrQuery == NULL) {
        printf("failed to allocate bstrQuery BSTR!\r\n");
        return false;
    }

    auto bstrGuard = SCOPEGUARD_INSTANCE([ &] { SysFreeString(bstrQuery); bstrQuery = NULL; });
    // Initialize COM and connect up to CIMOM
    //static_assert( FAILED( S_FALSE ), "" );
    const HRESULT initResult = CoInitialize(0);

    if (FAILED(initResult)) {
        printf("Failed to initialize COM!\r\n");
        return false;
    }

    //Somehow we're leaking here?
    auto comGuard = SCOPEGUARD_INSTANCE([ &] { CoUninitialize(); });
    //  When using asynchronous WMI API's remotely in an environment where the "Local System" account has no network identity (such as non-Kerberos domains), the authentication level of RPC_C_AUTHN_LEVEL_NONE is needed. However, lowering the authentication level to RPC_C_AUTHN_LEVEL_NONE makes your application less secure. It is wise to use semi-synchronous API's for accessing WMI data and events instead of the asynchronous ones.
    const HRESULT initSecurity = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_SECURE_REFS, NULL);

    if (FAILED(initSecurity)) {
        printf("Failed to init security!\r\n");
        return false;
        //goto cleanup;
    }

    PVOID pLocator_temp = NULL;
    //bool bRet = true;
    //change EOAC_SECURE_REFS to EOAC_NONE if you change dwAuthnLevel to RPC_C_AUTHN_LEVEL_NONE
    const HRESULT createInstanceResult = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, &pLocator_temp);

    if (FAILED(createInstanceResult)) {
        printf("\tSomething went wrong in CoCreateInstance!\r\n");
        return false;
    }

    IWbemLocator*         pLocator   = static_cast<IWbemLocator*>(pLocator_temp);
    auto locatorGuard = SCOPEGUARD_INSTANCE([ &] { pLocator->Release(); pLocator = NULL; });
    IWbemServices*        pNamespace = NULL;
    const HRESULT connectServerResult = pLocator->ConnectServer(path, NULL, NULL, NULL, 0, NULL, NULL, &pNamespace);

    if (connectServerResult != WBEM_S_NO_ERROR) {
        printf("\tSomething went wrong in pLocator->ConnectServer!\r\n");
        return false;
    }

    auto namespaceGuard = SCOPEGUARD_INSTANCE([ &] { pNamespace->Release(); pNamespace = NULL; });
    const HRESULT setBlanketResult = CoSetProxyBlanket(pNamespace, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    if (FAILED(setBlanketResult)) {
        printf("\tSomething went wrong in CoSetProxyBlanket!\r\n ");
        return false;
    }

    //Following comment kept for hilarity.
    //Yes, I'd really never seen that operator before :)
    /*
    See http://stackoverflow.com/a/1238643/625687 for info on usage of arrow ('->')
    ExecQuery:
    WQL:                            Query Language
    bstrQuery:                      Query to Execute
    WBEM_FLAG_RETURN_IMMEDIATELY:   Make a semi-synchronous call
    NULL:                           Context
    pEnum:                          Enumeration Interface
    */
    IEnumWbemClassObject* pEnum      = NULL;
    //On [ExecQuery] failure, you can obtain any available information from the COM function GetErrorInfo.
    const HRESULT execQueryResult = pNamespace->ExecQuery(
                                        _bstr_t(L"WQL"),
                                        bstrQuery,
                                        WBEM_FLAG_RETURN_IMMEDIATELY,
                                        NULL,
                                        &pEnum
                                    );

    if (FAILED(execQueryResult)) {
        printf("\tSomething went wrong in pNamespace->ExecQuery!\r\n");
        printCOMerror();
        return false;
    }

    auto enumGuard = SCOPEGUARD_INSTANCE([ &] { pEnum->Release(); pEnum = NULL; });
    HRESULT hr = WBEM_S_NO_ERROR;
    static_assert(SUCCEEDED(WBEM_S_NO_ERROR), "");
    //while ( SUCCEEDED( hr ) ) {
    ULONG ulReturned;
    IWbemClassObject *pObj;
    //Get the Next Object from the collection
    //On [Next] failure, you can obtain any available information from the COM function GetErrorInfo.
    const HRESULT enumNext = pEnum->Next(WBEM_INFINITE, 1, &pObj, &ulReturned);

    if (FAILED(enumNext)) {
        printf("\tSomething went wrong in pEnum->Next!\r\n");
        printCOMerror();
        return false;
    }

    auto pObjGuard = SCOPEGUARD_INSTANCE([&] { pObj->Release(); pObj = NULL; });
    IWbemClassObject*     pClass     = NULL;
    // Get the class object
    const HRESULT getObjResult = pNamespace->GetObject(ClassPath, 0, NULL, &pClass, NULL);

    if (FAILED(getObjResult)) {
        printf("\tSomething went wrong in pNamespace->GetObject!\r\n");
        return false;
    }

    auto pClassGuard = SCOPEGUARD_INSTANCE([ &] { pClass->Release(); pClass = NULL; });
    IWbemClassObject*     pInClass   = NULL;
    // Get the input argument and set the property
    const HRESULT getMethod = pClass->GetMethod(MethodName, 0, &pInClass, NULL);

    if (FAILED(getMethod)) {
        printf("\tSomething went wrong in pClass->GetMethod!\r\n");
        return false;
    }

    auto inClassGuard = SCOPEGUARD_INSTANCE([ &] { pInClass->Release(); pInClass = NULL; });
    IWbemClassObject*     pInInst    = NULL;
    const HRESULT spawnRes = pInClass->SpawnInstance(0, &pInInst);

    if (FAILED(spawnRes)) {
        printf("\tSomething went wrong in pInClass->SpawnInstance!\r\n");
        return false;
    }

    auto instGuard = SCOPEGUARD_INSTANCE([ &] { pInInst->Release(); pInInst = NULL; });
    VARIANT var1;
    VariantInit(&var1);
    V_VT(&var1) = VT_BSTR;
    V_BSTR(&var1) = SysAllocString(L"0");
    const HRESULT putRes = pInInst->Put(ArgName0, 0, &var1, CIM_UINT32);   //CIM_UINT64
    VariantClear(&var1);

    if (FAILED(putRes)) {
        printf("pInInst->Put failed!!\r\n");
        return false;
    }

    VARIANT var;
    VariantInit(&var);
    V_VT(&var) = VT_BSTR;
    const rsize_t bufSize = 20;
    wchar_t buf[ bufSize ] = { 0 };
    //swprintf_s( buf, bufSize, L"%ld", val );
    const HRESULT fmt_res = StringCchPrintfW(buf, bufSize, L"%ld", val);

    if (FAILED(fmt_res)) {
        VariantClear(&var);
        printf("Failed to format number %ld into buffer!\r\n", val);
        return false;
    }

    V_BSTR(&var) = SysAllocString(buf);
    const HRESULT putArgOne = pInInst->Put(ArgName1, 0, &var, CIM_UINT8);
    VariantClear(&var);

    if (FAILED(putArgOne)) {
        printf("\tSomething went wrong in pInInst->Put!\r\n");
        return false;
    }

    // Call the method
    VARIANT pathVariable;
    VariantInit(&pathVariable);
    const HRESULT getPath = pObj->Get(_bstr_t(L"__PATH"), 0, &pathVariable, NULL, NULL);

    if (FAILED(getPath)) {
        VariantClear(&pathVariable);
        printf("\tSomething went wrong in pObj->Get!\r\n");
        return false;
    }

    const HRESULT execMethod = pNamespace->ExecMethod(pathVariable.bstrVal, MethodName, 0, NULL, pInInst, NULL, NULL);
    VariantClear(&pathVariable);

    if (FAILED(execMethod)) {
        printf("\tSomething went wrong in pNamespace->ExecMethod!\r\n");
        return false;
    }

    return true;
    //}
    //return false;
}

void main()
{
    int getBrightInt = NULL;
    HMODULE hModule = ::GetModuleHandleW(NULL);

    if (hModule == NULL) {
        printf("Fatal Error: GetModuleHandle failed\r\n");
        std::terminate();
    }

    int ass = GetBrightness();
    printf("Got brightness %i via WMI\r\n", ass);
    Sleep(100);
    bool getBrightSucess = ddcGetBrightness();

    if (!getBrightSucess) {
        printf("Failed to get brightness via DDC/CI!\r\n");
        Sleep(100);
    } else if (getBrightSucess) {
        printf("Got brightness: %i via DDC/CI\r\n", getBrightInt);
        Sleep(100);
    }

    DWORD newBrightness = 55;

    if (!ddcSetBrightness(newBrightness)) {
        printf("Failed to set brightness %u via DDC/CI!\r\n\r\n", newBrightness);
        Sleep(100);
    } else {
        printf("Successfully set brightness %u via DDC/CI!\r\n", newBrightness);
        Sleep(100);

        if (!ddcSetBrightness(DWORD(ass))) {
            printf("\tFailed to reset brightness to %i via DDC/CI!\r\n\r\n", ass);
        }
    }

    SetBrightness(0);
    Sleep(100);
    SetBrightness(100);
    Sleep(100);
    SetBrightness(0);
    Sleep(100);
    SetBrightness(ass);
}

