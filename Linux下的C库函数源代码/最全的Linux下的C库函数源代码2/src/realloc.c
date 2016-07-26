/***
*realloc.c - Reallocate a block of memory in the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the realloc() and _expand() functions.
*
*******************************************************************************/

#ifdef WINHEAP

#include <cruntime.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <winheap.h>
#include <windows.h>
#include <internal.h>
#include <mtdll.h>
#include <dbgint.h>
#include <rtcsup.h>


/***
*void *realloc(pblock, newsize) - reallocate a block of memory in the heap
*
*Purpose:
*       Reallocates a block in the heap to newsize bytes. newsize may be
*       either greater or less than the original size of the block. The
*       reallocation may result in moving the block as well as changing
*       the size. If the block is moved, the contents of the original block
*       are copied over.
*
*       Special ANSI Requirements:
*
*       (1) realloc(NULL, newsize) is equivalent to malloc(newsize)
*
*       (2) realloc(pblock, 0) is equivalent to free(pblock) (except that
*           NULL is returned)
*
*       (3) if the realloc() fails, the object pointed to by pblock is left
*           unchanged
*
*Entry:
*       void *pblock    - pointer to block in the heap previously allocated
*                         by a call to malloc(), realloc() or _expand().
*
*       size_t newsize  - requested size for the re-allocated block
*
*Exit:
*       Success:  Pointer to the re-allocated memory block
*       Failure:  NULL
*
*Uses:
*
*Exceptions:
*       If pblock does not point to a valid allocation block in the heap,
*       realloc() will behave unpredictably and probably corrupt the heap.
*
*******************************************************************************/

