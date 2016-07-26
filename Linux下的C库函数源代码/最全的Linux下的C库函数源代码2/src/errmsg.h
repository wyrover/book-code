/***
*errmsg.h - defines error message numbers
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the constants for error message numbers.
*       Same as errmsg.inc
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_ERRMSG

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#define STCKOVR 0
#define NULLERR 1
#define NOFP    2
#define DIVZR   3
#define BADVERS 4
#define NOMEM   5
#define BADFORM 6
#define BADENV  7
#define NOARGV  8
#define NOENVP  9
#define ABNORM  10
#define UNKNOWN 11

#define CRT_NERR 11

#define _INC_ERRMSG
#endif  /* _INC_ERRMSG */
