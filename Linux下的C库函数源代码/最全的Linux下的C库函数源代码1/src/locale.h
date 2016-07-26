/***
*locale.h - definitions/declarations for localization routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the structures, values, macros, and functions
*       used by the localization routines.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_LOCALE
#define _INC_LOCALE

#include <crtdefs.h>

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

/* define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

/* Locale categories */

#define LC_ALL          0
#define LC_COLLATE      1
#define LC_CTYPE        2
#define LC_MONETARY     3
#define LC_NUMERIC      4
#define LC_TIME         5

#define LC_MIN          LC_ALL
#define LC_MAX          LC_TIME

/* Locale convention structure */

#ifndef _LCONV_DEFINED
struct lconv {
        char *decimal_point;
        char *thousands_sep;
        char *grouping;
        char *int_curr_symbol;
        char *currency_symbol;
        char *mon_decimal_point;
        char *mon_thousands_sep;
        char *mon_grouping;
        char *positive_sign;
        char *negative_sign;
        char int_frac_digits;
        char frac_digits;
        char p_cs_precedes;
        char p_sep_by_space;
        char n_cs_precedes;
        char n_sep_by_space;
        char p_sign_posn;
        char n_sign_posn;
        };
#define _LCONV_DEFINED
#endif  /* _LCONV_DEFINED */

/* ANSI: char lconv members default is CHAR_MAX which is compile time
   dependent. Defining and using _charmax here causes CRT startup code
   to initialize lconv members properly */

#ifdef _CHAR_UNSIGNED
extern int _charmax;
extern __inline int __dummy(void) { return _charmax; }
#endif  /* _CHAR_UNSIGNED */

/* function prototypes */

#ifndef _CONFIG_LOCALE_SWT
#define _ENABLE_PER_THREAD_LOCALE           0x1
#define _DISABLE_PER_THREAD_LOCALE          0x2
#define _ENABLE_PER_THREAD_LOCALE_GLOBAL    0x10
#define _DISABLE_PER_THREAD_LOCALE_GLOBAL   0x20
#define _ENABLE_PER_THREAD_LOCALE_NEW       0x100
#define _DISABLE_PER_THREAD_LOCALE_NEW      0x200
#define _CONFIG_LOCALE_SWT
#endif  /* _CONFIG_LOCALE_SWT */

_CRTIMP __checkReturn_opt int __cdecl _configthreadlocale(__in int _Flag);
_CRTIMP __checkReturn_opt char * __cdecl setlocale(__in int _Category, __in_z_opt const char * _Locale);
_CRTIMP __checkReturn_opt struct lconv * __cdecl localeconv(void);
_CRTIMP __checkReturn_opt _locale_t __cdecl _get_current_locale(void);
_CRTIMP __checkReturn_opt _locale_t __cdecl _create_locale(__in int _Category, __in_z const char * _Locale);
_CRTIMP void __cdecl _free_locale(__in_opt _locale_t _Locale);

/* use _get_current_locale, _create_locale and _free_locale, instead of these functions with double leading underscore */
_CRT_OBSOLETE(_get_current_locale) _CRTIMP __checkReturn _locale_t __cdecl __get_current_locale(void);
_CRT_OBSOLETE(_create_locale) _CRTIMP __checkReturn _locale_t __cdecl __create_locale(__in int _Category, __in_z const char * _Locale);
_CRT_OBSOLETE(_free_locale) _CRTIMP void __cdecl __free_locale(__in_opt _locale_t _Locale);

#ifndef _WLOCALE_DEFINED

/* wide function prototypes, also declared in wchar.h  */

_CRTIMP __checkReturn_opt wchar_t * __cdecl _wsetlocale(__in int _Category, __in_z_opt const wchar_t * _Locale);

#define _WLOCALE_DEFINED
#endif  /* _WLOCALE_DEFINED */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_LOCALE */
