#include "uac_elevation.h"
#include "path.h"
#include <ShellAPI.h>

// Dynamically load CreateWellKnownSid() as it may not be on the target system
BOOL cactus::CreateWellKnownSid(WELL_KNOWN_SID_TYPE WellKnownSidType, PSID DomainSid, PSID pSid, DWORD *cbSid)
{
    typedef BOOL (WINAPI * LPFN_CREATEWELLKNOWNSID)(WELL_KNOWN_SID_TYPE, PSID, PSID, DWORD *);
    LPFN_CREATEWELLKNOWNSID fnCreateWellKnownSid = (LPFN_CREATEWELLKNOWNSID) GetProcAddress(
                GetModuleHandle(TEXT("Advapi32")), "CreateWellKnownSid");

    if (NULL != fnCreateWellKnownSid) {
        return fnCreateWellKnownSid(WellKnownSidType, DomainSid, pSid, cbSid);
    }

    return TRUE;
}

// Dynamically load CheckTokenMembership() as it may not be on the target system
BOOL cactus::CheckTokenMembership(HANDLE TokenHandle, PSID SidToCheck, PBOOL IsMember)
{
    typedef BOOL (WINAPI * LPFN_CHECKTOKENMEMBERSHIP)(HANDLE, PSID, PBOOL);
    LPFN_CHECKTOKENMEMBERSHIP fnCheckTokenMembership = (LPFN_CHECKTOKENMEMBERSHIP) GetProcAddress(
                GetModuleHandle(TEXT("Advapi32")), "CheckTokenMembership");

    if (NULL != fnCheckTokenMembership) {
        return fnCheckTokenMembership(TokenHandle, SidToCheck, IsMember);
    }

    return TRUE;
}

// Smart pointer for PSID
struct sid_free {
    static void close(PSID pSid)
    {
        ::FreeSid(pSid);
    }
};

// typedef auto_any<PSID, sid_free> auto_psid;

#pragma region "Helper Functions for Admin Privileges and Elevation Status"

//
//   FUNCTION: IsUserInAdminGroup()
//
//   PURPOSE: The function checks whether the primary access token of the
//   process belongs to user account that is a member of the local
//   Administrators group, even if it currently is not elevated.
//
//   RETURN VALUE: Returns TRUE if the primary access token of the process
//   belongs to user account that is a member of the local Administrators
//   group. Returns FALSE if the token does not.
//
//   EXCEPTION: If this function fails, it throws a C++ std::exception.
//
bool cactus::IsUserInAdminGroup()
{
    BOOL fInAdminGroup = FALSE;
    HANDLE hToken = NULL;
    HANDLE hTokenToCheck = NULL;
    DWORD cbSize = 0;
    OSVERSIONINFO osver = { sizeof(osver) };
    // Open the primary access token of the process for query and duplicate.
    ::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &hToken);
    //auto_handle hToken_handle(hToken);
    // Determine whether system is running Windows Vista or later operating
    // systems (major version >= 6) because they support linked tokens, but
    // previous versions (major version < 6) do not.
    ::GetVersionEx(&osver);

    if (osver.dwMajorVersion >= 6) {
        // Running Windows Vista or later (major version >= 6).
        // Determine token type: limited, elevated, or default.
        TOKEN_ELEVATION_TYPE elevType;
        ::GetTokenInformation(hToken, (TOKEN_INFORMATION_CLASS)TokenElevationType, &elevType, sizeof(elevType), &cbSize);

        // If limited, get the linked elevated token for further check.
        if (TokenElevationTypeLimited == elevType) {
            ::GetTokenInformation(hToken, (TOKEN_INFORMATION_CLASS)TokenLinkedToken, &hTokenToCheck, sizeof(hTokenToCheck), &cbSize);
        }
    }

    // CheckTokenMembership requires an impersonation token. If we just got a
    // linked token, it already is an impersonation token.  If we did not get
    // a linked token, duplicate the original into an impersonation token for
    // CheckTokenMembership.
    if (!hTokenToCheck) {
        ::DuplicateToken(hToken, SecurityIdentification, &hTokenToCheck);
    }

    //auto_handle hTokenToCheck_handle(hTokenToCheck);
    // Create the SID corresponding to the Administrators group.
    BYTE adminSID[SECURITY_MAX_SID_SIZE];
    cbSize = sizeof(adminSID);
    cactus::CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &cbSize);
    // Check if the token to be checked contains admin SID.
    // http://msdn.microsoft.com/en-us/library/aa379596(VS.85).aspx:
    // To determine whether a SID is enabled in a token, that is, whether it
    // has the SE_GROUP_ENABLED attribute, call CheckTokenMembership.
    CheckTokenMembership(hTokenToCheck, &adminSID, &fInAdminGroup);

    if (hTokenToCheck)
        ::CloseHandle(hTokenToCheck);

    if (hToken)
        ::CloseHandle(hToken);

    return fInAdminGroup ? true : false;
}