void * __cdecl _realloc_base (void * pBlock, size_t newsize)
{
        void *      pvReturn;
        size_t      origSize = newsize;

        //  if ptr is NULL, call malloc
        if (pBlock == NULL)
            return(_malloc_base(newsize));

        //  if ptr is nonNULL and size is zero, call free and return NULL
        if (newsize == 0)
        {
            _free_base(pBlock);
            return(NULL);
        }


#ifndef _WIN64
        if ( __active_heap == __V6_HEAP )
        {
            PHEADER     pHeader;
            size_t      oldsize;

            for (;;)
            {
                pvReturn = NULL;
                if (newsize <= _HEAP_MAXREQ)
                {
                    _mlock( _HEAP_LOCK );
                    __try
                    {

                    //  test if current block is in the small-block heap
                    if ((pHeader = __sbh_find_block(pBlock)) != NULL)
                    {
                        //  if the new size is not over __sbh_threshold, attempt
                        //  to reallocate within the small-block heap
                        if (newsize <= __sbh_threshold)
                        {
                            if (__sbh_resize_block(pHeader, pBlock, (int)newsize))
                                pvReturn = pBlock;
                            else if ((pvReturn = __sbh_alloc_block((int)newsize)) != NULL)
                            {
                                oldsize = ((PENTRY)((char *)pBlock -
                                                    sizeof(int)))->sizeFront - 1;
                                memcpy(pvReturn, pBlock, __min(oldsize, newsize));
                                // headers may have moved, get pHeader again
                                pHeader = __sbh_find_block(pBlock);
                                __sbh_free_block(pHeader, pBlock);
                            }
                        }

                        //  If the reallocation has not been (successfully)
                        //  performed in the small-block heap, try to allocate
                        //  a new block with HeapAlloc.
                        if (pvReturn == NULL)
                        {
                            if (newsize == 0)
                                newsize = 1;
                            newsize = (newsize + BYTES_PER_PARA - 1) &
                                      ~(BYTES_PER_PARA - 1);
                            if ((pvReturn = HeapAlloc(_crtheap, 0, newsize)) != NULL)
                            {
                                oldsize = ((PENTRY)((char *)pBlock -
                                                    sizeof(int)))->sizeFront - 1;
                                memcpy(pvReturn, pBlock, __min(oldsize, newsize));
                                __sbh_free_block(pHeader, pBlock);
                            }
                        }
                    }

                    }
                    __finally
                    {
                        _munlock( _HEAP_LOCK );
                    }

                    //  the current block is NOT in the small block heap iff pHeader
                    //  is NULL
                    if ( pHeader == NULL )
                    {
                        if (newsize == 0)
                            newsize = 1;
                        newsize = (newsize + BYTES_PER_PARA - 1) &
                                  ~(BYTES_PER_PARA - 1);
                        pvReturn = HeapReAlloc(_crtheap, 0, pBlock, newsize);
                    }
                }
                else    /* newsize > _HEAP_MAXREQ */
                {
                    _callnewh(newsize);
                    errno = ENOMEM;
                    return NULL;
                }

                if ( pvReturn || _newmode == 0)
                {
                    if (pvReturn)
                    {
                        RTCCALLBACK(_RTC_Free_hook, (pBlock, 0));
                        RTCCALLBACK(_RTC_Allocate_hook, (pvReturn, newsize, 0));
                    }
                    else
                    {
                        if (pHeader == NULL)
                        {
                            /* we called HeapReAlloc, so we can use GetLastError */
                            errno = _get_errno_from_oserr(GetLastError());
                        }
                        else
                        {
                            errno = ENOMEM;
                        }
                    }
                    return pvReturn;
                }

                /*  call installed new handler */
                if (!_callnewh(newsize))
                {
                    if (pHeader == NULL)
                    {
                        /* we called HeapReAlloc, so we can use GetLastError */
                        errno = _get_errno_from_oserr(GetLastError());
                    }
                    else
                    {
                        errno = ENOMEM;
                    }
                    return NULL;
                }

                /*  new handler was successful -- try to allocate again */
            }
        }
#ifdef CRTDLL
        else if ( __active_heap == __V5_HEAP )
        {
            __old_sbh_region_t *preg;
            __old_sbh_page_t *  ppage;
            __old_page_map_t *  pmap;
            size_t              oldsize;

            //  round up to the nearest paragrap
            if ( newsize <= _HEAP_MAXREQ )
                if ( newsize > 0 )
                    newsize = (newsize + _OLD_PARASIZE - 1) & ~(_OLD_PARASIZE - 1);
                else
                    newsize = _OLD_PARASIZE;

            for (;;)
            {
                pvReturn = NULL;
                if ( newsize <= _HEAP_MAXREQ )
                {
                    _mlock( _HEAP_LOCK );
                    __try
                    {
                    if ( (pmap = __old_sbh_find_block(pBlock, &preg, &ppage)) != NULL )
                    {
                        //  If the new size falls below __sbh_threshold, try to
                        //  carry out the reallocation within the small-block
                        //  heap.
                        if ( newsize < __old_sbh_threshold )
                        {
                            if ( __old_sbh_resize_block(preg, ppage, pmap,
                                 newsize >> _OLD_PARASHIFT) )
                            {
                                pvReturn = pBlock;
                            }
                            else if ( (pvReturn = __old_sbh_alloc_block(newsize >>
                                       _OLD_PARASHIFT)) != NULL )
                            {
                                oldsize = ((size_t)(*pmap)) << _OLD_PARASHIFT ;
                                memcpy(pvReturn, pBlock, __min(oldsize, newsize));
                                __old_sbh_free_block(preg, ppage, pmap);
                            }
                        }

                        //  If the reallocation has not been (successfully)
                        //  performed in the small-block heap, try to allocate a
                        //  new block with HeapAlloc.
                        if ( (pvReturn == NULL) &&
                             ((pvReturn = HeapAlloc(_crtheap, 0, newsize)) != NULL) )
                        {
                            oldsize = ((size_t)(*pmap)) << _OLD_PARASHIFT;
                            memcpy(pvReturn, pBlock, __min(oldsize, newsize));
                            __old_sbh_free_block(preg, ppage, pmap);
                        }
                    }
                    else
                    {
                        pvReturn = HeapReAlloc(_crtheap, 0, pBlock, newsize);
                    }

                    }
                    __finally
                    {
                        _munlock(_HEAP_LOCK);
                    }
                }
                else    /* newsize > _HEAP_MAXREQ */
                {
                    _callnewh(newsize);
                    errno = ENOMEM;
                    return NULL;
                }

                if ( pvReturn || _newmode == 0)
                {
                    if (pvReturn)
                    {
                        RTCCALLBACK(_RTC_Free_hook, (pBlock, 0));
                        RTCCALLBACK(_RTC_Allocate_hook, (pvReturn, newsize, 0));
                    }
                    else
                    {
                        if (pmap == NULL)
                        {
                            /* we called HeapReAlloc, so we can use GetLastError */
                            errno = _get_errno_from_oserr(GetLastError());
                        }
                        else
                        {
                            errno = ENOMEM;
                        }
                    }
                    return pvReturn;
                }

                //  call installed new handler
                if (!_callnewh(newsize))
                {
                    if (pmap == NULL)
                    {
                        /* we called HeapReAlloc, so we can use GetLastError */
                        errno = _get_errno_from_oserr(GetLastError());
                    }
                    else
                    {
                        errno = ENOMEM;
                    }

                    return NULL;
                }

                //  new handler was successful -- try to allocate again
            }
        }
#endif  /* CRTDLL */
        else    //  __active_heap == __SYSTEM_HEAP )
#endif  /* _WIN64 */
        {
            for (;;) {

                pvReturn = NULL;
                if (newsize <= _HEAP_MAXREQ)
                {
                    if (newsize == 0)
                        newsize = 1;
                    pvReturn = HeapReAlloc(_crtheap, 0, pBlock, newsize);
                }
                else
                {
                    _callnewh(newsize);
                    errno = ENOMEM;
                    return NULL;
                }

                if ( pvReturn || _newmode == 0)
                {
                    if (pvReturn)
                    {
                        RTCCALLBACK(_RTC_Free_hook, (pBlock, 0));
                        RTCCALLBACK(_RTC_Allocate_hook, (pvReturn, newsize, 0));
                    }
                    else
                    {
                        errno = _get_errno_from_oserr(GetLastError());
                    }
                    return pvReturn;
                }

                //  call installed new handler
                if (!_callnewh(newsize))
                {
                    errno = _get_errno_from_oserr(GetLastError());
                    return NULL;
                }

                //  new handler was successful -- try to allocate again
            }
        }
}

