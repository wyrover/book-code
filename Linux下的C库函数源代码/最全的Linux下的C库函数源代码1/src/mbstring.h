/***
* mbstring.h - MBCS string manipulation macros and functions
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       This file contains macros and function declarations for the MBCS
*       string manipulation functions.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_MBSTRING
#define _INC_MBSTRING

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

#ifndef _INTERNAL_IFSTRIP_
/*
 * MBCS - Multi-Byte Character Set
 */
#ifndef _THREADMBCINFO
typedef struct threadmbcinfostruct {
        int refcount;
        int mbcodepage;
        int ismbcodepage;
        int mblcid;
        unsigned short mbulinfo[6];
        unsigned char mbctype[257];
        unsigned char mbcasemap[256];
} threadmbcinfo;
#define _THREADMBCINFO
#endif  /* _THREADMBCINFO */
extern pthreadmbcinfo __ptmbcinfo;
pthreadmbcinfo __cdecl __updatetmbcinfo(void);
#endif  /* _INTERNAL_IFSTRIP_ */

#ifndef _MBSTRING_DEFINED

/* function prototypes */

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma push_macro("_mbsdup")
#undef _mbsdup
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn unsigned char * __cdecl _mbsdup(__in_z const unsigned char * _Str);

#if defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC)
#pragma pop_macro("_mbsdup")
#endif  /* defined (_DEBUG) && defined (_CRTDBG_MAP_ALLOC) */

