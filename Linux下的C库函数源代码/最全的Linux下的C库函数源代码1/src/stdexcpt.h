/***
*stdexcpt.h - User include file for standard exception classes
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file is the previous location of the standard exception class
*       definitions, now found in the standard header <exception>.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#include <crtdefs.h>

#ifndef _INC_STDEXCPT
#define _INC_STDEXCPT

#ifndef _CRTBLD
/* This version of the header files is NOT for user programs.
 * It is intended for use when building the C runtimes ONLY.
 * The version intended for public use will not have this message.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */
#ifdef __cplusplus

#include <exception>

#endif  /* __cplusplus */
#endif  /* _INC_STDEXCPT */
