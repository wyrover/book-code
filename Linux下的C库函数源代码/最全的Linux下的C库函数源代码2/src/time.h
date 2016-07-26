/***
*time.h - definitions/declarations for time routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file has declarations of time routines and defines
*       the structure returned by the localtime and gmtime routines and
*       used by asctime.
*       [ANSI/System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_TIME
#define _INC_TIME

#include <crtdefs.h>

#if !defined (_WIN32)
#error ERROR: Only Win32 target supported!
#endif  /* !defined (_WIN32) */

#ifndef _CRTBLD
/* This version of the header files is NOT for user programs.
 * It is intended for use when building the C runtimes ONLY.
 * The version intended for public use will not have this message.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef _MSC_VER
/*
 * Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef _INTERNAL_IFSTRIP_
#include <cruntime.h>
#endif  /* _INTERNAL_IFSTRIP_ */

#if !defined (_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#define _W64
#endif  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#endif  /* !defined (_W64) */

/* Define _CRTIMP */

#ifndef _CRTIMP
#ifdef CRTDLL
#define _CRTIMP __declspec(dllexport)
#else  /* CRTDLL */
#ifdef _DLL
#define _CRTIMP __declspec(dllimport)
#else  /* _DLL */
#define _CRTIMP
#endif  /* _DLL */
#endif  /* CRTDLL */
#endif  /* _CRTIMP */


/* Define __cdecl for non-Microsoft compilers */

#if (!defined (_MSC_VER) && !defined (__cdecl))
#define __cdecl
#endif  /* (!defined (_MSC_VER) && !defined (__cdecl)) */

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif  /* _WCHAR_T_DEFINED */

#ifndef _TIME32_T_DEFINED
typedef _W64 long __time32_t;   /* 32-bit time value */
#define _TIME32_T_DEFINED
#endif  /* _TIME32_T_DEFINED */

#ifndef _TIME64_T_DEFINED
#if _INTEGRAL_MAX_BITS >= 64   
typedef __int64 __time64_t;     /* 64-bit time value */
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
#define _TIME64_T_DEFINED
#endif  /* _TIME64_T_DEFINED */

#ifndef _TIME_T_DEFINED
#ifdef _USE_32BIT_TIME_T
typedef __time32_t time_t;      /* time value */
#else  /* _USE_32BIT_TIME_T */
typedef __time64_t time_t;      /* time value */
#endif  /* _USE_32BIT_TIME_T */
#define _TIME_T_DEFINED         /* avoid multiple def's of time_t */
#endif  /* _TIME_T_DEFINED */

#ifndef _CLOCK_T_DEFINED
typedef long clock_t;
#define _CLOCK_T_DEFINED
#endif  /* _CLOCK_T_DEFINED */

#ifndef _SIZE_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64    size_t;
#else  /* _WIN64 */
typedef _W64 unsigned int   size_t;
#endif  /* _WIN64 */
#define _SIZE_T_DEFINED
#endif  /* _SIZE_T_DEFINED */

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */


#ifndef _TM_DEFINED
struct tm {
        int tm_sec;     /* seconds after the minute - [0,59] */
        int tm_min;     /* minutes after the hour - [0,59] */
        int tm_hour;    /* hours since midnight - [0,23] */
        int tm_mday;    /* day of the month - [1,31] */
        int tm_mon;     /* months since January - [0,11] */
        int tm_year;    /* years since 1900 */
        int tm_wday;    /* days since Sunday - [0,6] */
        int tm_yday;    /* days since January 1 - [0,365] */
        int tm_isdst;   /* daylight savings time flag */
        };
#define _TM_DEFINED
#endif  /* _TM_DEFINED */


/* Clock ticks macro - ANSI version */

#define CLOCKS_PER_SEC  1000


/* Extern declarations for the global variables used by the ctime family of
 * routines.
 */
