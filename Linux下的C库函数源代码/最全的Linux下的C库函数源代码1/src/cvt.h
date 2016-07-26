/***
*cvt.h - definitions used by formatting routines
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       cvt.h contains definitions used by the formatting routines [efg]cvt and
*       _output and _input.
*
*       [Internal]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_CVT
#define _INC_CVT

#ifndef _CRTBLD
/*
 * This is an internal C runtime header file. It is used when building
 * the C runtimes only. It is not to be used as a public header file.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#define _SHORT  1
#define _LONG   2
#define _USIGN  4
#define _NEAR   8
#define _FAR    16

#define OCTAL   8
#define DECIMAL 10
#define HEX     16

#define MUL10(x)        ( (((x)<<2) + (x))<<1 )
#define ISDIGIT(c)      ( ((c) >= '0') && ((c) <= '9') )

#endif  /* _INC_CVT */
