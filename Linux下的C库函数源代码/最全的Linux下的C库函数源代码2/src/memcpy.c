/***
*memcpy.c - contains memcpy routine
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       memcpy() copies a source memory buffer to a destination buffer.
*       Overlapping buffers are not treated specially, so propogation may occur.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>

#ifdef _MSC_VER
#pragma function(memcpy)
#endif  /* _MSC_VER */

/***
*memcpy - Copy source buffer to destination buffer
*
*Purpose:
*       memcpy() copies a source memory buffer to a destination memory buffer.
*       This routine does NOT recognize overlapping buffers, and thus can lead
*       to propogation.
*
*       For cases where propogation must be avoided, memmove() must be used.
*
*Entry:
*       void *dst = pointer to destination buffer
*       const void *src = pointer to source buffer
*       size_t count = number of bytes to copy
*
*Exit:
*       Returns a pointer to the destination buffer
*
*Exceptions:
*******************************************************************************/

void * __cdecl memcpy (
        void * dst,
        const void * src,
        size_t count
        )
{
        void * ret = dst;

#if defined (_M_IA64)

        {


        __declspec(dllimport)


        void RtlCopyMemory( void *, const void *, size_t count );

        RtlCopyMemory( dst, src, count );

        }

#else  /* defined (_M_IA64) */
        /*
         * copy from lower addresses to higher addresses
         */
        while (count--) {
                *(char *)dst = *(char *)src;
                dst = (char *)dst + 1;
                src = (char *)src + 1;
        }
#endif  /* defined (_M_IA64) */

        return(ret);
}
