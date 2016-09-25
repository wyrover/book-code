//原作者：不明（来源于网络）
//整理者：Tesla.Angela(GDUT.HWL)

/*
使用方法：
修改DRV_NAME和DRV_FILENAME定义
HANDLE openDriver(void)：打开驱动建立的符号链接的句柄
void uninstallDriver(void)：卸载驱动（只有把openDriver打开的句柄关闭才可以成功卸载）
*/
/*
使用示例：
VOID TEST()
{
    HANDLE hSSDTDrv;
    hSSDTDrv=openDriver();
    IoControl(hSSDTDrv ,CTL_CODE_GEN(0x802), &fn_index, 4, &ssdt_func_addr, 8);
    uninstallDriver();
}
*/

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Advapi32.lib")

#define DRV_NAME        "emSSDT64"//驱动名
#define DRV_FILENAME    "emSSDT64.sys"//驱动文件

#define STATUS_SUCCESS     ((NTSTATUS)0x00000000L)

typedef LONG NTSTATUS;

typedef struct _STRING {
    USHORT  Length;
    USHORT  MaximumLength;
    PCHAR  Buffer;
} ANSI_STRING, *PANSI_STRING;

typedef struct _UNICODE_STRING {
    USHORT  Length;
    USHORT  MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

VOID AfxMessageBox(char *sz)
{
    LoadLibraryA("user32.dll");
    MessageBoxA(0, sz, DRV_NAME, 0);
}

//*********************************************************************************************
// Assign loaddriver priviledge to our process, so we can load our support driver.
//
//*********************************************************************************************

BOOL getLoadDriverPriv()
{
    HANDLE hToken;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken)) {
        LUID huid;

        if (LookupPrivilegeValueA(NULL, "SeLoadDriverPrivilege", &huid)) {
            LUID_AND_ATTRIBUTES priv;
            priv.Attributes = SE_PRIVILEGE_ENABLED;
            priv.Luid = huid;
            TOKEN_PRIVILEGES tp;
            tp.PrivilegeCount = 1;
            tp.Privileges[0] = priv;

            if (AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL)) {
                return TRUE;
            }
        }
    }

    return FALSE;
}


//*********************************************************************************************
// Sets up the necessary registry settings to load the support driver
//
//*********************************************************************************************

