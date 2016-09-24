// PPspt.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


// 这是导出函数的一个示例。
extern "C"
PPSPT_API int fnPPspt(void)
{
    return 42;
}

extern "C"
PPSPT_API BOOL fnInitApp(
    IN OUT PVOID* pShareMemory
)
{
    HANDLE  hFileMap = NULL;
    HANDLE  hFileSys = NULL;
    HANDLE  hEventProMon = NULL;
    BOOL    bRet = FALSE;
    DWORD   dwRet = 0;
    int     flagControl = 0;
    bRet = fnInitReg(&flagControl);

    if (FALSE == bRet) {
        DbgPrintA(("[fnInitApp] fnInitReg Failre!\n"));
        CloseHandle(hFileSys);
        return FALSE;
    }

    hFileSys = CreateFileW(
                   L"\\\\.\\PProtect",
                   GENERIC_READ | GENERIC_WRITE,
                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                   NULL,
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_NORMAL,
                   NULL
               );

    if (INVALID_HANDLE_VALUE == hFileSys) {
        DbgPrintA(("[fnInitApp] CreateFile Failure!"));
        return FALSE;
    }

    bRet = DeviceIoControl(
               hFileSys,
               IOCTL_SendFlagControl,
               (PVOID)&flagControl,
               4,
               NULL,
               0,
               &dwRet,
               NULL
           );

    if (!bRet) {
        DbgPrintA(("[fnInitApp] DeviceIoControl IOCTL_SetFlagControl Failure!\n"));
        CloseHandle(hFileSys);
        return FALSE;
    }

    hEventProMon = CreateEvent(NULL, TRUE, FALSE, L"Global\\EventProMon");
    bRet = DeviceIoControl(
               hFileSys,
               IOCTL_SendEventProMon,
               (PVOID)hEventProMon,
               4,
               NULL,
               0,
               &dwRet,
               NULL
           );

    if (!bRet) {
        DbgPrintA(("[fnInitApp] DeviceIoControl Failure!\n"));
        CloseHandle(hFileSys);
        return FALSE;
    }

    bRet = DeviceIoControl(
               hFileSys,
               IOCTL_GetShareMemory,
               NULL,
               0,
               pShareMemory,
               1024,
               &dwRet,
               NULL
           );

    if (!bRet) {
        DbgPrintA(("[fnInitApp] DeviceIoControl Failure!\n"));
        CloseHandle(hFileSys);
        return FALSE;
    }

    CloseHandle(hFileSys);
    CloseHandle(
        CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE)InitFnSigVerif,
            pShareMemory,
            0,
            NULL
        )
    );
    return TRUE;
}


