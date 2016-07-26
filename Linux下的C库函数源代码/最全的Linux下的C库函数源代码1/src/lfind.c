/***
*lfind.c - do a linear search
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _lfind() - do a linear search of an array.
*
*******************************************************************************/

#include <cruntime.h>
#include <search.h>
#include <stddef.h>
#include <internal.h>

#if defined (_M_CEE)
#define __fileDECL  __clrcall
#else  /* defined (_M_CEE) */
#define __fileDECL  __cdecl
#endif  /* defined (_M_CEE) */

/***
*char *_lfind(key, base, num, width, compare) - do a linear search
*
*Purpose:
*       Performs a linear search on the array, looking for the value key
*       in an array of num elements of width bytes in size.  Returns
*       a pointer to the array value if found, NULL if not found.
*
*Entry:
*       char *key - key to search for
*       char *base - base of array to search
*       unsigned *num - number of elements in array
*       int width - number of bytes in each array element
*       int (*compare)() - pointer to function that compares two
*               array values, returning 0 if they are equal and non-0
*               if they are different.  Two pointers to array elements
*               are passed to this function.
*
*Exit:
*       if key found:
*               returns pointer to array element
*       if key not found:
*               returns NULL
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
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
void *__fileDECL _lfind_s (
        REG2 const void *key,
        REG1 const void *base,
        REG3 unsigned int *num,
        size_t width,
        int (__fileDECL *compare)(void *, const void *, const void *),
        void *context
        )
#else  /* __USE_CONTEXT */
void *__fileDECL _lfind (
        REG2 const void *key,
        REG1 const void *base,
        REG3 unsigned int *num,
        unsigned int width,
        int (__fileDECL *compare)(const void *, const void *)
        )
#endif  /* __USE_CONTEXT */
{
        unsigned int place = 0;

        /* validation section */
        _VALIDATE_RETURN(key != NULL, EINVAL, NULL);
        _VALIDATE_RETURN(num != NULL, EINVAL, NULL);
        _VALIDATE_RETURN(base != NULL || *num == 0, EINVAL, NULL);
        _VALIDATE_RETURN(width > 0, EINVAL, NULL);
        _VALIDATE_RETURN(compare != NULL, EINVAL, NULL);

        while (place < *num)
        {
                if (__COMPARE(context, key, base) == 0)
                {
                        return (void *)base;
                }
                else
                {
                        base = (char*)base + width;
                        place++;
                }
        }
        return NULL;
}

#undef __fileDECL
#undef __COMPARE
