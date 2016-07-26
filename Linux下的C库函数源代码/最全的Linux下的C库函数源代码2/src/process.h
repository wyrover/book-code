/***
*process.h - definition and declarations for process control functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file defines the modeflag values for spawnxx calls.
*       Also contains the function argument declarations for all
*       process control related routines.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_PROCESS
#define _INC_PROCESS

#include <crtdefs.h>


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* modeflag values for _spawnxx routines */

#define _P_WAIT         0
#define _P_NOWAIT       1
#define _OLD_P_OVERLAY  2
#define _P_NOWAITO      3
#define _P_DETACH       4

#define _P_OVERLAY      2

/* Action codes for _cwait(). The action code argument to _cwait is ignored
   on Win32 though it is accepted for compatibilty with old MS CRT libs */
#define _WAIT_CHILD      0
#define _WAIT_GRANDCHILD 1


/* function prototypes */

_CRTIMP uintptr_t __cdecl _beginthread (__in void (__cdecl * _StartAddress) (void *),
        __in unsigned _StackSize, __in_opt void * _ArgList);
_CRTIMP void __cdecl _endthread(void);
_CRTIMP uintptr_t __cdecl _beginthreadex(__in_opt void * _Security, __in unsigned _StackSize,
        __in unsigned (__stdcall * _StartAddress) (void *), __in_opt void * _ArgList,
        __in unsigned _InitFlag, __in_opt unsigned * _ThrdAddr);
_CRTIMP void __cdecl _endthreadex(__in unsigned _Retval);

#if defined (__cplusplus) && defined (_M_CEE)
extern "C++"
{
_MCRTIMP uintptr_t __cdecl _beginthread (__in void (__clrcall * _StartAddress) (void *),
         __in unsigned _StackSize, __in_opt void * _ArgList);
_MCRTIMP uintptr_t __cdecl _beginthreadex(__in_opt void * _Security, __in unsigned _StackSize,
        __in unsigned (__clrcall * _StartAddress) (void *), __in_opt void * _ArgList,
        __in unsigned _InitFlag, __out_opt unsigned * _ThrdAddr);
}
#endif  /* defined (__cplusplus) && defined (_M_CEE) */


#ifndef _CRT_TERMINATE_DEFINED
#define _CRT_TERMINATE_DEFINED
_CRTIMP __declspec(noreturn) void __cdecl exit(__in int _Code);
_CRTIMP __declspec(noreturn) void __cdecl _exit(__in int _Code);
_CRTIMP void __cdecl abort(void);
#endif  /* _CRT_TERMINATE_DEFINED */

_CRTIMP void __cdecl _cexit(void);
_CRTIMP void __cdecl _c_exit(void);
_CRTIMP int __cdecl _getpid(void);

