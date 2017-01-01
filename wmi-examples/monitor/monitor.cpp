#include "monitor.h"
#include <tchar.h>
#include <iostream>
#include <set>
#include <algorithm>
#include <objbase.h>
#include <wbemidl.h>
#include <comdef.h>
#include <tracetool/tracetool.h>


//WCHAR *ReadRegStr(HKEY keySub, const WCHAR *keyName, const WCHAR *valName) {
//  WCHAR *val = NULL;
//  REGSAM access = KEY_READ;
//  HKEY hKey;
//TryAgainWOW64:
//  LONG res = RegOpenKeyEx(keySub, keyName, 0, access, &hKey);
//  if (ERROR_SUCCESS == res) {
//      DWORD valLen;
//      res = RegQueryValueEx(hKey, valName, NULL, NULL, NULL, &valLen);
//      if (ERROR_SUCCESS == res) {
//          val = AllocArray<WCHAR>(valLen / sizeof(WCHAR) + 1);
//          res = RegQueryValueEx(hKey, valName, NULL, NULL, (LPBYTE)val, &valLen);
//          if (ERROR_SUCCESS != res)
//              str::ReplacePtr(&val, nullptr);
//      }
//      RegCloseKey(hKey);
//  }
//  if (ERROR_FILE_NOT_FOUND == res && HKEY_LOCAL_MACHINE == keySub && KEY_READ == access) {
//      // try the (non-)64-bit key as well, as HKLM\Software is not shared between 32-bit and
//      // 64-bit applications per http://msdn.microsoft.com/en-us/library/aa384253(v=vs.85).aspx
//#ifdef _WIN64
//      access = KEY_READ | KEY_WOW64_32KEY;
//#else
//      access = KEY_READ | KEY_WOW64_64KEY;
//#endif
//      goto TryAgainWOW64;
//  }
//  return val;
//}


void cactus::GetCurrentDesktopRes(int* width, int* height)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    (*width) = (int)desktop.right;
    (*height) = (int)desktop.bottom;
}

BOOL cactus::getCurrentUserDPI(DWORD* value)
{
    HKEY key;
    BOOL retval = FALSE;

    do {
        LSTATUS regStatus = RegOpenKeyExW(HKEY_CURRENT_USER, L"Control Panel\\Desktop", 0, (IsWow64() ? KEY_WOW64_64KEY : 0) | KEY_ALL_ACCESS, &key);

        if (regStatus != ERROR_SUCCESS) {
            RegCloseKey(key);
            break;
        }

        DWORD dwType = REG_DWORD;
        DWORD nResult(0);
        DWORD BufferSize = sizeof(DWORD);
        regStatus = RegQueryValueExW(key, L"LogPixels", 0, NULL, reinterpret_cast<LPBYTE>(&nResult), &BufferSize);

        if (regStatus != ERROR_SUCCESS) {
            RegCloseKey(key);
            break;
        }

        *value = nResult;
        retval = TRUE;
    } while (0);

    return retval;
}

BOOL cactus::setCurrentUserDPI(DWORD value)
{
    HKEY key;
    BOOL retval = FALSE;

    do {
        LSTATUS regStatus = RegOpenKeyExW(HKEY_CURRENT_USER, L"Control Panel\\Desktop", 0, (IsWow64() ? KEY_WOW64_64KEY : 0) | KEY_ALL_ACCESS, &key);

        if (regStatus != ERROR_SUCCESS) {
            RegCloseKey(key);
            break;
        }

        DWORD dwType = REG_DWORD;
        DWORD BufferSize = sizeof(DWORD);
        // Win8DpiScaling  win8
        // LogPixels
        // DesktopDPIOverride
        regStatus = RegSetValueExW(key, L"LogPixels", 0, dwType, reinterpret_cast<LPBYTE>(&value), BufferSize);

        if (regStatus != ERROR_SUCCESS) {
            RegCloseKey(key);
            break;
        }

        retval = TRUE;
    } while (0);

    return retval;
}

