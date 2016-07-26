
/***
*new.h - declarations and definitions for C++ memory allocation functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the declarations for C++ memory allocation functions.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_NEW
#define _INC_NEW

#ifdef __cplusplus

#ifndef _MSC_EXTENSIONS
#include <new>
#endif  /* _MSC_EXTENSIONS */

#include <crtdefs.h>

/* Protect against #define of new */
#pragma push_macro("new")
#undef  new

#ifndef _USE_OLD_STDCPP
/* Define _CRTIMP2 */
#ifndef _CRTIMP2
#if defined (CRTDLL2)
#define _CRTIMP2 __declspec(dllexport)
#else  /* defined (CRTDLL2) */
#if defined (_DLL) && !defined (_STATIC_CPPLIB)
#define _CRTIMP2 __declspec(dllimport)
#else  /* defined (_DLL) && !defined (_STATIC_CPPLIB) */
#define _CRTIMP2
#endif  /* defined (_DLL) && !defined (_STATIC_CPPLIB) */
#endif  /* defined (CRTDLL2) */
#endif  /* _CRTIMP2 */
#endif  /* _USE_OLD_STDCPP */

/* Define __cdecl for non-Microsoft compilers */

#if (!defined (_MSC_VER) && !defined (__cdecl))
#define __cdecl
#endif  /* (!defined (_MSC_VER) && !defined (__cdecl)) */

#ifdef _MSC_EXTENSIONS
#ifdef _USE_OLD_STDCPP
typedef void (__cdecl * new_handler) ();
_CRTIMP new_handler __cdecl set_new_handler(__in_opt new_handler _NewHandler);
#else  /* _USE_OLD_STDCPP */

namespace std {

#ifdef _M_CEE_PURE
typedef void (__clrcall * new_handler) ();
#else  /* _M_CEE_PURE */
typedef void (__cdecl * new_handler) ();
#endif  /* _M_CEE_PURE */
#ifdef _M_CEE
typedef void (__clrcall * _new_handler_m) ();
#endif  /* _M_CEE */
_CRTIMP2 new_handler __cdecl set_new_handler(__in_opt new_handler _NewHandler) throw();
#ifdef _M_CEE_MIXED
extern "C++" _MRTIMP _new_handler_m __cdecl set_new_handler(__in_opt _new_handler_m _NewHandler) throw();
extern "C++" _CRTIMP2 new_handler __cdecl set_new_handler(int) throw();
#endif  /* _M_CEE_MIXED */

};

#ifdef _M_CEE
using std::_new_handler_m;
#endif  /* _M_CEE */
using std::new_handler;
using std::set_new_handler;
#endif  /* _USE_OLD_STDCPP */
#endif  /* _MSC_EXTENSIONS */

#ifndef __NOTHROW_T_DEFINED
#define __NOTHROW_T_DEFINED
namespace std {
        /* placement new tag type to suppress exceptions */
        struct nothrow_t {};

        /* constant for placement new tag */
        extern const nothrow_t nothrow;
};

__bcount_opt(_Size) void *__CRTDECL operator new(size_t _Size, const std::nothrow_t&) throw();
__bcount_opt(_Size) void *__CRTDECL operator new[](size_t _Size, const std::nothrow_t&) throw();
void __CRTDECL operator delete(void *, const std::nothrow_t&) throw();
void __CRTDECL operator delete[](void *, const std::nothrow_t&) throw();
#endif  /* __NOTHROW_T_DEFINED */

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline void *__CRTDECL operator new(size_t, void *_Where)
        {return (_Where); }
#if _MSC_VER >= 1200
inline void __CRTDECL operator delete(void *, void *)
        {return; }
#endif  /* _MSC_VER >= 1200 */
#endif  /* __PLACEMENT_NEW_INLINE */


/*
 * new mode flag -- when set, makes malloc() behave like new()
 */

_CRTIMP int __cdecl _query_new_mode( void );
_CRTIMP int __cdecl _set_new_mode( __in int _NewMode);

#ifndef _PNH_DEFINED
#ifdef _M_CEE_PURE
typedef int (__clrcall * _PNH)( size_t );
#else  /* _M_CEE_PURE */
typedef int (__cdecl * _PNH)( size_t );
#endif  /* _M_CEE_PURE */
#define _PNH_DEFINED
#endif  /* _PNH_DEFINED */

_CRTIMP _PNH __cdecl _query_new_handler( void );
_CRTIMP _PNH __cdecl _set_new_handler( __in_opt _PNH _NewHandler);

#if _MSC_VER >= 1400 && defined(_M_CEE)
#ifndef __MPNH_DEFINED
typedef int (__clrcall * __MPNH)( size_t );
#define __MPNH_DEFINED
#endif  /* __MPNH_DEFINED */

_MRTIMP __MPNH __cdecl __query_new_handler_m( void );
#ifdef _M_CEE_MIXED
/* _set_new_handler(int) is used to disambiguate NULL/0 */
_CRTIMP _PNH __cdecl _set_new_handler( __in int _NewHandler);
extern "C++" _MRTIMP __MPNH __cdecl _set_new_handler(__in_opt __MPNH _NewHandler);
#endif  /* _M_CEE_MIXED */
#endif  /* _MSC_VER >= 1400 && defined(_M_CEE) */

/*
 * Microsoft extension:
 *
 * _NO_ANSI_NEW_HANDLER de-activates the ANSI new_handler. Use this special value
 * to support old style (_set_new_handler) behavior.
 */

#ifndef _NO_ANSI_NH_DEFINED
#define _NO_ANSI_NEW_HANDLER  ((new_handler)-1)
#define _NO_ANSI_NEW_HANDLER_M  ((_new_handler_m)-1)
#define _NO_ANSI_NH_DEFINED
#endif  /* _NO_ANSI_NH_DEFINED */

#pragma pop_macro("new")

#endif  /* __cplusplus */

#endif  /* _INC_NEW */
