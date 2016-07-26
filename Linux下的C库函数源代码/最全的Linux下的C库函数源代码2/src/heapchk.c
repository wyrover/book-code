/***
*heapchk.c - perform a consistency check on the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the _heapchk() and _heapset() functions
*
*******************************************************************************/


#ifdef WINHEAP


#include <cruntime.h>
#include <windows.h>
#include <errno.h>
#include <malloc.h>
#include <mtdll.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <winheap.h>


/***
*int _heapchk()         - Validate the heap
*int _heapset(_fill)    - Obsolete function!
*
*Purpose:
*       Both functions perform a consistency check on the heap. The old
*       _heapset used to fill free blocks with _fill, in addition to
*       performing the consistency check. The current _heapset ignores the
*       passed parameter and just returns _heapchk.
*
*Entry:
*       For heapchk()
*           No arguments
*       For heapset()
*           int _fill - ignored
*
*Exit:
*       Returns one of the following values:
*
*           _HEAPOK         - completed okay
*           _HEAPEMPTY      - heap not initialized
*           _HEAPBADBEGIN   - can't find initial header info
*           _HEAPBADNODE    - malformed node somewhere
*
*       Debug version prints out a diagnostic message if an error is found
*       (see errmsg[] above).
*
*       NOTE:  Add code to support memory regions.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _heapchk (void)
{
        int retcode = _HEAPOK;

#ifndef _WIN64
        if ( __active_heap == __V6_HEAP )
        {
            _mlock( _HEAP_LOCK );
            __try {

            if ( __sbh_heap_check() < 0 )
                retcode = _HEAPBADNODE;

            }
            __finally {
                _munlock( _HEAP_LOCK );
            }
        }
#ifdef CRTDLL
        else if ( __active_heap == __V5_HEAP )
        {
            _mlock( _HEAP_LOCK );
            __try {

            if ( __old_sbh_heap_check() < 0 )
                retcode = _HEAPBADNODE;

            }
            __finally {
                _munlock( _HEAP_LOCK );
            }
        }
#endif  /* CRTDLL */
#endif  /* _WIN64 */

        if (!HeapValidate(_crtheap, 0, NULL))
        {
            if (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)
            {
                _doserrno = ERROR_CALL_NOT_IMPLEMENTED;
                errno = ENOSYS;
            }
            else
                retcode = _HEAPBADNODE;
        }
        return retcode;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

int __cdecl _heapset (
        unsigned int _fill
        )
{
        return _heapchk();
}



#else  /* WINHEAP */


#include <cruntime.h>
#include <heap.h>
#include <malloc.h>
#include <mtdll.h>
#include <stddef.h>
#include <string.h>

static int __cdecl _heap_checkset(unsigned int _fill);

/* Debug error values */
#define _EMPTYHEAP   0
#define _BADROVER    1
#define _BADRANGE    2
#define _BADSENTINEL 3
#define _BADEMPTY    4
#define _EMPTYLOOP   5
#define _BADFREE     6
#define _BADORDER    7


#define _PRINTERR(msg)



/***
*int _heapchk()      - Validate the heap
*int _heapset(_fill) - Validate the heap and fill in free entries
*
*Purpose:
*   Performs a consistency check on the heap.
*
*Entry:
*   For heapchk()
*       No arguments
*   For heapset()
*       int _fill - value to be used as filler in free entries
*
*Exit:
*   Returns one of the following values:
*
*       _HEAPOK          - completed okay
*       _HEAPEMPTY       - heap not initialized
*       _HEAPBADBEGIN    - can't find initial header info
*       _HEAPBADNODE     - malformed node somewhere
*
*   Debug version prints out a diagnostic message if an error is found
*   (see errmsg[] above).
*
*   NOTE:  Add code to support memory regions.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _heapchk(void)
{
    return(_heap_checkset((unsigned int)_HEAP_NOFILL));
}


int __cdecl _heapset (
    unsigned int _fill
    )
{
    return(_heap_checkset(_fill));
}


/***
*static int _heap_checkset(_fill) - check the heap and fill in the
*   free entries
*
*Purpose:
*   Workhorse routine for both _heapchk() and _heapset().
*
*Entry:
*   int _fill - either _HEAP_NOFILL or a value to be used as filler in
*              free entries
*
*Exit:
*   See description of _heapchk()/_heapset()
*
*******************************************************************************/