BOOL cactus::IsWow64()
{
    typedef BOOL (WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL bIsWow64 = FALSE;
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

    if (NULL != fnIsWow64Process) {
        fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
    }

    return bIsWow64;
}

DWORD cactus::ScalingProcentToRegNum(int num)
{
    //See http://technet.microsoft.com/en-us/library/dn528846.aspx
    //under "System-wide scale factor in Windows 8 scaling mode"
    //For more info
    return (DWORD)(0.96f * num);
}

//void cactus::UpdateDisplay( unsigned int width, unsigned int height, int zoom )
//{
//  SetResolution(width, height);
//  HKEY key;
//  OpenRegKey(&key);
//  SetLogPixels(key, ScalingProcentToRegNum(zoom));
//  RegCloseKey(key);
//}

void cactus::GetBestDevMode(unsigned int width, unsigned int height, DEVMODE* devOut)
{
    devOut->dmSize = sizeof(devOut);

    for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, devOut) != 0; iModeNum++) {
        std::cout << "Mode #" << iModeNum << " = " << devOut->dmPelsWidth << "x" << devOut->dmPelsHeight << std::endl;

        if (devOut->dmPelsWidth == width && devOut->dmPelsHeight == height) {
            printf("MODE FOUND");
            break;
        }
    }
}

int cactus::SetResolution(unsigned int width, unsigned int height)
{
    //Find compatible mode
    DEVMODE devmode = { 0 };
    GetBestDevMode(width, height, &devmode);
    //Reapply settings to screen
    int ChangeDisplayResult = ChangeDisplaySettingsW(&devmode, CDS_UPDATEREGISTRY);

    //Check if successfull
    if (ChangeDisplayResult != DISP_CHANGE_SUCCESSFUL) {
        //DISP_CHANGE
        printf("\nDun goffed #2: %d\n", ChangeDisplayResult);
        system("pause");
        return 0;
    }

    //Returns 1 if successfull
    return 1;
}

bool cactus::setDisplayMode(int width, int height, int bpp, DEVMODE* oldMode)
{
    // 获取旧模式配置
    EnumDisplaySettings(NULL, ENUM_REGISTRY_SETTINGS, oldMode);
    DEVMODE displayMode;
    memset(&displayMode, 0 , sizeof(displayMode));
    displayMode.dmSize          = sizeof(displayMode);
    displayMode.dmPelsWidth     = width;
    displayMode.dmPelsHeight    = height;
    displayMode.dmBitsPerPel    = (bpp == -1 ? oldMode->dmBitsPerPel : bpp);
    displayMode.dmFields        =  DM_BITSPERPEL | DM_PELSWIDTH  | DM_PELSHEIGHT;

    if (ChangeDisplaySettings(&displayMode , CDS_FULLSCREEN)
        != DISP_CHANGE_SUCCESSFUL) {
        TRACE_SEND_A("SScreenmode : Displaymode changing failed!\n");
        return false;
    }

    // 永久设置
    ChangeDisplaySettings(&displayMode, CDS_UPDATEREGISTRY);
    TRACE_SEND_A("SScreenmode : Displaymode changed successfully!\n");
    return true;
}

void cactus::Reset96DPI()
{
    SetProcessDPIAware();
    DEVMODE devMode;
    memset(&devMode, 0, sizeof(DEVMODE));
    devMode.dmSize = sizeof(devMode);
    ::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
    DEVMODE mode;
    memset(&mode, 0, sizeof(DEVMODE));
    mode.dmSize = sizeof(DEVMODE);
    memcpy(&mode, &devMode, sizeof(DEVMODE));
    mode.dmLogPixels = 96;
    mode.dmFields = DM_LOGPIXELS;

    if (ChangeDisplaySettings(&mode, 0) != DISP_CHANGE_SUCCESSFUL) {
        ChangeDisplaySettings(&devMode, 0);
    }
}

void cactus::ListDisplayDevices()
{
    int index = 0;
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    while (EnumDisplayDevices(NULL, index++, &dd, 0)) {
        if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
            TRACE_SEND_W(L"* ");

        TRACE_SEND_FORMAT(L"%s, %s\n", dd.DeviceName, dd.DeviceString);
    }
}