extern "C"
PPSPT_API BOOL fnInitReg(OUT int* flagControl)
{
    HKEY    hKey, hKeyProMon;
    DWORD   dwDis;
    long    lRet;
    DWORD   dwcbData = 4;
    DWORD   dwDefaultFlag = 0;
    DWORD   dwType = 0;
    lRet = RegCreateKeyExW(
               HKEY_LOCAL_MACHINE,
               L"SOFTWARE\\PProtect",
               NULL,
               NULL,
               REG_OPTION_NON_VOLATILE,
               KEY_ALL_ACCESS,
               NULL,
               &hKey,
               &dwDis
           );

    if (lRet != ERROR_SUCCESS) {
        DbgPrintA(("[fnInitReg] RegCreateKeyExW Failure!\n"));
        return FALSE;
    }

    lRet = RegCreateKeyExW(
               HKEY_LOCAL_MACHINE,
               L"SOFTWARE\\PProtect\\ProMon",
               NULL,
               NULL,
               REG_OPTION_NON_VOLATILE,
               KEY_ALL_ACCESS,
               NULL,
               &hKeyProMon,
               &dwDis
           );

    if (lRet != ERROR_SUCCESS) {
        DbgPrintA(("[fnInitReg] RegCreateKeyExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    lRet = RegQueryValueExW(
               hKey,
               L"FlagControl",
               NULL,
               &dwType,
               (LPBYTE)flagControl,
               &dwcbData
           );

    if (lRet != ERROR_SUCCESS) {
        SetBit(dwDefaultFlag, fPProtect);
        SetBit(dwDefaultFlag, fAppRun);
        *flagControl = dwDefaultFlag;
    } else {
        SetBit(*flagControl, fAppRun);
    }

    lRet = RegSetValueExW(
               hKey,
               L"FlagControl",
               0,
               REG_DWORD,
               (BYTE*)flagControl,
               4
           );

    if (lRet != ERROR_SUCCESS) {
        DbgPrintA(("[fnInitReg] RegSetValueExW Failure!\n"));
        RegCloseKey(hKey);
        RegCloseKey(hKeyProMon);
        return FALSE;
    }

    RegCloseKey(hKey);
    RegCloseKey(hKeyProMon);
    return TRUE;
}




extern "C"
PPSPT_API BOOL fnGetProMonInfo(WCHAR* detailInfo)
{
    pNotifyBlock p = NULL;
    PVOID pShareMem = NULL;
    HANDLE hFileSys;
    BOOL bRet = FALSE;
    DWORD dwRet = 0;
    hFileSys = CreateFileW(
                   L"\\\\.\\PProtect",
                   GENERIC_READ | GENERIC_WRITE,
                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                   NULL,
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_NORMAL,
                   NULL
               );

    if (INVALID_HANDLE_VALUE == hFileSys) {
        DbgPrintA(("[fnGetProMonInfo] CreateFile Failure!\n"));
        return FALSE;
    }

    bRet = DeviceIoControl(
               hFileSys,
               IOCTL_GetShareMemPointer,
               NULL,
               0,
               &pShareMem,
               1024,
               &dwRet,
               NULL
           );

    if (!bRet) {
        DbgPrintA(("[fnGetProMonInfo] DeviceIoControl IOCTL_GetShareMemPointer Failure!\n"));
        CloseHandle(hFileSys);
        return FALSE;
    }

    CloseHandle(hFileSys);
    p = (pNotifyBlock)pShareMem;

    if (NULL == p) {
        return FALSE;
    }

    if (p->u.ProMon.SHAChanged) {
        wsprintfW(detailInfo, L"父进程: %ws\r\n\r\n子进程(将要被创建的进程): %ws\r\n\r\n该子进程的映像文件已经被修改，可能为恶意程序所为，建议禁止运行此程序",
                  p->CurPro, p->u.ProMon.R_Pro);
    } else {
        wsprintfW(detailInfo, L"父进程: %ws\r\n\r\n子进程(将要被创建的进程): %ws",
                  p->CurPro, p->u.ProMon.R_Pro);
    }

    DbgPrintW((L"[fnGetProMonInfo] detailInfo: size:%d \nconnect:%ws\n", wcslen(detailInfo), detailInfo));
    return TRUE;
}
extern "C"
PPSPT_API BOOL fnUpdateFlag(int bit, BOOL bDirect)
{
    int     flagControl = 0;
    BOOL    bRet = FALSE;
    long    lRet = 0;
    HKEY    hKey;
    DWORD   dwDis;
    DWORD   dwType = 0, dwcbData = 4;
    HANDLE  hFileSys;
    DWORD   dwRet = 0;
    lRet = RegCreateKeyExW(
               HKEY_LOCAL_MACHINE,
               L"SOFTWARE\\PProtect",
               NULL,
               NULL,
               REG_OPTION_NON_VOLATILE,
               KEY_ALL_ACCESS,
               NULL,
               &hKey,
               &dwDis
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnUpdateFlag] RegCreateKeyExW Failure!\n"));
        return FALSE;
    }

    lRet = RegQueryValueExW(
               hKey,
               L"FlagControl",
               NULL,
               &dwType,
               (LPBYTE)&flagControl,
               &dwcbData
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnUpdateFlag] RegQueryValueExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    (bDirect == TRUE) ? SetBit(flagControl, bit) : ClearBit(flagControl, bit);
    lRet = RegSetValueExW(hKey, L"FlagControl", 0, REG_DWORD, (BYTE*)&flagControl, 4);

    if (lRet != ERROR_SUCCESS) {
        DbgPrintA(("[fnUpdateFlag] RegSetValueExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    RegCloseKey(hKey);
    hFileSys = CreateFileW(
                   L"\\\\.\\PProtect",
                   GENERIC_READ | GENERIC_WRITE,
                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                   NULL,
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_NORMAL,
                   NULL
               );

    if (INVALID_HANDLE_VALUE == hFileSys) {
        DbgPrintA(("[fnUpdateFlag] CreateFile Failure!"));
        return FALSE;
    }

    bRet = DeviceIoControl(
               hFileSys,
               IOCTL_SendFlagControl,
               (PVOID)&flagControl,
               4,
               NULL,
               0,
               &dwRet,
               NULL
           );

    if (!bRet) {
        DbgPrintA(("[fnUpdateFlag] DeviceIoControl IOCTL_SendFlagControl Failure!\n"));
        CloseHandle(hFileSys);
        return FALSE;
    }

    CloseHandle(hFileSys);
    return TRUE;
}

extern "C"
PPSPT_API BOOL fnHandleProMonChoose(BOOL AllowRun, BOOL TipChecked)
{
    HANDLE hFileSys;
    BOOL bRet = FALSE;
    DWORD dwRet = 0;
    PVOID pShareMem = NULL;
    pNotifyBlock p = NULL;
    char cTemp[21] = {0};
    hFileSys = CreateFileW(
                   L"\\\\.\\PProtect",
                   GENERIC_READ | GENERIC_WRITE,
                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                   NULL,
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_NORMAL,
                   NULL
               );

    if (INVALID_HANDLE_VALUE == hFileSys) {
        DbgPrintA(("[fnHandleProMonChoose] CreateFile Failure!\n"));
        return FALSE;
    }

    bRet = DeviceIoControl(
               hFileSys,
               IOCTL_GetShareMemPointer,
               NULL,
               0,
               (PVOID)&pShareMem,
               4,
               &dwRet,
               NULL
           );

    if (!bRet) {
        DbgPrintA(("[fnHandleProMonChoose] DeviceIoControl IOCTL_GetShareMemPointer Failure!\n"));
        CloseHandle(hFileSys);
        return FALSE;
    }

    p = (pNotifyBlock)pShareMem;

    if (NULL == p) {
        CloseHandle(hFileSys);
        return FALSE;
    }

    if (TRUE == AllowRun) {
        if (TipChecked) {
            DbgPrintA(("[fnHandleProMonChoose] 添加到白名单!\n"));
            Add2ProMonRules(TRUE, p->u.ProMon.R_Pro);
        }

        bRet = DeviceIoControl(
                   hFileSys,
                   IOCTL_Choose_T,
                   NULL,
                   0,
                   NULL,
                   0,
                   &dwRet,
                   NULL);

        if (FALSE == bRet) {
            CloseHandle(hFileSys);
            return FALSE;
        }
    } else {
        if (TipChecked) {
            DbgPrintA(("[fnHandleProMonChoose] 添加到黑名单!\n"));
            Add2ProMonRules(FALSE, p->u.ProMon.R_Pro);
        }

        bRet = DeviceIoControl(
                   hFileSys,
                   IOCTL_Choose_F,
                   NULL,
                   0,
                   NULL,
                   0,
                   &dwRet,
                   NULL);

        if (FALSE == bRet) {
            CloseHandle(hFileSys);
            return FALSE;
        }
    }

    CloseHandle(hFileSys);
    return TRUE;
}

extern "C"
PPSPT_API BOOL fnStartChoose()
{
    HANDLE hProcessSnap;
    PROCESSENTRY32W pe32;
    WCHAR* szName = NULL;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32FirstW(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return FALSE ;
    }

    do {
        szName = wcsupr(pe32.szExeFile);
        DbgPrintW((L"[fnStartChoose] szName: %ws\n", szName));

        if (wcsstr(szName, L"PRESENTATIONFONTCACHE")) {
            CloseHandle(hProcessSnap);
            return TRUE;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return FALSE ;
}

extern "C"
PPSPT_API BOOL fnSigVerif()
{
    HANDLE  hFile ;
    BOOL    bRet = FALSE;
    HCATADMIN hCatAdmin;
    DWORD   cbHash = 0;
    PVOID   pbHash = NULL;
    PVOID   pPolicyCallbackData = NULL;
    WCHAR   pwszSourceFile[MAX_PATH] = {0};
    pNotifyBlock    p = NULL;
    HANDLE  hFileSys;
    PVOID   pShareMem = NULL;
    DWORD   dwRet = 0;

    do {
        hFileSys = CreateFileW(
                       L"\\\\.\\PProtect",
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL
                   );

        if (INVALID_HANDLE_VALUE == hFileSys) {
            DbgPrintA(("[fnSigVerif] CreateFile Failure!"));
            bRet = FALSE;
            break;
        }

        bRet = DeviceIoControl(
                   hFileSys,
                   IOCTL_GetShareMemPointer,
                   NULL,
                   0,
                   (PVOID)&pShareMem,
                   4,
                   &dwRet,
                   NULL
               );

        if (!bRet) {
            DbgPrintA(("[fnSigVerif] DeviceIoControl IOCTL_GetShareMemPointer Failure!\n"));
            CloseHandle(hFileSys);
            bRet = FALSE;
            break;
        }

        CloseHandle(hFileSys);
        p = (pNotifyBlock)pShareMem;

        if (NULL == p) {
            DbgPrintA(("[fnSigVerif] p is NULL\n"));
            bRet = FALSE;
            break;
        }

        wcscpy(pwszSourceFile, p->u.ProMon.R_Pro);
        hFile = CreateFileW(
                    pwszSourceFile,
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);

        if (INVALID_HANDLE_VALUE == hFile) {
            DbgPrintA(("[fnSigVerif] CreateFileW(%ws) Failure,err:%d\n",
                       pwszSourceFile,
                       GetLastError()
                      ));
            bRet = FALSE;
            break;
        }

        cbHash = 0x64;
        pbHash = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x64);

        if (NULL == pbHash) {
            DbgPrintA(("[fnSigVerif] HeapAlloc Failure!\n"));
            CloseHandle(hFile);
            bRet = FALSE;
            break;
        }

        bRet = CryptCATAdminCalcHashFromFileHandle(
                   hFile,
                   &cbHash,
                   (BYTE*)pbHash,
                   0
               );

        if (FALSE == bRet) {
            DbgPrintA(("[fnSigVerif] CryptCATAdminCalcHashFromFileHandle Failure!\n"));
            CloseHandle(hFile);
            bRet = FALSE;
            break;
        }

        CloseHandle(hFile);
        bRet = CryptCATAdminAcquireContext(&hCatAdmin, NULL, 0);

        if (FALSE == bRet) {
            DbgPrintA(("[fnSigVerif] CryptCATAdminAcquireContext Failure!\n"));
            bRet = FALSE;
            break;
        }

        HCATINFO hCatInfo, hPreCatInfo = NULL;
        hCatInfo = CryptCATAdminEnumCatalogFromHash(
                       hCatAdmin,
                       (BYTE*)pbHash,
                       cbHash,
                       0,
                       &hPreCatInfo
                   );

        if (NULL == hCatInfo) {
            DbgPrintA(("[fnSigVerif] CryptCATAdminEnumCatalogFromHash Failure!\n"));
            CryptCATAdminReleaseContext(hCatAdmin, 0);
            bRet = FALSE;
            break;
        }

        CATALOG_INFO psCatInfo;
        psCatInfo.cbStruct = sizeof(CATALOG_INFO);
        bRet = CryptCATCatalogInfoFromContext(
                   hCatInfo,
                   &psCatInfo,
                   0
               );

        if (FALSE == bRet) {
            DbgPrintA(("[fnSigVerif] CryptCATCatalogInfoFromContext Failure!\n"));
            CryptCATAdminReleaseContext(hCatAdmin, 0);
            bRet = FALSE;
            break;
        }

        CryptCATAdminReleaseContext(hCatAdmin, 0);
        GUID ActionID = DRIVER_ACTION_VERIFY;
        WINTRUST_DATA WVTData = {0};
        pPolicyCallbackData = HeapAlloc(
                                  GetProcessHeap(),
                                  HEAP_ZERO_MEMORY,
                                  0x440
                              );

        if (NULL == pPolicyCallbackData) {
            DbgPrintA(("[fnSigVerif] pPolicyCallbackData is NULL\n"));
            bRet = FALSE;
            break;
        }

        *(ULONG*)pPolicyCallbackData = 0x440;
        WINTRUST_CATALOG_INFO CatLog = {0};
        WCHAR pcwszMemberTag[MAX_PATH] = {0};
        WCHAR* p1 = (WCHAR*)wcsrchr(pwszSourceFile, '\\');

        if (NULL == p1) {
            DbgPrintA(("[fnSigVerif] wcsrchr(%ws,\\) Failure!\n", pwszSourceFile));
            p1 = (WCHAR*)pwszSourceFile;
        } else {
            ++p;
        }

        wcscpy(pcwszMemberTag, p1);
        CatLog.cbStruct = 0x24;
        CatLog.dwCatalogVersion = 0;
        CatLog.pcwszCatalogFilePath = psCatInfo.wszCatalogFile;
        CatLog.pcwszMemberTag = pcwszMemberTag;
        CatLog.pcwszMemberFilePath = NULL;
        CatLog.hMemberFile = NULL;
        ULONG uTemp = (ULONG)&CatLog.hMemberFile;
        *(ULONG*)(uTemp + 4) = (ULONG)pbHash;
        *(ULONG*)(uTemp + 8) = cbHash;
        WVTData.cbStruct = sizeof(WINTRUST_DATA);
        WVTData.pPolicyCallbackData = pPolicyCallbackData;
        WVTData.pSIPClientData = NULL;
        WVTData.dwUIChoice = WTD_UI_NONE;
        WVTData.fdwRevocationChecks = 0;
        WVTData.dwUnionChoice = WTD_CHOICE_CATALOG;
        WVTData.pCatalog = &CatLog;
        WVTData.dwStateAction = WTD_STATEACTION_AUTO_CACHE;
        WVTData.hWVTStateData = 0;
        WVTData.pwszURLReference = NULL;
        WVTData.dwProvFlags = 0;
        WVTData.dwUIContext = 0;
        LONG lRet = WinVerifyTrust(NULL, &ActionID, &WVTData);

        if (ERROR_SUCCESS == lRet) {
            DbgPrintW((L"[fnSigVerif] WinVerifyTrust(%ws) OK!\n", pwszSourceFile));
            bRet = TRUE;
            break;
        } else {
            DbgPrintA(("[fnSigVerif] WinVerifyTrust(%ws) Failure!\n", pwszSourceFile));
            bRet = FALSE;
            break;
        }
    } while (FALSE);

    if (NULL != pbHash) {
        HeapFree(GetProcessHeap(), 0, pbHash);
    }

    if (NULL != pPolicyCallbackData) {
        HeapFree(GetProcessHeap(), 0, pPolicyCallbackData);
    }

    return bRet;
}


extern "C"
PPSPT_API int fnGetRegProItemCount()
{
    long lRet = 0;
    HKEY hKey;
    DWORD dwSubKeys = 0;
    lRet = RegOpenKeyExW(
               HKEY_LOCAL_MACHINE,
               L"SOFTWARE\\PProtect\\ProMon",
               0,
               KEY_ALL_ACCESS,
               &hKey
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItemCount] RegOpenKeyExW Failure!\n"));
        return 0;
    }

    lRet = RegQueryInfoKey(
               hKey,
               NULL,
               NULL,
               NULL,
               &dwSubKeys,
               NULL,
               NULL,
               NULL,
               NULL,
               NULL,
               NULL,
               NULL);

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItemCount] RegQueryInfoKey Failure!\n"));
        RegCloseKey(hKey);
        return 0;
    }

    RegCloseKey(hKey);
    return dwSubKeys;
}
extern "C"
PPSPT_API BOOL fnGetRegProItem(int index, ProMonItem* item)
{
    long lRet = 0;
    HKEY hKey, hKeyQuery;
    lRet = RegOpenKeyExW(
               HKEY_LOCAL_MACHINE,
               L"SOFTWARE\\PProtect\\ProMon",
               0,
               KEY_ALL_ACCESS,
               &hKey
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItem] RegOpenKeyExW Failure!\n"));
        return FALSE;
    }

    WCHAR wKeyName[MAX_PATH] = {0};
    DWORD dwNameLen = MAX_PATH;
    lRet = RegEnumKeyExW(
               hKey,
               index,
               wKeyName,
               &dwNameLen,
               NULL,
               NULL,
               NULL,
               NULL
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItem] RegEnumKeyExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    ZeroMemory(item->wProcessName, 100);
    wcscpy(item->wProcessName, wKeyName);
    lRet = RegOpenKeyExW(
               hKey,
               wKeyName,
               0,
               KEY_ALL_ACCESS,
               &hKeyQuery
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItem] RegOpenKeyExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    DWORD   dwType = 0;
    DWORD   oper = 0, cbData = 0;
    cbData = 4;
    lRet = RegQueryValueExW(
               hKeyQuery,
               L"oper",
               NULL,
               &dwType,
               (BYTE*)&oper,
               &cbData
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItem] RegQueryValueExW Failure!\n"));
        RegCloseKey(hKey);
        RegCloseKey(hKeyQuery);
        return FALSE;
    }

    item->oper = oper;
    WCHAR wLastTime[MAX_PATH] = {0};
    cbData = MAX_PATH * 2;
    lRet = RegQueryValueExW(
               hKeyQuery,
               L"lastTime",
               NULL,
               &dwType,
               (BYTE*)wLastTime,
               &cbData
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItem] RegQueryValueExW(lastTime) Failure!\n"));
        RegCloseKey(hKey);
        RegCloseKey(hKeyQuery);
        return FALSE;
    }

    ZeroMemory(item->wLastTime, 40);
    wcscpy(item->wLastTime, wLastTime);
    BYTE sha1[21] = {0};
    char cSHA1[21] = {0};
    WCHAR wSHA1[21] = {0};
    cbData = 21;
    lRet = RegQueryValueExW(
               hKeyQuery,
               L"sha1",
               NULL,
               &dwType,
               sha1,
               &cbData);

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItem] RegQueryValueExW(sha1) Failure!\n"));
        RegCloseKey(hKey);
        RegCloseKey(hKeyQuery);
        return FALSE;
    }

    for (int i = 0; i < 20; ++i) {
        sprintf(&cSHA1[i], "%02x", sha1[i]);
    }

    MultiByteToWideChar(CP_ACP, 0, cSHA1, 21, wSHA1, 21);
    ZeroMemory(item->sha1, 42);
    wcscpy(item->sha1, wSHA1);
    WCHAR wProcessPath[MAX_PATH] = {0};
    cbData = MAX_PATH * 2;
    lRet = RegQueryValueExW(
               hKeyQuery,
               L"fullpath",
               NULL,
               &dwType,
               (BYTE*)wProcessPath,
               &cbData
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[fnGetRegProItem] RegQueryValueExW(fullpath) Failure!\n"));
        RegCloseKey(hKey);
        RegCloseKey(hKeyQuery);
        return FALSE;
    }

    ZeroMemory(item->wProcessPath, 400);
    wcscpy(item->wProcessPath, wProcessPath);
    DbgPrintW((L"[fnGetRegProItem] ProcessName: %ws\noper:%d\nProcessPath:%ws\nsha1:%ws\nlastTime:%ws\n",
               item->wProcessName, item->oper, item->wProcessPath, item->sha1, item->wLastTime));
    RegCloseKey(hKey);
    RegCloseKey(hKeyQuery);
    return TRUE;
}