//
//   FUNCTION: IsRunAsAdmin()
//
//   PURPOSE: The function checks whether the current process is run as
//   administrator. In other words, it dictates whether the primary access
//   token of the process belongs to user account that is a member of the
//   local Administrators group and it is elevated.
//
//   RETURN VALUE: Returns TRUE if the primary access token of the process
//   belongs to user account that is a member of the local Administrators
//   group and it is elevated. Returns FALSE if the token does not.
//
//   EXCEPTION: If this function fails, it throws a C++ std::exception.
//
bool cactus::IsRunAsAdmin()
{
    BOOL fIsRunAsAdmin = FALSE;
    PSID pAdministratorsGroup = NULL;
    // Allocate and initialize a SID of the administrators group.
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    ::AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &pAdministratorsGroup);
    //auto_psid psid_ptr(pAdministratorsGroup);
    // Determine whether the SID of administrators group is enabled in
    // the primary access token of the process.
    CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin);

    if (pAdministratorsGroup)
        ::FreeSid(pAdministratorsGroup);

    return fIsRunAsAdmin ? true : false;
}

//
//   FUNCTION: IsProcessElevated()
//
//   PURPOSE: The function gets the elevation information of the current
//   process. It dictates whether the process is elevated or not. Token
//   elevation is only available on Windows Vista and newer operating
//   systems, thus IsProcessElevated throws a C++ exception if it is called
//   on systems prior to Windows Vista. It is not appropriate to use this
//   function to determine whether a process is run as administartor.
//
//   RETURN VALUE: Returns TRUE if the process is elevated. Returns FALSE if
//   it is not.
//
//   EXCEPTION: If this function fails, it throws a C++ std::exception.
//   For example, if IsProcessElevated is called on systems prior to
//   Windows Vista, the error code will be ERROR_INVALID_PARAMETER.
//
//   NOTE: TOKEN_INFORMATION_CLASS provides TokenElevationType to check the
//   elevation type (TokenElevationTypeDefault / TokenElevationTypeLimited /
//   TokenElevationTypeFull) of the process. It is different from
//   TokenElevation in that, when UAC is turned off, elevation type always
//   returns TokenElevationTypeDefault even though the process is elevated
//   (Integrity Level == High). In other words, it is not safe to say if the
//   process is elevated based on elevation type. Instead, we should use
//   TokenElevation.
//
bool cactus::IsProcessElevated()
{
    HANDLE hToken = NULL;
    // Open the primary access token of the process with TOKEN_QUERY.
    ::OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
    //auto_handle hToken_handle(hToken);
    // Retrieve token elevation information.
    TOKEN_ELEVATION elevation = { 0 };
    DWORD dwSize = 0;
    // When the process is run on operating systems prior to Windows Vista, GetTokenInformation returns FALSE with the
    // ERROR_INVALID_PARAMETER error code because TokenElevation is not supported on those operating systems.
    ::GetTokenInformation(hToken, (TOKEN_INFORMATION_CLASS)TokenElevation, &elevation, sizeof(elevation), &dwSize);

    if (hToken)
        ::CloseHandle(hToken);

    return 0 != elevation.TokenIsElevated;
}

#pragma endregion

bool cactus::IsElevationSupported()
{
    OSVERSIONINFO osver = { sizeof(osver) };
    return GetVersionEx(&osver) && osver.dwMajorVersion >= 6;
}

//
// Restart the current process elevated.
//
bool cactus::RestartElevated(HWND hwnd, const std::wstring& cmdline)
{
    return ShellElevated(hwnd, cactus::GetModuleFileNameW(), cactus::GetModuleDirectoryW(), cmdline);
}

//
// Run process elevated.
// Returns true is the process was started or false if the user refused elevation.
//
bool cactus::ShellElevated(HWND hwnd, const std::wstring& file, const std::wstring& directory, const std::wstring& cmdline)
{
    OSVERSIONINFO osver = { sizeof(osver) };
    GetVersionEx(&osver);
    // Relaunch installation as administrator.
    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = file.c_str();
    sei.lpDirectory = directory.c_str();
    sei.lpParameters = cmdline.c_str();
    sei.hwnd = hwnd;
    sei.nShow = SW_NORMAL;

    if (! ShellExecuteExW(&sei)) {
        DWORD dwErr = ::GetLastError();

        // The user refused the elevation
        if (dwErr == ERROR_CANCELLED) {
            return false;
        }

        // Other error
        //CHECK_WIN32_DWORD(dwErr, L"ShellExecuteExW");
        // Not really required but fixes warning
        return false;
    } else {
        return true;
    }
}