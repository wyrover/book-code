/***
*bsearch.c - do a binary search
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   defines bsearch() - do a binary search an an array
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <search.h>
#include <internal.h>

#if defined (_M_CEE)
#define __fileDECL  __clrcall
#else  /* defined (_M_CEE) */
#define __fileDECL  __cdecl
#endif  /* defined (_M_CEE) */
/***
*char *bsearch() - do a binary search on an array
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
*           array elements.
*
*Exit:
*   if key is found:
*           returns pointer to occurrence of key in array
*   if key is not found:
*           returns NULL
*
*Exceptions:
*   Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

#ifdef __USE_CONTEXT
#define __COMPARE(context, p1, p2) (*compare)(context, p1, p2)
#else  /* __USE_CONTEXT */
#define __COMPARE(context, p1, p2) (*compare)(p1, p2)
#endif  /* __USE_CONTEXT */

#if !defined (_M_CEE)
_CRTIMP
#endif  /* !defined (_M_CEE) */

#ifdef __USE_CONTEXT
void * __fileDECL bsearch_s (
    REG4 const void *key,
    const void *base,
    size_t num,
    size_t width,
    int (__fileDECL *compare)(void *, const void *, const void *),
    void *context
    )
#else  /* __USE_CONTEXT */
void * __fileDECL bsearch (
    REG4 const void *key,
    const void *base,
    size_t num,
    size_t width,
    int (__fileDECL *compare)(const void *, const void *)
    )
#endif  /* __USE_CONTEXT */
{
    REG1 char *lo = (char *)base;
    REG2 char *hi = (char *)base + (num - 1) * width;
    REG3 char *mid;
    size_t half;
    int result;

    /* validation section */
    _VALIDATE_RETURN(base != NULL || num == 0, EINVAL, NULL);
    _VALIDATE_RETURN(width > 0, EINVAL, NULL);
    _VALIDATE_RETURN(compare != NULL, EINVAL, NULL);

        /*
        We allow a NULL key here because it breaks some older code and because we do not dereference
        this ourselves so we can't be sure that it's a problem for the comparison function
        */

    while (lo <= hi)
    {
        if ((half = num / 2) != 0)
        {
            mid = lo + (num & 1 ? half : (half - 1)) * width;
            if (!(result = __COMPARE(context, key, mid)))
                return(mid);
            else if (result < 0)
            {
                hi = mid - width;
                num = num & 1 ? half : half-1;
            }
            else
            {
                lo = mid + width;
                num = half;
            }
        }
        else if (num)
            return (__COMPARE(context, key, lo) ? NULL : lo);
        else
            break;
    }

    return NULL;
}

#undef __fileDECL
#undef __COMPARE
