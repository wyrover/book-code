#include "wow64.h"

namespace cactus
{

ZLWow64::ZLWow64()
{
}

ZLWow64::~ZLWow64()
{
}

BOOL ZLWow64::CheckIsWow64Process(HANDLE hProcess, BOOL* pbWow64Process)
{
    static Fn_IsWow64Process* s_pfnIsWow64 = (Fn_IsWow64Process*)::GetProcAddress(
                ::GetModuleHandle(_T("kernel32.dll")),
                "IsWow64Process");
    return s_pfnIsWow64 ? s_pfnIsWow64(hProcess, pbWow64Process) : FALSE;
}

BOOL ZLWow64::CheckCurrentProcessIsWow64Process(BOOL* pbWow64Process)
{
    static BOOL s_bIsWow64Process = FALSE;
    static BOOL s_bResult = CheckIsWow64Process(::GetCurrentProcess(), &s_bIsWow64Process);

    if (pbWow64Process) {
        *pbWow64Process = s_bIsWow64Process;
    }

    return s_bResult;
}

BOOL ZLWow64::Wow64DisableWow64FsRedirection(PVOID* ppVoidValue)
{
    static Fn_Wow64DisableWow64FsRedirectionFunction* s_pfnWow64DisableWow64FsRedirection =
        (Fn_Wow64DisableWow64FsRedirectionFunction*)::GetProcAddress(
            ::GetModuleHandle(_T("kernel32.dll")),
            "Wow64DisableWow64FsRedirection");
    return s_pfnWow64DisableWow64FsRedirection ? s_pfnWow64DisableWow64FsRedirection(ppVoidValue) : FALSE;
}

BOOL ZLWow64::Wow64RevertWow64FsRedirection(PVOID pOldValue)
{
    static Fn_Wow64RevertWow64FsRedirectionFunction* s_pfnWow64RevertWow64FsRedirection =
        (Fn_Wow64RevertWow64FsRedirectionFunction*)::GetProcAddress(
            ::GetModuleHandle(_T("kernel32.dll")),
            "Wow64RevertWow64FsRedirection");
    return s_pfnWow64RevertWow64FsRedirection ? s_pfnWow64RevertWow64FsRedirection(pOldValue) : FALSE;
}

ZLWow64Guard::ZLWow64Guard() : bIsWow64_(FALSE)
    , pVoidValue_(NULL)
{
    if (ZLWow64::CheckCurrentProcessIsWow64Process(&bIsWow64_) && bIsWow64_) {
        ZLWow64::Wow64DisableWow64FsRedirection(&pVoidValue_);
    }
}

ZLWow64Guard::~ZLWow64Guard()
{
    if (bIsWow64_) {
        ZLWow64::Wow64RevertWow64FsRedirection(pVoidValue_);
    }
}

}