#else  /* WINHEAP */


#include <cruntime.h>
#include <heap.h>
#include <malloc.h>
#include <mtdll.h>
#include <stddef.h>
#include <string.h>
#include <dbgint.h>

/* useful macro to compute the size of an allocation block given both a
 * pointer to the descriptor and a pointer to the user area of the block
 * (more efficient variant of _BLKSIZE macro, given the extra information)
 */
#define BLKSZ(pdesc_m,pblock_m)   ((unsigned)_ADDRESS((pdesc_m)->pnextdesc) - \
                    (unsigned)(pblock_m))

/* expand an allocation block, in place, up to or beyond a specified size
 * by coalescing it with subsequent free blocks (if possible)
 */
static int __cdecl _heap_expand_block(_PBLKDESC, size_t *, size_t);

/***
*void *realloc(void *pblock, size_t newsize) - reallocate a block of memory in
*       the heap
*
*Purpose:
*       Re-allocates a block in the heap to newsize bytes. newsize may be
*       either greater or less than the original size of the block. The
*       re-allocation may result in moving the block as well as changing
*       the size. If the block is moved, the contents of the original block
*       are copied over.
*
*       Special ANSI Requirements:
*
*       (1) realloc(NULL, newsize) is equivalent to malloc(newsize)
*
*       (2) realloc(pblock, 0) is equivalent to free(pblock) (except that
*           NULL is returned)
*
*       (3) if the realloc() fails, the object pointed to by pblock is left
*           unchanged
*
*       Special Notes For Multi-thread: The heap is locked immediately prior
*       to assigning pdesc. This is after special cases (1) and (2), listed
*       above, are taken care of. The lock is released immediately prior to
*       the final return statement.
*
*Entry:
*       void *pblock - pointer to block in the heap previously allocated
*                 by a call to malloc(), realloc() or _expand().
*
*       size_t newsize  - requested size for the re-allocated block
*
*Exit:
*       Success:  Pointer to the re-allocated memory block
*       Failure:  NULL
*
*Uses:
*
*Exceptions:
*       If pblock does not point to a valid allocation block in the heap,
*       realloc() will behave unpredictably and probably corrupt the heap.
*
*******************************************************************************/

