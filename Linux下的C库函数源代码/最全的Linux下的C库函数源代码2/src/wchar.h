/***
*wchar.h - declarations for wide character functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the types, macros and function declarations for
*       all wide character-related functions.  They may also be declared in
*       individual header files on a functional basis.
*       [ISO]
*
*       Note: keep in sync with ctype.h, stdio.h, stdlib.h, string.h, time.h.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */


#ifndef _INC_WCHAR
#define _INC_WCHAR

#include <crtdefs.h>

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*
 *  According to the standard, WCHAR_MIN and WCHAR_MAX need to be
 *  "constant expressions suitable for use in #if preprocessing directives,
 *  and this expression shall have the same type as would an expression that
 *  is an object of the corresponding type converted according to the integer
 *  promotions".
 */
#define WCHAR_MIN       0
#define WCHAR_MAX       0xffff

#ifndef _VA_LIST_DEFINED
#ifdef _M_CEE_PURE
typedef System::ArgIterator va_list;
#else  /* _M_CEE_PURE */
typedef char *  va_list;
#endif  /* _M_CEE_PURE */
#define _VA_LIST_DEFINED
#endif  /* _VA_LIST_DEFINED */

#ifndef WEOF
#define WEOF (wint_t)(0xFFFF)
#endif  /* WEOF */

#ifndef _FILE_DEFINED
struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif  /* _FILE_DEFINED */

/* Declare _iob[] array */

