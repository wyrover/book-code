/***
*oscalls.h - contains declarations of Operating System types and constants.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Declares types and constants that are defined by the target OS.
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_OSCALLS
#define _INC_OSCALLS

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#include <crtdefs.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef _WIN32

#ifdef NULL
#undef  NULL
#endif  /* NULL */

#if defined (_DEBUG) && defined (_WIN32)

void DbgBreakPoint(void);
int DbgPrint(__in_z __format_string char *_Format, ...);

#endif  /* defined (_DEBUG) && defined (_WIN32) */

#define NOMINMAX

#define _WIN32_FUSION 0x0100
#include <windows.h>

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

/* File time and date types */

typedef struct _FTIME {         /* ftime */
    unsigned short twosecs : 5;
    unsigned short minutes : 6;
    unsigned short hours   : 5;
} FTIME;
typedef FTIME   *PFTIME;

typedef struct _FDATE {         /* fdate */
    unsigned short day     : 5;
    unsigned short month   : 4;
    unsigned short year    : 7;
} FDATE;
typedef FDATE   *PFDATE;

#else  /* _WIN32 */


#error ERROR - ONLY WIN32 OR POSIX TARGETS SUPPORTED!


#endif  /* _WIN32 */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _INC_OSCALLS */
