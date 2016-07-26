/***
*msize.c - calculate the size of a memory block in the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the following function:
*           _msize()    - calculate the size of a block in the heap
*
*******************************************************************************/


#ifdef WINHEAP


#include <cruntime.h>
#include <malloc.h>
#include <mtdll.h>
#include <winheap.h>
#include <windows.h>
#include <dbgint.h>
#include <internal.h>

/***
*size_t _msize(pblock) - calculate the size of specified block in the heap
*
*Purpose:
*       Calculates the size of memory block (in the heap) pointed to by
*       pblock.
*
*Entry:
*       void *pblock - pointer to a memory block in the heap
*
*Return:
*       size of the block
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

size_t __cdecl _msize_base (void * pblock)
{
        size_t      retval;

        /* validation section */
        _VALIDATE_RETURN(pblock != NULL, EINVAL, -1);


#ifndef _WIN64
        if ( __active_heap == __V6_HEAP )
        {
            PHEADER     pHeader;

            _mlock( _HEAP_LOCK );
            __try {
            if ((pHeader = __sbh_find_block(pblock)) != NULL)
                retval = (size_t)
                         (((PENTRY)((char *)pblock - sizeof(int)))->sizeFront - 0x9);
            }
            __finally {
                _munlock( _HEAP_LOCK );
            }
            if ( pHeader == NULL )
                retval = (size_t)HeapSize(_crtheap, 0, pblock);
        }
#ifdef CRTDLL
        else if ( __active_heap == __V5_HEAP )
        {
            __old_sbh_region_t *preg;
            __old_sbh_page_t *  ppage;
            __old_page_map_t *  pmap;
            _mlock(_HEAP_LOCK);
            __try {
            if ( (pmap = __old_sbh_find_block(pblock, &preg, &ppage)) != NULL )
                retval = ((size_t)(*pmap)) << _OLD_PARASHIFT;
            }
            __finally {
                _munlock( _HEAP_LOCK );
            }
            if ( pmap == NULL )
                retval = (size_t) HeapSize( _crtheap, 0, pblock );
        }
#endif  /* CRTDLL */
        else    /* __active_heap == __SYSTEM_HEAP */
#endif  /* _WIN64 */
        {
            retval = (size_t)HeapSize(_crtheap, 0, pblock);
        }

        return retval;

}

#else  /* WINHEAP */


#include <cruntime.h>
#include <heap.h>
#include <malloc.h>
#include <mtdll.h>
#include <stdlib.h>
#include <dbgint.h>

/***
*size_t _msize(pblock) - calculate the size of specified block in the heap
*
*Purpose:
*       Calculates the size of memory block (in the heap) pointed to by
*       pblock.
*
*Entry:
*       void *pblock - pointer to a memory block in the heap
*
*Return:
*       size of the block
*
*******************************************************************************/


size_t __cdecl _msize_base (
        void *pblock
        )
{
        size_t  retval;

        /* lock the heap
         */
        _mlock(_HEAP_LOCK);

        retval = _msize_nolock(pblock);

        /* release the heap lock
         */
        _munlock(_HEAP_LOCK);

        return retval;
}

size_t __cdecl _msize_nolock (


        void *pblock
        )
{


        return( (size_t) ((char *)_ADDRESS(_BACKPTR(pblock)->pnextdesc) -
        (char *)pblock) );
}


#endif  /* WINHEAP */
