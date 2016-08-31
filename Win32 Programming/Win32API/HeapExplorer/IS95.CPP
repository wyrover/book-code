#include "stdafx.h"

BOOL Is95()
    {
     OSVERSIONINFO info;
     info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
     ::GetVersionEx(&info);
     return (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
    }

BOOL Is3()
    {
     OSVERSIONINFO info;
     info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
     ::GetVersionEx(&info);
     return (info.dwMajorVersion < 4);
    }
