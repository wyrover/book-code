/***
*eh.h - User include file for exception handling.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       User include file for exception handling.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#include <crtdefs.h>

#ifndef _INC_EH
#define _INC_EH
#ifndef RC_INVOKED
#ifndef _CRTBLD
/* This version of the header files is NOT for user programs.
 * It is intended for use when building the C runtimes ONLY.
 * The version intended for public use will not have this message.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */
#ifdef _MSC_VER
/* Currently, all MS C compilers for Win32 platforms default to 8 byte
 * alignment.
 */
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#ifndef __cplusplus
#error "eh.h is only for C++!"
#endif  /* __cplusplus */

/* terminate_handler is the standard name; terminate_function is supported for historical reasons */
#ifndef _M_CEE_PURE
typedef void (__cdecl *terminate_function)();
typedef void (__cdecl *terminate_handler)();
typedef void (__cdecl *unexpected_function)();
typedef void (__cdecl *unexpected_handler)();
#else  /* _M_CEE_PURE */
typedef void (__clrcall *terminate_function)();
typedef void (__clrcall *terminate_handler)();
typedef void (__clrcall *unexpected_function)();
typedef void (__clrcall *unexpected_handler)();
#endif  /* _M_CEE_PURE */

#ifdef _M_CEE
typedef void (__clrcall *__terminate_function_m)();
typedef void (__clrcall *__terminate_handler_m)();
typedef void (__clrcall *__unexpected_function_m)();
typedef void (__clrcall *__unexpected_handler_m)();
#endif  /* _M_CEE */

struct _EXCEPTION_POINTERS;
#ifndef _M_CEE_PURE
typedef void (__cdecl *_se_translator_function)(unsigned int, struct _EXCEPTION_POINTERS*);
#endif  /* _M_CEE_PURE */

_CRTIMP __declspec(noreturn) void __cdecl terminate(void);
_CRTIMP void __cdecl unexpected(void);

_CRTIMP int __cdecl _is_exception_typeof(__in const type_info &_Type, __in struct _EXCEPTION_POINTERS * _ExceptionPtr);

#ifndef _M_CEE_PURE
/* only __clrcall versions provided by the MRT exist in pure */
_CRTIMP terminate_function __cdecl set_terminate(__in_opt terminate_function _NewPtFunc);
extern "C" _CRTIMP terminate_function __cdecl _get_terminate(void);
_CRTIMP unexpected_function __cdecl set_unexpected(__in_opt unexpected_function _NewPtFunc);
extern "C" _CRTIMP unexpected_function __cdecl _get_unexpected(void);
#endif  /* _M_CEE_PURE */

#ifndef _M_CEE_PURE
/* set_se_translator cannot be a managed implementation, and so cannot be called from _M_CEE_PURE code */
_CRTIMP _se_translator_function __cdecl _set_se_translator(__in_opt _se_translator_function _NewPtFunc);
#endif  /* _M_CEE_PURE */
_CRTIMP bool __cdecl __uncaught_exception();

/*
 * These overload helps in resolving NULL
 */
#ifdef _M_CEE_MIXED
_CRTIMP terminate_function __cdecl set_terminate(__in_opt int _Zero);
_CRTIMP unexpected_function __cdecl set_unexpected(__in_opt int _Zero);
#endif  /* _M_CEE_MIXED */

#ifdef _M_CEE
_MRTIMP __terminate_function_m __cdecl set_terminate(__in_opt __terminate_function_m _NewPtFunc);
_MRTIMP __unexpected_function_m __cdecl set_unexpected(__in_opt __unexpected_function_m _NewPtFunc);
#endif  /* _M_CEE */

#ifndef _INTERNAL_IFSTRIP_
#if defined (CRTDLL) && !defined (_SYSCRT)
/*
 * All these function pointer are used for creating global state of CRT
 * functions. Either all of them will be set or all of them will be NULL
 */
typedef terminate_function (__cdecl *_set_terminate_function)(terminate_function);
typedef terminate_function (__cdecl *_get_terminate_function)(void);
typedef unexpected_function (__cdecl *_set_unexpected_function)(unexpected_function);
typedef unexpected_function (__cdecl *_get_unexpected_function)(void);
extern _set_terminate_function _set_terminate_server;
extern _get_terminate_function _get_terminate_server;
extern _set_unexpected_function _set_unexpected_server;
extern _get_unexpected_function _get_unexpected_server;
extern terminate_function _terminate_server;
extern unexpected_function _unexpected_server;
#endif  /* defined (CRTDLL) && !defined (_SYSCRT) */
#endif  /* _INTERNAL_IFSTRIP_ */
#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */
#endif  /* RC_INVOKED */
#endif  /* _INC_EH */