extern "C"
PPSPT_API BOOL fnDelRelateKey(WCHAR *wProcessName)
{
    WCHAR lpSubKey[MAX_PATH] = {0};
    DbgPrintW((L"[fnDelRelateKey] Key:%ws\n", wProcessName));
    wsprintf(lpSubKey, L"SOFTWARE\\PProtect\\ProMon\\%ws",
             wProcessName);
    return DelRegKey(HKEY_LOCAL_MACHINE, lpSubKey);
}

extern "C"
PPSPT_API BOOL fnGetlastWriteTime(WCHAR *wLastWriteTime)
{
    LONG lRet = 0;
    HKEY hKey;
    DWORD dwDis, cbData, dwType;
    lRet = RegCreateKeyExW(
               HKEY_LOCAL_MACHINE,
               L"SOFTWARE\\PProtect\\ProMon",
               NULL,
               NULL,
               REG_OPTION_NON_VOLATILE,
               KEY_ALL_ACCESS,
               NULL,
               &hKey,
               &dwDis
           );

    if (ERROR_SUCCESS != lRet) {
        return FALSE;
    }

    cbData = 40;
    lRet = RegQueryValueExW(
               hKey,
               L"lastWriteTime",
               NULL,
               &dwType,
               (BYTE*)wLastWriteTime,
               &cbData
           );

    if (ERROR_SUCCESS != lRet) {
        RegCloseKey(hKey);
        return FALSE;
    }

    RegCloseKey(hKey);
    return TRUE;
}

