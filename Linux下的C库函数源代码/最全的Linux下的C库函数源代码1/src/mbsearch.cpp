/***
*mbsearch.cpp - do a binary search
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines bsearch() - do a binary search an an array
*
*******************************************************************************/

#ifdef MRTDLL
#undef MRTDLL
#endif  /* MRTDLL */

#if defined (_M_CEE)
#include "bsearch.c"

#ifdef __USE_CONTEXT
#error __USE_CONTEXT should be undefined
#endif  /* __USE_CONTEXT */

#define __USE_CONTEXT
#include "bsearch.c"
#endif  /* defined (_M_CEE) */