#ifndef _INTERNAL_IFSTRIP_
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP int * __cdecl __p__daylight(void);
_CRTIMP long * __cdecl __p__dstbias(void);
_CRTIMP long * __cdecl __p__timezone(void);
_CRTIMP char ** __cdecl __p__tzname(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#endif  /* _INTERNAL_IFSTRIP_ */

/* non-zero if daylight savings time is used */
_CRT_INSECURE_DEPRECATE_GLOBALS(_get_daylight) _CRTIMP __checkReturn int* __cdecl __daylight(void);
#define _daylight (*__daylight())

/* offset for Daylight Saving Time */
_CRT_INSECURE_DEPRECATE_GLOBALS(_get_dstbias) _CRTIMP __checkReturn long* __cdecl __dstbias(void);
#define _dstbias (*__dstbias())

/* difference in seconds between GMT and local time */
_CRT_INSECURE_DEPRECATE_GLOBALS(_get_timezone) _CRTIMP __checkReturn long* __cdecl __timezone(void);
#define _timezone (*__timezone())

/* standard/daylight savings time zone names */
_CRT_INSECURE_DEPRECATE_GLOBALS(_get_tzname) _CRTIMP __checkReturn __deref_out_z char ** __cdecl __tzname(void);
#define _tzname (__tzname())

_CRTIMP errno_t __cdecl _get_daylight(__out int * _Daylight);
_CRTIMP errno_t __cdecl _get_dstbias(__out long * _Daylight_savings_bias);
_CRTIMP errno_t __cdecl _get_timezone(__out long * _Timezone);
_CRTIMP errno_t __cdecl _get_tzname(__out size_t *_ReturnValue, __out_ecount_z(_SizeInBytes) char *_Buffer, __in size_t _SizeInBytes, __in int _Index);

#ifndef _INTERNAL_IFSTRIP_

_DEFINE_SET_FUNCTION(_set_daylight, int, _daylight)
_DEFINE_SET_FUNCTION(_set_dstbias, long, _dstbias)
_DEFINE_SET_FUNCTION(_set_timezone, long, _timezone)

#endif  /* _INTERNAL_IFSTRIP_ */

/* Function prototypes */
_CRT_INSECURE_DEPRECATE(asctime_s) _CRTIMP __checkReturn char * __cdecl asctime(__in const struct tm * _Tm);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_wat errno_t __cdecl asctime_s(__out_ecount_part(_SizeInBytes, 26) char *_Buf, __in size_t _SizeInBytes, __in const struct tm * _Tm);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, asctime_s, __out_ecount_part(_Size,26) char, _Buffer, __in const struct tm *, _Time)

_CRT_INSECURE_DEPRECATE(_ctime32_s) _CRTIMP char * __cdecl _ctime32(__in const __time32_t * _Time);
_CRTIMP errno_t __cdecl _ctime32_s(__out_ecount_part(_SizeInBytes, 26) char *_Buf, __in size_t _SizeInBytes, __in const __time32_t *_Time);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _ctime32_s, __out_ecount_part(_Size, 26) char, _Buffer, __in const __time32_t *, _Time)

_CRTIMP __checkReturn clock_t __cdecl clock(void);
_CRTIMP double __cdecl _difftime32(__in __time32_t _Time1, __in __time32_t _Time2);

_CRT_INSECURE_DEPRECATE(_gmtime32_s) _CRTIMP __checkReturn struct tm * __cdecl _gmtime32(__in const __time32_t * _Time);
_CRTIMP __checkReturn_wat errno_t __cdecl _gmtime32_s(__in struct tm *_Tm, __in const __time32_t * _Time);

_CRT_INSECURE_DEPRECATE(_localtime32_s) _CRTIMP struct tm * __cdecl _localtime32(__in const __time32_t * _Time);
_CRTIMP errno_t __cdecl _localtime32_s(__out struct tm *_Tm, __in const __time32_t * _Time);

_CRTIMP size_t __cdecl strftime(__out_ecount_z(_SizeInBytes) char * _Buf, __in size_t _SizeInBytes, __in_z __format_string const char * _Format, __in const struct tm * _Tm);
_CRTIMP size_t __cdecl _strftime_l(__out_z char *_Buf, __in size_t _Max_size, __in_z __format_string const char * _Format, __in const struct tm *_Tm, __in_opt _locale_t _Locale);

_CRTIMP __checkReturn_wat errno_t __cdecl _strdate_s(__out_ecount_part(_SizeInBytes, 9) char *_Buf, __in size_t _SizeInBytes);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _strdate_s, __out_ecount_part(_Size, 9) char, _Buffer)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(char *, __RETURN_POLICY_DST, _CRTIMP, _strdate, __out_ecount_z(9) char, _Buffer)

_CRTIMP __checkReturn_wat errno_t __cdecl _strtime_s(__out_ecount_part(_SizeInBytes, 9) char *_Buf , __in size_t _SizeInBytes);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _strtime_s, __out_ecount_part(_Size,9) char, _Buffer)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(char *, __RETURN_POLICY_DST, _CRTIMP, _strtime, __out_ecount_z(9) char, _Buffer)

_CRTIMP __time32_t __cdecl _time32(__out_opt __time32_t * _Time);
_CRTIMP __time32_t __cdecl _mktime32(__inout struct tm * _Tm);
_CRTIMP __time32_t __cdecl _mkgmtime32(__inout struct tm * _Tm);

_CRTIMP void __cdecl _tzset(void);

#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP __checkReturn double __cdecl _difftime64(__in __time64_t _Time1, __in __time64_t _Time2);
_CRT_INSECURE_DEPRECATE(_ctime64_s) _CRTIMP char * __cdecl _ctime64(__in const __time64_t * _Time);
_CRTIMP errno_t __cdecl _ctime64_s(__out_ecount_z(_SizeInBytes) char *_Buf, __in size_t _SizeInBytes, __in const __time64_t * _Time);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _ctime64_s, __out_ecount(_Size) char, _Buffer, __in const __time64_t *, _Time)

_CRT_INSECURE_DEPRECATE(_gmtime64_s) _CRTIMP struct tm * __cdecl _gmtime64(__in const __time64_t * _Time);
_CRTIMP errno_t __cdecl _gmtime64_s(__out struct tm *_Tm, __in const __time64_t *_Time);

