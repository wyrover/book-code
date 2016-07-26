/* xmtx.h internal header */
#pragma once
#ifndef _XMTX
#define _XMTX
#include <stdlib.h>
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#ifndef MRTDLL
#ifndef _M_CEE_PURE
_C_LIB_DECL
#endif
#endif

#ifdef MRTDLL
/* We want to declare Enter/LeaveCriticalSection as p/invoke */
#define EnterCriticalSection _undefined_EnterCriticalSection
#define LeaveCriticalSection _undefined_LeaveCriticalSection
#endif

#include <windows.h>

#ifdef MRTDLL
#undef EnterCriticalSection
#undef LeaveCriticalSection

_RELIABILITY_CONTRACT
[System::Security::SuppressUnmanagedCodeSecurity]
[System::Runtime::InteropServices::DllImport("kernel32.dll")]
void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

_RELIABILITY_CONTRACT
[System::Security::SuppressUnmanagedCodeSecurity]
[System::Runtime::InteropServices::DllImport("kernel32.dll")]
void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

#endif

typedef CRITICAL_SECTION _Rmtx;

#ifdef _M_CEE_PURE
void __clrcall _Mtxinit(_Rmtx *);
void __clrcall _Mtxdst(_Rmtx *);
void __clrcall _Mtxlock(_Rmtx *);
void __clrcall _Mtxunlock(_Rmtx *);
#else
_MRTIMP2 void __cdecl _Mtxinit(_Rmtx *);
_MRTIMP2 void __cdecl _Mtxdst(_Rmtx *);
_MRTIMP2 void __cdecl _Mtxlock(_Rmtx *);
_MRTIMP2 void __cdecl _Mtxunlock(_Rmtx *);
#endif

 #if !_MULTI_THREAD
  #define _Mtxinit(mtx)
  #define _Mtxdst(mtx)
  #define _Mtxlock(mtx)
  #define _Mtxunlock(mtx)

typedef char _Once_t;

  #define _Once(cntrl, func)	if (*(cntrl) == 0) (func)(), *(cntrl) = 2
  #define _ONCE_T_INIT	0

 #elif _WIN32_C_LIB
typedef long _Once_t;

#ifdef _M_CEE_PURE
void __clrcall _Once(_Once_t *, void (__cdecl *)(void));
#else
_MRTIMP2 void __cdecl _Once(_Once_t *, void (__cdecl *)(void));
#endif
 #define _ONCE_T_INIT	0

 #endif /* _MULTI_THREAD */
#ifndef MRTDLL
#ifndef _M_CEE_PURE
_END_C_LIB_DECL
#endif
#endif
#endif /* _XMTX */

/*
 * Copyright (c) 1992-2004 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V4.04:0009 */
