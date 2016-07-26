/***
*dbgrptw.c - Debug CRT Reporting Functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/


#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#include "dbgrpt.c"

#ifdef _DEBUG

#ifdef __cplusplus

#if defined (_NATIVE_WCHAR_T_DEFINED)

extern "C++"
_CRTIMP int __cdecl _CrtDbgReportW(
        int nRptType,
        const unsigned short * szFile,
        int nLine,
        const unsigned short * szModule,
        const unsigned short * szFormat,
        ...
        )
{
    int retval;
    va_list arglist;

    va_start(arglist,szFormat);

    retval = _CrtDbgReportTV(nRptType, reinterpret_cast<const wchar_t*>(szFile), nLine, reinterpret_cast<const wchar_t*>(szModule), reinterpret_cast<const wchar_t*>(szFormat), arglist);

    va_end(arglist);

    return retval;
}

#endif  /* defined (_NATIVE_WCHAR_T_DEFINED) */

#endif  /* __cplusplus */

#endif  /* _DEBUG */


