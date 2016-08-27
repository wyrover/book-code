#include "restore_point.h"
#include "bstr.h"
#include <tracetool/tracetool.h>


//template<class T, class F> T format(F input, int width = 0, int prec = -1)
//{
//  std::wstringstream A;
//  T res;
//
//  if (prec != -1) {
//      A << std::fixed << std::setprecision(prec);
//  }
//
//  A << std::setw(width) << std::setfill(L'0') << input;
//  A >> res;
//  return res;
//}
//
//std::wstring GetSizeAsString(std::wstring sSize)
//{
//  float fSize = _wtof(sSize.c_str());
//  // construct string to represent size of volume
//  std::wstring sUnits = L" B";
//
//  if (fSize > 1024) {
//      fSize /= 1024;
//      sUnits = L" KB";
//
//      if (fSize > 1024) {
//          fSize /= 1024;
//          sUnits = L" MB";
//
//          if (fSize > 1024) {
//              fSize /= 1024;
//              sUnits = L" GB";
//
//              if (fSize > 1024) {
//                  fSize /= 1024;
//                  sUnits = L" TB";
//
//                  if (fSize > 1024) {
//                      fSize /= 1024;
//                      sUnits = L" PB";
//                  }
//              }
//          }
//      }
//  }
//
//  return format<std::wstring, float>(fSize, -1, 2) + sUnits;
//}

// Win32_Volume.DriveType
#define DRIVE_TYPE_UNKNOWN   0
#define DRIVE_TYPE_NO_ROOT   1
#define DRIVE_TYPE_REMOVABLE 2
#define DRIVE_TYPE_LOCAL     3
#define DRIVE_TYPE_NETWORK   4
#define DRIVE_TYPE_COMPACT   5
#define DRIVE_TYPE_RAM       6

std::wstring GetDriveType(INT iType)
{
    std::wstring s;

    switch (iType) {
    case DRIVE_TYPE_UNKNOWN  :
        s = L"Unknown";
        break;

    case DRIVE_TYPE_NO_ROOT  :
        s = L"No Root Directory";
        break;

    case DRIVE_TYPE_REMOVABLE :
        s = L"Removable Disk";
        break;

    case DRIVE_TYPE_LOCAL :
        s = L"Local Disk";
        break;

    case DRIVE_TYPE_NETWORK :
        s = L"Network Drive";
        break;

    case DRIVE_TYPE_COMPACT :
        s = L"Compact Disk";
        break;

    case DRIVE_TYPE_RAM :
        s = L"RAM Disk";
        break;

    default:
        s = L"Unknown";
        break;
    }

    return s;
}



namespace cactus
{

CSnapshotManager::CSnapshotManager() : result_(-1), loc_(NULL), svc_(NULL), pEnum_(NULL), pls_(NULL)
{
}

CSnapshotManager::~CSnapshotManager()
{
    if (pls_ != NULL) {
        pls_->Release();
    }

    if (svc_ != NULL) {
        svc_->Release();
    }

    if (pEnum_ != NULL) {
        pEnum_->Release();
    }

    if (loc_ != NULL) {
        loc_->Release();
    }

    //Closes the COM library
    CoUninitialize();
}

BOOL CSnapshotManager::init()
{
    //用CoInitializeEx函数初始化COM接口
    result_ = CoInitializeEx(0, COINIT_MULTITHREADED);

    if (FAILED(result_)) {
        return FALSE;
    }

    //用CoInitializeSecurity函数注册并设置进程的默认的安全值
    result_ = CoInitializeSecurity(
                  NULL,
                  -1,
                  NULL,
                  NULL,
                  RPC_C_AUTHN_LEVEL_DEFAULT,
                  RPC_C_IMP_LEVEL_IMPERSONATE,
                  NULL,
                  EOAC_NONE,
                  NULL
              );

    if (FAILED(result_)) {
        return FALSE;
    }

    //用CoCreateInstance函数来初始化IWbemLocator接口
    result_ = CoCreateInstance(
                  CLSID_WbemLocator,
                  0,
                  CLSCTX_INPROC_SERVER,
                  IID_IWbemLocator,
                  (LPVOID *)&loc_
              );

    if (FAILED(result_)) {
        return FALSE;
    }

    /*USES_CONVERSION*/;
    result_ = loc_->ConnectServer(
                  bstr_t("ROOT\\CIMV2"),
                  NULL,
                  NULL,
                  0,
                  NULL,
                  0,
                  0,
                  &svc_
              );

    if (FAILED(result_)) {
        return FALSE;
    }

    //用CoSetProxyBlanket函数设置IWbemServices代理的安全值
    result_ = CoSetProxyBlanket(
                  svc_,
                  RPC_C_AUTHN_WINNT,
                  RPC_C_AUTHZ_NONE,
                  NULL,
                  RPC_C_AUTHN_LEVEL_CALL,
                  RPC_C_IMP_LEVEL_IMPERSONATE,
                  NULL,
                  EOAC_NONE
              );

    if (FAILED(result_)) {
        return FALSE;
    }

    return TRUE;
}

BOOL CSnapshotManager::listSnapshotList()
{
    try {
        if ((!svc_))
            return FALSE;

        HRESULT hres;
        IEnumWbemClassObject* pEnumerator = NULL;
        TRACE_SEND_A("ExecQuery begin...");
        hres = svc_->ExecQuery(
                   bstr_t("WQL"),
                   bstr_t("SELECT * FROM Win32_ShadowCopy"),
                   //WBEM_FLAG_RETURN_IMMEDIATELY,
                   WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                   NULL,
                   &pEnumerator);

        if (FAILED(hres)) {
            return FALSE;
        }

        TRACE_SEND_A("ExecQuery end...");
        IWbemClassObject *pclsObj;
        ULONG uReturn = 0;

        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (uReturn == 0) {
                break;
            }

            TRACE_SEND_A("0000000000000");
            VARIANT vtProp;
            VariantInit(&vtProp);
            hr = pclsObj->Get(L"ID", 0, &vtProp, 0, 0);

            if (vtProp.vt == VT_BSTR) {
                TRACE_SEND_W(ConvertBSTRToStdWstring(vtProp.bstrVal).c_str());
            }

            VariantClear(&vtProp);
            /*hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr)) {

                if (vtProp.vt == VT_BSTR) {
                    TRACE_SEND_W(ConvertBSTRToStdWstring(vtProp.bstrVal).c_str());
                }
            }
            VariantClear(&vtProp);*/
            pclsObj->Release();
        }

        pEnumerator->Release();
        return TRUE;
    } catch (...) {
        result_ = WBEM_E_FAILED;
        return FALSE;
    }

