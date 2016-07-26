/***
*awint.h - internal definitions for A&W Win32 wrapper routines.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains internal definitions/declarations for A&W wrapper functions.
*       Not included in internal.h since windows.h is required for these.
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifdef _WIN32

#ifndef _INC_AWINC
#define _INC_AWINC

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <sal.h>
#include <windows.h>

/* internal A&W routines */
struct  threadlocaleinfostruct;
typedef struct threadlocaleinfostruct * pthreadlocinfo;

int __cdecl __crtCompareStringW
(
    __in_opt _locale_t _Plocinfo,
    __in LCID     _Locale,
    __in DWORD    _DwCmpFlags,
    __in_ecount(_CchCount1) LPCWSTR  _LpString1,
    __in int      _CchCount1,
    __in_ecount(_CchCount2) LPCWSTR  _LpString2,
    __in int      _CchCount2,
    __in int      _Code_page
);

int __cdecl __crtCompareStringA
(
    __in_opt _locale_t _Plocinfo,
    __in LCID     _Locale,
    __in DWORD    _DwCmpFlags,
    __in_ecount(_CchCount1) LPCSTR   _LpString1,
    __in int      _CchCount1,
    __in_ecount(_CchCount2) LPCSTR   _LpString2,
    __in int      _CchCount2,
    __in int      _Code_page
);

int __cdecl __crtGetLocaleInfoW
(
    __in_opt _locale_t _Plocinfo,
    __in LCID    _Locale,
    __in LCTYPE  _LCType,
    __out_ecount_opt(_CchData) LPWSTR  _LpLCData,
    __in int     _CchData,
    __in int     _Code_page
);

int __cdecl __crtGetLocaleInfoA
(
    __in_opt _locale_t _Plocinfo,
    __in LCID    _Locale,
    __in LCTYPE  _LCType,
    __out_ecount_opt(_CchData) LPSTR   _LpLCData,
    __in int     _CchData,
    __in int     _Code_page
);

int __cdecl __crtLCMapStringW
(
    __in_opt _locale_t _Plocinfo,
    __in LCID _Locale,
    __in DWORD _DWMapFlag,
    __in_ecount(_CchSrc) LPCWSTR _LpSrcStr ,
    __in int _CchSrc,
    __out_ecount_opt(_CchDest) LPWSTR _LpDestStr,
    __in int _CchDest,
    __in int _Code_page
);

int __cdecl __crtLCMapStringA
(
    __in_opt _locale_t _Plocinfo,
    __in LCID _Locale,
        __in DWORD _DwMapFlag,
    __in_ecount(_CchSrc) LPCSTR _LpSrcStr,
    __in int _CchSrc,
    __out_ecount_opt(_CchDest) LPSTR _LpDestStr,
    __in int _CchDest,
    __in int _Code_page,
    __in BOOL _BError
);

BOOL __cdecl __crtGetStringTypeW
(
    __in_opt _locale_t _Plocinfo,
    __in DWORD _DWInfoType,
    __in_ecount(_CchSrc) LPCWSTR _LpSrcStr,
    __in int _CchSrc,
    __out LPWORD _LpCharType,
    __in int _Code_page,
    __in int _Lcid
);

BOOL __cdecl __crtGetStringTypeA
(
    __in_opt _locale_t _Plocinfo,
    __in DWORD _DWInfoType,
    __in LPCSTR _LpSrcStr,
    __in int _CchSrc,
    __out LPWORD _LpCharType,
    __in int _Code_page,
    __in int _Lcid,
    __in BOOL _BError
);

LPVOID __cdecl __crtGetEnvironmentStringsW(VOID);
LPVOID __cdecl __crtGetEnvironmentStringsA(VOID);

LPWSTR __cdecl __crtGetCommandLineW(VOID);

int __cdecl __crtMessageBoxA
(
    __in LPCSTR _LpText,
    __in LPCSTR _LpCaption,
    __in UINT _UType
);

int __cdecl __crtMessageBoxW
(
    __in LPCWSTR _LpText,
    __in LPCWSTR _LpCaption,
    __in UINT _UType
);


int __cdecl __crtCompareStringW_mt
(
    __in_opt _locale_t _Plocinfo,
    __in LCID     _Locale,
    __in DWORD    _DwCmpFlags,
    __in_ecount(_CchCount1) LPCWSTR  _LpString1,
    __in int      _CchCount1,
    __in_ecount(_CchCount2) LPCWSTR  _LpString2,
    __in int      _CchCount2,
    __in int      _Code_page
);