_CRT_INSECURE_DEPRECATE(_localtime64_s) _CRTIMP struct tm * __cdecl _localtime64(__in const __time64_t * _Time);
_CRTIMP errno_t __cdecl _localtime64_s(__out struct tm *_Tm, __in const __time64_t *_Time);

_CRTIMP __time64_t __cdecl _mktime64(__inout struct tm * _Tm);
_CRTIMP __time64_t __cdecl _mkgmtime64(__inout struct tm * _Tm);
_CRTIMP __time64_t __cdecl _time64(__out_opt __time64_t * _Time);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

/* The Win32 API GetLocalTime and SetLocalTime should be used instead. */
_CRT_OBSOLETE(GetLocalTime) unsigned __cdecl _getsystime(__out struct tm * _Tm);
_CRT_OBSOLETE(SetLocalTime) unsigned __cdecl _setsystime(__in struct tm * _Tm, unsigned _MilliSec);


#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif  /* _SIZE_T_DEFINED */

#ifndef _WTIME_DEFINED

/* wide function prototypes, also declared in wchar.h */

_CRT_INSECURE_DEPRECATE(_wasctime_s) _CRTIMP wchar_t * __cdecl _wasctime(__in const struct tm * _Tm);
_CRTIMP errno_t __cdecl _wasctime_s(__out_ecount_part(_SizeInWords, 26) wchar_t *_Buf, __in size_t _SizeInWords, __in const struct tm * _Tm);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _wasctime_s, __out_ecount_part(_Size,26) wchar_t, _Buffer, __in const struct tm *, _Time)

_CRT_INSECURE_DEPRECATE(_wctime32_s) _CRTIMP wchar_t * __cdecl _wctime32(__in const __time32_t *_Time);
_CRTIMP errno_t __cdecl _wctime32_s(__out_ecount_part(_SizeInWords, 26) wchar_t* _Buf, __in size_t _SizeInWords, __in const __time32_t * _Time);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _wctime32_s, __out_ecount_part(_Size,26) wchar_t, _Buffer, __in const __time32_t *, _Time)

_CRTIMP size_t __cdecl wcsftime(__out_ecount_z(_SizeInWords) wchar_t * _Buf, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format,  __in const struct tm * _Tm);
_CRTIMP size_t __cdecl _wcsftime_l(__out_ecount_z(_SizeInWords) wchar_t * _Buf, __in size_t _SizeInWords, __in_z __format_string const wchar_t *_Format, __in const struct tm *_Tm, __in_opt _locale_t _Locale);

_CRTIMP errno_t __cdecl _wstrdate_s(__out_ecount_part(_SizeInWords, 9) wchar_t * _Buf, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wstrdate_s, __out_ecount_part(_Size, 9) wchar_t, _Buffer)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wstrdate, __out_ecount_z(9) wchar_t, _Buffer)

_CRTIMP errno_t __cdecl _wstrtime_s(__out_ecount_part(_SizeInWords,9) wchar_t * _Buf, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wstrtime_s, __out_ecount_part(_Size, 9) wchar_t, _Buffer)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wstrtime, __out_ecount_z(9) wchar_t, _Buffer)

#if _INTEGRAL_MAX_BITS >= 64   
_CRT_INSECURE_DEPRECATE(_wctime64_s) _CRTIMP wchar_t * __cdecl _wctime64(__in const __time64_t * _Time);
_CRTIMP errno_t __cdecl _wctime64_s(__out_ecount_part(_SizeInWords, 26) wchar_t* _Buf, __in size_t _SizeInWords, __in const __time64_t *_Time);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _wctime64_s, __out_ecount_part(_Size,26) wchar_t, _Buffer, __in const __time64_t *, _Time)
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#if !defined (RC_INVOKED) && !defined (__midl)
#include <wtime.inl>
#endif  /* !defined (RC_INVOKED) && !defined (__midl) */

#define _WTIME_DEFINED
#endif  /* _WTIME_DEFINED */

#if !defined (RC_INVOKED) && !defined (__midl)
#include <time.inl>
#endif  /* !defined (RC_INVOKED) && !defined (__midl) */

#if !__STDC__

/* Non-ANSI names for compatibility */

#define CLK_TCK  CLOCKS_PER_SEC

/*
daylight, timezone, and tzname are not available under /clr:pure.
Please use _daylight, _timezone, and _tzname or
_get_daylight, _get_timezone, and _get_tzname instead.
*/
#if !defined (_M_CEE_PURE)
_CRT_INSECURE_DEPRECATE_GLOBALS(_get_daylight) _CRTIMP extern int daylight;
_CRT_INSECURE_DEPRECATE_GLOBALS(_get_timezone) _CRTIMP extern long timezone;
_CRT_INSECURE_DEPRECATE_GLOBALS(_get_tzname) _CRTIMP extern char * tzname[2];
#endif  /* !defined (_M_CEE_PURE) */

_CRT_NONSTDC_DEPRECATE(_tzset) _CRTIMP void __cdecl tzset(void);

#endif  /* !__STDC__ */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_TIME */