void * __cdecl _realloc_base (
        REG1 void *pblock,
        size_t newsize
        )
{
        REG2 _PBLKDESC pdesc;
        _PBLKDESC pdesc2;
        void *retp;
        size_t oldsize;
        size_t currsize;

        /* special cases, handling mandated by ANSI
         */
        if ( pblock == NULL )
            /* just do a malloc of newsize bytes and return a pointer to
             * the new block
              */
            return( _malloc_base(newsize) );

        if ( newsize == 0 ) {
            /* free the block and return NULL
             */
            _free_base(pblock);
            return( NULL );
        }

        /* make newsize a valid allocation block size (i.e., round up to the
         * nearest whole number of dwords)
         */
        newsize = _ROUND2(newsize, _GRANULARITY);


        /* if multi-thread support enabled, lock the heap here
         */
        _mlock(_HEAP_LOCK);

        /* set pdesc to point to the descriptor for *pblock
         */
        pdesc = _BACKPTR(pblock);

        if ( _ADDRESS(pdesc) != ((char *)pblock - _HDRSIZE) )
            _heap_abort();

        /* see if pblock is big enough already, or can be expanded (in place)
         * to be big enough.
         */
        if ( ((oldsize = currsize = BLKSZ(pdesc, pblock)) > newsize) ||
             (_heap_expand_block(pdesc, &currsize, newsize) == 0) ) {

            /* if necessary, mark pdesc as inuse
             */
            if ( _IS_FREE(pdesc) ) {
                _SET_INUSE(pdesc);
            }

            /* trim pdesc down to be exactly newsize bytes, if necessary
             */
            if ( (currsize > newsize) &&
                 ((pdesc2 = _heap_split_block(pdesc, newsize)) != NULL) )
            {
                _SET_FREE(pdesc2);
            }

            retp = pblock;
            goto realloc_done;
        }

        /* try malloc-ing a new block of the requested size. if successful,
         * copy over the data from the original block and free it.
         */
        if ( (retp = _malloc_base(newsize)) != NULL ) {
            memcpy(retp, pblock, oldsize);
            _free_base_nolock(pblock);
        }
        /* else if unsuccessful, return retp (== NULL) */

realloc_done:
        /* if multi-thread support is enabled, unlock the heap here
         */
        _munlock(_HEAP_LOCK);

        return(retp);
}


/***
*void *_expand(void *pblock, size_t newsize) - expand/contract a block of memory
*       in the heap
*
*Purpose:
*       Resizes a block in the heap to newsize bytes. newsize may be either
*       greater (expansion) or less (contraction) than the original size of
*       the block. The block is NOT moved. In the case of expansion, if the
*       block cannot be expanded to newsize bytes, it is expanded as much as
*       possible.
*
*       Special Notes For Multi-thread: The heap is locked just before pdesc
*       is assigned and unlocked immediately prior to the return statement.
*
*Entry:
*       void *pblock - pointer to block in the heap previously allocated
*                 by a call to malloc(), realloc() or _expand().
*
*       size_t newsize  - requested size for the resized block
*
*Exit:
*       Success:  Pointer to the resized memory block (i.e., pblock)
*       Failure:  NULL
*
*Uses:
*
*Exceptions:
*       If pblock does not point to a valid allocation block in the heap,
*       _expand() will behave unpredictably and probably corrupt the heap.
*
*******************************************************************************/

void * __cdecl _expand_base (
        REG1 void *pblock,
        size_t newsize
        )
{
        REG2 _PBLKDESC pdesc;
        _PBLKDESC pdesc2;
        void *retp;
        size_t oldsize;
        size_t currsize;
        int index;

        /* make newsize a valid allocation block size (i.e., round up to the
         * nearest whole number of dwords)
         */
        newsize = _ROUND2(newsize, _GRANULARITY);


        retp = pblock;

        /* validate size */
        if ( newsize > _HEAP_MAXREQ )
            newsize = _HEAP_MAXREQ;

        /* if multi-thread support enabled, lock the heap here
         */
        _mlock(_HEAP_LOCK);

        /* set pdesc to point to the descriptor for *pblock
         */
        pdesc = _BACKPTR(pblock);

        /* see if pblock is big enough already, or can be expanded (in place)
         * to be big enough.
         */
        if ( ((oldsize = currsize = BLKSZ(pdesc, pblock)) >= newsize) ||
             (_heap_expand_block(pdesc, &currsize, newsize) == 0) ) {
            /* pblock is (now) big enough. trim it down, if necessary
             */
            if ( (currsize > newsize) &&
                 ((pdesc2 = _heap_split_block(pdesc, newsize)) != NULL) )
            {
                _SET_FREE(pdesc2);
                currsize = newsize;
            }
            goto expand_done;
        }

        /* if the heap block is at the end of a region, attempt to grow the
         * region
         */
        if ( (pdesc->pnextdesc == &_heap_desc.sentinel) ||
             _IS_DUMMY(pdesc->pnextdesc) ) {

            /* look up the region index
             */
            for ( index = 0 ; index < _HEAP_REGIONMAX ; index++ )
                if ( (_heap_regions[index]._regbase < pblock) &&
                     (((char *)(_heap_regions[index]._regbase) +
                       _heap_regions[index]._currsize) >=
                     (char *)pblock) )
                    break;

            /* make sure a valid region index was obtained (pblock could
             * lie in a portion of heap memory donated by a user call to
             * _heapadd(), which therefore would not appear in the region
             * table)
             */
            if ( index == _HEAP_REGIONMAX ) {
                retp = NULL;
                goto expand_done;
            }

            /* try growing the region. the difference between newsize and
             * the current size of the block, rounded up to the nearest
             * whole number of pages, is the amount the region needs to
             * be grown. if successful, try expanding the block again
             */
            if ( (_heap_grow_region(index, _ROUND2(newsize - currsize,
                  _PAGESIZE_)) == 0) &&
                 (_heap_expand_block(pdesc, &currsize, newsize) == 0) )
            {
                /* pblock is (now) big enough. trim it down to be
                 * exactly size bytes, if necessary
                 */
                if ( (currsize > newsize) && ((pdesc2 =
                       _heap_split_block(pdesc, newsize)) != NULL) )
                {
                    _SET_FREE(pdesc2);
                    currsize = newsize;
                }
            }
            else
                retp = NULL;
        }
        else
            retp = NULL;

expand_done:
        /* if multi-thread support is enabled, unlock the heap here
         */
        _munlock(_HEAP_LOCK);

        return(retp);
}