int __cdecl __crtCompareStringA_mt
(
    __in_opt _locale_t _Plocinfo,
    __in LCID     _Locale,
    __in DWORD    _DwCmpFlags,
    __in_ecount(_CchCount1) LPCSTR   _LpString1,
    __in int      _CchCount1,
    __in_ecount(_CchCount2) LPCSTR   _LpString2,
    __in int      _CchCount2,
    __in int      _Code_page
);

int __cdecl __crtLCMapStringW_mt
(
    __in_opt _locale_t _Plocinfo,
    __in LCID _Locale,
    __in DWORD _DWMapFlag,
    __in_ecount(_CchSrc) LPCWSTR _LpSrcStr ,
    __in int _CchSrc,
    __out_ecount_opt(_CchDest) LPWSTR _LpDestStr,
    __in int _CchDest,
    __in int _Code_page
);

int __cdecl __crtLCMapStringA_mt
(
    __in_opt _locale_t _Plocinfo,
    __in LCID _Locale,
        __in DWORD _DwMapFlag,
    __in_ecount(_CchSrc) LPCSTR _LpSrcStr,
    __in int _CchSrc,
    __out_ecount_opt(_CchDest) LPSTR _LpDestStr,
    __in int _CchDest,
    __in int _Code_page,
    __in BOOL _BError
);

BOOL __cdecl __crtGetStringTypeW_mt
(
    __in_opt _locale_t _Plocinfo,
    __in DWORD _DWInfoType,
    __in_ecount(_CchSrc) LPCWSTR _LpSrcStr,
    __in int _CchSrc,
    __out LPWORD _LpCharType,
    __in int _Code_page,
    __in int _Lcid
);

BOOL __cdecl __crtGetStringTypeA_mt
(
    __in_opt _locale_t _Plocinfo,
    __in DWORD _DWInfoType,
    __in LPCSTR _LpSrcStr,
    __in int _CchSrc,
    __out LPWORD _LpCharType,
    __in int _Code_page,
    __in int _Lcid,
    __in BOOL _BError
);

/* internal routines for supporting A&W routines */

int __cdecl __ansicp(__in int _Lcid);
char * __cdecl __convertcp
(
    __in int _FromCp,
    __in int _ToCp,
    __in_ecount_z(_PCchSrc) const char * _LpSrcStr,
    __inout int * _PCchSrc,
    __out_ecount_z_opt(_CchDest) char * _LpDestStr,
    __in int _CchDest
);


#define __CRTCOMPARESTRINGW_MT(ploci, Locale, dwCmpFlags,                       \
        lpString1, cchCount1, lpString2, cchCount2, code_page)                  \
    __crtCompareStringW_mt(ploci, Locale, dwCmpFlags,                           \
        lpString1, cchCount1, lpString2, cchCount2, code_page)

#define __CRTCOMPARESTRINGA_MT(ploci, Locale, dwCmpFlags,                       \
        lpString1, cchCount1, lpString2, cchCount2, code_page)                  \
    __crtCompareStringA_mt(ploci, Locale, dwCmpFlags,                           \
        lpString1, cchCount1, lpString2, cchCount2, code_page)


#define __CRTLCMAPSTRINGW_MT(ploci, Locale, dwCmpFlags,                         \
        lpString1, cchCount1, lpString2, cchCount2, code_page)                  \
    __crtLCMapStringW_mt(ploci, Locale, dwCmpFlags,                             \
        lpString1, cchCount1, lpString2, cchCount2, code_page)

#define __CRTLCMAPSTRINGA_MT(ploci, Locale, dwCmpFlags,                         \
        lpString1, cchCount1,lpString2,cchCount2, code_page, bError)            \
    __crtLCMapStringA_mt(ploci, Locale, dwCmpFlags,                             \
        lpString1, cchCount1, lpString2, cchCount2, code_page, bError)


#define __CRTGETSTRINGTYPEW_MT(ploci, dwInfoType, lpSrcStr,                     \
        cchSrc, lpCharType, code_page, lcid)                                    \
    __crtGetStringTypeW_mt(ploci, dwInfoType, lpSrcStr,                         \
        cchSrc, lpCharType, code_page, lcid)

#define __CRTGETSTRINGTYPEA_MT(ploci, dwInfoType, lpSrcStr,                     \
        cchSrc, lpCharType, code_page, lcid, bError)                            \
    __crtGetStringTypeA_mt(ploci, dwInfoType, lpSrcStr,                         \
        cchSrc, lpCharType, code_page, lcid, bError)




#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _INC_AWINC */

#endif  /* _WIN32 */