    return TRUE;
}

BOOL CSnapshotManager::listVolumes()
{
    BOOL retval = FALSE;
    IEnumWbemClassObject* pEnumerator = NULL;
    std::wstring query = L"SELECT * FROM Win32_Volume";
    HRESULT hr = svc_->ExecQuery(bstr_t(L"WQL"), bstr_t(query.c_str()),
                                 WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

    do {
        if (FAILED(hr))
            break;

        IWbemClassObject *pclsObj = NULL;
        ULONG uReturn = 0;
        wprintf(L"\n  %-15s  %-30s  %-20s  %-10s  %-10s",
                std::wstring(15, L'-').c_str(),
                std::wstring(30, L'-').c_str(),
                std::wstring(20, L'-').c_str(),
                std::wstring(10, L'-').c_str(),
                std::wstring(10, L'-').c_str());
        wprintf(L"\n  %-15s  %-30s  %-20s  %-10s  %-10s",
                L"Drive Letter", L"Label", L"Drive Type", L"Capacity", L"Free Space");
        wprintf(L"\n  %-15s  %-30s  %-20s  %-8s  %-10s",
                std::wstring(15, L'-').c_str(),
                std::wstring(30, L'-').c_str(),
                std::wstring(20, L'-').c_str(),
                std::wstring(10, L'-').c_str(),
                std::wstring(10, L'-').c_str());

        while (pEnumerator) {
            hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            // if failure or no objects returned, break
            if (FAILED(hr) || uReturn == 0) {
                break;
            }

            // get drive letter
            std::wstring drive     = GetVarString(pclsObj, L"DriveLetter");
            // drive type
            std::wstring sType     = GetDriveType(GetVarInt(pclsObj, L"DriveType"));
            // drive label
            std::wstring label     = GetVarString(pclsObj, L"Label");
            // capacity
            std::wstring capacity  = GetVarString(pclsObj, L"Capacity");
            // free space
            std::wstring freespace = GetVarString(pclsObj, L"FreeSpace");
            // format size as something we can read
            //capacity  = GetSizeAsString(capacity);
            //freespace = GetSizeAsString(freespace);
            //wprintf(L"\n  %-15s  %-30s  %-20s  %10s  %10s",
            //        drive.c_str(), label.c_str(), sType.c_str(), capacity.c_str(), freespace.c_str());
            pclsObj->Release();
            pclsObj = NULL;
        }
    } while (0);

    return retval;
}

BOOL CSnapshotManager::createSnapshot(std::wstring volume)
{
    TRACE_SEND_A("Entering CSnapshotManager::createSnapshot()");
    std::wstring shadowID, shadowObj;
    DWORD dwError = -1;
    IWbemClassObject* shadowObject = NULL;
    HRESULT hr = svc_->GetObject(L"Win32_ShadowCopy", 0, NULL, &shadowObject, NULL);

    if (SUCCEEDED(hr)) {
        IWbemClassObject* paramsObj = NULL;
        IWbemClassObject* resultsObj = NULL;
        hr = shadowObject->GetMethod(L"Create", 0, &paramsObj, &resultsObj);

        if (SUCCEEDED(hr)) {
            IWbemClassObject* params = NULL;
            hr = paramsObj->SpawnInstance(0, &params);

            // set the volume we want to backup
            if (SUCCEEDED(hr)) {
                hr = SetVarString(params, L"Volume", volume);

                // set the context...unsure if this required
                if (SUCCEEDED(hr)) {
                    hr = SetVarString(params, L"Context", L"ClientAccessible");

                    // get the path of our object, mightn't be required either
                    if (SUCCEEDED(hr)) {
                        // get our path
                        std::wstring path = GetVarString(shadowObject, L"__PATH");

                        // execute the Create method with our above parameters
                        if (SUCCEEDED(hr)) {
                            hr = svc_->ExecMethod(BSTR(path.c_str()),  L"Create", 0,
                                                  NULL, params, &resultsObj, NULL);
                            // if create method succeeded, obtain the return code first
                            // and if that's good, we have valid shadow copy made
                            dwError = GetVarInt(resultsObj, L"ReturnValue");

                            // if we're good, grab the shadow id + device object
                            if (dwError == ERROR_SUCCESS) {
                                shadowID  = GetVarString(resultsObj, L"ShadowID");
                                // device object requires finding object first..
                                //GetDeviceObject();
                            }
                        }
                    }
                }

                params->Release();
            }

            paramsObj->Release();
            resultsObj->Release();
        }

        shadowObject->Release();
    }

    TRACE_SEND_A("Leaving Entering CSnapshotManager::createSnapshot()");
    return dwError == ERROR_SUCCESS;
}

}