/***
*int _heap_expand_block(pdesc, pcurrsize, newsize) - expand an allocation block
*       in place (without trying to 'grow' the heap)
*
*Purpose:
*
*Entry:
*       _PBLKDESC pdesc   - pointer to the allocation block descriptor
*       size_t *pcurrsize - pointer to size of the allocation block (i.e.,
*                   *pcurrsize == _BLKSIZE(pdesc), on entry)
*       size_t newsize    - requested minimum size for the expanded allocation
*                   block (i.e., newsize >= _BLKSIZE(pdesc), on exit)
*
*Exit:
*       Success:  0
*       Failure: -1
*       In either case, *pcurrsize is updated with the new size of the block
*
*Exceptions:
*       It is assumed that pdesc points to a valid allocation block descriptor.
*       It is also assumed that _BLKSIZE(pdesc) == *pcurrsize on entry. If
*       either of these assumptions is violated, _heap_expand_block will almost
*       certainly trash the heap.
*
*******************************************************************************/

static int __cdecl _heap_expand_block (
        REG1 _PBLKDESC pdesc,
        REG3 size_t *pcurrsize,
        size_t newsize
        )
{
        REG2 _PBLKDESC pdesc2;

        _ASSERTE(("_heap_expand_block: bad pdesc arg", _CHECK_PDESC(pdesc)));
        _ASSERTE(("_heap_expand_block: bad pcurrsize arg", *pcurrsize == _BLKSIZE(pdesc)));

        for ( pdesc2 = pdesc->pnextdesc ; _IS_FREE(pdesc2) ;
              pdesc2 = pdesc->pnextdesc ) {

            /* coalesce with pdesc. check for special case of pdesc2
             * being proverdesc.
             */
            pdesc->pnextdesc = pdesc2->pnextdesc;

            if ( pdesc2 == _heap_desc.proverdesc )
                _heap_desc.proverdesc = pdesc;

            /* update *pcurrsize, place *pdesc2 on the empty descriptor
             * list and see if the coalesced block is now big enough
             */
            *pcurrsize += _MEMSIZE(pdesc2);

            _PUTEMPTY(pdesc2)
        }

        if ( *pcurrsize >= newsize )
            return(0);
        else
            return(-1);
}


#endif  /* WINHEAP */

/***
*void *_recalloc_base(pblock, count, newsize) - reallocate a block of memory in the heap
*
*Purpose:
*       Reallocates a block in the heap to count*newsize bytes. newsize may be
*       either greater or less than the original size of the block. The
*       reallocation may result in moving the block as well as changing
*       the size. If the block is moved, the contents of the original block
*       are copied over.
*
*Entry:
*       void *pblock    - pointer to block in the heap previously allocated
*                         by a call to malloc(), realloc() or _expand().
*
*       size_t count    - count of items
*       size_t newsize  - requested size for the re-allocated items
*
*Exit:
*       Success:  Pointer to the re-allocated memory block
*       Failure:  NULL
*
*Uses:
*
*Exceptions:
*       If pblock does not point to a valid allocation block in the heap,
*       realloc() will behave unpredictably and probably corrupt the heap.
*
*******************************************************************************/

void * __cdecl _recalloc_base(void * memblock, size_t count, size_t size)
{
    size_t  size_orig=0;

    /* ensure that (size * count) does not overflow */
    if (count > 0)
    {
        _VALIDATE_RETURN((_HEAP_MAXREQ / count) >= size, ENOMEM, NULL);
    }
    size_orig = size * count;

    return _realloc_base(memblock, size_orig);

}