BOOL setupRegistry()
{
    HKEY hkey;

    if (RegCreateKeyA(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Services\\"DRV_NAME, &hkey) != ERROR_SUCCESS)
        return FALSE;

    DWORD val;
    val = 1;

    if (RegSetValueExA(hkey, "Type", 0, REG_DWORD, (PBYTE)&val, sizeof(val)) != ERROR_SUCCESS)
        return FALSE;

    if (RegSetValueExA(hkey, "ErrorControl", 0, REG_DWORD, (PBYTE)&val, sizeof(val)) != ERROR_SUCCESS)
        return FALSE;

    val = 3;

    if (RegSetValueExA(hkey, "Start", 0, REG_DWORD, (PBYTE)&val, sizeof(val)) != ERROR_SUCCESS)
        return FALSE;

    char *imgName = "System32\\DRIVERS\\"DRV_FILENAME;

    if (RegSetValueExA(hkey, "ImagePath", 0, REG_EXPAND_SZ, (PBYTE)imgName, strlen(imgName)) != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}


//*********************************************************************************************
// Actual code to load our driver into memory
//
//*********************************************************************************************

BOOL loadDriver()
{
    HMODULE hntdll;
    NTSTATUS(WINAPI * _RtlAnsiStringToUnicodeString)(PUNICODE_STRING  DestinationString, IN PANSI_STRING  SourceString, IN
            BOOLEAN);
    VOID (WINAPI * _RtlInitAnsiString)
    (IN OUT PANSI_STRING  DestinationString,
     IN PCHAR  SourceString);
    NTSTATUS(WINAPI * _ZwLoadDriver)
    (IN PUNICODE_STRING DriverServiceName);
    NTSTATUS(WINAPI * _ZwUnloadDriver)
    (IN PUNICODE_STRING DriverServiceName);
    VOID (WINAPI * _RtlFreeUnicodeString)
    (IN PUNICODE_STRING  UnicodeString);
    hntdll = GetModuleHandleA("ntdll.dll");
    *(FARPROC *)&_ZwLoadDriver = GetProcAddress(hntdll, "NtLoadDriver");
    *(FARPROC *)&_ZwUnloadDriver = GetProcAddress(hntdll, "NtUnloadDriver");
    *(FARPROC *)&_RtlAnsiStringToUnicodeString =
        GetProcAddress(hntdll, "RtlAnsiStringToUnicodeString");
    *(FARPROC *)&_RtlInitAnsiString =
        GetProcAddress(hntdll, "RtlInitAnsiString");
    *(FARPROC *)&_RtlFreeUnicodeString =
        GetProcAddress(hntdll, "RtlFreeUnicodeString");

    if (_ZwLoadDriver && _ZwUnloadDriver && _RtlAnsiStringToUnicodeString &&
        _RtlInitAnsiString && _RtlFreeUnicodeString) {
        ANSI_STRING aStr;
        _RtlInitAnsiString(&aStr,
                           "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\"DRV_NAME);
        UNICODE_STRING uStr;

        if (_RtlAnsiStringToUnicodeString(&uStr, &aStr, TRUE) != STATUS_SUCCESS)
            return FALSE;
        else {
            if (_ZwLoadDriver(&uStr) == STATUS_SUCCESS) {
                _RtlFreeUnicodeString(&uStr);
                return TRUE;
            }

            _RtlFreeUnicodeString(&uStr);
        }
    }

    return FALSE;
}


//*********************************************************************************************
// Actual code to remove our driver from memory
//
//*********************************************************************************************

BOOL unloadDriver()
{
// call ntdll APIs
    HMODULE hntdll;
    NTSTATUS(WINAPI * _RtlAnsiStringToUnicodeString)
    (PUNICODE_STRING  DestinationString,
     IN PANSI_STRING  SourceString,
     IN BOOLEAN);
    VOID (WINAPI * _RtlInitAnsiString)
    (IN OUT PANSI_STRING  DestinationString,
     IN PCHAR  SourceString);
    NTSTATUS(WINAPI * _ZwLoadDriver)
    (IN PUNICODE_STRING DriverServiceName);
    NTSTATUS(WINAPI * _ZwUnloadDriver)
    (IN PUNICODE_STRING DriverServiceName);
    VOID (WINAPI * _RtlFreeUnicodeString)
    (IN PUNICODE_STRING  UnicodeString);
    hntdll = GetModuleHandleA("ntdll.dll");
    *(FARPROC *)&_ZwLoadDriver = GetProcAddress(hntdll, "NtLoadDriver");
    *(FARPROC *)&_ZwUnloadDriver = GetProcAddress(hntdll, "NtUnloadDriver");
    *(FARPROC *)&_RtlAnsiStringToUnicodeString =
        GetProcAddress(hntdll, "RtlAnsiStringToUnicodeString");
    *(FARPROC *)&_RtlInitAnsiString =
        GetProcAddress(hntdll, "RtlInitAnsiString");
    *(FARPROC *)&_RtlFreeUnicodeString =
        GetProcAddress(hntdll, "RtlFreeUnicodeString");

    if (_ZwLoadDriver && _ZwUnloadDriver && _RtlAnsiStringToUnicodeString &&
        _RtlInitAnsiString && _RtlFreeUnicodeString) {
        ANSI_STRING aStr;
        _RtlInitAnsiString(&aStr,
                           "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\"DRV_NAME);
        UNICODE_STRING uStr;

        if (_RtlAnsiStringToUnicodeString(&uStr, &aStr, TRUE) != STATUS_SUCCESS)
            return FALSE;
        else {
            if (_ZwUnloadDriver(&uStr) == STATUS_SUCCESS) {
                _RtlFreeUnicodeString(&uStr);
                return TRUE;
            }

            _RtlFreeUnicodeString(&uStr);
        }
    }

    return FALSE;
}


//*********************************************************************************************
// Removes our driver file and registry settings
//
//*********************************************************************************************

void cleanupDriver(void)
{
    char sysDir[MAX_PATH + 1];
    GetSystemDirectoryA(sysDir, MAX_PATH);
    strncat(sysDir, "\\drivers\\"DRV_FILENAME, MAX_PATH);
    DeleteFileA(sysDir);
    RegDeleteKeyA(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Services\\"DRV_NAME"\\Enum");
    RegDeleteKeyA(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Services\\"DRV_NAME);
}


//*********************************************************************************************
// Attempts to get a handle to our kernel driver.  If fails, try to install the driver.
//
//*********************************************************************************************

HANDLE openDriver(void)
{
    HANDLE hDevice;
    //CreateFile打开驱动建立的符号链接，得根据驱动中建立的名字更改
    hDevice = CreateFileA("\\\\.\\"DRV_NAME, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        char drvFullPath[MAX_PATH + 1];
        char *filePart;
        ZeroMemory(drvFullPath, MAX_PATH);
        GetFullPathNameA(DRV_FILENAME, MAX_PATH, drvFullPath, &filePart);
        HANDLE hFile = CreateFileA(drvFullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL, 0);

        if (hFile == INVALID_HANDLE_VALUE) {
            AfxMessageBox("Cannot find required driver file");
            return INVALID_HANDLE_VALUE;
        } else {
            CloseHandle(hFile);
            char sysDir[MAX_PATH + 1];
            GetSystemDirectoryA(sysDir, MAX_PATH);
            strncat(sysDir, "\\drivers\\"DRV_FILENAME, MAX_PATH);
            CopyFileA(drvFullPath, sysDir, TRUE);

            if (!getLoadDriverPriv()) {
                AfxMessageBox("Error getting load driver privilege! ");
            } else {
                if (!setupRegistry()) {
                    AfxMessageBox("Error setting driver registry keys! Make sure you are running this as Administrator. ");
                } else {
                    loadDriver();
                    hDevice = CreateFileA("\\\\.\\"DRV_NAME, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                          NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

                    if (hDevice == INVALID_HANDLE_VALUE) {
                        AfxMessageBox("Error loading kernel support driver! Make sure you are running this as Administrator. ");
                    } else {
                        AfxMessageBox("loading kernel support driver success");
                    }
                }
            }

            cleanupDriver();
        }
    }

    return hDevice;
}


//*********************************************************************************************
// Remove our kernel driver from memory
//
//*********************************************************************************************

void uninstallDriver(void)
{
    char drvFullPath[MAX_PATH + 1];
    char *filePart;
    ZeroMemory(drvFullPath, MAX_PATH);
    GetFullPathNameA(DRV_FILENAME, MAX_PATH, drvFullPath, &filePart);
    HANDLE hFile = CreateFileA(drvFullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL, 0);

    if (hFile == INVALID_HANDLE_VALUE) {
        AfxMessageBox("Cannot find required driver file ");
        return;
    } else {
        CloseHandle(hFile);
        char sysDir[MAX_PATH + 1];
        GetSystemDirectoryA(sysDir, MAX_PATH);
        strncat(sysDir, "\\drivers\\"DRV_FILENAME, MAX_PATH);
        CopyFileA(drvFullPath, sysDir, TRUE);

        if (!getLoadDriverPriv()) {
            AfxMessageBox("Error getting load driver privilege! ");
        } else {
            if (!setupRegistry()) {
                AfxMessageBox("Error setting driver registry keys! Make sure you are running this as Administrator. ");
            } else {
                if (unloadDriver())
                    AfxMessageBox("Support driver successfully unloaded. ");
                else
                    AfxMessageBox("Unload support driver failed.  It is probably not loaded. ");
            }
        }

        cleanupDriver();
    }
}

DWORD CTL_CODE_GEN(DWORD lngFunction)
{
    return (FILE_DEVICE_UNKNOWN * 65536) | (FILE_ANY_ACCESS * 16384) | (lngFunction * 4) | METHOD_BUFFERED;
}

BOOL IoControl(HANDLE hDrvHandle, DWORD dwIoControlCode, PVOID lpInBuffer, DWORD nInBufferSize, PVOID lpOutBuffer, DWORD nOutBufferSize)
{
    DWORD lDrvRetSize;
    return DeviceIoControl(hDrvHandle, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, &lDrvRetSize, 0);
}