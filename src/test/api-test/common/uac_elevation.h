#ifndef uac_elevation_h__
#define uac_elevation_h__

#include <Windows.h>
#include <string>

#pragma region "Windows SDK version 6.0a definitions"

//
// These are from the Windows SDK version 6.0a or later.
// VS2005 does not define these in tis version of the SDK and does not
// reference the installed SDKs properly so define them here.
//

#if !defined BCM_SETSHIELD
    #define BCM_FIRST       0x1600      // Button control messages
    #define BCM_SETSHIELD   (BCM_FIRST + 0x000C)
#endif

#if !defined TOKEN_MANDATORY_POLICY_OFF // Only defined in SDK version 6.0a or later

//
// Token Information Class. Define new Token types not in VS2005 SDK.
//
typedef enum _DNI_TOKEN_INFORMATION_CLASS {
    TokenElevationType = MaxTokenInfoClass,
    TokenLinkedToken,
    TokenElevation,
    TokenHasRestrictions,
    TokenAccessInformation,
    TokenVirtualizationAllowed,
    TokenVirtualizationEnabled,
    TokenIntegrityLevel,
    TokenUIAccess,
    TokenMandatoryPolicy,
    TokenLogonSid
} DNI_TOKEN_INFORMATION_CLASS, *PDNI_TOKEN_INFORMATION_CLASS;

//
// Token elevation values describe the relative strength of a given token.
// A full token is a token with all groups and privileges to which the principal
// is authorized.  A limited token is one with some groups or privileges removed.
//
typedef enum _TOKEN_ELEVATION_TYPE {
    TokenElevationTypeDefault = 1,
    TokenElevationTypeFull,
    TokenElevationTypeLimited,
} TOKEN_ELEVATION_TYPE, *PTOKEN_ELEVATION_TYPE;

typedef struct _TOKEN_ELEVATION {
    DWORD TokenIsElevated;
} TOKEN_ELEVATION, *PTOKEN_ELEVATION;

#endif

#pragma endregion


namespace cactus
{
BOOL CreateWellKnownSid(WELL_KNOWN_SID_TYPE WellKnownSidType, PSID DomainSid, PSID pSid, DWORD *cbSid);
BOOL CheckTokenMembership(HANDLE TokenHandle, PSID SidToCheck, PBOOL IsMember);

bool IsUserInAdminGroup();
bool IsRunAsAdmin();
bool IsProcessElevated();
bool RestartElevated(HWND hwnd, const std::wstring& cmdline);
bool ShellElevated(HWND hwnd, const std::wstring& file, const std::wstring& directory, const std::wstring& cmdline);
bool IsElevationSupported();
}


#endif // uac_elevation_h__
