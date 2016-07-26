/***
*mlfind.cpp - do a binary search
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _lfind() - do a binary search an an array
*
*******************************************************************************/
#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#if defined (_M_CEE)
#include "lfind.c"

#ifdef __USE_CONTEXT
#error __USE_CONTEXT should be undefined
#endif  /* __USE_CONTEXT */

#define __USE_CONTEXT
#include "lfind.c"
#endif  /* defined (_M_CEE) */
