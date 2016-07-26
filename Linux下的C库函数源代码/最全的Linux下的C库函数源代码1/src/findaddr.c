/***
*findaddr.c - Find a heap entry
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the _heap_findaddr routine
*
*******************************************************************************/

#ifndef WINHEAP

#include <cruntime.h>
#include <heap.h>
#include <stddef.h>

#define TRUE    1

/***
*int _heap_findaddr() - Find a heap entry
*
*Purpose:
*       Given an address, find the corresponding heap entry.
*
*Entry:
*       void * address = address to look for
*       PBLKDESC * ppdesc = pointer to pointer to heap descriptor to be
*               filled in by this routine.
*
*Exit:
*
*       _HEAPFIND_EXACT  =  0 = exact fit, pdesc holds heap descriptor address
*       _HEAPFIND_WITHIN =  1 = not exact fit, pdesc holds previous heap
*                               descriptor address
*
*       _HEAPFIND_BEFORE = -1 = address is before the heap (pdesc NOT filled in)
*       _HEAPFIND_AFTER  = -2 = address is after the heap (pdesc NOT filled in)
*       _HEAPFIND_EMPTY  = -3 = no memory in heap (pdesc NOT filled in)
*
*       [If return is negative, supplied pdesc is NOT filled in.]
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

int  __cdecl _heap_findaddr (
        void * address,
        _PBLKDESC * ppdesc
        )
{
        REG1 _PBLKDESC pcurr;

        /*
         * See if heap there's anything in the heap
         */

        if (_heap_desc.pfirstdesc == &_heap_desc.sentinel)
                return(_HEAPFIND_EMPTY);

        /*
         * See if entry is in the heap or not
         */

        if (_ADDRESS(_heap_desc.pfirstdesc) > address)
                return(_HEAPFIND_BEFORE);

        if (_ADDRESS(&_heap_desc.sentinel) <= address)
                return(_HEAPFIND_AFTER);

        /*
         * Find the entry
         */

        for (pcurr = _heap_desc.pfirstdesc; TRUE; pcurr = pcurr->pnextdesc) {

                if ( _ADDRESS(pcurr->pnextdesc) > address ) {

                        /* Address is contained in this entry */
                        *ppdesc = pcurr;

                        /* Check for an exact fit */
                        if ( _ADDRESS(pcurr) == address)
                                return(_HEAPFIND_EXACT);
                        else
                                return(_HEAPFIND_WITHIN);
                }
        }

}

#endif  /* WINHEAP */
