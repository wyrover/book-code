/***
*sys/timeb.h - definition/declarations for _ftime()
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file define the _ftime() function and the types it uses.
*       [System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_TIMEB
#define _INC_TIMEB

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

/* Structure returned by _ftime system call */

#ifndef _TIMEB_DEFINED
struct __timeb32 {
        __time32_t time;
        unsigned short millitm;
        short timezone;
        short dstflag;
        };

#if !__STDC__

/* Non-ANSI name for compatibility */
struct timeb {
        time_t time;
        unsigned short millitm;
        short timezone;
        short dstflag;
        };


#endif  /* !__STDC__ */

#if _INTEGRAL_MAX_BITS >= 64   
struct __timeb64 {
        __time64_t time;
        unsigned short millitm;
        short timezone;
        short dstflag;
        };
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#ifdef _USE_32BIT_TIME_T

#define _timeb      __timeb32
#define _ftime      _ftime32
#define _ftime_s    _ftime32_s

#else  /* _USE_32BIT_TIME_T */

#define _timeb      __timeb64
#define _ftime      _ftime64
#define _ftime_s    _ftime64_s

#endif  /* _USE_32BIT_TIME_T */
#define _TIMEB_DEFINED
#endif  /* _TIMEB_DEFINED */

#include <crtdefs.h>

/* Function prototypes */

_CRT_INSECURE_DEPRECATE(_ftime32_s) _CRTIMP void __cdecl _ftime32(__out struct __timeb32 * _Time);
_CRTIMP errno_t __cdecl _ftime32_s(__out struct __timeb32 * _Time);
#if _INTEGRAL_MAX_BITS >= 64   
_CRT_INSECURE_DEPRECATE(_ftime64_s) _CRTIMP void __cdecl _ftime64(__out struct __timeb64 * _Time);
_CRTIMP errno_t __cdecl _ftime64_s(__out struct __timeb64 * _Time);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#if !defined (RC_INVOKED) && !defined (__midl)
#include <sys/timeb.inl>
#endif  /* !defined (RC_INVOKED) && !defined (__midl) */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_TIMEB */
