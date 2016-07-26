/***
*rtcsup.h - declarations and definitions for RTC support (import lib support)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the function declarations for all 'hook' function used from
*       within an external library to support RTC checks.
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_RTCSUP
#define _INC_RTCSUP

#ifdef _RTC

#if !defined (_WIN32)
#error ERROR: Only Win32 target supported!
#endif  /* !defined (_WIN32) */

#include <rtcapi.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

# define RTCCALLBACK(a, b)

extern void *_RTC_api_change_mutex;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#else  /* _RTC */

#define RTCCALLBACK(a, b)

#endif  /* _RTC */

#endif  /* _INC_RTCSUP */