DISPLAY_DEVICE cactus::GetPrimaryDevice()
{
    int index = 0;
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    while (EnumDisplayDevices(NULL, index++, &dd, 0)) {
        if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) return dd;
    }

    return dd;
}

void cactus::ListDisplaySettings(int index)
{
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(DISPLAY_DEVICE);

    if (!EnumDisplayDevices(NULL, index, &dd, 0)) {
        TRACE_SEND_FORMAT(L"EnumDisplayDevices failed:%d\n", GetLastError());
        return;
    }

    DISPLAY_DEVICE monitor;
    monitor.cb = sizeof(DISPLAY_DEVICE);

    if (!EnumDisplayDevices(dd.DeviceName, index, &monitor, 0)) {
        TRACE_SEND_FORMAT(L"EnumDisplayDevices failed:%d\n", GetLastError());
        return;
    }

    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);

    if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm)) {
        TRACE_SEND_FORMAT(L"EnumDisplaySettings failed:%d\n", GetLastError());
        return;
    }

    TRACE_SEND_FORMAT(L"Device name: %s\n", dd.DeviceName);
    TRACE_SEND_FORMAT(L"Monitor name: %s\n", monitor.DeviceID);
    TRACE_SEND_FORMAT(L"Refresh rate, in hertz: %d\n", dm.dmDisplayFrequency);
    TRACE_SEND_FORMAT(L"Color depth: %d\n", dm.dmBitsPerPel);
    TRACE_SEND_FORMAT(L"Screen resolution, in pixels: %d x %d\n",
                      dm.dmPelsWidth, dm.dmPelsHeight);
}

bool cactus::SetDisplayResolution(long PelsWidth, long PelsHeight)
{
    DISPLAY_DEVICE dd = GetPrimaryDevice();
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);

    if (!EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm)) {
        TRACE_SEND_FORMAT(L"EnumDisplaySettings failed:%d\n", GetLastError());
        return FALSE;
    }

    dm.dmPelsWidth = PelsWidth;
    dm.dmPelsHeight = PelsHeight;
    dm.dmFields = (DM_PELSWIDTH | DM_PELSHEIGHT);

    if (ChangeDisplaySettings(&dm, CDS_TEST) != DISP_CHANGE_SUCCESSFUL) {
        TRACE_SEND_FORMAT(L"\nIllegal graphics mode: %d\n", GetLastError());
        return FALSE;
    }

    return (ChangeDisplaySettings(&dm, 0) == DISP_CHANGE_SUCCESSFUL);
}

void cactus::SetDisplayDefaults()
{
    //changes the settings of the default display device
    //to the default mode
    ChangeDisplaySettings(NULL, 0);
}

void cactus::getDisplayModes(std::vector<DisplayMode>& modes)
{
    std::set<std::pair<int, int> > unique_resolutions;
    std::vector<std::pair<int, int> > sorted_modes;
    DEVMODE display_setting;
    display_setting.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &display_setting);
    int i = 0;
    int bpp;
    bpp = display_setting.dmBitsPerPel;
    sorted_modes.push_back(std::make_pair(display_setting.dmPelsWidth, display_setting.dmPelsHeight));

    while (EnumDisplaySettings(NULL, i, &display_setting)) {
        if (display_setting.dmBitsPerPel == bpp) {
            unique_resolutions.insert(std::make_pair(display_setting.dmPelsWidth, display_setting.dmPelsHeight));
        }

        ++i;
    }

    for (std::set<std::pair<int, int> >::iterator it = unique_resolutions.begin(); it != unique_resolutions.end(); ++it) {
        sorted_modes.push_back(*it);
    }

    std::sort(sorted_modes.begin() + 1, sorted_modes.end());

    //mode_count = sorted_modes.size();
    //modes = new DisplayMode[mode_count];

    for (unsigned int i = 0; i < sorted_modes.size(); ++i) {
        DisplayMode mode;
        mode.width = sorted_modes[i].first;
        mode.height = sorted_modes[i].second;
        modes.push_back(mode);
        //modes[i].width = sorted_modes[i].first;
        //modes[i].height = sorted_modes[i].second;
    }
}