#ifndef _STDIO_DEFINED
#ifndef _INTERNAL_IFSTRIP_
/* These functions are for enabling STATIC_CPPLIB functionality */
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP extern FILE * __cdecl __p__iob(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#ifndef _M_CEE_PURE
_CRTIMP extern FILE _iob[];
#endif  /* _M_CEE_PURE */
#endif  /* _INTERNAL_IFSTRIP_ */
_CRTIMP FILE * __cdecl __iob_func(void);
#endif  /* _STDIO_DEFINED */

#ifndef _STDSTREAM_DEFINED
#define stdin  (&__iob_func()[0])
#define stdout (&__iob_func()[1])
#define stderr (&__iob_func()[2])
#define _STDSTREAM_DEFINED
#endif  /* _STDSTREAM_DEFINED */

#ifndef _FSIZE_T_DEFINED
typedef unsigned long _fsize_t; /* Could be 64 bits for Win32 */
#define _FSIZE_T_DEFINED
#endif  /* _FSIZE_T_DEFINED */

#ifndef _WFINDDATA_T_DEFINED

struct _wfinddata32_t {
        unsigned    attrib;
        __time32_t  time_create;    /* -1 for FAT file systems */
        __time32_t  time_access;    /* -1 for FAT file systems */
        __time32_t  time_write;
        _fsize_t    size;
        wchar_t     name[260];
};

#if _INTEGRAL_MAX_BITS >= 64   

struct _wfinddata32i64_t {
        unsigned    attrib;
        __time32_t  time_create;    /* -1 for FAT file systems */
        __time32_t  time_access;    /* -1 for FAT file systems */
        __time32_t  time_write;
        __int64     size;
        wchar_t     name[260];
};

struct _wfinddata64i32_t {
        unsigned    attrib;
        __time64_t  time_create;    /* -1 for FAT file systems */
        __time64_t  time_access;    /* -1 for FAT file systems */
        __time64_t  time_write;
        _fsize_t    size;
        wchar_t     name[260];
};

struct _wfinddata64_t {
        unsigned    attrib;
        __time64_t  time_create;    /* -1 for FAT file systems */
        __time64_t  time_access;    /* -1 for FAT file systems */
        __time64_t  time_write;
        __int64     size;
        wchar_t     name[260];
};

#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#ifdef _USE_32BIT_TIME_T
#define _wfinddata_t    _wfinddata32_t
#define _wfinddatai64_t _wfinddata32i64_t

#define _wfindfirst     _wfindfirst32
#define _wfindnext      _wfindnext32
#define _wfindfirsti64  _wfindfirst32i64
#define _wfindnexti64   _wfindnext32i64

#else  /* _USE_32BIT_TIME_T */
#define _wfinddata_t    _wfinddata64i32_t
#define _wfinddatai64_t _wfinddata64_t

#define _wfindfirst     _wfindfirst64i32
#define _wfindnext      _wfindnext64i32
#define _wfindfirsti64  _wfindfirst64
#define _wfindnexti64   _wfindnext64

#endif  /* _USE_32BIT_TIME_T */

#define _WFINDDATA_T_DEFINED
#endif  /* _WFINDDATA_T_DEFINED */


/* define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

#ifndef _CONST_RETURN
#ifdef __cplusplus
#define _CONST_RETURN  const
#define _CRT_CONST_CORRECT_OVERLOADS
#else  /* __cplusplus */
#define _CONST_RETURN
#endif  /* __cplusplus */
#endif  /* _CONST_RETURN */

/* For backwards compatibility */
#define _WConst_return _CONST_RETURN

#ifndef _CRT_CTYPEDATA_DEFINED
#define _CRT_CTYPEDATA_DEFINED
#ifndef _CTYPE_DISABLE_MACROS
#ifndef _INTERNAL_IFSTRIP_
extern const unsigned short __newctype[];
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP const unsigned short ** __cdecl __p__pctype(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#endif  /* _INTERNAL_IFSTRIP_ */

#ifndef __PCTYPE_FUNC
#if defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL)
#define __PCTYPE_FUNC  _pctype
#else  /* defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL) */
#define __PCTYPE_FUNC   __pctype_func()
#endif  /* defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL) */
#endif  /* __PCTYPE_FUNC */

_CRTIMP const unsigned short * __cdecl __pctype_func(void);
#if !defined (_M_CEE_PURE)
_CRTIMP extern const unsigned short *_pctype;
#else  /* !defined (_M_CEE_PURE) */
#define _pctype (__pctype_func())
#endif  /* !defined (_M_CEE_PURE) */
#endif  /* _CTYPE_DISABLE_MACROS */
#endif  /* _CRT_CTYPEDATA_DEFINED */

#ifndef _CRT_WCTYPEDATA_DEFINED
#define _CRT_WCTYPEDATA_DEFINED
#ifndef _CTYPE_DISABLE_MACROS
#if !defined (_M_CEE_PURE)
_CRTIMP extern const unsigned short _wctype[];
#endif  /* !defined (_M_CEE_PURE) */
#ifndef _INTERNAL_IFSTRIP_
extern const unsigned short __newctype[];
#if defined (_DLL) && defined (_M_IX86)
/* Retained for compatibility with VC++ 5.0 and earlier versions */
_CRTIMP const wctype_t ** __cdecl __p__pwctype(void);
#endif  /* defined (_DLL) && defined (_M_IX86) */
#endif  /* _INTERNAL_IFSTRIP_ */

_CRTIMP const wctype_t * __cdecl __pwctype_func(void);
#if !defined (_M_CEE_PURE)
_CRTIMP extern const wctype_t *_pwctype;
#else  /* !defined (_M_CEE_PURE) */
#define _pwctype (__pwctype_func())
#endif  /* !defined (_M_CEE_PURE) */
#endif  /* _CTYPE_DISABLE_MACROS */
#endif  /* _CRT_WCTYPEDATA_DEFINED */

/* set bit masks for the possible character types */

#define _UPPER          0x1     /* upper case letter */
#define _LOWER          0x2     /* lower case letter */
#define _DIGIT          0x4     /* digit[0-9] */
#define _SPACE          0x8     /* tab, carriage return, newline, */
                                /* vertical tab or form feed */
#define _PUNCT          0x10    /* punctuation character */
#define _CONTROL        0x20    /* control character */
#define _BLANK          0x40    /* space char */
#define _HEX            0x80    /* hexadecimal digit */

#define _LEADBYTE       0x8000                  /* multibyte leadbyte */
#define _ALPHA          (0x0100|_UPPER|_LOWER)  /* alphabetic character */


/* Function prototypes */

#ifndef _WCTYPE_DEFINED

/* Character classification function prototypes */
/* also declared in ctype.h */

_CRTIMP __checkReturn int __cdecl iswalpha(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswalpha_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswupper(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswupper_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswlower(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswlower_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswdigit(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswdigit_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswxdigit(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswxdigit_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswspace(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswspace_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswpunct(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswpunct_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswalnum(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswalnum_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswprint(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswprint_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswgraph(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswgraph_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswcntrl(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswcntrl_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswascii(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl isleadbyte(__in int _C);
_CRTIMP __checkReturn int __cdecl _isleadbyte_l(__in int _C, __in_opt _locale_t _Locale);

_CRTIMP __checkReturn wint_t __cdecl towupper(__in wint_t _C);
_CRTIMP __checkReturn wint_t __cdecl _towupper_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn wint_t __cdecl towlower(__in wint_t _C);
_CRTIMP __checkReturn wint_t __cdecl _towlower_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl iswctype(__in wint_t _C, __in wctype_t _Type);
_CRTIMP __checkReturn int __cdecl _iswctype_l(__in wint_t _C, __in wctype_t _Type, __in_opt _locale_t _Locale);

_CRTIMP __checkReturn int __cdecl __iswcsymf(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswcsymf_l(__in wint_t _C, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl __iswcsym(__in wint_t _C);
_CRTIMP __checkReturn int __cdecl _iswcsym_l(__in wint_t _C, __in_opt _locale_t _Locale);

_CRT_OBSOLETE(iswctype) _CRTIMP int __cdecl is_wctype(__in wint_t _C, __in wctype_t _Type);

#define _WCTYPE_DEFINED
#endif  /* _WCTYPE_DEFINED */

#ifndef _WDIRECT_DEFINED

/* also declared in direct.h */

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("_wgetcwd")
#pragma push_macro("_wgetdcwd")
#pragma push_macro("_wgetdcwd_nolock")
#undef _wgetcwd
#undef _wgetdcwd
#undef _wgetdcwd_nolock
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn __out_z_opt wchar_t * __cdecl _wgetcwd(__out_ecount_opt(_SizeInWords) wchar_t * _DstBuf, __in int _SizeInWords);
_CRTIMP __checkReturn __out_z_opt wchar_t * __cdecl _wgetdcwd(__in int _Drive, __out_ecount_opt(_SizeInWords) wchar_t * _DstBuf, __in int _SizeInWords);
__checkReturn __out_z_opt wchar_t * __cdecl _wgetdcwd_nolock(__in int _Drive, __out_ecount_opt(_SizeInWords) wchar_t * _DstBuf, __in int _SizeInWords);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("_wgetcwd")
#pragma pop_macro("_wgetdcwd")
#pragma pop_macro("_wgetdcwd_nolock")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn int __cdecl _wchdir(__in_z const wchar_t * _Path);
_CRTIMP __checkReturn int __cdecl _wmkdir(__in_z const wchar_t * _Path);
_CRTIMP __checkReturn int __cdecl _wrmdir(__in_z const wchar_t * _Path);

#define _WDIRECT_DEFINED
#endif  /* _WDIRECT_DEFINED */

#ifndef _WIO_DEFINED

_CRTIMP __checkReturn int __cdecl _waccess(__in_z const wchar_t * _Filename, __in int _AccessMode);
_CRTIMP __checkReturn_wat errno_t __cdecl _waccess_s(__in_z const wchar_t * _Filename, __in int _AccessMode);
_CRTIMP __checkReturn int __cdecl _wchmod(__in_z const wchar_t * _Filename, __in int _Mode);
_CRT_INSECURE_DEPRECATE(_wsopen_s) _CRTIMP __checkReturn int __cdecl _wcreat(__in_z const wchar_t * _Filename, __in int _PermissionMode);
_CRTIMP __checkReturn intptr_t __cdecl _wfindfirst32(__in_z const wchar_t * _Filename, __out struct _wfinddata32_t * _FindData);
_CRTIMP __checkReturn int __cdecl _wfindnext32(__in intptr_t _FindHandle, __out struct _wfinddata32_t * _FindData);
_CRTIMP __checkReturn int __cdecl _wunlink(__in_z const wchar_t * _Filename);
_CRTIMP __checkReturn int __cdecl _wrename(__in_z const wchar_t * _NewFilename, __in_z const wchar_t * _OldFilename);
_CRTIMP errno_t __cdecl _wmktemp_s(__inout_ecount_z(_SizeInWords) wchar_t * _TemplateName, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wmktemp_s, __inout_ecount(_Size) wchar_t, _TemplateName)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wmktemp, __inout_z wchar_t, _TemplateName)

#if _INTEGRAL_MAX_BITS >= 64
_CRTIMP __checkReturn intptr_t __cdecl _wfindfirst32i64(__in_z const wchar_t * _Filename, __out struct _wfinddata32i64_t * _FindData);
_CRTIMP __checkReturn intptr_t __cdecl _wfindfirst64i32(__in_z const wchar_t * _Filename, __out struct _wfinddata64i32_t * _FindData);
_CRTIMP __checkReturn intptr_t __cdecl _wfindfirst64(__in_z const wchar_t * _Filename, __out struct _wfinddata64_t * _FindData);
_CRTIMP __checkReturn int __cdecl _wfindnext32i64(__in intptr_t _FindHandle, __out struct _wfinddata32i64_t * _FindData);
_CRTIMP __checkReturn int __cdecl _wfindnext64i32(__in intptr_t _FindHandle, __out struct _wfinddata64i32_t * _FindData);
_CRTIMP __checkReturn int __cdecl _wfindnext64(__in intptr_t _FindHandle, __out struct _wfinddata64_t * _FindData);
#endif  /* _INTEGRAL_MAX_BITS >= 64 */

_CRTIMP __checkReturn_wat errno_t __cdecl _wsopen_s(__out int * _FileHandle, __in_z const wchar_t * _Filename, __in int _OpenFlag, __in int _ShareFlag, __in int _PermissionFlag);

#if !defined (__cplusplus) || !defined (_M_IX86)

_CRT_INSECURE_DEPRECATE(_wsopen_s) _CRTIMP __checkReturn int __cdecl _wopen(__in_z const wchar_t * _Filename, __in int _OpenFlag, ...);
_CRT_INSECURE_DEPRECATE(_wsopen_s) _CRTIMP __checkReturn int __cdecl _wsopen(__in_z const wchar_t * _Filename, __in int _OpenFlag, int _ShareFlag, ...);

#else  /* !defined (__cplusplus) || !defined (_M_IX86) */

/* these function do not validate pmode; use _sopen_s */
extern "C++" _CRT_INSECURE_DEPRECATE(_wsopen_s) _CRTIMP int __cdecl _wopen(__in_z const wchar_t * _Filename, __in int _OpenFlag, __in int _PermissionMode = 0);
extern "C++" _CRT_INSECURE_DEPRECATE(_wsopen_s) _CRTIMP int __cdecl _wsopen(__in_z const wchar_t * _Filename, __in int _OpenFlag, __in int _ShareFlag, int _PermissionMode = 0);

#endif  /* !defined (__cplusplus) || !defined (_M_IX86) */

#define _WIO_DEFINED
#endif  /* _WIO_DEFINED */

#ifndef _WLOCALE_DEFINED

/* wide function prototypes, also declared in wchar.h  */

_CRTIMP __checkReturn_opt wchar_t * __cdecl _wsetlocale(__in int _Category, __in_z_opt const wchar_t * _Locale);

#define _WLOCALE_DEFINED
#endif  /* _WLOCALE_DEFINED */

#ifndef _WPROCESS_DEFINED

/* also declared in process.h */

_CRTIMP intptr_t __cdecl _wexecl(__in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wexecle(__in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wexeclp(__in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wexeclpe(__in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wexecv(__in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList);
_CRTIMP intptr_t __cdecl _wexecve(__in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList,
        __in_z_opt const wchar_t * const * _Env);
_CRTIMP intptr_t __cdecl _wexecvp(__in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList);
_CRTIMP intptr_t __cdecl _wexecvpe(__in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList,
        __in_z_opt const wchar_t * const * _Env);
_CRTIMP intptr_t __cdecl _wspawnl(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wspawnle(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wspawnlp(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wspawnlpe(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * _ArgList, ...);
_CRTIMP intptr_t __cdecl _wspawnv(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList);
_CRTIMP intptr_t __cdecl _wspawnve(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList,
        __in_z_opt const wchar_t * const * _Env);
_CRTIMP intptr_t __cdecl _wspawnvp(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList);
_CRTIMP intptr_t __cdecl _wspawnvpe(__in int _Mode, __in_z const wchar_t * _Filename, __in_z const wchar_t * const * _ArgList,
        __in_z_opt const wchar_t * const * _Env);
#ifndef _CRT_WSYSTEM_DEFINED
#define _CRT_WSYSTEM_DEFINED
_CRTIMP int __cdecl _wsystem(__in_z_opt const wchar_t * _Command);
#endif  /* _CRT_WSYSTEM_DEFINED */

#define _WPROCESS_DEFINED
#endif  /* _WPROCESS_DEFINED */

#ifndef _WCTYPE_INLINE_DEFINED

#ifdef _CRTBLD
#ifndef _INTERNAL_IFSTRIP_
#define _CRT_WCTYPE_NOINLINE
#else  /* _INTERNAL_IFSTRIP_ */
#undef _CRT_WCTYPE_NOINLINE
#endif  /* _INTERNAL_IFSTRIP_ */
#endif  /* _CRTBLD */

#if !defined (__cplusplus) || defined (_M_CEE_PURE) || defined (MRTDLL) || defined (_CRT_WCTYPE_NOINLINE)
#define iswalpha(_c)    ( iswctype(_c,_ALPHA) )
#define iswupper(_c)    ( iswctype(_c,_UPPER) )
#define iswlower(_c)    ( iswctype(_c,_LOWER) )
#define iswdigit(_c)    ( iswctype(_c,_DIGIT) )
#define iswxdigit(_c)   ( iswctype(_c,_HEX) )
#define iswspace(_c)    ( iswctype(_c,_SPACE) )
#define iswpunct(_c)    ( iswctype(_c,_PUNCT) )
#define iswalnum(_c)    ( iswctype(_c,_ALPHA|_DIGIT) )
#define iswprint(_c)    ( iswctype(_c,_BLANK|_PUNCT|_ALPHA|_DIGIT) )
#define iswgraph(_c)    ( iswctype(_c,_PUNCT|_ALPHA|_DIGIT) )
#define iswcntrl(_c)    ( iswctype(_c,_CONTROL) )
#define iswascii(_c)    ( (unsigned)(_c) < 0x80 )

#define _iswalpha_l(_c,_p)    ( _iswctype_l(_c,_ALPHA, _p) )
#define _iswupper_l(_c,_p)    ( _iswctype_l(_c,_UPPER, _p) )
#define _iswlower_l(_c,_p)    ( _iswctype_l(_c,_LOWER, _p) )
#define _iswdigit_l(_c,_p)    ( _iswctype_l(_c,_DIGIT, _p) )
#define _iswxdigit_l(_c,_p)   ( _iswctype_l(_c,_HEX, _p) )
#define _iswspace_l(_c,_p)    ( _iswctype_l(_c,_SPACE, _p) )
#define _iswpunct_l(_c,_p)    ( _iswctype_l(_c,_PUNCT, _p) )
#define _iswalnum_l(_c,_p)    ( _iswctype_l(_c,_ALPHA|_DIGIT, _p) )
#define _iswprint_l(_c,_p)    ( _iswctype_l(_c,_BLANK|_PUNCT|_ALPHA|_DIGIT, _p) )
#define _iswgraph_l(_c,_p)    ( _iswctype_l(_c,_PUNCT|_ALPHA|_DIGIT, _p) )
#define _iswcntrl_l(_c,_p)    ( _iswctype_l(_c,_CONTROL, _p) )
#ifndef _CTYPE_DISABLE_MACROS
#define isleadbyte(_c)  ( __PCTYPE_FUNC[(unsigned char)(_c)] & _LEADBYTE)
#endif  /* _CTYPE_DISABLE_MACROS */

#endif  /* !defined (__cplusplus) || defined (_M_CEE_PURE) || defined (MRTDLL) || defined (_CRT_WCTYPE_NOINLINE) */
#define _WCTYPE_INLINE_DEFINED
#endif  /* _WCTYPE_INLINE_DEFINED */



/* define structure for returning status information */

#ifndef _INO_T_DEFINED
typedef unsigned short _ino_t;      /* i-node number (not used on DOS) */
#if !__STDC__
/* Non-ANSI name for compatibility */
typedef unsigned short ino_t;
#endif  /* !__STDC__ */
#define _INO_T_DEFINED
#endif  /* _INO_T_DEFINED */

#ifndef _DEV_T_DEFINED
typedef unsigned int _dev_t;        /* device code */
#if !__STDC__
/* Non-ANSI name for compatibility */
typedef unsigned int dev_t;
#endif  /* !__STDC__ */
#define _DEV_T_DEFINED
#endif  /* _DEV_T_DEFINED */

#ifndef _OFF_T_DEFINED
typedef long _off_t;                /* file offset value */
#if !__STDC__
/* Non-ANSI name for compatibility */
typedef long off_t;
#endif  /* !__STDC__ */
#define _OFF_T_DEFINED
#endif  /* _OFF_T_DEFINED */

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



#ifndef _WCONIO_DEFINED

/* wide function prototypes, also declared in conio.h */

#ifndef WEOF
#define WEOF (wint_t)(0xFFFF)
#endif  /* WEOF */

_CRTIMP __checkReturn_wat errno_t __cdecl _cgetws_s(__out_ecount_part(_SizeInWords, *_SizeRead) wchar_t * _Buffer, size_t _SizeInWords, __out size_t * _SizeRead);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _cgetws_s, __out_ecount_part(_Size, *_Buffer) wchar_t, _Buffer, size_t *, _Size)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0_CGETS(wchar_t *, _CRTIMP, _cgetws, __inout_z wchar_t, _Buffer)
_CRTIMP __checkReturn wint_t __cdecl _getwch(void);
_CRTIMP __checkReturn wint_t __cdecl _getwche(void);
_CRTIMP __checkReturn wint_t __cdecl _putwch(wchar_t _WCh);
_CRTIMP __checkReturn wint_t __cdecl _ungetwch(wint_t _WCh);
_CRTIMP __checkReturn_opt int __cdecl _cputws(__in_z const wchar_t * _String);
_CRTIMP __checkReturn_opt int __cdecl _cwprintf(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _cwprintf_s(__in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_cwscanf_s) _CRTIMP __checkReturn_opt int __cdecl _cwscanf(__in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_cwscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _cwscanf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _cwscanf_s(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _cwscanf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vcwprintf(__in_z __format_string const wchar_t *_Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vcwprintf_s(__in_z __format_string const wchar_t *_Format, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _cwprintf_p(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vcwprintf_p(__in_z __format_string const wchar_t*  _Format, va_list _ArgList);

_CRTIMP int __cdecl _cwprintf_l(__in_z const __format_string wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP int __cdecl _cwprintf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP int __cdecl _vcwprintf_l(__in_z __format_string const wchar_t *_Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP int __cdecl _vcwprintf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP int __cdecl _cwprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP int __cdecl _vcwprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

__checkReturn_opt wint_t __cdecl _putwch_nolock(wchar_t _WCh);
__checkReturn wint_t __cdecl _getwch_nolock(void);
__checkReturn wint_t __cdecl _getwche_nolock(void);
__checkReturn_opt wint_t __cdecl _ungetwch_nolock(wint_t _WCh);


#define _WCONIO_DEFINED
#endif  /* _WCONIO_DEFINED */

#ifndef _WSTDIO_DEFINED

/* wide function prototypes, also declared in stdio.h  */

#ifndef WEOF
#define WEOF (wint_t)(0xFFFF)
#endif  /* WEOF */

_CRTIMP __checkReturn FILE * __cdecl _wfsopen(__in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode, __in int _ShFlag);

_CRTIMP __checkReturn_opt wint_t __cdecl fgetwc(__inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl _fgetwchar(void);
_CRTIMP __checkReturn_opt wint_t __cdecl fputwc(__in_z wchar_t _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl _fputwchar(__in_z wchar_t _Ch);
_CRTIMP __checkReturn wint_t __cdecl getwc(__inout FILE * _File);
_CRTIMP __checkReturn wint_t __cdecl getwchar(void);
_CRTIMP __checkReturn_opt wint_t __cdecl putwc(__in_z wchar_t _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl putwchar(__in_z wchar_t _Ch);
_CRTIMP __checkReturn_opt wint_t __cdecl ungetwc(__in wint_t _Ch, __inout FILE * _File);

_CRTIMP __checkReturn_opt wchar_t * __cdecl fgetws(__out_ecount_z(_SizeInWords) wchar_t * _Dst, __in int _SizeInWords, __inout FILE * _File);
_CRTIMP __checkReturn_opt int __cdecl fputws(__in_z const wchar_t * _Str, __inout FILE * _File);
_CRTIMP __checkReturn_opt wchar_t * __cdecl _getws_s(__out_ecount_z(_SizeInWords) wchar_t * _Str, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(wchar_t *, _getws_s, __out_ecount(_Size) wchar_t, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_SAME, _CRTIMP, _getws, __out_z wchar_t, _String)
_CRTIMP __checkReturn_opt int __cdecl _putws(__in_z const wchar_t * _Str);

_CRTIMP __checkReturn_opt int __cdecl fwprintf(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl fwprintf_s(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl wprintf(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl wprintf_s(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn int __cdecl _scwprintf(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl vfwprintf(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vfwprintf_s(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vwprintf(__in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl vwprintf_s(__in_z __format_string const wchar_t * _Format, va_list _ArgList);

_CRTIMP_ALTERNATIVE int __cdecl swprintf_s(__out_ecount_z(_SizeInWords) wchar_t * _Dst, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format, ...);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1_ARGLIST(int, swprintf_s, vswprintf_s, __out_ecount(_Size) wchar_t, _Dest, __in_z __format_string const wchar_t *, _Format)
_CRTIMP_ALTERNATIVE int __cdecl vswprintf_s(__out_ecount_z(_SizeInWords) wchar_t * _Dst, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(int, vswprintf_s, __out_ecount(_Size) wchar_t, _Dest, __in_z __format_string const wchar_t *, _Format, va_list, _Args)

_CRTIMP __checkReturn_opt int __cdecl _swprintf_c(__out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vswprintf_c(__out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in_z __format_string const wchar_t * _Format, va_list _ArgList);

_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snwprintf_s(__out_ecount_z(_DstSizeInWords) wchar_t * _DstBuf, __in size_t _DstSizeInWords, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, ...);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2_ARGLIST(int, _snwprintf_s, _vsnwprintf_s, __out_ecount(_Size) wchar_t, _Dest, __in size_t, _Count, __in_z __format_string const wchar_t *, _Format)
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _vsnwprintf_s(__out_ecount_z(_DstSizeInWords) wchar_t * _DstBuf, __in size_t _DstSizeInWords, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_3(int, _vsnwprintf_s, __out_ecount(_Size) wchar_t, _Dest, __in size_t, _Count, __in_z __format_string const wchar_t *, _Format, va_list, _Args)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_ARGLIST_EX(int, __RETURN_POLICY_SAME, _CRTIMP, _snwprintf, _vsnwprintf, __out_z wchar_t, __out_ecount(_Count) wchar_t, _Dest, __in size_t, _Count, __in_z __format_string const wchar_t *, _Format)

_CRTIMP __checkReturn_opt int __cdecl _fwprintf_p(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _wprintf_p(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vfwprintf_p(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vwprintf_p(__in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _swprintf_p(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn_opt int __cdecl _vswprintf_p(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn int __cdecl _scwprintf_p(__in_z __format_string const wchar_t * _Format, ...);
_CRTIMP __checkReturn int __cdecl _vscwprintf_p(__in_z __format_string const wchar_t * _Format, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _wprintf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _wprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _wprintf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vwprintf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vwprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vwprintf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _fwprintf_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _fwprintf_p_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _fwprintf_s_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vfwprintf_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vfwprintf_p_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vfwprintf_s_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRTIMP __checkReturn_opt int __cdecl _swprintf_c_l(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _swprintf_p_l(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _swprintf_s_l(__out_ecount_z(_DstSize) wchar_t * _DstBuf, __in size_t _DstSize, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn_opt int __cdecl _vswprintf_c_l(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP __checkReturn_opt int __cdecl _vswprintf_p_l(__out_ecount_z(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _vswprintf_s_l(__out_ecount_z(_DstSize) wchar_t * _DstBuf, __in size_t _DstSize, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRTIMP __checkReturn int __cdecl _scwprintf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn int __cdecl _scwprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP __checkReturn int __cdecl _vscwprintf_p_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

_CRT_INSECURE_DEPRECATE(_snwprintf_s_l) _CRTIMP __checkReturn_opt int __cdecl _snwprintf_l(__out_ecount(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snwprintf_s_l(__out_ecount_z(_DstSize) wchar_t * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(_vsnwprintf_s_l) _CRTIMP __checkReturn_opt int __cdecl _vsnwprintf_l(__out_ecount(_MaxCount) wchar_t * _DstBuf, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _vsnwprintf_s_l(__out_ecount_z(_DstSize) wchar_t * _DstBuf, __in size_t _DstSize, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);

#ifndef _CRT_NON_CONFORMING_SWPRINTFS

#define _SWPRINTFS_DEPRECATED _CRT_DEPRECATE_TEXT("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")

#else  /* _CRT_NON_CONFORMING_SWPRINTFS */

#define _SWPRINTFS_DEPRECATED

#endif  /* _CRT_NON_CONFORMING_SWPRINTFS */

/* we could end up with a double deprecation, disable warnings 4141 and 4996 */
#pragma warning(push)
#pragma warning(disable:4141 4996)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_ARGLIST_EX(int, __RETURN_POLICY_SAME, _SWPRINTFS_DEPRECATED _CRTIMP, _swprintf, _swprintf_s, _vswprintf, vswprintf_s, __out_z wchar_t, _Dest, __in_z __format_string const wchar_t *, _Format)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_2_ARGLIST_EX(int, __RETURN_POLICY_SAME, _SWPRINTFS_DEPRECATED _CRTIMP, __swprintf_l, __vswprintf_l, _vswprintf_s_l, __out_z wchar_t, __out wchar_t, _Dest, __in_z __format_string const wchar_t *, _Format, _locale_t, _Plocinfo)
#pragma warning(pop)

#if !defined (RC_INVOKED) && !defined (__midl)
#include <swprintf.inl>
#endif  /* !defined (RC_INVOKED) && !defined (__midl) */

#ifdef _CRT_NON_CONFORMING_SWPRINTFS
#ifndef __cplusplus
#define swprintf _swprintf
#define vswprintf _vswprintf
#define _swprintf_l __swprintf_l
#define _vswprintf_l __vswprintf_l
#endif  /* __cplusplus */
#endif  /* _CRT_NON_CONFORMING_SWPRINTFS */

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("_wtempnam")
#undef _wtempnam
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn wchar_t * __cdecl _wtempnam(__in_z_opt const wchar_t * _Directory, __in_z_opt const wchar_t * _FilePrefix);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("_wtempnam")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn int __cdecl _vscwprintf(__in_z __format_string const wchar_t * _Format, va_list _ArgList);
_CRTIMP __checkReturn int __cdecl _vscwprintf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, va_list _ArgList);
_CRT_INSECURE_DEPRECATE(fwscanf_s) _CRTIMP __checkReturn_opt int __cdecl fwscanf(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_fwscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _fwscanf_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_opt int __cdecl fwscanf_s(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP __checkReturn_opt int __cdecl _fwscanf_s_l(__inout FILE * _File, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(swscanf_s) _CRTIMP __checkReturn_opt int __cdecl swscanf(__in_z const wchar_t * _Src, __in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_swscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _swscanf_l(__in_z const wchar_t * _Src, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl swscanf_s(__in_z const wchar_t *_Src, __in_z __format_string const wchar_t * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _swscanf_s_l(__in_z const wchar_t * _Src, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(_snwscanf_s) _CRTIMP __checkReturn_opt int __cdecl _snwscanf(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_snwscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _snwscanf_l(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snwscanf_s(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, ...);
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _snwscanf_s_l(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount, __in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
_CRT_INSECURE_DEPRECATE(wscanf_s) _CRTIMP __checkReturn_opt int __cdecl wscanf(__in_z __format_string const wchar_t * _Format, ...);
_CRT_INSECURE_DEPRECATE(_wscanf_s_l) _CRTIMP __checkReturn_opt int __cdecl _wscanf_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl wscanf_s(__in_z __format_string const wchar_t * _Format, ...);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP_ALTERNATIVE __checkReturn_opt int __cdecl _wscanf_s_l(__in_z __format_string const wchar_t * _Format, __in_opt _locale_t _Locale, ...);

_CRTIMP __checkReturn FILE * __cdecl _wfdopen(__in int _FileHandle , __in_z const wchar_t * _Mode);
_CRT_INSECURE_DEPRECATE(_wfopen_s) _CRTIMP __checkReturn FILE * __cdecl _wfopen(__in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode);
_CRTIMP __checkReturn_wat errno_t __cdecl _wfopen_s(__deref_out_opt FILE ** _File, __in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode);
_CRT_INSECURE_DEPRECATE(_wfreopen_s) _CRTIMP __checkReturn FILE * __cdecl _wfreopen(__in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode, __inout FILE * _OldFile);
_CRTIMP __checkReturn_wat errno_t __cdecl _wfreopen_s(__deref_out_opt FILE ** _File, __in_z const wchar_t * _Filename, __in_z const wchar_t * _Mode, __inout FILE * _OldFile);

#ifndef _CRT_WPERROR_DEFINED
#define _CRT_WPERROR_DEFINED
_CRTIMP void __cdecl _wperror(__in_z_opt const wchar_t * _ErrMsg);
#endif  /* _CRT_WPERROR_DEFINED */
_CRTIMP __checkReturn FILE * __cdecl _wpopen(__in_z const wchar_t *_Command, __in_z const wchar_t * _Mode);
_CRTIMP __checkReturn int __cdecl _wremove(__in_z const wchar_t * _Filename);
_CRTIMP __checkReturn_wat errno_t __cdecl _wtmpnam_s(__out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wtmpnam_s, __out_ecount_z(_Size) wchar_t, _Buffer)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wtmpnam, __out_z_opt wchar_t, _Buffer)

_CRTIMP __checkReturn_opt wint_t __cdecl _fgetwc_nolock(__inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl _fputwc_nolock(__in_z wchar_t _Ch, __inout FILE * _File);
_CRTIMP __checkReturn_opt wint_t __cdecl _ungetwc_nolock(__in wint_t _Ch, __inout FILE * _File);

#ifdef _CRTBLD
#ifndef _INTERNAL_IFSTRIP_
#define _CRT_GETPUTWCHAR_NOINLINE
#else  /* _INTERNAL_IFSTRIP_ */
#undef _CRT_GETPUTWCHAR_NOINLINE
#endif  /* _INTERNAL_IFSTRIP_ */
#endif  /* _CRTBLD */

#if !defined (__cplusplus) || defined (_M_CEE_PURE) || defined (_CRT_GETPUTWCHAR_NOINLINE)
#define getwchar()      fgetwc(stdin)
#define putwchar(_c)    fputwc((_c),stdout)
#else  /* !defined (__cplusplus) || defined (_M_CEE_PURE) || defined (_CRT_GETPUTWCHAR_NOINLINE) */
inline wint_t __CRTDECL getwchar()
        {return (fgetwc(stdin)); }   /* stdin */
inline wint_t __CRTDECL putwchar(wchar_t _C)
        {return (fputwc(_C, stdout)); }       /* stdout */
#endif  /* !defined (__cplusplus) || defined (_M_CEE_PURE) || defined (_CRT_GETPUTWCHAR_NOINLINE) */

#define getwc(_stm)             fgetwc(_stm)
#define putwc(_c,_stm)          fputwc(_c,_stm)
#define _putwc_nolock(_c,_stm)     _fputwc_nolock(_c,_stm)
#define _getwc_nolock(_c)          _fgetwc_nolock(_c)

#if defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL)
#define fgetwc(_stm)            _getwc_nolock(_stm)
#define fputwc(_c,_stm)         _putwc_nolock(_c,_stm)
#define ungetwc(_c,_stm)        _ungetwc_nolock(_c,_stm)
#endif  /* defined (_CRT_DISABLE_PERFCRIT_LOCKS) && !defined (_DLL) */

#define _WSTDIO_DEFINED
#endif  /* _WSTDIO_DEFINED */

#ifndef _WSTDLIB_DEFINED

/* wide function prototypes, also declared in stdlib.h  */

_CRTIMP __checkReturn_wat errno_t __cdecl _itow_s (__in int _Val, __out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in int _Radix);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_1_1(errno_t, _itow_s, __in int, _Value, __out_ecount(_Size) wchar_t, _Dest, __in int, _Radix)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_1_1(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _itow, __in int, _Value, __out_z wchar_t, _Dest, __in int, _Radix)
_CRTIMP __checkReturn_wat errno_t __cdecl _ltow_s (__in long _Val, __out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in int _Radix);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_1_1(errno_t, _ltow_s, __in long, _Value, __out_ecount(_Size) wchar_t, _Dest, __in int, _Radix)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_1_1(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _ltow, __in long, _Value, __out_z wchar_t, _Dest, __in int, _Radix)
_CRTIMP __checkReturn_wat errno_t __cdecl _ultow_s (__in unsigned long _Val, __out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in int _Radix);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_1_1(errno_t, _ultow_s, __in unsigned long, _Value, __out_ecount(_Size) wchar_t, _Dest, __in int, _Radix)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_1_1(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _ultow, __in unsigned long, _Value, __out_z wchar_t, _Dest, __in int, _Radix)
_CRTIMP __checkReturn double __cdecl wcstod(__in_z const wchar_t * _Str, __deref_opt_out_z wchar_t ** _EndPtr);
_CRTIMP __checkReturn double __cdecl _wcstod_l(__in_z const wchar_t *_Str, __deref_opt_out_z wchar_t ** _EndPtr, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn long   __cdecl wcstol(__in_z const wchar_t *_Str, __deref_opt_out_z wchar_t ** _EndPtr, int _Radix);
_CRTIMP __checkReturn long   __cdecl _wcstol_l(__in_z const wchar_t *_Str, __deref_opt_out_z wchar_t **_EndPtr, int _Radix, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned long __cdecl wcstoul(__in_z const wchar_t *_Str, __deref_opt_out_z wchar_t ** _EndPtr, int _Radix);
_CRTIMP __checkReturn unsigned long __cdecl _wcstoul_l(__in_z const wchar_t *_Str, __deref_opt_out_z wchar_t **_EndPtr, int _Radix, __in_opt _locale_t _Locale);
_CRTIMP _CRT_INSECURE_DEPRECATE(_wdupenv_s) __checkReturn wchar_t * __cdecl _wgetenv(__in_z const wchar_t * _VarName);
_CRTIMP __checkReturn_wat errno_t __cdecl _wgetenv_s(__out size_t * _ReturnSize, __out_ecount_z(_DstSizeInWords) wchar_t * _DstBuf, __in size_t _DstSizeInWords, __in_z const wchar_t * _VarName);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_1_1(errno_t, _wgetenv_s, __out size_t *, _ReturnSize, __out_ecount(_Size) wchar_t, _Dest, __in_z const wchar_t *, _VarName)
_CRTIMP __checkReturn_wat errno_t __cdecl _wdupenv_s(__deref_out_ecount_z_opt(*_BufferSizeInWords) wchar_t **_Buffer, __out_opt size_t *_BufferSizeInWords, __in_z const wchar_t *_VarName);
#ifndef _CRT_WSYSTEM_DEFINED
#define _CRT_WSYSTEM_DEFINED
_CRTIMP int __cdecl _wsystem(__in_z_opt const wchar_t * _Command);
#endif  /* _CRT_WSYSTEM_DEFINED */
_CRTIMP __checkReturn double __cdecl _wtof(__in_z const wchar_t *_Str);
_CRTIMP __checkReturn double __cdecl _wtof_l(__in_z const wchar_t *_Str, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _wtoi(__in_z const wchar_t *_Str);
_CRTIMP __checkReturn int __cdecl _wtoi_l(__in_z const wchar_t *_Str, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn long __cdecl _wtol(__in_z const wchar_t *_Str);
_CRTIMP __checkReturn long __cdecl _wtol_l(__in_z const wchar_t *_Str, __in_opt _locale_t _Locale);

#if _INTEGRAL_MAX_BITS >= 64   
_CRTIMP __checkReturn_wat errno_t __cdecl _i64tow_s(__in __int64 _Val, __out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in int _Radix);
_CRTIMP _CRT_INSECURE_DEPRECATE(_i65tow_s) wchar_t * __cdecl _i64tow(__in __int64 _Val, __out_z wchar_t * _DstBuf, __in int _Radix);
_CRTIMP __checkReturn_wat errno_t __cdecl _ui64tow_s(__in unsigned __int64 _Val, __out_ecount_z(_SizeInWords) wchar_t * _DstBuf, __in size_t _SizeInWords, __in int _Radix);
_CRTIMP _CRT_INSECURE_DEPRECATE(_ui64tow_s) wchar_t * __cdecl _ui64tow(__in unsigned __int64 _Val, __out_z wchar_t * _DstBuf, __in int _Radix);
_CRTIMP __checkReturn __int64   __cdecl _wtoi64(__in_z const wchar_t *_Str);
_CRTIMP __checkReturn __int64   __cdecl _wtoi64_l(__in_z const wchar_t *_Str, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn __int64   __cdecl _wcstoi64(__in_z const wchar_t * _Str, __deref_opt_out_z wchar_t ** _EndPtr, __in int _Radix);
_CRTIMP __checkReturn __int64   __cdecl _wcstoi64_l(__in_z const wchar_t * _Str, __deref_opt_out_z wchar_t ** _EndPtr, __in int _Radix, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned __int64  __cdecl _wcstoui64(__in_z const wchar_t * _Str, __deref_opt_out_z wchar_t ** _EndPtr, __in int _Radix);
_CRTIMP __checkReturn unsigned __int64  __cdecl _wcstoui64_l(__in_z const wchar_t *_Str , __deref_opt_out_z wchar_t ** _EndPtr, __in int _Radix, __in_opt _locale_t _Locale);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#define _WSTDLIB_DEFINED
#endif  /* _WSTDLIB_DEFINED */


#ifndef _WSTDLIBP_DEFINED

/* wide function prototypes, also declared in stdlib.h  */

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("_wfullpath")
#undef _wfullpath
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn wchar_t * __cdecl _wfullpath(__out_ecount_z_opt(_SizeInWords) wchar_t * _FullPath, __in_z const wchar_t * _Path, __in size_t _SizeInWords);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("_wfullpath")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl _wmakepath_s(__out_ecount_z(_SizeInWords) wchar_t * _PathResult, __in_opt size_t _SizeInWords, __in_z_opt const wchar_t * _Drive, __in_z_opt const wchar_t * _Dir, __in_z_opt const wchar_t * _Filename,
        __in_z_opt const wchar_t * _Ext);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_4(errno_t, _wmakepath_s, __out_ecount(_Size) wchar_t, _ResultPath, __in_z_opt const wchar_t *, _Drive, __in_z_opt const wchar_t *, _Dir, __in_z_opt const wchar_t *, _Filename, __in_z_opt const wchar_t *, _Ext)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_4(void, __RETURN_POLICY_VOID, _CRTIMP, _wmakepath, __out_z wchar_t, _ResultPath, __in_z_opt const wchar_t *, _Drive, __in_z_opt const wchar_t *, _Dir, __in_z_opt const wchar_t *, _Filename, __in_z_opt const wchar_t *, _Ext)
#ifndef _CRT_WPERROR_DEFINED
#define _CRT_WPERROR_DEFINED
_CRTIMP void __cdecl _wperror(__in_z_opt const wchar_t * _ErrMsg);
#endif  /* _CRT_WPERROR_DEFINED */
_CRTIMP __checkReturn int    __cdecl _wputenv(__in_z const wchar_t * _EnvString);
_CRTIMP __checkReturn_wat errno_t __cdecl _wputenv_s(__in_z const wchar_t * _Name, __in_z const wchar_t * _Value);
_CRTIMP errno_t __cdecl _wsearchenv_s(__in_z const wchar_t * _Filename, __in_z const wchar_t * _EnvVar, __out_ecount_z(_SizeInWords) wchar_t * _ResultPath, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_2_0(errno_t, _wsearchenv_s, __in_z const wchar_t *, _Filename, __in_z const wchar_t *, _EnvVar, __out_ecount(_Size) wchar_t, _ResultPath)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_2_0(void, __RETURN_POLICY_VOID, _CRTIMP, _wsearchenv, __in_z const wchar_t *, _Filename, __in_z const wchar_t *, _EnvVar, __out_z wchar_t, _ResultPath)
_CRT_INSECURE_DEPRECATE(_wsplitpath_s) _CRTIMP void   __cdecl _wsplitpath(__in_z const wchar_t * _FullPath, __out_z_opt wchar_t * _Drive, __out_z_opt wchar_t * _Dir, __out_z_opt wchar_t * _Filename, __out_z_opt wchar_t * _Ext);
_CRTIMP_ALTERNATIVE errno_t __cdecl _wsplitpath_s(__in_z const wchar_t * _FullPath,
                __out_ecount_z_opt(_DriveSizeInWords) wchar_t * _Drive, __in size_t _DriveSizeInWords,
                __out_ecount_z_opt(_DirSizeInWords) wchar_t * _Dir, __in size_t _DirSizeInWords,
                __out_ecount_z_opt(_FilenameSizeInWords) wchar_t * _Filename, __in size_t _FilenameSizeInWords,
                __out_ecount_z_opt(_ExtSizeInWords) wchar_t * _Ext, __in size_t _ExtSizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_SPLITPATH(errno_t, _wsplitpath_s, wchar_t, _Path)

#define _WSTDLIBP_DEFINED
#endif  /* _WSTDLIBP_DEFINED */



#ifndef _WSTRING_DEFINED

/* wide function prototypes, also declared in string.h  */

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("_wcsdup")
#undef _wcsdup
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn wchar_t * __cdecl _wcsdup(__in_z const wchar_t * _Str);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("_wcsdup")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl wcscat_s(__inout_ecount_z(_DstSize) wchar_t * _Dst, __in rsize_t _DstSize, const wchar_t * _Src);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, wcscat_s, __inout_ecount(_Size) wchar_t, _Dest, __in_z const wchar_t *, _Source)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, wcscat, __inout_z wchar_t, _Dest, __in_z const wchar_t *, _Source)
_CRTIMP __checkReturn _CONST_RETURN wchar_t * __cdecl wcschr(__in_z const wchar_t * _Str, wchar_t _Ch);
_CRTIMP __checkReturn int __cdecl wcscmp(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl wcscpy_s(__out_ecount_z(_DstSize) wchar_t * _Dst, __in rsize_t _DstSize, __in_z const wchar_t * _Src);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, wcscpy_s, __out_ecount(_Size) wchar_t, _Dest, __in_z const wchar_t *, _Source)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, wcscpy, __out_z wchar_t, _Dest, __in_z const wchar_t *, _Source)
_CRTIMP __checkReturn size_t __cdecl wcscspn(__in_z const wchar_t * _Str, __in_z const wchar_t * _Control);
_CRTIMP __checkReturn size_t __cdecl wcslen(__in_z const wchar_t * _Str);
_CRTIMP __checkReturn size_t __cdecl wcsnlen(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount);
#if __STDC_WANT_SECURE_LIB__
static __inline __checkReturn size_t __CRTDECL wcsnlen_s(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount)
{
    return wcsnlen(_Src, _MaxCount);
}
#endif  /* __STDC_WANT_SECURE_LIB__ */
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl wcsncat_s(__inout_ecount_z(_DstSize) wchar_t * _Dst, __in rsize_t _DstSize, __in_z const wchar_t * _Src, __in rsize_t _MaxCount);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, wcsncat_s, __inout_ecount_z(_Size) wchar_t, _Dest, __in_z const wchar_t *, _Source, __in size_t, _Count)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_EX(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, wcsncat, wcsncat_s, __inout_z wchar_t, __inout_ecount_z(_Count) wchar_t, _Dest, __in_z const wchar_t *, _Source, __in size_t, _Count)
_CRTIMP __checkReturn int __cdecl wcsncmp(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in size_t _MaxCount);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl wcsncpy_s(__out_ecount_z(_DstSize) wchar_t * _Dst, __in rsize_t _DstSize, __in_z const wchar_t * _Src, __in rsize_t _MaxCount);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, wcsncpy_s, __out_ecount(_Size) wchar_t, _Dest, __in_z const wchar_t *, _Source, __in size_t, _Count)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_EX(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, wcsncpy, wcsncpy_s, __out_z wchar_t, __out_ecount(_Count) wchar_t, _Dest, __in_z const wchar_t *, _Source, __in size_t, _Count)
_CRTIMP __checkReturn _CONST_RETURN wchar_t * __cdecl wcspbrk(__in_z const wchar_t * _Str, __in_z const wchar_t * _Control);
_CRTIMP __checkReturn _CONST_RETURN wchar_t * __cdecl wcsrchr(__in_z const wchar_t * _Str, __in wchar_t _Ch);
_CRTIMP __checkReturn size_t __cdecl wcsspn(__in_z const wchar_t * _Str, __in_z const wchar_t * _Control);
_CRTIMP __checkReturn _CONST_RETURN wchar_t * __cdecl wcsstr(__in_z const wchar_t * _Str, __in_z const wchar_t * _SubStr);
_CRT_INSECURE_DEPRECATE(wcstok_s) _CRTIMP __checkReturn wchar_t * __cdecl wcstok(__inout_z_opt wchar_t * _Str, __in_z const wchar_t * _Delim);
_CRTIMP_ALTERNATIVE __checkReturn wchar_t * __cdecl wcstok_s(__inout_z_opt wchar_t * _Str, __in_z const wchar_t * _Delim, __deref_inout_z_opt wchar_t ** _Context);
_CRT_INSECURE_DEPRECATE(_wcserror_s) _CRTIMP __checkReturn wchar_t * __cdecl _wcserror(__in int _ErrNum);
_CRTIMP __checkReturn_wat errno_t __cdecl _wcserror_s(__out_ecount_z_opt(_SizeInWords) wchar_t * _Buf, __in size_t _SizeInWords, __in int _ErrNum);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _wcserror_s, __out_ecount(_Size) wchar_t, _Buffer, __in int, _Error)
_CRT_INSECURE_DEPRECATE(__wcserror_s) _CRTIMP __checkReturn wchar_t * __cdecl __wcserror(__in_z_opt const wchar_t * _Str);
_CRTIMP __checkReturn_wat errno_t __cdecl __wcserror_s(__out_ecount_z_opt(_SizeInWords) wchar_t * _Buffer, __in size_t _SizeInWords, __in_z const wchar_t * _ErrMsg);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, __wcserror_s, __out_ecount(_Size) wchar_t, _Buffer, __in_z const wchar_t *, _ErrorMessage)

_CRTIMP __checkReturn int __cdecl _wcsicmp(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2);
_CRTIMP __checkReturn int __cdecl _wcsicmp_l(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _wcsnicmp(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _wcsnicmp_l(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRT_INSECURE_DEPRECATE(_wcsnset_s) _CRTIMP wchar_t * __cdecl _wcsnset(__inout_z wchar_t * _Str, __in_z wchar_t _Val, __in size_t _MaxCount);
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl _wcsnset_s(__inout_ecount_z(_DstSizeInWords) wchar_t * _Dst, __in size_t _DstSizeInWords, wchar_t _Val, __in size_t _MaxCount);
_CRTIMP wchar_t * __cdecl _wcsrev(__inout_z wchar_t * _Str);
_CRT_INSECURE_DEPRECATE(_wcsset_s) _CRTIMP wchar_t * __cdecl _wcsset(__inout_z wchar_t * _Str, wchar_t _Val);
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl _wcsset_s(__inout_ecount_z(_SizeInWords) wchar_t * _Str, __in size_t _SizeInWords, wchar_t _Val);

_CRTIMP __checkReturn_wat errno_t __cdecl _wcslwr_s(__inout_ecount_z(_SizeInWords) wchar_t * _Str, __in size_t _SizeInWords);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wcslwr_s, __inout_ecount(_Size) wchar_t, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wcslwr, __inout_z wchar_t, _String)
_CRTIMP __checkReturn_wat errno_t __cdecl _wcslwr_s_l(__inout_ecount_z(_SizeInWords) wchar_t * _Str, __in size_t _SizeInWords, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _wcslwr_s_l, __inout_ecount(_Size) wchar_t, _String, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_EX(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wcslwr_l, _wcslwr_s_l, __inout_z wchar_t, _String, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn_wat errno_t __cdecl _wcsupr_s(__inout_ecount_z(_Size) wchar_t * _Str, __in size_t _Size);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wcsupr_s, __inout_ecount(_Size) wchar_t, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wcsupr, __inout_z wchar_t, _String)
_CRTIMP __checkReturn_wat errno_t __cdecl _wcsupr_s_l(__inout_ecount_z(_Size) wchar_t * _Str, __in size_t _Size, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _wcsupr_s_l, __inout_ecount(_size) wchar_t, _String, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_EX(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wcsupr_l, _wcsupr_s_l, __inout_z wchar_t, _String, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn_opt size_t __cdecl wcsxfrm(__out_z_opt wchar_t * _Dst, __in_z const wchar_t * _Src, __in size_t _MaxCount);
_CRTIMP __checkReturn_opt size_t __cdecl _wcsxfrm_l(__out_z_opt wchar_t * _Dst, __in_z const wchar_t *_Src, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl wcscoll(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2);
_CRTIMP __checkReturn int __cdecl _wcscoll_l(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _wcsicoll(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2);
_CRTIMP __checkReturn int __cdecl _wcsicoll_l(__in_z const wchar_t * _Str1, __in_z const wchar_t *_Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _wcsncoll(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _wcsncoll_l(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _wcsnicoll(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _wcsnicoll_l(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);

#ifdef __cplusplus
#ifndef _CPP_WIDE_INLINES_DEFINED
#define _CPP_WIDE_INLINES_DEFINED
extern "C++" {
inline __checkReturn wchar_t * __CRTDECL wcschr(__in_z wchar_t *_Str, wchar_t _Ch)
        {return ((wchar_t *)wcschr((const wchar_t *)_Str, _Ch)); }
inline __checkReturn wchar_t * __CRTDECL wcspbrk(__in_z wchar_t *_Str, __in_z const wchar_t *_Control)
        {return ((wchar_t *)wcspbrk((const wchar_t *)_Str, _Control)); }
inline __checkReturn wchar_t * __CRTDECL wcsrchr(__in_z wchar_t *_Str, __in wchar_t _Ch)
        {return ((wchar_t *)wcsrchr((const wchar_t *)_Str, _Ch)); }
inline __checkReturn wchar_t * __CRTDECL wcsstr(__in_z wchar_t *_Str, __in_z const wchar_t *_SubStr)
        {return ((wchar_t *)wcsstr((const wchar_t *)_Str, _SubStr)); }
}
#endif  /* _CPP_WIDE_INLINES_DEFINED */
#endif  /* __cplusplus */

#if !__STDC__

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("wcsdup")
#undef wcsdup
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRT_NONSTDC_DEPRECATE(_wcsdup) _CRTIMP __checkReturn wchar_t * __cdecl wcsdup(__in_z const wchar_t * _Str);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("wcsdup")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

/* old names */
#define wcswcs wcsstr

/* prototypes for oldnames.lib functions */
_CRT_NONSTDC_DEPRECATE(_wcsicmp) _CRTIMP __checkReturn int __cdecl wcsicmp(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2);
_CRT_NONSTDC_DEPRECATE(_wcsnicmp) _CRTIMP __checkReturn int __cdecl wcsnicmp(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2, __in size_t _MaxCount);
_CRT_NONSTDC_DEPRECATE(_wcsnset) _CRTIMP wchar_t * __cdecl wcsnset(__inout_z wchar_t * _Str, __in_z wchar_t _Val, __in size_t _MaxCount);
_CRT_NONSTDC_DEPRECATE(_wcsrev) _CRTIMP wchar_t * __cdecl wcsrev(__inout_z wchar_t * _Str);
_CRT_NONSTDC_DEPRECATE(_wcsset) _CRTIMP wchar_t * __cdecl wcsset(__inout_z wchar_t * _Str, wchar_t _Val);
_CRT_NONSTDC_DEPRECATE(_wcslwr) _CRTIMP wchar_t * __cdecl wcslwr(__inout_z wchar_t * _Str);
_CRT_NONSTDC_DEPRECATE(_wcsupr) _CRTIMP wchar_t * __cdecl wcsupr(__inout_z wchar_t * _Str);
_CRT_NONSTDC_DEPRECATE(_wcsicoll) _CRTIMP __checkReturn int __cdecl wcsicoll(__in_z const wchar_t * _Str1, __in_z const wchar_t * _Str2);

#endif  /* !__STDC__ */

#define _WSTRING_DEFINED
#endif  /* _WSTRING_DEFINED */

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

#ifndef _WTIME_DEFINED

/* wide function prototypes, also declared in time.h */

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


typedef int mbstate_t;
typedef wchar_t _Wint_t;

_CRTIMP wint_t __cdecl btowc(int);
_CRTIMP size_t __cdecl mbrlen(__in_bcount_z_opt(_SizeInBytes) const char * _Ch, __in size_t _SizeInBytes,
                              __out_opt mbstate_t * _State);
_CRTIMP size_t __cdecl mbrtowc(__out_z_opt wchar_t * _DstCh, __in_bcount_z_opt(_SizeInBytes) const char * _SrcCh,
                               __in size_t _SizeInBytes, __out_opt mbstate_t * _State);
_CRTIMP errno_t __cdecl mbsrtowcs_s(__out_opt size_t* _Retval, __out_ecount_z_opt(_SizeInWords) wchar_t * _Dst, __in size_t _SizeInWords, __deref_inout_opt const char ** _PSrc, __in size_t _N, __out_opt mbstate_t * _State);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_1_3(errno_t, mbsrtowcs_s, __out_opt size_t *, _Retval, __out_ecount_z(_Size) wchar_t, _Dest, __deref_inout_opt const char **, _PSource, __in size_t, _Count, __out_opt mbstate_t *, _State)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_3_SIZE(_CRTIMP, mbsrtowcs, __out_z wchar_t, _Dest, __deref_inout_opt const char **, _PSrc, __in size_t, _Count, __inout_opt mbstate_t *, _State)

_CRTIMP errno_t __cdecl wcrtomb_s(__out_opt size_t * _Retval, __out_bcount_z_opt(_SizeInBytes) char * _Dst,
        __in size_t _SizeInBytes, __in_z wchar_t _Ch, __out_opt mbstate_t * _State);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_1_2(errno_t, wcrtomb_s, __out_opt size_t *, _Retval, __out_bcount_z_opt(_Size) char, _Dest, __in_z wchar_t, _Source, __out_opt mbstate_t *, _State)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_2_SIZE(_CRTIMP, wcrtomb, __out_z_opt char, _Dest, __in_z wchar_t, _Source, __out_opt mbstate_t *, _State)
_CRTIMP errno_t __cdecl wcsrtombs_s(__out_opt size_t * _Retval, __out_bcount_part_opt(_SizeInBytes, *_Retval) char * _Dst,
        __in size_t _SizeInBytes, __deref_inout_z const wchar_t ** _Src, __in size_t _Size, __out_opt mbstate_t * _State);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_1_3(errno_t, wcsrtombs_s, __out_opt size_t *, _Retval, __out_bcount_z_opt(_Size) char, _Dest, __deref_inout_z const wchar_t **, _PSrc, __in size_t, _Count, __out_opt mbstate_t *, _State)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_3_SIZE(_CRTIMP, wcsrtombs, __out_z_opt char, _Dest, __deref_inout_z const wchar_t **, _PSource, __in size_t, _Count, __out_opt mbstate_t *, _State)
_CRTIMP int __cdecl wctob(__in wint_t _WCh);

#ifndef __midl

/* memcpy and memmove are defined just for use in wmemcpy and wmemmove */
#if defined (_M_IA64)
                _CRT_INSECURE_DEPRECATE_MEMORY(memmove_s) void *  __cdecl memmove(__out_bcount_full_opt(_MaxCount) void * _Dst, __in_bcount_opt(_MaxCount) const void * _Src, __in size_t _MaxCount);
#else  /* defined (_M_IA64) */
_CRTIMP _CRT_INSECURE_DEPRECATE_MEMORY(memmove_s) void *  __cdecl memmove(__out_bcount_full_opt(_MaxCount) void * _Dst, __in_bcount_opt(_MaxCount) const void * _Src, __in size_t _MaxCount);
#endif  /* defined (_M_IA64) */
_CRT_INSECURE_DEPRECATE_MEMORY(memcpy_s) void *  __cdecl memcpy(__out_bcount_full_opt(_MaxCount) void * _Dst, __in_bcount_opt(_MaxCount) const void * _Src, __in size_t _MaxCount);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP errno_t __cdecl memcpy_s(__out_bcount_part_opt(_DstSize, _MaxCount) void * _Dst, __in rsize_t _DstSize, __in_bcount_opt(_MaxCount) const void * _Src, __in rsize_t _MaxCount);
_CRTIMP errno_t __cdecl memmove_s(__out_bcount_part_opt(_DstSize, _MaxCount) void * _Dst, __in rsize_t _DstSize, __in_bcount_opt(_MaxCount) const void * _Src, __in rsize_t _MaxCount);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__inline int __CRTDECL fwide(__in_opt FILE * _F, int _M)
        {(void)_F; return (_M); }
__inline int __CRTDECL mbsinit(__in_opt const mbstate_t *_P)
        {return (_P == NULL || *_P == 0); }
__inline _CONST_RETURN wchar_t * __CRTDECL wmemchr(__in_ecount(_N) const wchar_t *_S, __in_opt wchar_t _C, __in size_t _N)
        {for (; 0 < _N; ++_S, --_N)
                if (*_S == _C)
                        return (_CONST_RETURN wchar_t *)(_S);
        return (0); }
__inline int __CRTDECL wmemcmp(__in_ecount(_N) const wchar_t *_S1, __in_ecount(_N) const wchar_t *_S2, __in size_t _N)
        {for (; 0 < _N; ++_S1, ++_S2, --_N)
                if (*_S1 != *_S2)
                        return (*_S1 < *_S2 ? -1 : +1);
        return (0); }

__inline _CRT_INSECURE_DEPRECATE_MEMORY(wmemcpy_s) wchar_t * __CRTDECL wmemcpy(__out_ecount_opt(_N) wchar_t *_S1, __in_ecount_opt(_N) const wchar_t *_S2, __in size_t _N)
        {
#pragma warning( push )
#pragma warning( disable : 4996 6386 )
            return (wchar_t *)memcpy(_S1, _S2, _N*sizeof(wchar_t));
#pragma warning( pop )
        }

#if __STDC_WANT_SECURE_LIB__
__inline errno_t __CRTDECL wmemcpy_s(__out_ecount_part_opt(_N1, _N) wchar_t *_S1, __in rsize_t _N1, __in_ecount_opt(_N) const wchar_t *_S2, rsize_t _N)
        {
#pragma warning( push )
#pragma warning( disable : 6386 )
            return memcpy_s(_S1, _N1*sizeof(wchar_t), _S2, _N*sizeof(wchar_t));
        }
#pragma warning( pop )
#endif  /* __STDC_WANT_SECURE_LIB__ */

__inline _CRT_INSECURE_DEPRECATE_MEMORY(wmemmove_s) wchar_t * __CRTDECL wmemmove(__out_ecount_full_opt(_N) wchar_t *_S1, __in_ecount_opt(_N) const wchar_t *_S2, __in size_t _N)
        {
#pragma warning( push )
#pragma warning( disable : 4996 6386 )
#pragma warning( disable : 6387)
                        /* prefast noise VSW 493303 */
            return (wchar_t *)memmove(_S1, _S2, _N*sizeof(wchar_t));
#pragma warning( pop )
        }

#if __STDC_WANT_SECURE_LIB__
__inline errno_t __CRTDECL wmemmove_s(__out_ecount_part_opt(_N1, _N) wchar_t *_S1, __in rsize_t _N1, __in_ecount_opt(_N) const wchar_t *_S2, __in rsize_t _N)
        {
#pragma warning( push )
#pragma warning( disable : 6386 )
            return memmove_s(_S1, _N1*sizeof(wchar_t), _S2, _N*sizeof(wchar_t));
#pragma warning( pop )
        }
#endif  /* __STDC_WANT_SECURE_LIB__ */

__inline wchar_t * __CRTDECL wmemset(__out_ecount_full(_N) wchar_t *_S, __in_z wchar_t _C, __in size_t _N)
        {
            wchar_t *_Su = _S;
            for (; 0 < _N; ++_Su, --_N)
            {
                *_Su = _C;
            }
            return (_S);
        }

#ifdef __cplusplus
extern "C++" {
inline wchar_t * __CRTDECL wmemchr(__in_ecount(_N) wchar_t *_S, __in_z wchar_t _C, __in size_t _N)
        { return (wchar_t *)wmemchr((const wchar_t *)_S, _C, _N); }
}
#endif  /* __cplusplus */
#endif  /* __midl */

#ifdef __cplusplus
}       /* end of extern "C" */
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_WCHAR */
