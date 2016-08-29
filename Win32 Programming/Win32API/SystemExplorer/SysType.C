#include "stdSDK.h"
#include "systype.h"

/****************************************************************************
*                                getSystemType
* Result: WORD
*       Contains a bit indicating which system type is being used, one of:
*               SYSTYPE_API3
*               SYSTYPE_API4
*               SYSTYPE_W32s
****************************************************************************/

WORD getSystemType()
    {
     OSVERSIONINFO osver;

     osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

     if(!GetVersionEx(&osver))
        return 0;  // should not be possible...

     switch(osver.dwPlatformId)
        { /* platform */
         case VER_PLATFORM_WIN32_NT:
                 return (osver.dwMajorVersion < 4 ? SYSTYPE_API3 
                                                 : SYSTYPE_API4);
         case VER_PLATFORM_WIN32s:
                 return SYSTYPE_W32s;
         case VER_PLATFORM_WIN32_WINDOWS:
                 return SYSTYPE_API4;
         default:
                 return 0;  // unknown
        } /* platform */
                     
    }
        