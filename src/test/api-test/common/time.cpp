#include "time.h"
#include <strsafe.h>

namespace cactus
{

FILETIME ZLTime::Time642FileTime(const __time64_t& tm)
{
    FILETIME ft;
    LONGLONG ll;
    ll = tm * 10000000 + 116444736000000000;
    ft.dwLowDateTime  = (DWORD)ll;
    ft.dwHighDateTime = (DWORD)(ll >> 32);
    return ft;
}

__time64_t ZLTime::FileTime2Time64(const FILETIME& pft)
{
    __time64_t cTime = pft.dwHighDateTime;
    cTime = (cTime << 32) + pft.dwLowDateTime;
    return ((cTime - 116444736000000000) / 10000000);
}

__time64_t ZLTime::SystemTime2Time64(const SYSTEMTIME& st)
{
    FILETIME ft;
    SystemTime2FileTime(st, ft);
    return FileTime2Time64(ft);
}

BOOL ZLTime::SystemTime2FileTime(const SYSTEMTIME& syst, FILETIME& filet)
{
    return ::SystemTimeToFileTime(&syst, &filet);
}

BOOL ZLTime::FileTime2SystemTime(const FILETIME& filet, SYSTEMTIME& syst)
{
    return ::FileTimeToSystemTime(&filet, &syst);
}

SYSTEMTIME ZLTime::GetLocalTime()
{
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    return st;
}

SYSTEMTIME ZLTime::GetSystemTime()
{
    SYSTEMTIME st;
    ::GetSystemTime(&st);
    return st;
}

BOOL ZLTime::Calc(LARGE_INTEGER& llTimeBegin, double& dfTimeInterval)
{
    LARGE_INTEGER freq = {0};
    LARGE_INTEGER timeEnd = {0};

    if (llTimeBegin.QuadPart != 0
        && ::QueryPerformanceFrequency(&freq)
        && ::QueryPerformanceCounter(&timeEnd)) {
        dfTimeInterval = double(timeEnd.QuadPart - llTimeBegin.QuadPart) / freq.QuadPart * 1000.0;
        return TRUE;
    }

    return FALSE;
}

BOOL ZLTime::GetTime(LARGE_INTEGER& llCurrentTime)
{
    return ::QueryPerformanceCounter(&llCurrentTime);
}

void PrintSystemTime( PCHAR lpszBuf, DWORD cbBuf )
{
	SYSTEMTIME SysDate;
	CHAR szSysDate[128], szSysTime[65];

	GetLocalTime(&SysDate);
	GetDateFormatA(0x0409, LOCALE_USE_CP_ACP, &SysDate, "yyyy-MM-dd", szSysDate, sizeof(szSysDate));
	GetTimeFormatA(0x0409, LOCALE_USE_CP_ACP, &SysDate, "HH':'mm':'ss", szSysTime, sizeof(szSysTime));

	_snprintf(lpszBuf, cbBuf, "%s %s", szSysDate, szSysTime);
}

}