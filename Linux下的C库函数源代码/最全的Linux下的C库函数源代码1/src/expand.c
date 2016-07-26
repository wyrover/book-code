/***
*expand.c - Win32 expand heap routine
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#ifdef WINHEAP

#include <cruntime.h>
#include <malloc.h>
#include <winheap.h>
#include <windows.h>
#include <mtdll.h>
#include <dbgint.h>
#include <rtcsup.h>
#include <internal.h>

/***
*void *_expand(void *pblck, size_t newsize) - expand/contract a block of memory
*       in the heap
*
*Purpose:
*       Resizes a block in the heap to newsize bytes. newsize may be either
*       greater (expansion) or less (contraction) than the original size of
*       the block. The block is NOT moved.
*
*       NOTES:
*
*       (1) In this implementation, if the block cannot be grown to the
*       desired size, the resulting block will NOT be grown to the max
*       possible size.  (That is, either it works or it doesn't.)
*
*       (2) Unlike other implementations, you can NOT pass a previously
*       freed block to this routine and expect it to work.
*
*Entry:
*       void *pblck - pointer to block in the heap previously allocated
*                 by a call to malloc(), realloc() or _expand().
*
*       size_t newsize  - requested size for the resized block
*
*Exit:
*       Success:  Pointer to the resized memory block (i.e., pblck)
*       Failure:  NULL, errno is set
*
*Uses:
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*       If pblck does not point to a valid allocation block in the heap,
*       _expand() will behave unpredictably and probably corrupt the heap.
*
*******************************************************************************/

void * __cdecl _expand_base (void * pBlock, size_t newsize)
{
        void *      pvReturn;


        /* validation section */
        _VALIDATE_RETURN(pBlock != NULL, EINVAL, NULL);
        if (newsize > _HEAP_MAXREQ) {
            errno = ENOMEM;
            return NULL;
        }

#ifndef _WIN64
        if ( __active_heap == __V6_HEAP )
        {
            PHEADER     pHeader;

            _mlock( _HEAP_LOCK );
            __try {

            //  if allocation block lies within the small-block heap,
            //  try to resize it there
            if ((pHeader = __sbh_find_block(pBlock)) != NULL)
            {
                pvReturn = NULL;
                if ( (newsize <= __sbh_threshold) &&
                     __sbh_resize_block(pHeader, pBlock, (int)newsize) )
                    pvReturn = pBlock;
            }

            }
            __finally {
                _munlock( _HEAP_LOCK );
            }

            if ( pHeader == NULL )
            {
                //  force nonzero size and round up to next paragraph
                if (newsize == 0)
                    newsize = 1;
                newsize = (newsize + BYTES_PER_PARA - 1) & ~(BYTES_PER_PARA - 1);

                pvReturn = HeapReAlloc(_crtheap, HEAP_REALLOC_IN_PLACE_ONLY,
                                       pBlock, newsize);

                if (pvReturn == NULL)
                {
                    errno = _get_errno_from_oserr(GetLastError());
                }
            }
        }
#ifdef CRTDLL
        else if ( __active_heap == __V5_HEAP )
        {
            __old_sbh_region_t *preg;
            __old_sbh_page_t *  ppage;
            __old_page_map_t *  pmap;

            //  force nonzero size and round up to next paragraph
            if (newsize == 0)
                newsize = 1;
            newsize = (newsize + _OLD_PARASIZE - 1) & ~(_OLD_PARASIZE - 1);
            _mlock(_HEAP_LOCK);
            __try {
            pmap = __old_sbh_find_block(pBlock, &preg, &ppage);

            //  allocation block lies within the small-block heap, try to resize
            //  it there.
            if ( pmap != NULL )
            {
                //  *pBlock lies within the small-block heap, try to resize it
                //  there
                pvReturn = NULL;
                if ( (newsize <= __old_sbh_threshold) &&
                     __old_sbh_resize_block(preg, ppage, pmap,
                                            newsize >> _OLD_PARASHIFT) )
                    pvReturn = pBlock;

                RTCCALLBACK(_RTC_Free_hook, (pBlock, 0));
                RTCCALLBACK(_RTC_Allocate_hook, (pvReturn, newsize, 0));
            }

            }
            __finally {
                _munlock(_HEAP_LOCK);
            }

            if (pmap != NULL)
                return pvReturn;

            if ( pmap == NULL )
            {
                pvReturn = HeapReAlloc(_crtheap, HEAP_REALLOC_IN_PLACE_ONLY,
                                       pBlock, newsize);

                if (pvReturn == NULL)
                {
                    errno = _get_errno_from_oserr(GetLastError());
                }
            }
        }
#endif  /* CRTDLL */
        else    // __active_heap == __SYSTEM_HEAP
#endif  /* _WIN64 */
        {
#ifdef _WIN64
            //  force nonzero size
                        {
                                size_t oldsize = (size_t)-1;

                                if (newsize == 0)
                                {
                                        newsize = 1;
                                }

                                /* On Win64 we use a Low Fragmentation Heap (LFH) by default, and thus
                                * HeapReAlloc(..., HEAP_REALLOC_IN_PLACE_ONLY, ..., ...) will
                                * sometimes return NULL, even for sizes smaller or equal than the original size.
                                * We explictly check for this case and we just return the original
                                * block. Note that HeapSize returns -1 to signal failure.
                                */
                                oldsize = (size_t)HeapSize(_crtheap, 0, pBlock);
                                pvReturn = HeapReAlloc(_crtheap, HEAP_REALLOC_IN_PLACE_ONLY, pBlock, newsize);
                                if(pvReturn==NULL)
                                {
                                        if (oldsize != (size_t)-1 && newsize <= oldsize)
                                        {
                                                /* hide the fact that we couldn't shrink and just reuse this block */
                                                pvReturn = pBlock;
                                        }
                                }
                        }
#else  /* _WIN64 */
            pvReturn = HeapReAlloc(_crtheap, HEAP_REALLOC_IN_PLACE_ONLY, pBlock, newsize);
#endif  /* _WIN64 */
            if (pvReturn == NULL)
            {
                errno = _get_errno_from_oserr(GetLastError());
            }
        }

        if (pvReturn)
        {
            RTCCALLBACK(_RTC_Free_hook, (pBlock, 0));
            RTCCALLBACK(_RTC_Allocate_hook, (pvReturn, newsize, 0));
        }

        return pvReturn;
}


#endif  /* WINHEAP */
