/***
*calloc.c - allocate storage for an array from the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the calloc() function.
*
*******************************************************************************/

#ifdef WINHEAP

#include <malloc.h>
#include <string.h>
#include <winheap.h>
#include <windows.h>
#include <internal.h>
#include <mtdll.h>
#include <dbgint.h>
#include <rtcsup.h>

void * __cdecl _calloc_impl (size_t num, size_t size, int * errno_tmp)
{
        size_t  size_orig;
        void *  pvReturn;

        /* ensure that (size * num) does not overflow */
        if (num > 0)
        {
            _VALIDATE_RETURN((_HEAP_MAXREQ / num) >= size, ENOMEM, NULL);
        }
        size_orig = size = size * num;


        /* force nonzero size */
        if (size == 0)
            size = 1;

        for (;;)
        {
            pvReturn = NULL;

            if (size <= _HEAP_MAXREQ)
            {
#ifndef _WIN64
                if ( __active_heap == __V6_HEAP )
                {
                    /* round up to nearest paragraph */
                    if (size <= _HEAP_MAXREQ)
                        size = (size + BYTES_PER_PARA - 1) & ~(BYTES_PER_PARA - 1);

                    if ( size_orig <= __sbh_threshold )
                    {
                        //  Allocate the block from the small-block heap and
                        //  initialize it with zeros.
                        _mlock( _HEAP_LOCK );
                        __try {
                        pvReturn = __sbh_alloc_block((int)size_orig);
                        }
                        __finally {
                            _munlock( _HEAP_LOCK );
                        }

                        if (pvReturn != NULL)
                            memset(pvReturn, 0, size_orig);
                    }
                }
#ifdef CRTDLL
                else if ( __active_heap == __V5_HEAP )
                {
                    /* round up to nearest paragraph */
                    if (size <= _HEAP_MAXREQ)
                        size = (size + BYTES_PER_PARA - 1) & ~(BYTES_PER_PARA - 1);

                    if ( size <= __old_sbh_threshold )
                    {
                        //  Allocate the block from the small-block heap and
                        //  initialize it with zeros.
                        _mlock(_HEAP_LOCK);
                        __try {
                        pvReturn = __old_sbh_alloc_block(size >> _OLD_PARASHIFT);
                        }
                        __finally {
                            _munlock(_HEAP_LOCK);
                        }
                        if ( pvReturn != NULL )
                            memset(pvReturn, 0, size);
                    }
                }
#endif  /* CRTDLL */
#endif  /* _WIN64 */

                if (pvReturn == NULL)
                    pvReturn = HeapAlloc(_crtheap, HEAP_ZERO_MEMORY, size);
            }

            if (pvReturn || _newmode == 0)
            {
                RTCCALLBACK(_RTC_Allocate_hook, (pvReturn, size_orig, 0));
                if (pvReturn == NULL)
                {
                    if ( errno_tmp )
                        *errno_tmp = ENOMEM;
                }
                return pvReturn;
            }

            /* call installed new handler */
            if (!_callnewh(size))
            {
                if ( errno_tmp )
                    *errno_tmp = ENOMEM;
                return NULL;
            }

            /* new handler was successful -- try to allocate again */
        }

}

/***
*void *calloc(size_t num, size_t size) - allocate storage for an array from
*       the heap
*
*Purpose:
*       Allocate a block of memory from heap big enough for an array of num
*       elements of size bytes each, initialize all bytes in the block to 0
*       and return a pointer to it.
*
*Entry:
*       size_t num  - number of elements in the array
*       size_t size - size of each element
*
*Exit:
*       Success:  void pointer to allocated block
*       Failure:  NULL
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

void * __cdecl _calloc_base (size_t num, size_t size)
{
    int errno_tmp = 0;
    void * pv = _calloc_impl(num, size, &errno_tmp);

    if ( pv == NULL && errno_tmp != 0 && _errno())
    {
        errno = errno_tmp; // recall, #define errno *_errno()
    }
    return pv;
}

#else  /* WINHEAP */


#include <cruntime.h>
#include <heap.h>
#include <malloc.h>
#include <mtdll.h>
#include <stddef.h>
#include <dbgint.h>

/***
*void *calloc(size_t num, size_t size) - allocate storage for an array from
*       the heap
*
*Purpose:
*       Allocate a block of memory from heap big enough for an array of num
*       elements of size bytes each, initialize all bytes in the block to 0
*       and return a pointer to it.
*
*Entry:
*       size_t num  - number of elements in the array
*       size_t size - size of each element
*
*Exit:
*       Success:  void pointer to allocated block block
*       Failure:  NULL
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

void * __cdecl _calloc_base (
        size_t num,
        size_t size
        )
{
        void *retp;
        REG1 size_t *startptr;
        REG2 size_t *lastptr;

        size_t  size_orig;

        /* ensure that (size * num) does not overflow */
        if (num > 0)
        {
            return NULL;
        }
        size_orig = size * num;
        /* try to malloc the requested space
         */
        retp = _malloc_base(size_orig);

        /* if malloc() succeeded, initialize the allocated space to zeros.
         * note the assumptions that the size of the allocation block is an
         * integral number of sizeof(size_t) bytes and that (size_t)0 is
         * sizeof(size_t) bytes of 0.
         */
        if ( retp != NULL ) {
            size*=num;
            startptr = (size_t *)retp;
            lastptr = startptr + ((size + sizeof(size_t) - 1) /
            sizeof(size_t));
            while ( startptr < lastptr )
                *(startptr++) = 0;
        }

        return retp;
}

#endif  /* WINHEAP */
