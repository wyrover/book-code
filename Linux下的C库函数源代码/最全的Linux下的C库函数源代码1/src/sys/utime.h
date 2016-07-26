/***
*sys/utime.h - definitions/declarations for utime()
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the structure used by the utime routine to set
*       new file access and modification times.  NOTE - MS-DOS
*       does not recognize access time, so this field will
*       always be ignored and the modification time field will be
*       used to set the new time.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#include <crtdefs.h>

#ifndef _INC_UTIME
#define _INC_UTIME

#if !defined (_WIN32)
#error ERROR: Only Win32 target supported!
#endif  /* !defined (_WIN32) */

#include <crtdefs.h>

#ifndef _CRTBLD
/* This version of the header files is NOT for user programs.
 * It is intended for use when building the C runtimes ONLY.
 * The version intended for public use will not have this message.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef _INTERNAL_IFSTRIP_
#include <cruntime.h>
#endif  /* _INTERNAL_IFSTRIP_ */


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

#if !defined (_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#define _W64
#endif  /* !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300 */
#endif  /* !defined (_W64) */

#ifdef _USE_32BIT_TIME_T
#ifdef _WIN64
#include <crtwrn.h>
#pragma _CRT_WARNING( _NO_32BIT_TIME_T )
#undef  _USE_32BIT_TIME_T
#endif  /* _WIN64 */
#else  /* _USE_32BIT_TIME_T */
#if _INTEGRAL_MAX_BITS < 64   
#define _USE_32BIT_TIME_T
#endif  /* _INTEGRAL_MAX_BITS < 64    */
#endif  /* _USE_32BIT_TIME_T */

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

/* define struct used by _utime() function */

#ifndef _UTIMBUF_DEFINED

struct _utimbuf {
        time_t actime;          /* access time */
        time_t modtime;         /* modification time */
        };

struct __utimbuf32 {
        __time32_t actime;      /* access time */
        __time32_t modtime;     /* modification time */
        };

#if _INTEGRAL_MAX_BITS >= 64   
struct __utimbuf64 {
        __time64_t actime;      /* access time */
        __time64_t modtime;     /* modification time */
        };
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#if !__STDC__
/* Non-ANSI name for compatibility */
struct utimbuf {
        time_t actime;          /* access time */
        time_t modtime;         /* modification time */
        };

struct utimbuf32 {
        __time32_t actime;      /* access time */
        __time32_t modtime;     /* modification time */
        };

#endif  /* !__STDC__ */

#define _UTIMBUF_DEFINED
#endif  /* _UTIMBUF_DEFINED */


/* Function Prototypes */

_CRTIMP int __cdecl _utime32(__in_z const char * _Filename, __in_opt struct __utimbuf32 * _Time);

_CRTIMP int __cdecl _futime32(__in int _FileDes, __in_opt struct __utimbuf32 * _Time);

/* Wide Function Prototypes */
_CRTIMP int __cdecl _wutime32(__in_z const wchar_t * _Filename, __in_opt struct __utimbuf32 * _Time);

#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP int __cdecl _utime64(__in_z const char * _Filename, __in_opt struct __utimbuf64 * _Time);
_CRTIMP int __cdecl _futime64(__in int _FileDes, __in_opt struct __utimbuf64 * _Time);
_CRTIMP int __cdecl _wutime64(__in_z const wchar_t * _Filename, __in_opt struct __utimbuf64 * _Time);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#if !defined (RC_INVOKED) && !defined (__midl)
#include <sys/utime.inl>
#endif  /* !defined (RC_INVOKED) && !defined (__midl) */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_UTIME */