int cactus::GetBrightness()
{
    int ret = -1;
    IWbemLocator *pLocator = NULL;
    IWbemServices *pNamespace = 0;
    IEnumWbemClassObject *pEnum = NULL;
    HRESULT hr = S_OK;
    BSTR path = SysAllocString(L"root\\wmi");
    BSTR ClassPath = SysAllocString(L"WmiMonitorBrightness");
    BSTR bstrQuery = SysAllocString(L"Select * from WmiMonitorBrightness");

    if (!path || !ClassPath) {
        goto cleanup;
    }

    // Initialize COM and connect up to CIMOM
    hr = CoInitialize(0);

    if (FAILED(hr)) {
        goto cleanup;
    }

    //  NOTE:
    //  When using asynchronous WMI API's remotely in an environment where the"Local System" account
    //  has no network identity (such as non-Kerberos domains), the authentication level of
    //  RPC_C_AUTHN_LEVEL_NONE is needed. However, lowering the authentication level to
    //  RPC_C_AUTHN_LEVEL_NONE makes your application less secure. It is wise to
    // use semi-synchronous API's for accessing WMI data and events instead of the asynchronous ones.
    hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
                              RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
                              RPC_C_IMP_LEVEL_IMPERSONATE,
                              NULL,
                              EOAC_SECURE_REFS, //change to EOAC_NONE if you change dwAuthnLevel to RPC_C_AUTHN_LEVEL_NONE
                              NULL);
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                          IID_IWbemLocator, (LPVOID *) &pLocator);

    if (FAILED(hr)) {
        goto cleanup;
    }

    hr = pLocator->ConnectServer(path, NULL, NULL, NULL, 0, NULL, NULL, &pNamespace);

    if (hr != WBEM_S_NO_ERROR) {
        goto cleanup;
    }

    hr = CoSetProxyBlanket(pNamespace,
                           RPC_C_AUTHN_WINNT,
                           RPC_C_AUTHZ_NONE,
                           NULL,
                           RPC_C_AUTHN_LEVEL_PKT,
                           RPC_C_IMP_LEVEL_IMPERSONATE,
                           NULL,
                           EOAC_NONE
                          );

    if (hr != WBEM_S_NO_ERROR) {
        goto cleanup;
    }

    hr = pNamespace->ExecQuery(_bstr_t(L"WQL"), //Query Language
                               bstrQuery, //Query to Execute
                               WBEM_FLAG_RETURN_IMMEDIATELY, //Make a semi-synchronous call
                               NULL, //Context
                               &pEnum //Enumeration Interface
                              );

    if (hr != WBEM_S_NO_ERROR) {
        goto cleanup;
    }

    hr = WBEM_S_NO_ERROR;

    while (WBEM_S_NO_ERROR == hr) {
        ULONG ulReturned;
        IWbemClassObject *pObj;
        //Get the Next Object from the collection
        hr = pEnum->Next(WBEM_INFINITE, //Timeout
                         1, //No of objects requested
                         &pObj, //Returned Object
                         &ulReturned //No of object returned
                        );

        if (hr != WBEM_S_NO_ERROR) {
            goto cleanup;
        }

        VARIANT var1;
        hr = pObj->Get(_bstr_t(L"CurrentBrightness"), 0, &var1, NULL, NULL);
        ret = V_UI1(&var1);
        VariantClear(&var1);

        if (hr != WBEM_S_NO_ERROR) {
            goto cleanup;
        }
    }

    // Free up resources
cleanup:
    SysFreeString(path);
    SysFreeString(ClassPath);
    SysFreeString(bstrQuery);

    if (pLocator)
        pLocator->Release();

    if (pNamespace)
        pNamespace->Release();

    CoUninitialize();
    return ret;
}

