/***
*minmax.h - familiar min & max macros
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines min and max macros.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_MINMAX
#define _INC_MINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif  /* max */

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif  /* min */

#endif  /* _INC_MINMAX */
