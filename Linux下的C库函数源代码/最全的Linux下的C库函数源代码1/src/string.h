/***
*string.h - declarations for string manipulation functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the function declarations for the string
*       manipulation functions.
*       [ANSI/System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_STRING
#define _INC_STRING

#include <crtdefs.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef _NLSCMP_DEFINED
#define _NLSCMPERROR    2147483647  /* currently == INT_MAX */
#define _NLSCMP_DEFINED
#endif  /* _NLSCMP_DEFINED */

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

/* For backwards compatibility */
#define _WConst_return _CONST_RETURN

/* Function prototypes */
#ifndef _CRT_MEMORY_DEFINED
#define _CRT_MEMORY_DEFINED
_CRTIMP void *  __cdecl _memccpy( __out_bcount_opt(_MaxCount) void * _Dst, __in const void * _Src, __in int _Val, __in size_t _MaxCount);
_CRTIMP __checkReturn _CONST_RETURN void *  __cdecl memchr( __in_bcount_opt(_MaxCount) const void * _Buf , __in int _Val, __in size_t _MaxCount);
_CRTIMP __checkReturn int     __cdecl _memicmp(__in_bcount_opt(_Size) const void * _Buf1, __in_bcount_opt(_Size) const void * _Buf2, __in size_t _Size);
_CRTIMP __checkReturn int     __cdecl _memicmp_l(__in_bcount_opt(_Size) const void * _Buf1, __in_bcount_opt(_Size) const void * _Buf2, __in size_t _Size, __in_opt _locale_t _Locale);
        __checkReturn int     __cdecl memcmp(__in_bcount_opt(_Size) const void * _Buf1, __in_bcount_opt(_Size) const void * _Buf2, __in size_t _Size);
        _CRT_INSECURE_DEPRECATE_MEMORY(memcpy_s) void *  __cdecl memcpy(__out_bcount_full_opt(_Size) void * _Dst, __in_bcount_opt(_Size) const void * _Src, __in size_t _Size);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP errno_t  __cdecl memcpy_s(__out_bcount_part_opt(_DstSize, _MaxCount) void * _Dst, __in rsize_t _DstSize, __in_bcount_opt(_MaxCount) const void * _Src, __in rsize_t _MaxCount);
#endif  /* __STDC_WANT_SECURE_LIB__ */
        void *  __cdecl memset(__out_bcount_full_opt(_Size) void * _Dst, __in int _Val, __in size_t _Size);

#if !__STDC__
/* Non-ANSI names for compatibility */
_CRT_NONSTDC_DEPRECATE(_memccpy) _CRTIMP void * __cdecl memccpy(__out_bcount_opt(_Size) void * _Dst, __in_bcount_opt(_Size) const void * _Src, __in int _Val, __in size_t _Size);
_CRT_NONSTDC_DEPRECATE(_memicmp) _CRTIMP __checkReturn int __cdecl memicmp(__in_bcount_opt(_Size) const void * _Buf1, __in_bcount_opt(_Size) const void * _Buf2, __in size_t _Size);
#endif  /* !__STDC__ */

#endif  /* _CRT_MEMORY_DEFINED */

        _CRT_INSECURE_DEPRECATE(_strset_s) char *  __cdecl _strset(__inout_z char * _Str, __in int _Val);
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl _strset_s(__inout_ecount_z(_DstSize) char * _Dst, __in size_t _DstSize, __in int _Value);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl strcpy_s(__out_ecount_z(_DstSize) char * _Dst, __in rsize_t _DstSize, __in_z const char * _Src);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, strcpy_s, __out_ecount_z(_Size) char, _Dest, __in_z const char *, _Source)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1(char *, __RETURN_POLICY_DST, __EMPTY_DECLSPEC, strcpy, __out_z char, _Dest, __in_z const char *, _Source)
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl strcat_s(__inout_ecount_z(_DstSize) char * _Dst, __in rsize_t _DstSize, __in_z const char * _Src);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, strcat_s, __inout_ecount_z(_Size) char, _Dest, __in_z const char *, _Source)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1(char *, __RETURN_POLICY_DST, __EMPTY_DECLSPEC, strcat, __inout_z char, _Dest, __in_z const char *, _Source)
        __checkReturn int     __cdecl strcmp(__in_z const char * _Str1, __in_z const char * _Str2);
        __checkReturn size_t  __cdecl strlen(__in_z  const char * _Str);
