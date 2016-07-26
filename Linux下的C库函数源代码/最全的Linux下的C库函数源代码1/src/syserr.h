/***
*syserr.h - constants/macros for error message routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains macros/constants for perror, strerror,
*       and _strerror.
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_SYSERR
#define _INC_SYSERR

#include <crtdefs.h>
#include <internal.h>

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

/* Macro for perror, strerror, and _strerror */

#define _sys_err_msg(m) _sys_errlist[(((m)<0)||((m)>=_sys_nerr)?_sys_nerr:(m))]

/* Maximum length of an error message.
   NOTE: This parameter value must be correspond to the length of the longest
   message in sys_errlist (source module syserr.c). */

#define _SYS_MSGMAX 38

__inline
const char *_get_sys_err_msg(int m)
{
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
    return _sys_err_msg(m);
_END_SECURE_CRT_DEPRECATION_DISABLE
}

#endif  /* _INC_SYSERR */