static int __cdecl _heap_checkset (
    unsigned int _fill
    )
{
    REG1 _PBLKDESC pdesc;
    REG2 _PBLKDESC pnext;
    int roverfound=0;
    int retval = _HEAPOK;

    /*
     * lock the heap
     */

    _mlock(_HEAP_LOCK);

    /*
     * Validate the sentinel
     */

    if (_heap_desc.sentinel.pnextdesc != NULL) {
        _PRINTERR(_BADSENTINEL);
        retval = _HEAPBADNODE;
        goto done;
    }

    /*
     * Test for an empty heap
     */

    if ( (_heap_desc.pfirstdesc == &_heap_desc.sentinel) &&
         (_heap_desc.proverdesc == &_heap_desc.sentinel) ) {
        retval = _HEAPEMPTY;
        goto done;
    }

    /*
     * Get and validate the first descriptor
     */

    if ((pdesc = _heap_desc.pfirstdesc) == NULL) {
        _PRINTERR(_EMPTYHEAP);
        retval = _HEAPBADBEGIN;
        goto done;
    }

    /*
     * Walk the heap descriptor list
     */

    while (pdesc != &_heap_desc.sentinel) {

        /*
         * Make sure address for this entry is in range.
         */

        if ( (_ADDRESS(pdesc) < _ADDRESS(_heap_desc.pfirstdesc)) ||
             (_ADDRESS(pdesc) > _heap_desc.sentinel.pblock) ) {
            _PRINTERR(_BADRANGE);
            retval = _HEAPBADNODE;
            goto done;
        }

        pnext = pdesc->pnextdesc;

        /*
         * Make sure the blocks corresponding to pdesc and pnext are
         * in proper order.
         */

        if ( _ADDRESS(pdesc) >= _ADDRESS(pnext) ) {
            _PRINTERR(_BADORDER);
            retval = _HEAPBADNODE;
            goto done;
        }

        /*
         * Check the backpointer.
         */

        if (_IS_INUSE(pdesc) || _IS_FREE(pdesc)) {

            if (!_CHECK_PDESC(pdesc)) {
                retval = _HEAPBADPTR;
                goto done;
            }
        }

        /*
         * Check for proverdesc
         */

        if (pdesc == _heap_desc.proverdesc)
            roverfound++;

        /*
         * If it is free, fill it in if appropriate
         */

        if ( _IS_FREE(pdesc) && (_fill != _HEAP_NOFILL) )
            memset( (void *)((unsigned)_ADDRESS(pdesc)+_HDRSIZE),
            _fill, _BLKSIZE(pdesc) );

        /*
         * Onto the next block
         */

        pdesc = pnext;
    }

    /*
     * Make sure we found 1 and only 1 rover
     */

    if (_heap_desc.proverdesc == &_heap_desc.sentinel)
        roverfound++;

    if (roverfound != 1) {
        _PRINTERR(_BADROVER);
        retval = _HEAPBADBEGIN;
        goto done;
    }

    /*
     * Walk the empty list.  We can't really compare values against
     * anything but we may loop forever or may cause a fault.
     */

    pdesc = _heap_desc.emptylist;

    while (pdesc != NULL) {


        pnext = pdesc->pnextdesc;

        /*
         * Header should only appear once
         */

        if (pnext == _heap_desc.emptylist) {
            _PRINTERR(_EMPTYLOOP)
            retval = _HEAPBADPTR;
            goto done;
        }

        pdesc = pnext;

    }


    /*
     * Common return
     */

done:
    /*
     * release the heap lock
     */

    _munlock(_HEAP_LOCK);

    return(retval);

}


#endif  /* WINHEAP */