bool cactus::SetBrightness(int val)
{
    bool bRet = true;
    IWbemLocator *pLocator = NULL;
    IWbemServices *pNamespace = 0;
    IWbemClassObject * pClass = NULL;
    IWbemClassObject * pInClass = NULL;
    IWbemClassObject * pInInst = NULL;
    IEnumWbemClassObject *pEnum = NULL;
    HRESULT hr = S_OK;
    BSTR path = SysAllocString(L"root\\wmi");
    BSTR ClassPath = SysAllocString(L"WmiMonitorBrightnessMethods");
    BSTR MethodName = SysAllocString(L"WmiSetBrightness");
    BSTR ArgName0 = SysAllocString(L"Timeout");
    BSTR ArgName1 = SysAllocString(L"Brightness");
    BSTR bstrQuery = SysAllocString(L"Select * from WmiMonitorBrightnessMethods");

    if (!path || ! ClassPath || !MethodName || ! ArgName0) {
        bRet = false;
        goto cleanup;
    }

    // Initialize COM and connect up to CIMOM
    hr = CoInitialize(0);

    if (FAILED(hr)) {
        bRet = false;
        goto cleanup;
    }

    //  NOTE:
    //  When using asynchronous WMI API's remotely in an environment where the"Local System" account
    //  has no network identity (such as non-Kerberos domains), the authentication level of
    //  RPC_C_AUTHN_LEVEL_NONE is needed. However, lowering the authentication level to
    //  RPC_C_AUTHN_LEVEL_NONE makes your application less secure. It is wise to
    // use semi-synchronous API's for accessing WMI data and events instead of the asynchronous ones.
    hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
                              RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
                              RPC_C_IMP_LEVEL_IMPERSONATE,
                              NULL,
                              EOAC_SECURE_REFS, //change to EOAC_NONE if you change dwAuthnLevel to RPC_C_AUTHN_LEVEL_NONE
                              NULL);
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                          IID_IWbemLocator, (LPVOID *) &pLocator);

    if (FAILED(hr)) {
        bRet = false;
        goto cleanup;
    }

    hr = pLocator->ConnectServer(path, NULL, NULL, NULL, 0, NULL, NULL, &pNamespace);

    if (hr != WBEM_S_NO_ERROR) {
        bRet = false;
        goto cleanup;
    }

    hr = CoSetProxyBlanket(pNamespace,
                           RPC_C_AUTHN_WINNT,
                           RPC_C_AUTHZ_NONE,
                           NULL,
                           RPC_C_AUTHN_LEVEL_PKT,
                           RPC_C_IMP_LEVEL_IMPERSONATE,
                           NULL,
                           EOAC_NONE
                          );

    if (hr != WBEM_S_NO_ERROR) {
        bRet = false;
        goto cleanup;
    }

    hr = pNamespace->ExecQuery(_bstr_t(L"WQL"), //Query Language
                               bstrQuery, //Query to Execute
                               WBEM_FLAG_RETURN_IMMEDIATELY, //Make a semi-synchronous call
                               NULL, //Context
                               &pEnum //Enumeration Interface
                              );

    if (hr != WBEM_S_NO_ERROR) {
        bRet = false;
        goto cleanup;
    }

    hr = WBEM_S_NO_ERROR;

    while (WBEM_S_NO_ERROR == hr) {
        ULONG ulReturned;
        IWbemClassObject *pObj;
        //Get the Next Object from the collection
        hr = pEnum->Next(WBEM_INFINITE, //Timeout
                         1, //No of objects requested
                         &pObj, //Returned Object
                         &ulReturned //No of object returned
                        );

        if (hr != WBEM_S_NO_ERROR) {
            bRet = false;
            goto cleanup;
        }

        // Get the class object
        hr = pNamespace->GetObject(ClassPath, 0, NULL, &pClass, NULL);

        if (hr != WBEM_S_NO_ERROR) {
            bRet = false;
            goto cleanup;
        }

        // Get the input argument and set the property
        hr = pClass->GetMethod(MethodName, 0, &pInClass, NULL);

        if (hr != WBEM_S_NO_ERROR) {
            bRet = false;
            goto cleanup;
        }

        hr = pInClass->SpawnInstance(0, &pInInst);

        if (hr != WBEM_S_NO_ERROR) {
            bRet = false;
            goto cleanup;
        }

        VARIANT var1;
        VariantInit(&var1);
        V_VT(&var1) = VT_BSTR;
        V_BSTR(&var1) = SysAllocString(L"0");
        hr = pInInst->Put(ArgName0, 0, &var1, CIM_UINT32); //CIM_UINT64
        //var1.vt = VT_I4;
        //var1.ullVal = 0;
        //   hr = pInInst->Put(ArgName0, 0, &var1, 0);
        VariantClear(&var1);

        if (hr != WBEM_S_NO_ERROR) {
            bRet = false;
            goto cleanup;
        }

        VARIANT var;
        VariantInit(&var);
        V_VT(&var) = VT_BSTR;
        WCHAR buf[10] = {0};
        _stprintf_s(buf, _countof(buf), L"%d", val);
        V_BSTR(&var) = SysAllocString(buf);
        hr = pInInst->Put(ArgName1, 0, &var, CIM_UINT8);
        //var.vt=VT_UI1;
        //var.uiVal = 100;
        //hr = pInInst->Put(ArgName1, 0, &var, 0);
        VariantClear(&var);

        if (hr != WBEM_S_NO_ERROR) {
            bRet = false;
            goto cleanup;
        }

        // Call the method
        VARIANT pathVariable;
        VariantInit(&pathVariable);
        hr = pObj->Get(_bstr_t(L"__PATH"), 0, &pathVariable, NULL, NULL);

        if (hr != WBEM_S_NO_ERROR)
            goto cleanup;

        hr = pNamespace->ExecMethod(pathVariable.bstrVal, MethodName, 0, NULL, pInInst, NULL, NULL);
        VariantClear(&pathVariable);

        if (hr != WBEM_S_NO_ERROR) {
            bRet = false;
            goto cleanup;
        }
    }

    // Free up resources
