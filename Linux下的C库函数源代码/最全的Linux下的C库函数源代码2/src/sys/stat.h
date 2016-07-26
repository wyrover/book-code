/***
*sys/stat.h - defines structure used by stat() and fstat()
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the structure used by the _stat() and _fstat()
*       routines.
*       [System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_STAT
#define _INC_STAT

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


#include <sys/types.h>

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

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif  /* _WCHAR_T_DEFINED */


/* define structure for returning status information */

#ifndef _STAT_DEFINED

struct _stat32 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        __time32_t st_atime;
        __time32_t st_mtime;
        __time32_t st_ctime;
        };

#if !__STDC__
/* Non-ANSI names for compatibility */
struct stat {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };

#endif  /* !__STDC__ */

#if _INTEGRAL_MAX_BITS >= 64   

struct _stat32i64 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        __int64    st_size;
        __time32_t st_atime;
        __time32_t st_mtime;
        __time32_t st_ctime;
        };

struct _stat64i32 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;
        };

struct _stat64 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        __int64    st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;
        };

#endif  /* _INTEGRAL_MAX_BITS >= 64    */

/*
 * We have to have same name for structure and the fuction so as to do the
 * macro magic.we need the structure name and function name the same.
 */
#define __stat64    _stat64

#ifdef _USE_32BIT_TIME_T
#define _fstat      _fstat32
#define _fstati64   _fstat32i64
#define _stat       _stat32
#define _stati64    _stat32i64
#define _wstat      _wstat32
#define _wstati64   _wstat32i64

#else  /* _USE_32BIT_TIME_T */
#define _fstat      _fstat64i32
#define _fstati64   _fstat64
#define _stat       _stat64i32
#define _stati64    _stat64
#define _wstat      _wstat64i32
#define _wstati64   _wstat64

#endif  /* _USE_32BIT_TIME_T */

#define _STAT_DEFINED
#endif  /* _STAT_DEFINED */


#define _S_IFMT         0xF000          /* file type mask */
#define _S_IFDIR        0x4000          /* directory */
#define _S_IFCHR        0x2000          /* character special */
#define _S_IFIFO        0x1000          /* pipe */
#define _S_IFREG        0x8000          /* regular */
#define _S_IREAD        0x0100          /* read permission, owner */
#define _S_IWRITE       0x0080          /* write permission, owner */
#define _S_IEXEC        0x0040          /* execute/search permission, owner */


/* Function prototypes */

_CRTIMP int __cdecl _fstat32(__in int _FileDes, __out struct _stat32 * _Stat);
_CRTIMP int __cdecl _stat32(__in_z const char * _Name, __out struct _stat32 * _Stat);

#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP int __cdecl _fstat32i64(__in int _FileDes, __out struct _stat32i64 * _Stat);
_CRTIMP int __cdecl _fstat64i32(__in int _FileDes, __out struct _stat64i32 * _Stat);
_CRTIMP int __cdecl _fstat64(__in int _FileDes, __out struct _stat64 * _Stat);
_CRTIMP int __cdecl _stat32i64(__in_z const char * _Name, __out struct _stat32i64 * _Stat);
_CRTIMP int __cdecl _stat64i32(__in_z const char * _Name, __out struct _stat64i32 * _Stat);
_CRTIMP int __cdecl _stat64(__in_z const char * _Name, __out struct _stat64 * _Stat);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#ifndef _WSTAT_DEFINED

/* also declared in wchar.h */

_CRTIMP int __cdecl _wstat32(__in_z const wchar_t * _Name, __out struct _stat32 * _Stat);

#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP int __cdecl _wstat32i64(__in_z const wchar_t * _Name, __out struct _stat32i64 * _Stat);
_CRTIMP int __cdecl _wstat64i32(__in_z const wchar_t * _Name, __out struct _stat64i32 * _Stat);
_CRTIMP int __cdecl _wstat64(__in_z const wchar_t * _Name, __out struct _stat64 * _Stat);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#define _WSTAT_DEFINED
#endif  /* _WSTAT_DEFINED */

#if !__STDC__

/* Non-ANSI names for compatibility */

#define S_IFMT   _S_IFMT
#define S_IFDIR  _S_IFDIR
#define S_IFCHR  _S_IFCHR
#define S_IFREG  _S_IFREG
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC

#endif  /* !__STDC__ */

/*
 * This file is included for __inlined non stdc functions. i.e. stat and fstat
 */
#if !defined (RC_INVOKED) && !defined (__midl)
#include <sys/stat.inl>
#endif  /* !defined (RC_INVOKED) && !defined (__midl) */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_STAT */
