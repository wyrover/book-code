/*!
* \file cactus_environment.cpp
* \brief 操作系统环境
*
* 操作系统环境
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#include "cactus/cactus_environment.h"
#include "cactus/cactus_exception.h"
#include "cactus/cactus_string.h"
#include "cactus/cactus_file.h"
#include <cactus/cactus_file_version.h>
#include <sstream>
#include <iphlpapi.h>
#include <ShlObj.h>

#pragma comment(lib, "Iphlpapi.lib")


namespace cactus
{
string16 GetCurrentSystemVersion()
{
    static string16 strOsVer;
    typedef void (WINAPI * PGNSI)(LPSYSTEM_INFO);

    if (!strOsVer.empty())
        return strOsVer;

    OSVERSIONINFOEX osvi;
    SYSTEM_INFO si;
    PGNSI pGNSI;
    BOOL bOsVersionInfoEx;
    CString strVersion = _T("UnKnowVersion");
    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *) &osvi))) {
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

        if (! GetVersionEx((OSVERSIONINFO *) &osvi))
            return strVersion;
    }

    pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");

    if (NULL != pGNSI) {
        pGNSI(&si);
    } else {
        GetSystemInfo(&si);
    }

    switch (osvi.dwPlatformId) {
    case VER_PLATFORM_WIN32_NT:
        if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2) {
            strVersion = _T("Win8");
        } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) {
            if (osvi.wProductType == VER_NT_WORKSTATION)
                strVersion = _T("Win7");
            else
                strVersion = _T("WinS2008");
        } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) {
            if (osvi.wProductType == VER_NT_WORKSTATION)
                strVersion = _T("Vista");
            else
                strVersion = _T("WinS2008");
        } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
            if (GetSystemMetrics(SM_SERVERR2))
                strVersion = _T("WinS2003");
            else if (osvi.wProductType == VER_NT_WORKSTATION &&
                     si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
                strVersion = _T("WinXP");
            } else
                strVersion = _T("WinS2003");
        } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {
            strVersion = _T("WinXP");
        } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
            strVersion = _T("Win2000 ");
        } else if (osvi.dwMajorVersion <= 4) {
            strVersion = _T("WinNT ");
        }

        break;

    case VER_PLATFORM_WIN32_WINDOWS:
        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
            strVersion = _T("Win9x");
        } else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
            strVersion = _T("Win9x");
        } else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
            strVersion = _T("WinME");
        }

        break;

    case VER_PLATFORM_WIN32s:
        strVersion = _T("Win32s");
        break;
    }

    strOsVer = (LPCWSTR)strVersion;
    return strOsVer;
}

}


namespace cactus
{
enum {
    MAX_PATH_LEN = 32767
};


string16 Environment::get(const string16& name)
{
    DWORD len = GetEnvironmentVariableW(name.c_str(), 0, 0);

    if (len == 0)
        return StringUtil::sWEmpty;

    std::vector<wchar_t> buffer(len + 1);
    GetEnvironmentVariableW(name.c_str(), &buffer[0], len);
    string16 result(&buffer[0]);
    return result;
}

string16 Environment::get(const string16& name, const string16& defaultValue)
{
    if (has(name))
        return get(name);
    else
        return defaultValue;
}

bool Environment::has(const string16& name)
{
    DWORD len = GetEnvironmentVariableW(name.c_str(), 0, 0);
    return len > 0;
}

void Environment::set(const string16& name, const string16& value)
{
    if (SetEnvironmentVariableW(name.c_str(), value.c_str()) == 0) {
        std::string msg = "cannot set environment variable: ";
        msg.append(convert_to<string8>(name));
        throw SystemException(msg);
    }
}

string16 Environment::osName()
{
    OSVERSIONINFO vi;
    vi.dwOSVersionInfoSize = sizeof(vi);

    if (GetVersionEx(&vi) == 0) throw SystemException("Cannot get OS version information");

    switch (vi.dwPlatformId) {
    case VER_PLATFORM_WIN32s:
        return L"Windows 3.x";

    case VER_PLATFORM_WIN32_WINDOWS:
        return vi.dwMinorVersion == 0 ? L"Windows 95" : L"Windows 98";

    case VER_PLATFORM_WIN32_NT:
        return L"Windows NT";

    default:
        return L"Unknown";
    }
}

string16 Environment::osDisplayName()
{
    OSVERSIONINFO vi;
    vi.dwOSVersionInfoSize = sizeof(vi);

    if (GetVersionEx(&vi) == 0) throw SystemException("Cannot get OS version information");

    switch (vi.dwMajorVersion) {
    case 6:
        switch (vi.dwMinorVersion) {
        case 0:
            return L"Windows Vista/Server 2008";

        case 1:
            return L"Windows 7/Server 2008 R2";

        case 2:
            return L"Windows 8/Server 2012";

        default:
            return L"Unknown";
        }

    case 5:
        switch (vi.dwMinorVersion) {
        case 0:
            return L"Windows 2000";

        case 1:
            return L"Windows XP";

        case 2:
            return L"Windows Server 2003/Windows Server 2003 R2";

        default:
            return L"Unknown";
        }

    case 4:
        switch (vi.dwMinorVersion) {
        case 0:
            return L"Windows 95/Windows NT 4.0";

        case 10:
            return L"Windows 98";

        case 90:
            return L"Windows ME";

        default:
            return L"Unknown";
        }

    default:
        return L"Unknown";
    }
}

string16 Environment::osVersion()
{
    OSVERSIONINFOW vi;
    vi.dwOSVersionInfoSize = sizeof(vi);

    if (GetVersionExW(&vi) == 0) throw SystemException("Cannot get OS version information");

    std::wostringstream str;
    str << vi.dwMajorVersion << L"." << vi.dwMinorVersion << L" (Build " << (vi.dwBuildNumber & 0xFFFF);
    string16 version(vi.szCSDVersion);

    if (!version.empty()) str << L": " << version;

    str << L")";
    return str.str();
}

string16 Environment::osArchitecture()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    switch (si.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_INTEL:
        return L"IA32";

    case PROCESSOR_ARCHITECTURE_MIPS:
        return L"MIPS";

    case PROCESSOR_ARCHITECTURE_ALPHA:
        return L"ALPHA";

    case PROCESSOR_ARCHITECTURE_PPC:
        return L"PPC";

    case PROCESSOR_ARCHITECTURE_IA64:
        return L"IA64";
#ifdef PROCESSOR_ARCHITECTURE_IA32_ON_WIN64

    case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
        return L"IA64/32";
#endif
#ifdef PROCESSOR_ARCHITECTURE_AMD64

    case PROCESSOR_ARCHITECTURE_AMD64:
        return L"AMD64";
#endif

    default:
        return L"Unknown";
    }
}

string16 Environment::nodeName()
{
    wchar_t name[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

    if (GetComputerNameW(name, &size) == 0) throw SystemException("Cannot get computer name");

    string16 result(name);
    return result;
}

void Environment::nodeId(NodeId& id)
{
    std::memset(&id, 0, sizeof(id));
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = 0;
    ULONG len    = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(new char[len]);
    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into len
    DWORD rc = GetAdaptersInfo(pAdapterInfo, &len);

    if (rc == ERROR_BUFFER_OVERFLOW) {
        delete [] reinterpret_cast<char*>(pAdapterInfo);
        pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(new char[len]);
    } else if (rc != ERROR_SUCCESS) {
        return;
    }

    if (GetAdaptersInfo(pAdapterInfo, &len) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        bool found = false;

        while (pAdapter && !found) {
            if (pAdapter->Type == MIB_IF_TYPE_ETHERNET && pAdapter->AddressLength == sizeof(id)) {
                found = true;
                std::memcpy(&id, pAdapter->Address, pAdapter->AddressLength);
            }

            pAdapter = pAdapter->Next;
        }
    }

    delete [] reinterpret_cast<char*>(pAdapterInfo);
}

string16 Environment::nodeId()
{
    NodeId id;
    nodeId(id);
    char result[18];
    std::sprintf(result, "%02x:%02x:%02x:%02x:%02x:%02x",
                 id[0],
                 id[1],
                 id[2],
                 id[3],
                 id[4],
                 id[5]);
    return convert_to<string16>(std::string(result));
}

unsigned Environment::processorCount()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwNumberOfProcessors;
}

string16 Environment::desktop_dir()
{
    char16 buffer[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, buffer, CSIDL_DESKTOPDIRECTORY, FALSE);
    return Path::IncludeTrailingBackslash(string16(buffer));
}

string16 Environment::program_files_dir()
{
    char16 buffer[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, buffer, CSIDL_PROGRAM_FILES, FALSE);
    return Path::IncludeTrailingBackslash(string16(buffer));
}

string16 Environment::appdata_dir()
{
    char16 buffer[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, buffer, CSIDL_APPDATA, FALSE);
    return Path::IncludeTrailingBackslash(string16(buffer));
}

string16 Environment::common_appdata_dir()
{
    char16 buffer[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, buffer, CSIDL_COMMON_APPDATA, FALSE);
    return Path::IncludeTrailingBackslash(string16(buffer));
}

string16 Environment::user_home_dir()
{
    char16 buffer[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, buffer, CSIDL_PROFILE, FALSE);
    return Path::IncludeTrailingBackslash(string16(buffer));
}

string16 Environment::startmenu_dir()
{
    char16 buffer[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, buffer, CSIDL_STARTMENU, FALSE);
    return Path::IncludeTrailingBackslash(string16(buffer));
}

string16 Environment::common_startmenu_dir()
{
    char16 buffer[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, buffer, CSIDL_COMMON_STARTMENU, FALSE);
    return Path::IncludeTrailingBackslash(string16(buffer));
}

string16 Environment::common_desktop_dir()
{
    char16 buffer[MAX_PATH + 1];
    SHGetSpecialFolderPath(NULL, buffer, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
    return Path::IncludeTrailingBackslash(string16(buffer));
}

string16 Environment::system_dir()
{
    char16 buffer[MAX_PATH_LEN];
    DWORD n = ::GetSystemDirectory(buffer, MAX_PATH_LEN);

    if (n > 0) {
        n = GetLongPathNameW(buffer, buffer, static_cast<DWORD>(wcslen(buffer)));

        if (n > 0) {
            return Path::IncludeTrailingPathDelimiter(string16(buffer));
        }
    }

    return StringUtil::sWEmpty;
}

string16 Environment::windows_dir()
{
    char16 buffer[MAX_PATH + 1];
    ::GetWindowsDirectory(buffer, MAX_PATH);

    if (wcslen(buffer) > 0)
        return Path::IncludeTrailingPathDelimiter(string16(buffer));
    else
        return StringUtil::sWEmpty;
}

string16 Environment::temp_dir()
{
    char16 buffer[MAX_PATH_LEN];
    DWORD n = GetTempPathW(MAX_PATH_LEN, buffer);

    if (n > 0) {
        n = GetLongPathNameW(buffer, buffer, static_cast<DWORD>(wcslen(buffer)));

        if (n > 0) {
            return Path::IncludeTrailingPathDelimiter(string16(buffer));
        }
    }

    return StringUtil::sWEmpty;
}

string16 Environment::sys_drv()
{
    static string16 sysdrv;

    if (!sysdrv.empty())
        return sysdrv;

    std::vector<char16> buffer(MAX_PATH + 1);
    GetSystemDirectory(&buffer[0], MAX_PATH);
    sysdrv = string16(&buffer[0]);
    sysdrv = sysdrv.erase(sysdrv.find_first_of(L"\\/") + 1);
    return sysdrv;
}

bool Environment::is_admin_user()
{
    static HMODULE hModule = NULL;

    if (!hModule)
        hModule = LoadLibrary(_T("shell32.dll"));

    if (!hModule)
        return true;

    typedef BOOL (__stdcall * FunctionIsUserAdmin)();
    FunctionIsUserAdmin pfnIsUserAnAdmin = (FunctionIsUserAdmin)GetProcAddress(hModule, "IsUserAnAdmin");

    if (pfnIsUserAnAdmin)
        return pfnIsUserAnAdmin();

    return false;
}

LANGID Environment::lang_id()
{
    static LANGID dwLang = 0;

    if (dwLang == 0) {
        // GetSystemDefaultLangID();
        dwLang = GetSystemDefaultUILanguage();

        if (dwLang == 2057)
            dwLang = 1033;
    }

    return dwLang;
}

HRESULT Environment::GetDirectXVerionViaFileVersions(DWORD* pdwDirectXVersionMajor,
        DWORD* pdwDirectXVersionMinor,
        TCHAR* pcDirectXVersionLetter)
{
    ULARGE_INTEGER llFileVersion;
    TCHAR szPath[512];
    TCHAR szFile[512];
    BOOL bFound = false;

    if (GetSystemDirectory(szPath, MAX_PATH) != 0) {
        szPath[MAX_PATH - 1] = 0;
        // Switch off the ddraw version
        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\ddraw.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if (CompareLargeInts(llFileVersion, MakeInt64(4, 2, 0, 95)) >= 0) {    // Win9x version
                // flle is >= DX1.0 version, so we must be at least DX1.0
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 1;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }

            if (CompareLargeInts(llFileVersion, MakeInt64(4, 3, 0, 1096)) >= 0) {    // Win9x version
                // flle is is >= DX2.0 version, so we must DX2.0 or DX2.0a (no redist change)
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 2;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }

            if (CompareLargeInts(llFileVersion, MakeInt64(4, 4, 0, 68)) >= 0) {    // Win9x version
                // flle is is >= DX3.0 version, so we must be at least DX3.0
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 3;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }
        }

        // Switch off the d3drg8x.dll version
        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\d3drg8x.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if (CompareLargeInts(llFileVersion, MakeInt64(4, 4, 0, 70)) >= 0) {    // Win9x version
                // d3drg8x.dll is the DX3.0a version, so we must be DX3.0a or DX3.0b  (no redist change)
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 3;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT('a');

                bFound = true;
            }
        }

        // Switch off the ddraw version
        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\ddraw.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if (CompareLargeInts(llFileVersion, MakeInt64(4, 5, 0, 155)) >= 0) {    // Win9x version
                // ddraw.dll is the DX5.0 version, so we must be DX5.0 or DX5.2 (no redist change)
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 5;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }

            if (CompareLargeInts(llFileVersion, MakeInt64(4, 6, 0, 318)) >= 0) {    // Win9x version
                // ddraw.dll is the DX6.0 version, so we must be at least DX6.0
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 6;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }

            if (CompareLargeInts(llFileVersion, MakeInt64(4, 6, 0, 436)) >= 0) {    // Win9x version
                // ddraw.dll is the DX6.1 version, so we must be at least DX6.1
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 6;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 1;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }
        }

        // Switch off the dplayx.dll version
        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\dplayx.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if (CompareLargeInts(llFileVersion, MakeInt64(4, 6, 3, 518)) >= 0) {    // Win9x version
                // ddraw.dll is the DX6.1 version, so we must be at least DX6.1a
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 6;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 1;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT('a');

                bFound = true;
            }
        }

        // Switch off the ddraw version
        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\ddraw.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if (CompareLargeInts(llFileVersion, MakeInt64(4, 7, 0, 700)) >= 0) {    // Win9x version
                // TODO: find win2k version

                // ddraw.dll is the DX7.0 version, so we must be at least DX7.0
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 7;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }
        }

        // Switch off the dinput version
        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\dinput.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if (CompareLargeInts(llFileVersion, MakeInt64(4, 7, 0, 716)) >= 0) {    // Win9x version
                // ddraw.dll is the DX7.0 version, so we must be at least DX7.0a
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 7;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT('a');

                bFound = true;
            }
        }

        // Switch off the ddraw version
        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\ddraw.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if ((HIWORD(llFileVersion.HighPart) == 4 && CompareLargeInts(llFileVersion, MakeInt64(4, 8, 0, 400)) >= 0) ||     // Win9x version
                (HIWORD(llFileVersion.HighPart) == 5 && CompareLargeInts(llFileVersion, MakeInt64(5, 1, 2258, 400)) >= 0)) {    // Win2k/WinXP version
                // ddraw.dll is the DX8.0 version, so we must be at least DX8.0 or DX8.0a (no redist change)
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 8;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }
        }

        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\d3d8.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if ((HIWORD(llFileVersion.HighPart) == 4 && CompareLargeInts(llFileVersion, MakeInt64(4, 8, 1, 881)) >= 0) ||     // Win9x version
                (HIWORD(llFileVersion.HighPart) == 5 && CompareLargeInts(llFileVersion, MakeInt64(5, 1, 2600, 881)) >= 0)) {    // Win2k/WinXP version
                // d3d8.dll is the DX8.1 version, so we must be at least DX8.1
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 8;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 1;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }

            if ((HIWORD(llFileVersion.HighPart) == 4 && CompareLargeInts(llFileVersion, MakeInt64(4, 8, 1, 901)) >= 0) ||     // Win9x version
                (HIWORD(llFileVersion.HighPart) == 5 && CompareLargeInts(llFileVersion, MakeInt64(5, 1, 2600, 901)) >= 0)) {    // Win2k/WinXP version
                // d3d8.dll is the DX8.1a version, so we must be at least DX8.1a
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 8;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 1;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT('a');

                bFound = true;
            }
        }

        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\mpg2splt.ax"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if (CompareLargeInts(llFileVersion, MakeInt64(6, 3, 1, 885)) >= 0) {    // Win9x/Win2k/WinXP version
                // quartz.dll is the DX8.1b version, so we must be at least DX8.1b
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 8;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 1;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT('b');

                bFound = true;
            }
        }

        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\dpnet.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            if ((HIWORD(llFileVersion.HighPart) == 4 && CompareLargeInts(llFileVersion, MakeInt64(4, 9, 0, 134)) >= 0) ||     // Win9x version
                (HIWORD(llFileVersion.HighPart) == 5 && CompareLargeInts(llFileVersion, MakeInt64(5, 2, 3677, 134)) >= 0)) {    // Win2k/WinXP version
                // dpnet.dll is the DX8.2 version, so we must be at least DX8.2
                if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 8;

                if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 2;

                if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

                bFound = true;
            }
        }

        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\d3d9.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            // File exists, but be at least DX9
            if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 9;

            if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

            if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

            bFound = true;
        }

        // Assumption !!!
        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\d3d10.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            // File exists, but be at least DX10
            if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 10;

            if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

            if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

            bFound = true;
        }

        _tcscpy(szFile, szPath);
        _tcscat(szFile, TEXT("\\d3d11.dll"));

        if (GetFileVersion(szFile, llFileVersion)) {
            // File exists, but be at least DX11
            if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 11;

            if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

            if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');

            bFound = true;
        }
    }

    if (!bFound) {
        // No DirectX installed
        if (pdwDirectXVersionMajor) *pdwDirectXVersionMajor = 0;

        if (pdwDirectXVersionMinor) *pdwDirectXVersionMinor = 0;

        if (pcDirectXVersionLetter) *pcDirectXVersionLetter = TEXT(' ');
    }

    return S_OK;
}





}


namespace cactus
{

// static
int SysInfo::NumberOfProcessors()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return static_cast < int >(info.dwNumberOfProcessors);
}

// static
int64_t SysInfo::AmountOfPhysicalMemory()
{
    MEMORYSTATUSEX memory_info;
    memory_info.dwLength = sizeof(memory_info);

    if (!GlobalMemoryStatusEx(&memory_info)) {
        return 0;
    }

    int64_t rv = static_cast < int64_t >(memory_info.ullTotalPhys);

    if (rv < 0)
        rv = kint64max;

    return rv;
}

// static
int64_t SysInfo::AmountOfFreeDiskSpace(const std::wstring&path)
{
    ULARGE_INTEGER available, total, free;

    if (!GetDiskFreeSpaceExW(path.c_str(), &available, &total, &free)) {
        return -1;
    }

    int64_t rv = static_cast < int64_t >(available.QuadPart);

    if (rv < 0)
        rv = kint64max;

    return rv;
}

// static
bool SysInfo::HasEnvVar(const wchar_t *var)
{
    return GetEnvironmentVariable(var, NULL, 0) != 0;
}

// static
std::wstring SysInfo::GetEnvVar(const wchar_t *var)
{
    DWORD value_length = GetEnvironmentVariable(var, NULL, 0);

    if (value_length == 0) {
        return L"";
    }

    std::vector < wchar_t > value(value_length);
    GetEnvironmentVariable(var, &value[0], value_length);
    return std::wstring(&value[0]);
}

// static
std::string SysInfo::OperatingSystemName()
{
    return "Windows NT";
}

// static
std::string SysInfo::OperatingSystemVersion()
{
    OSVERSIONINFO info = { 0 };
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&info);
    char _val[MAX_PATH];
    sprintf_s(_val, MAX_PATH, "%lu.%lu", info.dwMajorVersion, info.dwMinorVersion);
    return std::string(_val);
}

// TODO: Implement OperatingSystemVersionComplete, which would include
// patchlevel/service pack number. See chrome/browser/views/bug_report_view.cc,
// BugReportView::SetOSVersion.

// static
std::string SysInfo::CPUArchitecture()
{
    // TODO: Make this vary when we support any other architectures.
    return "x86";
}

// static
void SysInfo::GetPrimaryDisplayDimensions(int *width, int *height)
{
    if (width)
        * width = GetSystemMetrics(SM_CXSCREEN);

    if (height)
        * height = GetSystemMetrics(SM_CYSCREEN);
}

// static
int SysInfo::DisplayCount()
{
    return GetSystemMetrics(SM_CMONITORS);
}

// static
size_t SysInfo::VMAllocationGranularity()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwAllocationGranularity;
}

// static
void SysInfo::OperatingSystemVersionNumbers(int32_t *major_version, int32_t *minor_version, int32_t *bugfix_version)
{
    OSVERSIONINFO info = { 0 };
    info.dwOSVersionInfoSize = sizeof(info);
    GetVersionEx(&info);
    * major_version = info.dwMajorVersion;
    * minor_version = info.dwMinorVersion;
    * bugfix_version = 0;
}
} // namespace cactus