cleanup:
    SysFreeString(path);
    SysFreeString(ClassPath);
    SysFreeString(MethodName);
    SysFreeString(ArgName0);
    SysFreeString(ArgName1);
    SysFreeString(bstrQuery);

    if (pClass)
        pClass->Release();

    if (pInInst)
        pInInst->Release();

    if (pInClass)
        pInClass->Release();

    if (pLocator)
        pLocator->Release();

    if (pNamespace)
        pNamespace->Release();

    CoUninitialize();
    return bRet;
}

//void cactus::ReadAndPrintDPI()
//{
//  //Get monitor handle
//  HMONITOR const monitor = MonitorFromWindow(GetActiveWindow(), MONITOR_DEFAULTTONEAREST);
//
//  unsigned int dpiX = 0, dpiY = 0;
//
//  //Read dpi and put into dpiX & dpiY
//  HRESULT result = GetDpiForMonitor(monitor, MDT_ANGULAR_DPI, &dpiX, &dpiY);
//
//  //Print to screen
//  TRACE_SEND_FORMAT("X: %d, Y: %d\n", dpiX, dpiY);
//}

// Populate the MonitorList with the informations from every active monitor


namespace
{

using namespace  cactus;
// Callback function for the EnumDisplayMonitors function
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    MONITORINFOEX monitorInfoEx;
    monitorInfoEx.cbSize = sizeof(MONITORINFOEX);
    BOOL res = GetMonitorInfo(hMonitor, &monitorInfoEx);

    if (res) {
        std::wstring name(monitorInfoEx.szDevice);
        bool isPrimary = monitorInfoEx.dwFlags & MONITORINFOF_PRIMARY;
        RECT rect = monitorInfoEx.rcMonitor;
        int left = rect.left,
            top = rect.top,
            right = rect.right,
            bottom = rect.bottom;
        Monitor mon(hMonitor, name, isPrimary, left, top, right, bottom);
        // Dereferencing the pointer to the pointer to the monitor list
        std::vector<Monitor>* monList = *((std::vector<Monitor>**)dwData);
        monList->push_back(mon);

        if (isPrimary) {
            monList->at(0) = mon;
        }

        //LOG(info) << ((isPrimary) ? "PRIMARY" : "") <<"Monitor \""<< name.c_str() <<"\"found, coordonates: [(" << left << "," << top << "), (" << right << "," << bottom << ")].";
    } else {
        TRACE_SEND_A("Unable to fill in monitor info.");
    }