_CRTIMP intptr_t __cdecl _cwait(__out_opt int * _TermStat, __in intptr_t _ProcHandle, __in int _Action);
_CRTIMP intptr_t __cdecl _execl(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _execle(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _execlp(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _execlpe(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _execv(__in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRTIMP intptr_t __cdecl _execve(__in_z const char * _Filename, __in_z const char * const * _ArgList, __in_z_opt const char * const * _Env);
_CRTIMP intptr_t __cdecl _execvp(__in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRTIMP intptr_t __cdecl _execvpe(__in_z const char * _Filename, __in_z const char * const * _ArgList, __in_z_opt const char * const * _Env);
_CRTIMP intptr_t __cdecl _spawnl(__in int _Mode, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _spawnle(__in int _Mode, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _spawnlp(__in int _Mode, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _spawnlpe(__in int _Mode, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRTIMP intptr_t __cdecl _spawnv(__in int _Mode, __in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRTIMP intptr_t __cdecl _spawnve(__in int _Mode, __in_z const char * _Filename, __in_z const char * const * _ArgList,
        __in_z_opt const char * const * _Env);
_CRTIMP intptr_t __cdecl _spawnvp(__in int _Mode, __in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRTIMP intptr_t __cdecl _spawnvpe(__in int _Mode, __in_z const char * _Filename, __in_z const char * const * _ArgList,
        __in_z_opt const char * const * _Env);

#ifndef _CRT_SYSTEM_DEFINED
#define _CRT_SYSTEM_DEFINED
_CRTIMP int __cdecl system(__in_z_opt const char * _Command);
#endif  /* _CRT_SYSTEM_DEFINED */

#ifndef _WPROCESS_DEFINED
/* wide function prototypes, also declared in wchar.h  */
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

/*
 * Security check initialization and failure reporting used by /GS security
 * checks.
 */
#if !defined (_M_CEE)
void __cdecl __security_init_cookie(void);
#ifdef _M_IX86
void __fastcall __security_check_cookie(__in uintptr_t _StackCookie);
__declspec(noreturn) void __cdecl __report_gsfailure(void);
#else  /* _M_IX86 */
void __cdecl __security_check_cookie(__in uintptr_t _StackCookie);
__declspec(noreturn) void __cdecl __report_gsfailure(__in uintptr_t _StackCookie);
#endif  /* _M_IX86 */
#endif  /* !defined (_M_CEE) */
extern uintptr_t __security_cookie;

/* --------- The following functions are OBSOLETE --------- */
/*
 * The Win32 API LoadLibrary, FreeLibrary and GetProcAddress should be used
 * instead.
 */

_CRT_OBSOLETE(LoadLibrary) intptr_t __cdecl _loaddll(__in_z char * _Filename);
_CRT_OBSOLETE(FreeLibrary) int __cdecl _unloaddll(__in intptr_t _Handle);
_CRT_OBSOLETE(GetProcAddress) int (__cdecl * __cdecl _getdllprocaddr(__in intptr_t _Handle, __in_z_opt char * _ProcedureName, __in intptr_t _Ordinal))(void);

/* --------- The preceding functions are OBSOLETE --------- */


#ifdef _DECL_DLLMAIN
/*
 * Declare DLL notification (initialization/termination) routines
 *      The preferred method is for the user to provide DllMain() which will
 *      be called automatically by the DLL entry point defined by the C run-
 *      time library code.  If the user wants to define the DLL entry point
 *      routine, the user's entry point must call _CRT_INIT on all types of
 *      notifications, as the very first thing on attach notifications and
 *      as the very last thing on detach notifications.
 */
#ifdef _WINDOWS_
#if defined (MRTDLL)
BOOL __clrcall DllMain(__in HANDLE _HDllHandle, __in DWORD _Reason, __in_opt LPVOID _Reserved);
#else  /* defined (MRTDLL) */
BOOL WINAPI DllMain(__in HANDLE _HDllHandle, __in DWORD _Reason, __in_opt LPVOID _Reserved);
#endif  /* defined (MRTDLL) */
#if defined (MRTDLL)
BOOL _CRT_INIT(__in HANDLE _HDllHandle, __in DWORD _Reason, __in_opt LPVOID _Reserved);
#else  /* defined (MRTDLL) */
BOOL WINAPI _CRT_INIT(__in HANDLE _HDllHandle, __in DWORD _Reason, __in_opt LPVOID _Reserved);
#endif  /* defined (MRTDLL) */
BOOL WINAPI _wCRT_INIT(__in HANDLE _HDllHandle, __in DWORD _Reason, __in_opt LPVOID _Reserved);
extern BOOL (WINAPI * const _pRawDllMain)(HANDLE, DWORD, LPVOID);
#else  /* _WINDOWS_ */
int __stdcall DllMain(__in void * _HDllHandle, __in unsigned _Reason, __in_opt void * _Reserved);
int __stdcall _CRT_INIT(__in void * _HDllHandle, __in unsigned _Reason, __in_opt void * _Reserved);
int __stdcall _wCRT_INIT(__in void * _HDllHandle, __in unsigned _Reason, __in_opt void * _Reserved);
extern int (__stdcall * const _pRawDllMain)(void *, unsigned, void *);
#endif  /* _WINDOWS_ */
#endif  /* _DECL_DLLMAIN */

#if !__STDC__

/* Non-ANSI names for compatibility */

#define P_WAIT          _P_WAIT
#define P_NOWAIT        _P_NOWAIT
#define P_OVERLAY       _P_OVERLAY
#define OLD_P_OVERLAY   _OLD_P_OVERLAY
#define P_NOWAITO       _P_NOWAITO
#define P_DETACH        _P_DETACH
#define WAIT_CHILD      _WAIT_CHILD
#define WAIT_GRANDCHILD _WAIT_GRANDCHILD

/* current declarations */
_CRT_NONSTDC_DEPRECATE(_cwait) _CRTIMP intptr_t __cdecl cwait(__out_opt int * _TermStat, __in intptr_t _ProcHandle, __in int _Action);
_CRT_NONSTDC_DEPRECATE(_execl) _CRTIMP intptr_t __cdecl execl(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRT_NONSTDC_DEPRECATE(_execle) _CRTIMP intptr_t __cdecl execle(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRT_NONSTDC_DEPRECATE(_execlp) _CRTIMP intptr_t __cdecl execlp(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRT_NONSTDC_DEPRECATE(_execlpe) _CRTIMP intptr_t __cdecl execlpe(__in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRT_NONSTDC_DEPRECATE(_execv) _CRTIMP intptr_t __cdecl execv(__in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRT_NONSTDC_DEPRECATE(_execve) _CRTIMP intptr_t __cdecl execve(__in_z const char * _Filename, __in_z const char * const * _ArgList, __in_z_opt const char * const * _Env);
_CRT_NONSTDC_DEPRECATE(_execvp) _CRTIMP intptr_t __cdecl execvp(__in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRT_NONSTDC_DEPRECATE(_execvpe) _CRTIMP intptr_t __cdecl execvpe(__in_z const char * _Filename, __in_z const char * const * _ArgList, __in_z_opt const char * const * _Env);
_CRT_NONSTDC_DEPRECATE(_spawnl) _CRTIMP intptr_t __cdecl spawnl(__in int, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRT_NONSTDC_DEPRECATE(_spawnle) _CRTIMP intptr_t __cdecl spawnle(__in int, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRT_NONSTDC_DEPRECATE(_spawnlp) _CRTIMP intptr_t __cdecl spawnlp(__in int, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRT_NONSTDC_DEPRECATE(_spawnlpe) _CRTIMP intptr_t __cdecl spawnlpe(__in int, __in_z const char * _Filename, __in_z const char * _ArgList, ...);
_CRT_NONSTDC_DEPRECATE(_spawnv) _CRTIMP intptr_t __cdecl spawnv(__in int, __in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRT_NONSTDC_DEPRECATE(_spawnve) _CRTIMP intptr_t __cdecl spawnve(__in int, __in_z const char * _Filename, __in_z const char * const * _ArgList,
        __in_z_opt const char * const * _Env);
_CRT_NONSTDC_DEPRECATE(_spawnvp) _CRTIMP intptr_t __cdecl spawnvp(__in int, __in_z const char * _Filename, __in_z const char * const * _ArgList);
_CRT_NONSTDC_DEPRECATE(_spawnvpe) _CRTIMP intptr_t __cdecl spawnvpe(__in int, __in_z const char * _Filename, __in_z const char * const * _ArgList,
        __in_z_opt const char * const * _Env);

_CRT_NONSTDC_DEPRECATE(_getpid) _CRTIMP int __cdecl getpid(void);

#endif  /* !__STDC__ */

#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif  /* _INC_PROCESS */
