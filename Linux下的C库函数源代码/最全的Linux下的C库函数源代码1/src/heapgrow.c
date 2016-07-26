/***
*heapgrow.c - Grow the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Get memory from OS and add to the heap.
*
*******************************************************************************/

#ifndef WINHEAP

#include <cruntime.h>
#include <oscalls.h>
#include <heap.h>
#include <malloc.h>
#include <stdlib.h>

static int __cdecl _heap_new_region(unsigned, size_t);


/***
*_heap_grow() - Grow the heap
*
*Purpose:
*       Get memory from the OS and add it to the heap.
*
*Entry:
*       size_t _size = user's block request
*
*Exit:
*        0 = success, new mem is in the heap
*       -1 = failure
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _heap_grow (
        REG1 size_t size
        )
{
        REG2 int index;
        int free_entry = -1;

        /*
         * Bump size to include header and round to nearest page boundary.
         */

        size += _HDRSIZE;
        size = _ROUND2(size,_PAGESIZE_);

        /*
         * Loop through the region table looking for an existing region
         * we can grow.  Remember the index of the first null region entry.
         *
         * size = size of grow request
         */

        for (index = 0; index < _HEAP_REGIONMAX; index++) {

                if ( (_heap_regions[index]._totalsize -
                    _heap_regions[index]._currsize) >= size )

                        /*
                         * Grow this region to satisfy the request.
                         */

                        return( _heap_grow_region(index, size) );


                if ( (free_entry == -1) &&
                    (_heap_regions[index]._regbase == NULL) )

                        /*
                         * Remember 1st free table entry for later
                         */

                        free_entry = index;

        }

        /*
         * Could not find any existing regions to grow.  Try to
         * get a new region.
         *
         * size = size of grow request
         * free_entry = index of first free entry in table
         */

        if ( free_entry >= 0 )

                /*
                 * Get a new region to satisfy the request.
                 */

                return( _heap_new_region(free_entry, size) );

        else
                /*
                 * No free table entries: return an error.
                 */

                return(-1);

}


/***
*_heap_new_region() - Get a new heap region
*
*Purpose:
*       Get a new heap region and put it in the region table.
*       Also, grow it large enough to support the caller's
*       request.
*
*       NOTES:
*       (1) Caller has verified that there is room in the _heap_region
*       table for another region.
*       (2) The caller must have rounded the size to a page boundary.
*
*Entry:
*       int index = index in table where new region data should go
*       size_t size = size of request (this has been rounded to a
*                       page-sized boundary)
*
*Exit:
*        0 = success
*       -1 = failure
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl _heap_new_region (
        REG1 unsigned index,
        size_t size
        )
{
        void * region;
        REG2 unsigned int regsize;


        /*
         * Round the heap region size to a page boundary (in case
         * the user played with it).
         */

        regsize = _ROUND2(_heap_regionsize, _PAGESIZE_);

        /*
         * To acommodate large users, request twice
         * as big a region next time around.
         */

        if ( _heap_regionsize < _heap_maxregsize )
                _heap_regionsize *= 2 ;

        /*
         * See if region is big enough for request
         */

        if (regsize < size)
                regsize = size;

        /*
         * Go get the new region
         */

        if (!(region = VirtualAlloc(NULL, regsize, MEM_RESERVE,
        PAGE_READWRITE)))
                goto error;

        /*
         * Put the new region in the table.
         */

         _heap_regions[index]._regbase = region;
         _heap_regions[index]._totalsize = regsize;
         _heap_regions[index]._currsize = 0;


        /*
         * Grow the region to satisfy the size request.
         */

        if (_heap_grow_region(index, size) != 0) {

                /*
                 * Ouch.  Allocated a region but couldn't commit
                 * any pages in it.  Free region and return error.
                 */

                _heap_free_region(index);
                goto error;
        }


        /*
         * Good return
         */

        /* done:   unreferenced label to be removed */
                return(0);

        /*
         * Error return
         */

        error:
                return(-1);

}


/***
*_heap_grow_region() - Grow a heap region
*
*Purpose:
*       Grow a region and add the new memory to the heap.
*
*       NOTES:
*       (1) The caller must have rounded the size to a page boundary.
*
*Entry:
*       unsigned index = index of region in the _heap_regions[] table
*       size_t size = size of request (this has been rounded to a
*                       page-sized boundary)
*
*Exit:
*        0 = success
*       -1 = failure
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _heap_grow_region (
        REG1 unsigned index,
        size_t size
        )
{
        size_t left;
        REG2 size_t growsize;
        void * base;
        unsigned dosretval;


        /*
         * Init some variables
         * left = space left in region
         * base = base of next section of region to validate
         */

        left = _heap_regions[index]._totalsize -
                _heap_regions[index]._currsize;

        base = (char *) _heap_regions[index]._regbase +
                _heap_regions[index]._currsize;

        /*
         * Make sure we can satisfy request
         */

        if (left < size)
                goto error;

        /*
         * Round size up to next _heap_growsize boundary.
         * (Must round _heap_growsize itself to page boundary, in
         * case user set it himself).
         */

        growsize = _ROUND2(_heap_growsize, _PAGESIZE_);
        growsize = _ROUND(size, growsize);

        if (left < growsize)
                growsize = left;

        /*
         * Validate the new portion of the region
         */

        if (!VirtualAlloc(base, growsize, MEM_COMMIT, PAGE_READWRITE))
                dosretval = GetLastError();
        else
                dosretval = 0;

        if (dosretval)
                /*
                 * Error committing pages.  If out of memory, return
                 * error, else abort.
                 */

                if (dosretval == ERROR_NOT_ENOUGH_MEMORY)
                        goto error;
                else
                        _heap_abort();


        /*
         * Update the region data base
         */

        _heap_regions[index]._currsize += growsize;




        /*
         * Add the memory to the heap
         */

        if (_heap_addblock(base, growsize) != 0)
                _heap_abort();


        /*
         * Good return
         */

        /* done:   unreferenced label to be removed */
                return(0);

        /*
         * Error return
         */

        error:
                return(-1);

}


/***
*_heap_free_region() - Free up a region
*
*Purpose:
*       Return a heap region to the OS and zero out
*       corresponding region data entry.
*
*Entry:
*       int index = index of region to be freed
*
*Exit:
*       void
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _heap_free_region (
        REG1 int index
        )
{

        /*
         * Give the memory back to the OS
         */

        if (!VirtualFree(_heap_regions[index]._regbase, 0, MEM_RELEASE))
                _heap_abort();

        /*
         * Zero out the heap region entry
         */

        _heap_regions[index]._regbase = NULL;
        _heap_regions[index]._currsize =
        _heap_regions[index]._totalsize = 0;

}


#endif  /* WINHEAP */
