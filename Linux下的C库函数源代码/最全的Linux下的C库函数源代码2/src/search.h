/***
*search.h - declarations for searcing/sorting routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the declarations for the sorting and
*       searching routines.
*       [System V]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_SEARCH
#define _INC_SEARCH

#include <crtdefs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* Function prototypes */

#ifndef _CRT_ALGO_DEFINED
#define _CRT_ALGO_DEFINED
#if __STDC_WANT_SECURE_LIB__
_CRTIMP __checkReturn void * __cdecl bsearch_s(__in const void * _Key, __in_bcount(_NumOfElements * _SizeOfElements) const void * _Base,
        __in rsize_t _NumOfElements, __in rsize_t _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(void *, const void *, const void *), void * _Context);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP __checkReturn void * __cdecl bsearch(__in const void * _Key, __in_bcount(_NumOfElements * _SizeOfElements) const void * _Base,
        __in size_t _NumOfElements, __in size_t _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(const void *, const void *));

#if __STDC_WANT_SECURE_LIB__
_CRTIMP void __cdecl qsort_s(__inout_bcount(_NumOfElements* _SizeOfElements) void * _Base,
        __in rsize_t _NumOfElements, __in rsize_t _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(void *, const void *, const void *), void *_Context);
#endif  /* __STDC_WANT_SECURE_LIB__ */
_CRTIMP void __cdecl qsort(__inout_bcount(_NumOfElements * _SizeOfElements) void * _Base,
        __in size_t _NumOfElements, __in size_t _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(const void *, const void *));
#endif  /* _CRT_ALGO_DEFINED */

_CRTIMP __checkReturn void * __cdecl _lfind_s(__in const void * _Key, __in_bcount((*_NumOfElements) * _SizeOfElements) const void * _Base,
        __inout unsigned int * _NumOfElements, __in size_t _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(void *, const void *, const void *), void * _Context);
_CRTIMP __checkReturn void * __cdecl _lfind(__in const void * _Key, __in_bcount((*_NumOfElements) * _SizeOfElements) const void * _Base,
        __inout unsigned int * _NumOfElements, __in unsigned int _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(const void *, const void *));

_CRTIMP __checkReturn void * __cdecl _lsearch_s(__in const void * _Key, __inout_bcount((*_NumOfElements ) * _SizeOfElements) void  * _Base,
        __inout unsigned int * _NumOfElements, __in size_t _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(void *, const void *, const void *), void * _Context);
_CRTIMP __checkReturn void * __cdecl _lsearch(__in const void * _Key, __inout_bcount((*_NumOfElements ) * _SizeOfElements) void  * _Base,
        __inout unsigned int * _NumOfElements, __in unsigned int _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(const void *, const void *));

#if _MSC_VER >= 1400 && defined(__cplusplus) && defined(_M_CEE)
/*
 * Managed search routines. Note __cplusplus, this is because we only support
 * managed C++.
 */
extern "C++"
{

#if __STDC_WANT_SECURE_LIB__
__checkReturn void * __clrcall bsearch_s(__in const void * _Key, __in_bcount(_NumOfElements*_SizeOfElements) const void * _Base,
        __in rsize_t _NumOfElements, __in rsize_t _SizeOfElements,
        __in int (__clrcall * _PtFuncCompare)(void *, const void *, const void *), void * _Context);
#endif  /* __STDC_WANT_SECURE_LIB__ */
__checkReturn void * __clrcall bsearch(__in const void * _Key, __in_bcount(_NumOfElements*_SizeOfElements) const void * _Base,
        __in size_t _NumOfElements, __in size_t _SizeOfElements,
        __in int (__clrcall * _PtFuncCompare)(const void *, const void *));

__checkReturn void * __clrcall _lfind_s(__in const void * _Key, __in_bcount(_NumOfElements*_SizeOfElements) const void * _Base,
        __inout unsigned int * _NumOfElements, __in size_t _SizeOfElements,
        __in int (__clrcall * _PtFuncCompare)(void *, const void *, const void *), void * _Context);
__checkReturn void * __clrcall _lfind(__in const void * _Key, __in_bcount((*_NumOfElements)*_SizeOfElements) const void * _Base,
        __inout unsigned int * _NumOfElements, __in unsigned int _SizeOfElements,
        __in int (__clrcall * _PtFuncCompare)(const void *, const void *));

__checkReturn void * __clrcall _lsearch_s(__in const void * _Key, __in_bcount((*_NumOfElements)*_SizeOfElements) void * _Base,
        __in unsigned int * _NumOfElements, __in size_t _SizeOfElements,
        __in int (__clrcall * _PtFuncCompare)(void *, const void *, const void *), void * _Context);
__checkReturn void * __clrcall _lsearch(__in const void * _Key, __inout_bcount((*_NumOfElements)*_SizeOfElements) void * _Base,
        __inout unsigned int * _NumOfElements, __in unsigned int _SizeOfElements,
        __in int (__clrcall * _PtFuncCompare)(const void *, const void *));

#if __STDC_WANT_SECURE_LIB__
void __clrcall qsort_s(__inout_bcount_full(_NumOfElements*_SizeOfElements) void * _Base,
        __in rsize_t _NumOfElements, __in rsize_t _SizeOfElment,
        __in int (__clrcall * _PtFuncCompare)(void *, const void *, const void *), void * _Context);
#endif  /* __STDC_WANT_SECURE_LIB__ */
void __clrcall qsort(__inout_bcount_full(_NumOfElements*_SizeOfElements) void * _Base,
        __in size_t _NumOfElements, __in size_t _SizeOfElements,
        __in int (__clrcall * _PtFuncCompare)(const void *, const void *));

}
#endif  /* _MSC_VER >= 1400 && defined(__cplusplus) && defined(_M_CEE) */


#if !__STDC__
/* Non-ANSI names for compatibility */

_CRTIMP _CRT_NONSTDC_DEPRECATE(_lfind) __checkReturn void * __cdecl lfind(__in const void * _Key, __in_bcount((*_NumOfElements) * _SizeOfElements) const void * _Base,
        __inout unsigned int * _NumOfElements, __in unsigned int _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(const void *, const void *));
_CRTIMP _CRT_NONSTDC_DEPRECATE(_lsearch) __checkReturn void * __cdecl lsearch(__in const void * _Key, __inout_bcount((*_NumOfElements) * _SizeOfElements) void  * _Base,
        __inout unsigned int * _NumOfElements, __in unsigned int _SizeOfElements,
        __in int (__cdecl * _PtFuncCompare)(const void *, const void *));

#endif  /* !__STDC__ */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _INC_SEARCH */