_CRTIMP __checkReturn unsigned int __cdecl _mbbtombc(__in unsigned int _Ch);
_CRTIMP __checkReturn unsigned int __cdecl _mbbtombc_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbbtype(__in unsigned char _Ch, __in int _CType);
_CRTIMP __checkReturn int __cdecl _mbbtype_l(__in unsigned char _Ch, __in int _CType, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned int __cdecl _mbctombb(__in unsigned int _Ch);
_CRTIMP __checkReturn unsigned int __cdecl _mbctombb_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP int __cdecl _mbsbtype(__in_bcount_z(_Pos) const unsigned char * _Str, __in size_t _Pos);
_CRTIMP int __cdecl _mbsbtype_l(__in_bcount_z(_Pos) const unsigned char * _Str, __in size_t _Pos, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbscat_s(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _mbscat_s, __inout_bcount(_Size) unsigned char, _Dst, __in_z const unsigned char *, _DstSizeInBytes)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbscat, __inout_z unsigned char, _Dest, __in_z const unsigned char *, _Source)
_CRTIMP errno_t __cdecl _mbscat_s_l(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, _mbscat_s_l, __inout_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_2_EX(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbscat_l, _mbscat_s_l, __inout_z unsigned char, __inout_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn  _CONST_RETURN unsigned char * __cdecl _mbschr(__in_z const unsigned char * _Str, __in unsigned int _Ch);
_CRTIMP __checkReturn  _CONST_RETURN unsigned char * __cdecl _mbschr_l(__in_z const unsigned char * _Str, __in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbscmp(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2);
_CRTIMP __checkReturn int __cdecl _mbscmp_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbscoll(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2);
_CRTIMP __checkReturn int __cdecl _mbscoll_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbscpy_s(__out_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _mbscpy_s, __out_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbscpy, __out_z unsigned char, _Dest, __in_z const unsigned char *, _Source)
_CRTIMP errno_t __cdecl _mbscpy_s_l(__out_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, _mbscpy_s, __out_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_2_EX(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbscpy_l, _mbscpy_s_l, __out_z unsigned char, __out_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn size_t __cdecl _mbscspn(__in_z const unsigned char * _Str, __in_z const unsigned char * _Control);
_CRTIMP __checkReturn size_t __cdecl _mbscspn_l(__in_z const unsigned char * _Str, __in_z const unsigned char * _Control, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned char * __cdecl _mbsdec(__in_ecount_z(_Pos-_Start +1) const unsigned char * _Start, __in_z const unsigned char * _Pos);
_CRTIMP __checkReturn unsigned char * __cdecl _mbsdec_l(__in_ecount_z(_Pos-_Start+1) const unsigned char * _Start, __in_z const unsigned char * _Pos, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbsicmp(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2);
_CRTIMP __checkReturn int __cdecl _mbsicmp_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbsicoll(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2);
_CRTIMP __checkReturn int __cdecl _mbsicoll_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned char * __cdecl _mbsinc(__in_z const unsigned char * _Ptr);
_CRTIMP __checkReturn unsigned char * __cdecl _mbsinc_l(__in_z const unsigned char * _Ptr, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn size_t __cdecl _mbslen(__in_z const unsigned char * _Str);
_CRTIMP __checkReturn size_t __cdecl _mbslen_l(__in_z const unsigned char * _Str, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn size_t __cdecl _mbsnlen(__in_bcount_z(_MaxCount) const unsigned char * _Str, __in size_t _MaxCount);
_CRTIMP __checkReturn size_t __cdecl _mbsnlen_l(__in_bcount_z(_MaxCount) const unsigned char * _Str, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP errno_t __cdecl _mbslwr_s(__inout_bcount_z_opt(_SizeInBytes) unsigned char *_Str, __in size_t _SizeInBytes);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _mbslwr_s, __inout_z unsigned char, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbslwr, __inout_z unsigned char, _String)
_CRTIMP errno_t __cdecl _mbslwr_s_l(__inout_bcount_z_opt(_SizeInBytes) unsigned char *_Str, __in size_t _SizeInBytes, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _mbslwr_s_l, __inout_bcount(_Size) unsigned char, _String, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_EX(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbslwr_l, _mbslwr_s_l, __inout_z unsigned char, _String, __in_opt _locale_t, _Locale)
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbsnbcat_s(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in size_t _MaxCount);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, _mbsnbcat_s, __inout_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsnbcat, __inout_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count)
_CRTIMP errno_t __cdecl _mbsnbcat_s_l(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in size_t _MaxCount, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_3(errno_t, _mbsnbcat_s_l, __inout_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_3_EX(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsnbcat_l, _mbsnbcat_s_l, __inout_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn int __cdecl _mbsnbcmp(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _mbsnbcmp_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbsnbcoll(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _mbsnbcoll_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn size_t __cdecl _mbsnbcnt(__in_bcount_z(_MaxCount) const unsigned char * _Str, __in size_t _MaxCount);
_CRTIMP __checkReturn size_t __cdecl _mbsnbcnt_l(__in_bcount_z(_MaxCount) const unsigned char * _Str, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbsnbcpy_s(__out_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in size_t _MaxCount);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, _mbsnbcpy_s, __out_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsnbcpy, __out_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count)
_CRTIMP errno_t __cdecl _mbsnbcpy_s_l(__out_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in size_t _MaxCount, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_3(errno_t, _mbsnbcpy_s_l, __out_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_3_EX(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsnbcpy_l, _mbsnbcpy_s_l, __out_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn int __cdecl _mbsnbicmp(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _mbsnbicmp_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbsnbicoll(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _mbsnbicoll_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbsnbset_s) unsigned char * __cdecl _mbsnbset(__inout_bcount_z(_MaxCount) unsigned char * _Str, __in unsigned int _Ch, __in size_t _MaxCount);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbsnbset_s_l) unsigned char * __cdecl _mbsnbset_l(__inout_bcount_z(_MaxCount) unsigned char * _Str, __in unsigned int _Ch, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbsnbset_s(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in unsigned int _Ch, __in size_t _MaxCount);
_CRTIMP errno_t __cdecl _mbsnbset_s_l(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in unsigned int _Ch, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbsncat_s(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in size_t _MaxCount);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, _mbsncat_s, __inout_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsncat, __inout_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count)
_CRTIMP errno_t __cdecl _mbsncat_s_l(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in size_t _MaxCount, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_3(errno_t, _mbsncat_s, __inout_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_3_EX(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsncat_l, _mbsncat_s_l, __inout_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn size_t __cdecl _mbsnccnt(__in_bcount_z(_MaxCount) const unsigned char * _Str, __in size_t _MaxCount);
_CRTIMP __checkReturn size_t __cdecl _mbsnccnt_l(__in_bcount_z(_MaxCount) const unsigned char * _Str, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbsncmp(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _mbsncmp_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbsncoll(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _mbsncoll_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbsncpy_s(__out_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in size_t _MaxCount);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, _mbsncpy_s, __out_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsncpy, __out_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count)
_CRTIMP errno_t __cdecl _mbsncpy_s_l(__out_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in_z const unsigned char * _Src, __in size_t _MaxCount, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_3(errno_t, _mbsncpy_s_l, __out_bcount(_Size) unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_3_EX(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsncpy_l, _mbsncpy_s_l, __out_z unsigned char, _Dest, __in_z const unsigned char *, _Source, __in size_t, _Count, __in_opt _locale_t, _Locale)
_CRTIMP __checkReturn unsigned int __cdecl _mbsnextc (__in_z const unsigned char * _Str);
_CRTIMP __checkReturn unsigned int __cdecl _mbsnextc_l(__in_z const unsigned char * _Str, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbsnicmp(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _mbsnicmp_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _mbsnicoll(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount);
_CRTIMP __checkReturn int __cdecl _mbsnicoll_l(__in_z const unsigned char * _Str1, __in_z const unsigned char * _Str2, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned char * __cdecl _mbsninc(__in_bcount_z(_Count) const unsigned char * _Str, __in size_t _Count);
_CRTIMP __checkReturn unsigned char * __cdecl _mbsninc_l(__in_bcount_z(_Count) const unsigned char * _Str, __in size_t _Count, __in_opt _locale_t _Locale);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbsnset_s) unsigned char * __cdecl _mbsnset(__inout_bcount_z(_MaxCount) unsigned char * _Dst, __in unsigned int _Val, __in size_t _MaxCount);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbsnset_s_l) unsigned char * __cdecl _mbsnset_l(__inout_bcount_z(_MaxCount) unsigned char * _Dst, __in unsigned int _Val, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbsnset_s(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in unsigned int _Val, __in size_t _MaxCount);
_CRTIMP errno_t __cdecl _mbsnset_s_l(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in unsigned int _Val, __in size_t _MaxCount, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn  _CONST_RETURN unsigned char * __cdecl _mbspbrk(__in_z const unsigned char * _Str, __in_z const unsigned char * _Control);
_CRTIMP __checkReturn  _CONST_RETURN unsigned char * __cdecl _mbspbrk_l(__in_z const unsigned char * _Str, __in_z const unsigned char * _Control, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn  _CONST_RETURN unsigned char * __cdecl _mbsrchr(__in_z const unsigned char * _Str, __in unsigned int _Ch);
_CRTIMP __checkReturn  _CONST_RETURN unsigned char * __cdecl _mbsrchr_l(__in_z const unsigned char *_Str, __in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP unsigned char * __cdecl _mbsrev(__inout_z unsigned char * _Str);
_CRTIMP unsigned char * __cdecl _mbsrev_l(__inout_z unsigned char *_Str, __in_opt _locale_t _Locale);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbsset_s) unsigned char * __cdecl _mbsset(__inout_z unsigned char * _Str, __in unsigned int _Val);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbsset_s_l) unsigned char * __cdecl _mbsset_l(__inout_z unsigned char * _Str, __in unsigned int _Val, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbsset_s(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in unsigned int _Val);
_CRTIMP errno_t __cdecl _mbsset_s_l(__inout_bcount_z(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __in unsigned int _Val, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn size_t __cdecl _mbsspn(__in_z const unsigned char *_Str, __in_z const unsigned char * _Control);
_CRTIMP __checkReturn size_t __cdecl _mbsspn_l(__in_z const unsigned char * _Str, __in_z const unsigned char * _Control, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned char * __cdecl _mbsspnp(__in_z const unsigned char * _Str1, __in_z const unsigned char *_Str2);
_CRTIMP __checkReturn unsigned char * __cdecl _mbsspnp_l(__in_z const unsigned char *_Str1, __in_z const unsigned char *_Str2, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn  _CONST_RETURN unsigned char * __cdecl _mbsstr(__in_z const unsigned char * _Str, __in_z const unsigned char * _Substr);
_CRTIMP __checkReturn  _CONST_RETURN unsigned char * __cdecl _mbsstr_l(__in_z const unsigned char * _Str, __in_z const unsigned char * _Substr, __in_opt _locale_t _Locale);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbstok_s) __checkReturn unsigned char * __cdecl _mbstok(__inout_z_opt unsigned char * _Str, __in_z const unsigned char * _Delim);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbstok_s_l) __checkReturn unsigned char * __cdecl _mbstok_l(__inout_z_opt unsigned char *_Str, __in_z const unsigned char * _Delim, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE __checkReturn unsigned char * __cdecl _mbstok_s(__inout_z_opt unsigned char *_Str, __in_z const unsigned char * _Delim, __deref_inout_z_opt unsigned char ** _Context);
_CRTIMP __checkReturn unsigned char * __cdecl _mbstok_s_l(__inout_z_opt unsigned char * _Str, __in_z const unsigned char * _Delim, __deref_inout_z_opt unsigned char ** _Context, __in_opt _locale_t _Locale);
_CRTIMP errno_t __cdecl _mbsupr_s(__inout_bcount_z(_SizeInBytes) unsigned char *_Str, __in size_t _SizeInBytes);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_0(errno_t, _mbsupr_s, __inout_bcount(_Size) unsigned char, _String)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_0(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsupr, __inout_z unsigned char, _String)
_CRTIMP errno_t __cdecl _mbsupr_s_l(__inout_bcount_z(_SizeInBytes) unsigned char *_Str, __in size_t _SizeInBytes, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_1(errno_t, _mbsupr_s_l, __inout_bcount(_Size) unsigned char, _String, __in_opt _locale_t, _Locale)
__DEFINE_CPP_OVERLOAD_STANDARD_FUNC_0_1_EX(unsigned char *, __RETURN_POLICY_DST, _CRTIMP, _mbsupr_l, _mbsupr_s_l, __inout_z unsigned char, _String, __in_opt _locale_t, _Locale)

_CRTIMP __checkReturn size_t __cdecl _mbclen(__in_z const unsigned char *_Str);
_CRTIMP __checkReturn size_t __cdecl _mbclen_l(__in_z const unsigned char * _Str, __in_opt _locale_t _Locale);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbccpy_s) void __cdecl _mbccpy(__out_bcount(2) unsigned char * _Dst, __in_z const unsigned char * _Src);
_CRTIMP _CRT_INSECURE_DEPRECATE(_mbccpy_s_l) void __cdecl _mbccpy_l(__out_bcount(2) unsigned char *_Dst, __in_z const unsigned char *_Src, __in_opt _locale_t _Locale);
_CRTIMP_ALTERNATIVE errno_t __cdecl _mbccpy_s(__out_bcount(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __out_opt int * _PCopied, __in_z const unsigned char * _Src);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2(errno_t, _mbccpy_s, __out_bcount(_Size) unsigned char, _Dest, __out_opt int *, _PCopied, __in_z const unsigned char *, _Source)
_CRTIMP errno_t __cdecl _mbccpy_s_l(__out_bcount(_DstSizeInBytes) unsigned char * _Dst, __in size_t _DstSizeInBytes, __out_opt int * _PCopied, __in_z const unsigned char * _Src, __in_opt _locale_t _Locale);
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_3(errno_t, _mbccpy_s_l, __out_bcount(_Size) unsigned char, _Dest, __out_opt int *,_PCopied, __in_z const unsigned char *,_Source, __in_opt _locale_t, _Locale)
#define _mbccmp(_cpc1, _cpc2) _mbsncmp((_cpc1),(_cpc2),1)

#ifdef __cplusplus
#ifndef _CPP_MBCS_INLINES_DEFINED
#define _CPP_MBCS_INLINES_DEFINED
extern "C++" {
inline __checkReturn unsigned char * __CRTDECL _mbschr(__in_z unsigned char *_String, __in unsigned int _Char)
{
    return ((unsigned char *)_mbschr((const unsigned char *)_String, _Char));
}

inline __checkReturn unsigned char * __CRTDECL _mbschr_l(__in_z unsigned char *_String, __in unsigned int _Char, __in_opt _locale_t _Locale)
{
    return ((unsigned char *)_mbschr_l((const unsigned char *)_String, _Char, _Locale));
}

inline __checkReturn unsigned char * __CRTDECL _mbspbrk(__in_z unsigned char *_String, __in_z const unsigned char *_CharSet)
{
    return ((unsigned char *)_mbspbrk((const unsigned char *)_String, _CharSet));
}

inline __checkReturn unsigned char * __CRTDECL _mbspbrk_l(__in_z unsigned char *_String, __in_z const unsigned char *_CharSet, __in_opt _locale_t _Locale)
{
    return ((unsigned char *)_mbspbrk_l((const unsigned char *)_String, _CharSet, _Locale));
}

inline __checkReturn unsigned char * __CRTDECL _mbsrchr(__in_z unsigned char *_String, __in unsigned int _Char)
{
    return ((unsigned char *)_mbsrchr((const unsigned char *)_String, _Char));
}

inline __checkReturn unsigned char * __CRTDECL _mbsrchr_l(__in_z unsigned char *_String, __in unsigned int _Char, __in_opt _locale_t _Locale)
{
    return ((unsigned char *)_mbsrchr_l((const unsigned char *)_String, _Char, _Locale));
}

inline __checkReturn unsigned char * __CRTDECL _mbsstr(__in_z unsigned char *_String, __in_z const unsigned char *_Match)
{
    return ((unsigned char *)_mbsstr((const unsigned char *)_String, _Match));
}

inline __checkReturn unsigned char * __CRTDECL _mbsstr_l(__in_z unsigned char *_String, __in_z const unsigned char *_Match, __in_opt _locale_t _Locale)
{
    return ((unsigned char *)_mbsstr_l((const unsigned char *)_String, _Match, _Locale));
}
}
#endif  /* _CPP_MBCS_INLINES_DEFINED */
#endif  /* __cplusplus */

/* character routines */

_CRTIMP __checkReturn int __cdecl _ismbcalnum(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcalnum_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcalpha(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcalpha_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcdigit(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcdigit_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcgraph(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcgraph_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbclegal(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbclegal_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbclower(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbclower_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcprint(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcprint_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcpunct(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcpunct_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcspace(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcspace_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcupper(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcupper_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);

_CRTIMP __checkReturn unsigned int __cdecl _mbctolower(__in unsigned int _Ch);
_CRTIMP __checkReturn unsigned int __cdecl _mbctolower_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned int __cdecl _mbctoupper(__in unsigned int _Ch);
_CRTIMP __checkReturn unsigned int __cdecl _mbctoupper_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);

#define _MBSTRING_DEFINED
#endif  /* _MBSTRING_DEFINED */

#ifndef _MBLEADTRAIL_DEFINED
_CRTIMP __checkReturn int __cdecl _ismbblead(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbblead_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbbtrail(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbbtrail_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbslead(__in_ecount_z(_Pos - _Str+1) const unsigned char * _Str, __in_z const unsigned char * _Pos);
_CRTIMP __checkReturn int __cdecl _ismbslead_l(__in_ecount_z(_Pos - _Str+1) const unsigned char * _Str, __in_z const unsigned char * _Pos, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbstrail(__in_ecount_z(_Pos - _Str+1) const unsigned char * _Str, __in_z const unsigned char * _Pos);
_CRTIMP __checkReturn int __cdecl _ismbstrail_l(__in_ecount_z(_Pos - _Str+1) const unsigned char * _Str, __in_z const unsigned char * _Pos, __in_opt _locale_t _Locale);
#define _MBLEADTRAIL_DEFINED
#endif  /* _MBLEADTRAIL_DEFINED */

/*  Kanji specific prototypes.  */

_CRTIMP __checkReturn int __cdecl _ismbchira(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbchira_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbckata(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbckata_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcsymbol(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcsymbol_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcl0(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcl0_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcl1(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcl1_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn int __cdecl _ismbcl2(__in unsigned int _Ch);
_CRTIMP __checkReturn int __cdecl _ismbcl2_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned int __cdecl _mbcjistojms(__in unsigned int _Ch);
_CRTIMP __checkReturn unsigned int __cdecl _mbcjistojms_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned int __cdecl _mbcjmstojis(__in unsigned int _Ch);
_CRTIMP __checkReturn unsigned int __cdecl _mbcjmstojis_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned int __cdecl _mbctohira(__in unsigned int _Ch);
_CRTIMP __checkReturn unsigned int __cdecl _mbctohira_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);
_CRTIMP __checkReturn unsigned int __cdecl _mbctokata(__in unsigned int _Ch);
_CRTIMP __checkReturn unsigned int __cdecl _mbctokata_l(__in unsigned int _Ch, __in_opt _locale_t _Locale);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_MBSTRING */