extern "C"
PPSPT_API BOOL fnGetIcoPath(OUT WCHAR* icoPath)
{
    WCHAR wPath[MAX_PATH] = {0}, *p = NULL;
    GetModuleFileNameW(NULL, wPath, MAX_PATH);
    DbgPrintW((L"[fnGetIcoPath] wPath: %ws\n", wPath));
    p = wcsrchr(wPath, L'\\');

    if (NULL == p) {
        return FALSE;
    }

    ++p;
    *p = L'\0';
    wcscat(wPath, L"PProtect.ico");
    wcscpy(icoPath, wPath);
    DbgPrintW((L"[fnGetIcoPath] icoPath: %ws\n", icoPath));
    return TRUE;
}

extern "C"
//
//return value:
//0--reject
//1--accept
//2--ask for users
PPSPT_API int fnGetProcessInRulesState()
{
    int iRet = 0;
    HANDLE  hFileSys;
    BOOL    bRet = FALSE;
    PVOID   pShareMem = NULL;
    DWORD   dwRet = 0;
    pNotifyBlock p = NULL;
    WCHAR   wProcessName[MAX_PATH] = {0};

    do {
        hFileSys = CreateFileW(
                       L"\\\\.\\PProtect",
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL
                   );

        if (INVALID_HANDLE_VALUE == hFileSys) {
            DbgPrintA(("[fnGetProcessInRulesState] CreateFile Failure!"));
            iRet = 2;
            break;
        }

        bRet = DeviceIoControl(
                   hFileSys,
                   IOCTL_GetShareMemPointer,
                   NULL,
                   0,
                   (PVOID)&pShareMem,
                   4,
                   &dwRet,
                   NULL
               );

        if (!bRet) {
            DbgPrintA(("[fnGetProcessInRulesState]  IOCTL_GetShareMemPointer Failure!\n"));
            CloseHandle(hFileSys);
            iRet = 2;
            break;
        }

        CloseHandle(hFileSys);
        p = (pNotifyBlock)pShareMem;

        if (NULL == p) {
            DbgPrintA(("[fnGetProcessInRulesState] p is NULL\n"));
            iRet = 2;
            break;
        }

        WCHAR *pTemp = NULL;
        pTemp = wcsrchr(p->u.ProMon.R_Pro, '\\');

        if (NULL == pTemp) {
            pTemp = p->u.ProMon.R_Pro;
        } else {
            ++pTemp;
        }

        wcscpy(wProcessName, pTemp);
        LONG lRet = 0;
        HKEY hKey;
        WCHAR wSubKey[MAX_PATH] = {0};
        char SHA1[20] = {0}, OrgSha1[20] = {0};
        DWORD   dwOper = 0;
        DWORD   dwType, cbData;
        wsprintf(wSubKey, L"SOFTWARE\\PProtect\\ProMon\\%ws",
                 wProcessName);
        lRet = RegOpenKeyExW(
                   HKEY_LOCAL_MACHINE,
                   wSubKey,
                   0,
                   KEY_ALL_ACCESS,
                   &hKey);

        if (ERROR_SUCCESS != lRet) {
            DbgPrintA(("[fnGetProcessInRulesState] RegOpenKeyExW Failure!\n"));
            iRet = 2;
            break;
        }

        GetSHA1(p->u.ProMon.R_Pro, SHA1);
        cbData = 20;
        lRet = RegQueryValueEx(
                   hKey,
                   L"sha1",
                   NULL,
                   &dwType,
                   (BYTE*)OrgSha1,
                   &cbData
               );

        if (ERROR_SUCCESS != lRet) {
            DbgPrintA(("[fnGetProcessInRulesState] RegQueryValueEx Failure!\n"));
            iRet = 2;
            RegCloseKey(hKey);
            break;
        }

        if (_strnicmp(SHA1, OrgSha1, 20)) {
            DbgPrintA(("[fnGetProcessInRulesState] SHA1 不一致!\n"));
            RegCloseKey(hKey);
            iRet = 2;
            p->u.ProMon.SHAChanged = TRUE;
            break;
        }

        cbData = 4;
        lRet = RegQueryValueEx(
                   hKey,
                   L"oper",
                   NULL,
                   &dwType,
                   (BYTE*)&dwOper,
                   &cbData
               );

        if (ERROR_SUCCESS != lRet) {
            DbgPrintA(("[fnGetProcessInRulesState] RegQueryValueEx Failure!\n"));
            iRet = 2;
            RegCloseKey(hKey);
            break;
        }

        iRet = dwOper;
        DbgPrintA(("[fnGetProcessInRulesState] iRet: %d\n", iRet));
        RegCloseKey(hKey);
    } while (FALSE);

    return iRet;
}
BOOL Add2ProMonRules(BOOL bAllowRun, WCHAR *wPath)
{
    BOOL    bRet = FALSE;
    char    SHA1[20] = {0};
    long    lRet;
    HKEY    hKey;
    DWORD   dwDis;
    WCHAR   wProcessName[MAX_PATH] = {0};
    WCHAR   wSubKey[MAX_PATH] = {0};
    SYSTEMTIME  utcTime, locTime;
    WCHAR   wLastTime[MAX_PATH] = {0};
    DWORD   dwOper = 0;
    DbgPrintW((L"[Add2ProMonRules] Add Process(%ws) to Process Monitor Rules!\n",
               wPath));
    WCHAR *pTemp = wcsrchr(wPath, '\\');

    if (NULL == pTemp) {
        pTemp = wPath;
    } else {
        ++pTemp;
    }

    wcscpy(wProcessName, pTemp);
    GetSHA1(wPath, SHA1);
    wsprintf(wSubKey, L"SOFTWARE\\PProtect\\ProMon\\%ws",
             wProcessName);
    lRet = RegCreateKeyExW(
               HKEY_LOCAL_MACHINE,
               wSubKey,
               NULL,
               NULL,
               REG_OPTION_NON_VOLATILE,
               KEY_ALL_ACCESS,
               NULL,
               &hKey,
               &dwDis
           );

    if (lRet != ERROR_SUCCESS) {
        DbgPrintA(("[Add2ProMonRules] RegCreateKeyExW 失败!\n"));
        return FALSE;
    }

    dwOper = bAllowRun ? 1 : 0;
    lRet = RegSetValueExW(
               hKey,
               L"oper",
               NULL,
               REG_DWORD,
               (BYTE*)&dwOper,
               4
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[Add2ProMonRules] RegSetValueExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    lRet = RegSetValueExW(
               hKey,
               L"sha1",
               NULL,
               REG_BINARY,
               (BYTE*)SHA1,
               20);

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[Add2ProMonRules] RegSetValueExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    lRet = RegSetValueExW(
               hKey,
               L"fullpath",
               NULL,
               REG_SZ,
               (BYTE*)wPath,
               (wcslen(wPath) + 1) * 2
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[Add2ProMonRules] RegSetValueExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    GetSystemTime(&utcTime);
    SystemTimeToTzSpecificLocalTime(NULL, &utcTime, &locTime);
    wsprintf(wLastTime, L"%02d/%02d/%d %02d:%02d\n",
             locTime.wMonth, locTime.wDay, locTime.wYear,
             locTime.wHour, locTime.wMinute);
    lRet = RegSetValueExW(
               hKey,
               L"lasttime",
               NULL,
               REG_SZ,
               (BYTE*)wLastTime,
               (wcslen(wLastTime) + 1) * 2
           );

    if (ERROR_SUCCESS != lRet) {
        DbgPrintA(("[Add2ProMonRules] RegSetValueExW Failure!\n"));
        RegCloseKey(hKey);
        return FALSE;
    }

    RegCloseKey(hKey);
    lRet = RegCreateKeyExW(
               HKEY_LOCAL_MACHINE,
               L"SOFTWARE\\PProtect\\ProMon",
               NULL,
               NULL,
               REG_OPTION_NON_VOLATILE,
               KEY_ALL_ACCESS,
               NULL,
               &hKey,
               &dwDis
           );

    if (ERROR_SUCCESS != lRet) {
        return FALSE;
    }

    lRet = RegSetValueExW(
               hKey,
               L"lastWriteTime",
               0,
               REG_SZ,
               (BYTE*)wLastTime,
               (wcslen(wLastTime) + 1) * 2
           );

    if (ERROR_SUCCESS != lRet) {
        RegCloseKey(hKey);
        return FALSE;
    }

    RegCloseKey(hKey);
    return TRUE;
}

BOOL GetSHA1(WCHAR *wPath, char *SHA1)
{
    sha     sh;
    HANDLE  hFile, hFileMap;
    DWORD   dwFileSize, i;
    PVOID   pFile;
    char    c;
    hFile = CreateFileW(
                wPath,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

    if (INVALID_HANDLE_VALUE == hFile) {
        DbgPrintW((L"[GetSHA1] CreateFile(%ws) Failure,err: %d\n",
                   wPath, GetLastError));
        return FALSE;
    }

    dwFileSize = GetFileSize(hFile, NULL);
    hFileMap = CreateFileMappingW(
                   hFile,
                   NULL,
                   PAGE_READONLY,
                   0,
                   0,
                   NULL
               );

    if (INVALID_HANDLE_VALUE == hFileMap) {
        DbgPrintA(("[GetSHA1] CreateFileMappingW Failure!\n"));
        CloseHandle(hFile);
        return FALSE;
    }

    pFile = MapViewOfFile(
                hFileMap,
                FILE_MAP_READ,
                0,
                0,
                0
            );

    if (!pFile) {
        DbgPrintA(("[GetSHA1] MapViewOfFile Failure!\n"));
        CloseHandle(hFile);
        return FALSE;
    }

    shs_init(&sh);

    for (i = 0; i < dwFileSize; i++) {
        c = *(char*)((char*)pFile + i);
        shs_process(&sh, c);
    }

    shs_hash(&sh, SHA1);
    UnmapViewOfFile(pFile);
    CloseHandle(hFileMap);
    CloseHandle(hFile);
    return TRUE;
}



DWORD WINAPI InitFnSigVerif(LPVOID lpParameter)
{
    WCHAR calcPath[MAX_PATH] = {0};
    PVOID pShareMem = NULL;
    pShareMem = *(PVOID*)lpParameter;
    GetSystemDirectory(calcPath, MAX_PATH);
    wcscat(calcPath, L"\\calc.exe");
    wcscpy(((pNotifyBlock)pShareMem)->u.ProMon.R_Pro, calcPath);
    fnSigVerif();
    return 0;
}


BOOL RegDelnode(HKEY hKeyRoot, WCHAR* lpSubKey)
{
    WCHAR* lpEnd;
    LONG lResult;
    DWORD dwSize;
    WCHAR szName[MAX_PATH];
    HKEY hKey;
    FILETIME ftWrite;
    // First, see if we can delete the key without having
    // to recurse.
    lResult = RegDeleteKey(hKeyRoot, lpSubKey);

    if (lResult == ERROR_SUCCESS) {
        return TRUE;
    }

    lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS) {
        if (lResult == ERROR_FILE_NOT_FOUND) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    // Check for an ending slash and add one if it is missing.
    lpEnd = lpSubKey + wcslen(lpSubKey);

    if (*(lpEnd - 1) != L'\\') {
        *lpEnd =  L'\\';
        lpEnd++;
        *lpEnd =  L'\0';
    }

    // Enumerate the keys
    dwSize = MAX_PATH;
    lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                           NULL, NULL, &ftWrite);

    if (lResult == ERROR_SUCCESS) {
        do {
            wcscpy(lpEnd, szName);

            if (!RegDelnode(hKeyRoot, lpSubKey)) {
                break;
            }

            dwSize = MAX_PATH;
            lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                                   NULL, NULL, &ftWrite);
        } while (lResult == ERROR_SUCCESS);
    }

    lpEnd--;
    *lpEnd = L'\0';
    RegCloseKey(hKey);
    // Try again to delete the key.
    lResult = RegDeleteKey(hKeyRoot, lpSubKey);

    if (lResult == ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}

BOOL
DelRegKey(
    HKEY hKeyRoot,
    WCHAR* lpSubKey
)
{
    WCHAR szDelKey[2 * MAX_PATH];
    wcscpy(szDelKey, lpSubKey);
    return RegDelnode(hKeyRoot, szDelKey);
}

//////////////////////////////////////////////////////////////////////////
//打印日志
//
//////////////////////////////////////////////////////////////////////////


VOID DebugPrintA(
    CHAR *Format,
    ...
)
{
    va_list args;
    CHAR    Msg[MAX_PATH] = {0};
    va_start(args, Format);
    vsprintf(Msg, Format, args);
    va_end(args);
    OutputDebugStringA(Msg);
}


VOID DebugPrintW(
    WCHAR *Format,
    ...
)
{
    va_list args;
    WCHAR   Msg[MAX_PATH] = {0};
    va_start(args, Format);
    vswprintf(Msg, Format, args);
    va_end(args);
    OutputDebugStringW(Msg);
}