_CRTIMP __checkReturn size_t  __cdecl strnlen(__in_ecount_z(_MaxCount)  const char * _Str, __in size_t _MaxCount);
#if __STDC_WANT_SECURE_LIB__ && !defined (__midl)
static __inline __checkReturn size_t  __CRTDECL strnlen_s(__in_ecount_z(_MaxCount)  const char * _Str, __in size_t _MaxCount)
{
    return strnlen(_Str, _MaxCount);
}
#endif  /* __STDC_WANT_SECURE_LIB__ && !defined (__midl) */
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_wat errno_t __cdecl memmove_s(__out_bcount_part_opt(_DstSize,_MaxCount) void * _Dst, __in rsize_t _DstSize, __in_bcount_opt(_MaxCount) const void * _Src, __in rsize_t _MaxCount);
#endif  /* __STDC_WANT_SECURE_LIB__ */

#if defined (_M_IA64)
        _CRT_INSECURE_DEPRECATE_MEMORY(memmove_s) void *  __cdecl memmove(__out_bcount_full_opt(_Size) void * _Dst, __in_bcount_opt(_Size) const void * _Src, __in size_t _Size);
#else  /* defined (_M_IA64) */
_CRTIMP _CRT_INSECURE_DEPRECATE_MEMORY(memmove_s) void *  __cdecl memmove(__out_bcount_full_opt(_Size) void * _Dst, __in_bcount_opt(_Size) const void * _Src, __in size_t _Size);
#endif  /* defined (_M_IA64) */

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("_strdup")
#undef _strdup
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn char *  __cdecl _strdup(__in_z_opt const char * _Src);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("_strdup")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn _CONST_RETURN char *  __cdecl strchr(__in_z const char * _Str, __in int _Val);
_CRTIMP __checkReturn int     __cdecl _stricmp(__in_z  const char * _Str1, __in_z  const char * _Str2);
_CRTIMP __checkReturn int     __cdecl _strcmpi(__in_z  const char * _Str1, __in_z  const char * _Str2);
_CRTIMP __checkReturn int     __cdecl _stricmp_l(__in_z  const char * _Str1, __in_z  const char * _Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int     __cdecl strcoll(__in_z  const char * _Str1, __in_z  const  char * _Str2);
_CRTIMP __checkReturn int     __cdecl _strcoll_l(__in_z  const char * _Str1, __in_z  const char * _Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int     __cdecl _stricoll(__in_z  const char * _Str1, __in_z  const char * _Str2);
_CRTIMP __checkReturn int     __cdecl _stricoll_l(__in_z  const char * _Str1, __in_z  const char * _Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int     __cdecl _strncoll  (__in_z const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int     __cdecl _strncoll_l(__in_z const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int     __cdecl _strnicoll (__in_z const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int     __cdecl _strnicoll_l(__in_z const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn size_t  __cdecl strcspn(__in_z  const char * _Str, __in_z  const char * _Control);
_CRT_INSECURE_DEPRECATE(_strerror_s) _CRTIMP __checkReturn char *  __cdecl _strerror(__in_z_opt const char * _ErrMsg);
_CRTIMP __checkReturn_wat errno_t __cdecl _strerror_s(__out_ecount_z(_SizeInBytes) char * _Buf, __in size_t _SizeInBytes, __in_z_opt const char * _ErrMsg);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _strerror_s, __out_ecount(_Size) char, _Buffer, __in_z_opt const char *, _ErrorMessage)
_CRT_INSECURE_DEPRECATE(strerror_s) _CRTIMP __checkReturn char *  __cdecl strerror(__in int);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn_wat errno_t __cdecl strerror_s(__out_ecount_z(_SizeInBytes) char * _Buf, __in size_t _SizeInBytes, __in int _ErrNum);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, strerror_s, __out_ecount(_Size) char, _Buffer, __in int, _ErrorMessage)
_CRTIMP __checkReturn_wat errno_t __cdecl _strlwr_s(__inout_ecount_z(_Size) char * _Str, __in size_t _Size);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _strlwr_s, __inout_ecount_z(_Size) char, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(char *, __RETURN_POLICY_DST, _CRTIMP, _strlwr, __inout_z char, _String)
_CRTIMP __checkReturn_wat errno_t __cdecl _strlwr_s_l(__inout_ecount_z(_Size) char * _Str, __in size_t _Size, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _strlwr_s_l, __inout_ecount_z(_Size) char, _String, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_EX(char *, __RETURN_POLICY_DST, _CRTIMP, _strlwr_l, _strlwr_s_l, __inout_z char, _String, __in_opt _locale_t, _Locale)
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl strncat_s(__inout_ecount_z(_DstSize) char * _Dst, __in rsize_t _DstSize, __in_z const char * _Src, __in rsize_t _MaxCount);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, strncat_s, __inout_ecount_z(_Size) char, _Dest, __in_z const char *, _Source, __in size_t, _Count)
#pragma warning(push)
#pragma warning(disable:4609 6059)
/* prefast noise VSW 489802 */
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_EX(char *, __RETURN_POLICY_DST, _CRTIMP, strncat, strncat_s, __inout_z char, __inout_ecount_z(_Count) char, _Dest, __in_z const char *, _Source, __in size_t, _Count)
#pragma warning(pop)
#if defined (_M_IA64)
        __checkReturn int     __cdecl strncmp(__in_z  const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount);
#else  /* defined (_M_IA64) */
_CRTIMP __checkReturn int     __cdecl strncmp(__in_z const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount);
#endif  /* defined (_M_IA64) */
_CRTIMP __checkReturn int     __cdecl _strnicmp(__in_z const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int     __cdecl _strnicmp_l(__in_z const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl strncpy_s(__out_ecount_z(_DstSize) char * _Dst, __in rsize_t _DstSize, __in_z_opt const char * _Src, __in rsize_t _MaxCount);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, strncpy_s, __out_ecount(_Size) char, _Dest, __in_z const char *, _Source, __in size_t, _Count)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_EX(char *, __RETURN_POLICY_DST, _CRTIMP_NOIA64, strncpy, strncpy_s, __out_z char, __out_ecount(_Count) char, _Dest, __in_z const char *, _Source, __in size_t, _Count)
_CRT_INSECURE_DEPRECATE(_strnset_s) _CRTIMP char *  __cdecl _strnset(__inout_z char * _Str, __in int _Val, __in size_t _MaxCount);
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl _strnset_s(__inout_ecount_z(_Size) char * _Str, __in size_t _Size, __in int _Val, __in size_t _MaxCount);
_CRTIMP __checkReturn _CONST_RETURN char *  __cdecl strpbrk(__in_z const char * _Str, __in_z const char * _Control);
_CRTIMP __checkReturn _CONST_RETURN char *  __cdecl strrchr(__in_z const char * _Str, __in int _Ch);
_CRTIMP char *  __cdecl _strrev(__inout_z char * _Str);
_CRTIMP __checkReturn size_t  __cdecl strspn(__in_z const char * _Str, __in_z const char * _Control);
_CRTIMP __checkReturn _CONST_RETURN char *  __cdecl strstr(__in_z const char * _Str, __in_z const char * _SubStr);
_CRT_INSECURE_DEPRECATE(strtok_s) _CRTIMP __checkReturn char *  __cdecl strtok(__inout_z_opt char * _Str, __in_z const char * _Delim);
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn char *  __cdecl strtok_s(__inout_z_opt char * _Str, __in_z const char * _Delim, __deref_inout_z_opt char ** _Context);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP __checkReturn_wat errno_t __cdecl _strupr_s(__inout_ecount_z(_Size) char * _Str, __in size_t _Size);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _strupr_s, __inout_ecount_z(_Size) char, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(char *, __RETURN_POLICY_DST, _CRTIMP, _strupr, __inout_z char, _String)
_CRTIMP __checkReturn_wat errno_t __cdecl _strupr_s_l(__inout_ecount_z(_Size) char * _Str, __in size_t _Size, _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _strupr_s_l, __inout_ecount_z(_Size) char, _String, _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_EX(char *, __RETURN_POLICY_DST, _CRTIMP, _strupr_l, _strupr_s_l, __inout_z char, _String, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn_opt size_t  __cdecl strxfrm (__out_z_opt char * _Dst, __in_z const char * _Src, __in size_t _MaxCount);
_CRTIMP __checkReturn_opt size_t  __cdecl _strxfrm_l(__out_z_opt char * _Dst, __in_z const char * _Src, __in size_t _MaxCount, __in_opt _locale_t _Locale);

#ifdef __cplusplus
extern "C++" {
#ifndef _CPP_NARROW_INLINES_DEFINED
#define _CPP_NARROW_INLINES_DEFINED
inline __checkReturn char * __CRTDECL strchr(__in_z char * _Str, __in int _Ch)
        { return (char*)strchr((const char*)_Str, _Ch); }
inline __checkReturn char * __CRTDECL strpbrk(__in_z char * _Str, __in_z const char * _Control)
        { return (char*)strpbrk((const char*)_Str, _Control); }
inline __checkReturn char * __CRTDECL strrchr(__in_z char * _Str, __in int _Ch)
        { return (char*)strrchr((const char*)_Str, _Ch); }
inline __checkReturn char * __CRTDECL strstr(__in_z char * _Str, __in_z const char * _SubStr)
        { return (char*)strstr((const char*)_Str, _SubStr); }
#endif  /* _CPP_NARROW_INLINES_DEFINED */
#ifndef _CPP_MEMCHR_DEFINED
#define _CPP_MEMCHR_DEFINED
inline __checkReturn void * __CRTDECL memchr(__in_bcount_opt(_N) void * _Pv, __in int _C, __in size_t _N)
        { return (void*)memchr((const void*)_Pv, _C, _N); }
#endif  /* _CPP_MEMCHR_DEFINED */
}
#endif  /* __cplusplus */

#if !__STDC__

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("strdup")
#undef strdup
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRT_NONSTDC_DEPRECATE(_strdup) _CRTIMP __checkReturn char * __cdecl strdup(__in_z_opt const char * _Src);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("strdup")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

/* prototypes for oldnames.lib functions */
_CRT_NONSTDC_DEPRECATE(_strcmpi) _CRTIMP __checkReturn int __cdecl strcmpi(__in_z const char * _Str1, __in_z const char * _Str2);
_CRT_NONSTDC_DEPRECATE(_stricmp) _CRTIMP __checkReturn int __cdecl stricmp(__in_z const char * _Str1, __in_z const char * _Str2);
_CRT_NONSTDC_DEPRECATE(_strlwr) _CRTIMP char * __cdecl strlwr(__inout_z char * _Str);
_CRT_NONSTDC_DEPRECATE(_strnicmp) _CRTIMP __checkReturn int __cdecl strnicmp(__in_z const char * _Str1, __in_z const char * _Str, __in size_t _MaxCount);
_CRT_NONSTDC_DEPRECATE(_strnset) _CRTIMP char * __cdecl strnset(__inout_z char * _Str, __in int _Val, __in size_t _MaxCount);
_CRT_NONSTDC_DEPRECATE(_strrev) _CRTIMP char * __cdecl strrev(__inout_z char * _Str);
_CRT_NONSTDC_DEPRECATE(_strset)         char * __cdecl strset(__inout_z char * _Str, __in int _Val);
_CRT_NONSTDC_DEPRECATE(_strupr) _CRTIMP char * __cdecl strupr(__inout_z char * _Str);

#endif  /* !__STDC__ */


#ifndef _WSTRING_DEFINED

/* wide function prototypes, also declared in wchar.h  */

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
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, wcscat_s, __inout_ecount_z(_Size) wchar_t, _Dest, __in_z const wchar_t *, _Source)
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
#if __STDC_WANT_SECURE_LIB__ && !defined (__midl)
static __inline __checkReturn size_t __CRTDECL wcsnlen_s(__in_ecount_z(_MaxCount) const wchar_t * _Src, __in size_t _MaxCount)
{
    return wcsnlen(_Src, _MaxCount);
}
#endif  /* __STDC_WANT_SECURE_LIB__ && !defined (__midl) */
#if __STDC_WANT_SECURE_LIB__
_CRTIMP_ALTERNATIVE __checkReturn_wat errno_t __cdecl wcsncat_s(__inout_ecount_z(_DstSize) wchar_t * _Dst, __in rsize_t _DstSize, __in_z const wchar_t * _Src, __in rsize_t _MaxCount);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, wcsncat_s, __inout_ecount_z(_Size) wchar_t, _Dest, __in_z const wchar_t *, _Source, __in size_t, _Count)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_EX(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, wcsncat, wcsncat_s, __inout_ecount_z(_Count) wchar_t, __inout wchar_t, _Dest, __in_z const wchar_t *, _Source, __in size_t, _Count)
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
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wcslwr_s, __inout_ecount_z(_Size) wchar_t, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wcslwr, __inout_z wchar_t, _String)
_CRTIMP __checkReturn_wat errno_t __cdecl _wcslwr_s_l(__inout_ecount_z(_SizeInWords) wchar_t * _Str, __in size_t _SizeInWords, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _wcslwr_s_l, __inout_ecount_z(_Size) wchar_t, _String, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_EX(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wcslwr_l, _wcslwr_s_l, __inout_z wchar_t, _String, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn_wat errno_t __cdecl _wcsupr_s(__inout_ecount_z(_Size) wchar_t * _Str, __in size_t _Size);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _wcsupr_s, __inout_ecount_z(_Size) wchar_t, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(wchar_t *, __RETURN_POLICY_DST, _CRTIMP, _wcsupr, __inout_z wchar_t, _String)
_CRTIMP __checkReturn_wat errno_t __cdecl _wcsupr_s_l(__inout_ecount_z(_Size) wchar_t * _Str, __in size_t _Size, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _wcsupr_s_l, __inout_ecount_z(_size) wchar_t, _String, __in_opt _locale_t, _Locale)
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

#ifndef _INTERNAL_IFSTRIP_
__checkReturn int __cdecl __ascii_memicmp(__in_bcount_opt(_Size) const void * _Buf1, __in_bcount_opt(_Size) const void * _Buf2, __in size_t _Size);
__checkReturn int __cdecl __ascii_stricmp(__in_z const char * _Str1, __in_z const char * _Str2);
__checkReturn int __cdecl __ascii_strnicmp(__in_z const char * _Str1, __in_z const char * _Str2, __in size_t _MaxCount);
#endif  /* _INTERNAL_IFSTRIP_ */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _INC_STRING */