    return TRUE;
}


void getMonitors(std::vector<Monitor>* monList)
{
    // Adding a placeholder for the primary monitor
    monList->push_back(Monitor());
    BOOL res = EnumDisplayMonitors(
                   NULL,
                   NULL,
                   &(MonitorEnumProc),
                   (LPARAM)&monList
               );
}

}

#define PRIMARY L"Primary"


cactus::Monitor::Monitor()
{
}

cactus::Monitor::Monitor(HMONITOR hMonitor, const std::wstring name, bool isPrimary, int left, int top, int right, int bottom)
{
    this->hMonitor = hMonitor;
    setName(name);
    this->isPrimary = isPrimary;
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
    width = right - left;
    height = bottom - top;
}

cactus::Monitor::~Monitor()
{
}

void cactus::Monitor::setName(std::wstring newName)
{
    name = newName;
}

std::wstring cactus::Monitor::getName()
{
    return name;
}


bool cactus::Monitor::GetCurrentSettings(DisplaySettings& settings)
{
    MONITORINFOEX info;
    info.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(this->hMonitor, &info);
    DEVMODE mode;
    mode.dmSize = sizeof(DEVMODE);
    mode.dmDriverExtra = 0;

    if (EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &mode)) {
        settings.Resolution.cx = mode.dmPelsWidth;
        settings.Resolution.cy = mode.dmPelsHeight;
        settings.RefreshRate = mode.dmDisplayFrequency;
        settings.BitsPerPixel = mode.dmBitsPerPel;
        return true;
    }

    return false;
}

void cactus::Monitor::GetSupportedSettings(std::vector<DisplaySettings>& settings)
{
    MONITORINFOEX info;
    info.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(this->hMonitor, &info);
    DEVMODE mode;
    mode.dmSize = sizeof(DEVMODE);
    mode.dmDriverExtra = 0;

    for (int i = 0; EnumDisplaySettings(info.szDevice, i, &mode); i++) {
        DisplaySettings setting;
        setting.Resolution.cx = mode.dmPelsWidth;
        setting.Resolution.cy = mode.dmPelsHeight;
        setting.RefreshRate = mode.dmDisplayFrequency;
        setting.BitsPerPixel = mode.dmBitsPerPel;
        settings.push_back(setting);
    }
}

bool cactus::Monitor::ChangeSettings(const DisplaySettings& settings)
{
    MONITORINFOEX info;
    info.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(this->hMonitor, &info);
    DEVMODE mode;
    mode.dmSize = sizeof(DEVMODE);
    mode.dmDriverExtra = 0;
    mode.dmPelsWidth = settings.Resolution.cx;
    mode.dmPelsHeight = settings.Resolution.cy;
    mode.dmDisplayFrequency = settings.RefreshRate;
    mode.dmBitsPerPel = settings.BitsPerPixel;
    mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
    return ChangeDisplaySettingsEx(info.szDevice, &mode, NULL, CDS_FULLSCREEN, NULL) == DISP_CHANGE_SUCCESSFUL;
}

cactus::MonitorList::MonitorList()
{
    monList = new std::vector <Monitor>();
}

cactus::MonitorList::~MonitorList()
{
    delete monList;
}

void cactus::MonitorList::update()
{
    monList->clear();
    getMonitors(monList);
}

Monitor* cactus::MonitorList::getMonitorByName(const std::wstring name)
{
    if (name == PRIMARY) {
        return getPrimaryMonitor();
    }

    std::vector<Monitor>::iterator it = monList->begin();

    for (; it != monList->end(); it++) {
        if (it->getName() == name) {
            return &(*it);
        }
    }

    //LOG(error) << "Monitor \"" << name << "\" not found, defaulting to primary.";
    return getPrimaryMonitor();
}

Monitor* cactus::MonitorList::getPrimaryMonitor()
{
    return &monList->at(0);
}

