/***
*bsearch_s.c - do a binary search
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines bsearch_s() - do a binary search an an array
*
*******************************************************************************/

/***
*char *bsearch_s() - do a binary search on an array
*
*Purpose:
*   Does a binary search of a sorted array for a key.
*
*Entry:
*   const char *key    - key to search for
*   const char *base   - base of sorted array to search
*   unsigned int num   - number of elements in array
*   unsigned int width - number of bytes per element
*   int (*compare)()   - pointer to function that compares two array
*           elements, returning neg when #1 < #2, pos when #1 > #2, and
*           0 when they are equal. Function is passed pointers to two
*           array elements, together with a pointer to a context.
*   void *context - pointer to the context in which the function is
*           called. This context is passed to the comparison function.
*
*Exit:
*   if key is found:
*           returns pointer to occurrence of key in array
*   if key is not found:
*           returns NULL
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

#ifdef __USE_CONTEXT
#error __USE_CONTEXT should be undefined
#endif  /* __USE_CONTEXT */

#define __USE_CONTEXT
#include "bsearch.c"
