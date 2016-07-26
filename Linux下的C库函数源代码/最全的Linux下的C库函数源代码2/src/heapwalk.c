/***
*heapwalk.c - walk the heap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the _heapwalk() function
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
#include <winheap.h>
#include <internal.h>


/***
*int _heapwalk() - Walk the heap
*
*Purpose:
*       Walk the heap returning information on one entry at a time.
*
*Entry:
*       struct _heapinfo {
*               int * _pentry;  heap entry pointer
*               size_t size;    size of heap entry
*               int _useflag;   free/inuse flag
*               } *entry;
*
*Exit:
*       Returns one of the following values:
*
*               _HEAPOK         - completed okay
*               _HEAPEMPTY      - heap not initialized
*               _HEAPBADPTR     - _pentry pointer is bogus
*               _HEAPBADBEGIN   - can't find initial header info
*               _HEAPBADNODE    - malformed node somewhere
*               _HEAPEND        - end of heap successfully reached
*
*Uses:
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

int __cdecl _heapwalk (
        struct _heapinfo *_entry
        )
{
        PROCESS_HEAP_ENTRY Entry;
        DWORD errval;
        int errflag;
        int retval = _HEAPOK;

        /* validation section */
        _VALIDATE_RETURN(_entry != NULL, EINVAL, _HEAPBADPTR);

        Entry.wFlags = 0;
        Entry.iRegionIndex = 0;
        if ( (Entry.lpData = _entry->_pentry) == NULL ) {
                if ( !HeapWalk( _crtheap, &Entry ) ) {
                        if ( GetLastError() == ERROR_CALL_NOT_IMPLEMENTED ) {
                                _doserrno = ERROR_CALL_NOT_IMPLEMENTED;
                                errno = ENOSYS;
                                return _HEAPEND;
                        }
                return _HEAPBADBEGIN;
                }
        }
        else {
                if ( _entry->_useflag == _USEDENTRY ) {
                        if ( !HeapValidate( _crtheap, 0, _entry->_pentry ) )
                                return _HEAPBADNODE;
                Entry.wFlags = PROCESS_HEAP_ENTRY_BUSY;
                }
nextBlock:
                /*
                 * Guard the HeapWalk call in case we were passed a bad pointer
                 * to an allegedly free block.
                 */
                __try {
                        errflag = 0;
                        if ( !HeapWalk( _crtheap, &Entry ) )
                                errflag = 1;
                }
                __except( GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ) {
                        errflag = 2;
                }

                /*
                 * Check errflag to see how HeapWalk fared...
                 */
                if ( errflag == 1 ) {
                        /*
                         * HeapWalk returned an error.
                         */
                        if ( (errval = GetLastError()) == ERROR_NO_MORE_ITEMS ) {
                        return _HEAPEND;
                        }
                        else if ( errval == ERROR_CALL_NOT_IMPLEMENTED ) {
                                _doserrno = errval;
                                errno = ENOSYS;
                                return _HEAPEND;
                        }
                        return _HEAPBADNODE;
                }
                else if ( errflag == 2 ) {
                        /*
                         * Exception occurred during the HeapWalk!
                         */
                        return _HEAPBADNODE;
                }
        }

        if ( Entry.wFlags & (PROCESS_HEAP_REGION |
                 PROCESS_HEAP_UNCOMMITTED_RANGE) )
        {
                goto nextBlock;
        }

        _entry->_pentry = Entry.lpData;
        _entry->_size = Entry.cbData;
        if ( Entry.wFlags & PROCESS_HEAP_ENTRY_BUSY ) {
                _entry->_useflag = _USEDENTRY;
        }
        else {
                _entry->_useflag = _FREEENTRY;
        }

        return( retval );
}



#else  /* WINHEAP */


#include <cruntime.h>
#include <heap.h>
#include <malloc.h>
#include <mtdll.h>
#include <stddef.h>


/***
*int _heapwalk() - Walk the heap
*
*Purpose:
*       Walk the heap returning information on one entry at a time.
*
*Entry:
*       struct _heapinfo {
*               int * _pentry;  heap entry pointer
*               size_t size;    size of heap entry
*               int _useflag;   free/inuse flag
*               } *entry;
*
*Exit:
*       Returns one of the following values:
*
*               _HEAPOK         - completed okay
*               _HEAPEMPTY      - heap not initialized
*               _HEAPBADPTR     - _pentry pointer is bogus
*               _HEAPBADBEGIN   - can't find initial header info
*               _HEAPBADNODE    - malformed node somewhere
*               _HEAPEND        - end of heap successfully reached
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _heapwalk (
        struct _heapinfo *_entry
        )
{
        REG1 _PBLKDESC pdesc;
        _PBLKDESC polddesc;
        int retval = _HEAPOK;

        /*
         * Lock the heap
         */

        _mlock(_HEAP_LOCK);

        /*
         * Quick header check
         */

        if ( (_heap_desc.pfirstdesc == NULL) ||
             (_heap_desc.proverdesc == NULL) ||
             (_heap_desc.sentinel.pnextdesc != NULL) ) {
                retval = _HEAPBADBEGIN;
                goto done;
        }

        /*
         * Check for an empty heap
         */

        if ( _heap_desc.pfirstdesc == &_heap_desc.sentinel ) {
                retval = _HEAPEMPTY;
                goto done;
        }

        /*
         * If _pentry is NULL, return info about the first entry.
         * Else, get info about the next entry in the heap.
         */

        if ( _entry->_pentry == NULL ) {
                pdesc = _heap_desc.pfirstdesc;
        }
        else {
                /*
                 * Find the entry we gave to the user last time around
                 */

                if ( _heap_findaddr( (void *)((char *)(_entry->_pentry) -
                    _HDRSIZE), &polddesc) != _HEAPFIND_EXACT ) {
                        retval = _HEAPBADPTR;
                        goto done;
                }

                pdesc = polddesc->pnextdesc;

        } /* else */


        /*
         * pdesc = entry to return info about
         */

        /*
         * Skip over dummy entries
         */

        while ( _IS_DUMMY(pdesc) )
                pdesc = pdesc->pnextdesc;


        /*
         * See if we're at the end of the heap
         */

        if ( pdesc == &_heap_desc.sentinel ) {
                retval = _HEAPEND;
                goto done;
        }

        /*
         * Check back pointer (note that pdesc cannot point to a dummy
         * descriptor since we have skipped over them)
         */

        if (!_CHECK_PDESC(pdesc)) {
                retval = _HEAPBADPTR;
                goto done;
        }

        /*
         * Return info on the next block
         */

        _entry->_pentry = ( (void *)((char *)_ADDRESS(pdesc) + _HDRSIZE) );
        _entry->_size = _BLKSIZE(pdesc);
        _entry->_useflag = ( _IS_INUSE(pdesc) ? _USEDENTRY : _FREEENTRY );


        /*
         * Common return
         */

done:
        /*
         * Release the heap lock
         */

        _munlock(_HEAP_LOCK);

        return(retval);

}


#endif  /* WINHEAP */
