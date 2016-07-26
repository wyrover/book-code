/***
*tidtable.c - Access thread data table
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains the following routines for multi-thread
*       data support:
*
*       _mtinit     = Initialize the mthread data
*       _getptd     = get the pointer to the per-thread data structure for
*                       the current thread
*       _freeptd    = free up a per-thread data structure and its
*                       subordinate structures
*       __threadid  = return thread ID for the current thread
*       __threadhandle = return pseudo-handle for the current thread
*
*******************************************************************************/


#include <sect_attribs.h>
#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <mtdll.h>
#include <memory.h>
#include <msdos.h>
#include <rterr.h>
#include <stdlib.h>
#include <stddef.h>
#include <dbgint.h>
#include <setlocal.h>
#include <mbstring.h>

extern pthreadmbcinfo __ptmbcinfo;

extern threadlocinfo __initiallocinfo;
extern threadmbcinfo __initialmbcinfo;
extern pthreadlocinfo __ptlocinfo;

void * __cdecl __removelocaleref( pthreadlocinfo);
void __cdecl __addlocaleref( pthreadlocinfo);
void __cdecl __freetlocinfo(pthreadlocinfo);

#ifndef _M_AMD64

//
// Define Fiber Local Storage function pointers.
//

PFLS_ALLOC_FUNCTION gpFlsAlloc = NULL;
PFLS_GETVALUE_FUNCTION gpFlsGetValue = NULL;
PFLS_SETVALUE_FUNCTION gpFlsSetValue = NULL;
PFLS_FREE_FUNCTION gpFlsFree = NULL;

#endif  /* _M_AMD64 */

unsigned long __flsindex = 0xffffffff;
unsigned long __getvalueindex = 0xffffffff;

#define _ENCODE_POINTER "EncodePointer"
#define _DECODE_POINTER "DecodePointer"
#define _KERNEL32 "KERNEL32.DLL"

/***
*void * _encode_pointer(void *ptr) -
*
*Purpose:
*   Uses the OS mechanism for encoding pointers to prevent hijacking
*
*Entry:
*   ptr - a pointer to be encoded (can be NULL)
*
*Exit:
*   The encoded pointer, or ptr if the OS mechanism is unavailable
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP void * __cdecl _encode_pointer(void *ptr)
{
    _ptiddata ptd;
    PVOID (APIENTRY *pfnEncodePointer)(PVOID) = NULL;

    if (
#ifndef _M_AMD64
        (FLS_GETVALUE == NULL) ||
#endif  /* _M_AMD64 */
        __flsindex==0xffffffff ||
        (ptd = (_ptiddata)FLS_GETVALUE(__flsindex)) == NULL)
    {
        // cached function pointer is not available - look it up the traditional way

        HINSTANCE hKernel32 = GetModuleHandle(_KERNEL32);

        if (hKernel32)
        {
            pfnEncodePointer = (PVOID (APIENTRY *)(PVOID))
                GetProcAddress(hKernel32, _ENCODE_POINTER);
        }
    }
    else
    {
        // use cached function pointer in TLS

        pfnEncodePointer = (PVOID (APIENTRY *)(PVOID)) (ptd->_encode_ptr);
    }

    if (pfnEncodePointer != NULL)
    {
        ptr = (*pfnEncodePointer)(ptr);
    }

    return ptr;
}

/***
*void * _encoded_null() -
*
*Purpose:
*   Gets the encoded null pointer for the process
*
*Entry:
*   No parameters
*
*Exit:
*   The process specific encoding for the null pointer
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP void * __cdecl _encoded_null()
{
    return _encode_pointer(NULL);
}

/***
*void * _decode_pointer(void *codedptr) -
*
*Purpose:
*   Uses the OS mechanism to retrieve a pointer from its encoded form
*
*Entry:
*   ptr - a pointer to be decoded (can be NULL)
*
*Exit:
*   The decoded pointer, or ptr if the OS mechanism is unavailable
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP void * __cdecl _decode_pointer(void *codedptr)
{
    _ptiddata ptd;
    PVOID (APIENTRY *pfnDecodePointer)(PVOID) = NULL;

    if (
#ifndef _M_AMD64
        (FLS_GETVALUE == NULL) ||
#endif  /* _M_AMD64 */
        __flsindex==0xffffffff ||
        (ptd = (_ptiddata)FLS_GETVALUE(__flsindex)) == NULL)
    {
        // cached function pointer is not available - look it up the traditional way

        HINSTANCE hKernel32 = GetModuleHandle(_KERNEL32);

        if (hKernel32)
        {
            pfnDecodePointer = (PVOID (APIENTRY *)(PVOID))
                GetProcAddress(hKernel32, _DECODE_POINTER);
        }
    }
    else
    {
        // use cached function pointer in TLS

        pfnDecodePointer = (PVOID (APIENTRY *)(PVOID)) (ptd->_decode_ptr);
    }

    if (pfnDecodePointer != NULL)
    {
        codedptr = (*pfnDecodePointer)(codedptr);
    }

    return codedptr;
}

/***
* __crtTlsAlloc - crt wrapper around TlsAlloc
*
* Purpose:
*    (1) Call to __crtTlsAlloc should look like call to FlsAlloc, this will
*        Help in redirecting the call to TlsAlloc and FlsAlloc using same
*        redirection variable.
*******************************************************************************/

DWORD WINAPI __crtTlsAlloc( PFLS_CALLBACK_FUNCTION lpCallBack)
{
    return TlsAlloc( );
}

/***
* __fls_getvalue - crt wrapper around FLS_GETVALUE macro
*
* Purpose:
*       This function helps msvcmXX.dll beginthread and beginthreadex APIs
*       to use FLS_GETVALUE functionsality.
*
*******************************************************************************/

_CRTIMP
PVOID
WINAPI __fls_getvalue (
    IN DWORD dwFlsIndex
    )
{
    return FLS_GETVALUE(dwFlsIndex);
}

/***
* __get_flsindex - crt wrapper around __flsindex
*
* Purpose:
*       This function helps msvcmXX.dll beginthread and beginthreadex APIs
*       to retrive __flsindex value.
*
*******************************************************************************/

_CRTIMP unsigned long __cdecl __get_flsindex()
{
    return __flsindex;
}

/***
* __set_flsgetvalue - crt wrapper for setting up FlsGetValue pointer in TLS
*
* Purpose:
*       This function helps msvcmXX.dll threadstart and threadstartex APIs
*       to set FlsGetValue pointer before calling __fls_getvalue.
*
*******************************************************************************/

_CRTIMP void __cdecl __set_flsgetvalue()
{
#ifndef _M_AMD64
    if (!FLS_GETVALUE)
    {
        TlsSetValue(__getvalueindex, _decode_pointer(gpFlsGetValue));
    }
#endif  /* _M_AMD64 */
}

/***
* __fls_setvalue - crt wrapper around FLS_SETVALUE macro
*
* Purpose:
*       This function helps msvcmXX.dll beginthread and beginthreadex APIs
*       to use FLS_SETVALUE functionsality.
*
*******************************************************************************/

_CRTIMP
BOOL
WINAPI __fls_setvalue (
    IN DWORD dwFlsIndex,
    IN PVOID lpFlsData
    )
{
    return FLS_SETVALUE(dwFlsIndex, lpFlsData);
}


/****
*_mtinit() - Init multi-thread data bases
*
*Purpose:
*       (1) Call _mtinitlocks to create/open all lock semaphores.
*       (2) Allocate a TLS index to hold pointers to per-thread data
*           structure.
*
*       NOTES:
*       (1) Only to be called ONCE at startup
*       (2) Must be called BEFORE any mthread requests are made
*
*Entry:
*       <NONE>
*Exit:
*       returns FALSE on failure
*
*Uses:
*       <any registers may be modified at init time>
*
*Exceptions:
*
*******************************************************************************/
int __cdecl _mtinit (
        void
        )
{
        _ptiddata ptd;

#ifndef _M_AMD64

        /*
         * Initialize fiber local storage function pointers.
         */

        HINSTANCE hKernel32 = GetModuleHandle(_KERNEL32);
        if (hKernel32 == NULL) {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }

        gpFlsAlloc = (PFLS_ALLOC_FUNCTION)GetProcAddress(hKernel32,
                                                            "FlsAlloc");

        gpFlsGetValue = (PFLS_GETVALUE_FUNCTION)GetProcAddress(hKernel32,
                                                                "FlsGetValue");

        gpFlsSetValue = (PFLS_SETVALUE_FUNCTION)GetProcAddress(hKernel32,
                                                                "FlsSetValue");

        gpFlsFree = (PFLS_FREE_FUNCTION)GetProcAddress(hKernel32,
                                                        "FlsFree");
        if (!gpFlsAlloc || !gpFlsGetValue || !gpFlsSetValue || !gpFlsFree) {
            gpFlsAlloc = (PFLS_ALLOC_FUNCTION)__crtTlsAlloc;

            gpFlsGetValue = (PFLS_GETVALUE_FUNCTION)TlsGetValue;

            gpFlsSetValue = (PFLS_SETVALUE_FUNCTION)TlsSetValue;

            gpFlsFree = (PFLS_FREE_FUNCTION)TlsFree;
        }

        /*
         * Allocate and initialize a TLS index to store FlsGetValue pointer
         * so that the FLS_* macros can work transparently
         */

        if ( (__getvalueindex = TlsAlloc()) == 0xffffffff ||
             !TlsSetValue(__getvalueindex, (LPVOID)gpFlsGetValue) ) {
            return FALSE;
        }
#endif  /* _M_AMD64 */

        _init_pointers();       /* initialize global function pointers */

#ifndef _M_AMD64
        /*
         * Encode the fiber local storage function pointers
         */

        gpFlsAlloc = (PFLS_ALLOC_FUNCTION) _encode_pointer(gpFlsAlloc);
        gpFlsGetValue = (PFLS_GETVALUE_FUNCTION) _encode_pointer(gpFlsGetValue);
        gpFlsSetValue = (PFLS_SETVALUE_FUNCTION) _encode_pointer(gpFlsSetValue);
        gpFlsFree = (PFLS_FREE_FUNCTION) _encode_pointer(gpFlsFree);
#endif  /* _M_AMD64 */

        /*
         * Initialize the mthread lock data base
         */

        if ( !_mtinitlocks() ) {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }

        /*
         * Allocate a TLS index to maintain pointers to per-thread data
         */
        if ( (__flsindex = FLS_ALLOC(&_freefls)) == 0xffffffff ) {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }

        /*
         * Create a per-thread data structure for this (i.e., the startup)
         * thread.
         */
        if ( ((ptd = _calloc_crt(1, sizeof(struct _tiddata))) == NULL) ||
             !FLS_SETVALUE(__flsindex, (LPVOID)ptd) )
        {
            _mtterm();
            return FALSE;       /* fail to load DLL */
        }

        /*
         * Initialize the per-thread data
         */

        _initptd(ptd,NULL);

        ptd->_tid = GetCurrentThreadId();
        ptd->_thandle = (uintptr_t)(-1);

        return TRUE;
}


/****
*_mtterm() - Clean-up multi-thread data bases
*
*Purpose:
*       (1) Call _mtdeletelocks to free up all lock semaphores.
*       (2) Free up the TLS index used to hold pointers to
*           per-thread data structure.
*
*       NOTES:
*       (1) Only to be called ONCE at termination
*       (2) Must be called AFTER all mthread requests are made
*
*Entry:
*       <NONE>
*Exit:
*       returns
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _mtterm (
        void
        )
{
    /*
     * Free up the TLS index
     *
     * (Set the variables __flsindex and __getvalueindex back to initial state (-1L).)
     */

    if ( __flsindex != 0xffffffff ) {
        FLS_FREE(__flsindex);
        __flsindex = 0xffffffff;
    }

    if ( __getvalueindex != 0xffffffff ) {
        TlsFree(__getvalueindex);
        __getvalueindex = 0xffffffff;
    }

    /*
     * Clean up the mthread lock data base
     */

    _mtdeletelocks();
}



/***
*void _initptd(_ptiddata ptd, pthreadlocinfo) - initialize a per-thread data structure
*
*Purpose:
*       This routine handles all of the per-thread initialization
*       which is common to _beginthread, _beginthreadex, _mtinit
*       and _getptd.
*
*Entry:
*       pointer to a per-thread data block
*
*Exit:
*       the common fields in that block are initialized
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP void __cdecl _initptd (
        _ptiddata ptd,
        pthreadlocinfo ptloci
        )
{
    HINSTANCE hKernel32 = GetModuleHandle(_KERNEL32);

    ptd->_pxcptacttab = (void *)_XcptActTab;
    ptd->_holdrand = 1L;

    if (hKernel32 != NULL)
    {
        // Initialize the function pointers in the ptd data
        ptd->_encode_ptr = GetProcAddress(hKernel32, _ENCODE_POINTER);
        ptd->_decode_ptr = GetProcAddress(hKernel32, _DECODE_POINTER);
    }

    // It is necessary to always have GLOBAL_LOCALE_BIT set in perthread data
    // because when doing bitwise or, we won't get __UPDATE_LOCALE to work when
    // global per thread locale is set.
    ptd->_ownlocale = _GLOBAL_LOCALE_BIT;

    // Initialize _setloc_data. These are the only valuse that need to be
    // initialized.
    ptd->_setloc_data._cachein[0]='C';
    ptd->_setloc_data._cacheout[0]='C';
    ptd->ptmbcinfo = &__initialmbcinfo;
    InterlockedIncrement(&(ptd->ptmbcinfo->refcount));

    // We need to make sure that ptd->ptlocinfo in never NULL, this saves us
    // perf counts when UPDATING locale.
    _mlock(_SETLOCALE_LOCK);
    __try {
        ptd->ptlocinfo = ptloci;
        /*
         * Note that and caller to _initptd could have passed __ptlocinfo, but
         * that will be a bug as between the call to _initptd and __addlocaleref
         * the global locale may have changed and ptloci may be pointing to invalid
         * memory. Thus if the wants to set the locale to global, NULL should
         * be passed.
         */
        if (ptd->ptlocinfo == NULL)
            ptd->ptlocinfo = __ptlocinfo;
        __addlocaleref(ptd->ptlocinfo);
    }
    __finally {
        _munlock(_SETLOCALE_LOCK);
    }
}

/***
*_ptiddata _getptd_noexit(void) - get per-thread data structure for the current thread
*
*Purpose:
*
*Entry:
*
*Exit:
*       success = pointer to _tiddata structure for the thread
*       failure = NULL
*
*Exceptions:
*
*******************************************************************************/

_ptiddata __cdecl _getptd_noexit (
        void
        )
{
    _ptiddata ptd;
    DWORD   TL_LastError;

    TL_LastError = GetLastError();

    /*
     * Initialize FlsGetValue function pointer in TLS
     */
    __set_flsgetvalue();

    if ( (ptd = FLS_GETVALUE(__flsindex)) == NULL ) {
        /*
         * no per-thread data structure for this thread. try to create
         * one.
         */
        if ((ptd = _calloc_crt(1, sizeof(struct _tiddata))) != NULL) {

            if (FLS_SETVALUE(__flsindex, (LPVOID)ptd) ) {

                /*
                 * Initialize of per-thread data
                 */

                _initptd(ptd,NULL);

                ptd->_tid = GetCurrentThreadId();
                ptd->_thandle = (uintptr_t)(-1);
            }
            else {

                /*
                 * Return NULL to indicate failure
                 */

                _free_crt(ptd);
                ptd = NULL;
            }
        }
    }

    SetLastError(TL_LastError);

    return(ptd);
}

/***
*_ptiddata _getptd(void) - get per-thread data structure for the current thread
*
*Purpose:
*
*Entry:
*       unsigned long tid
*
*Exit:
*       success = pointer to _tiddata structure for the thread
*       failure = fatal runtime exit
*
*Exceptions:
*
*******************************************************************************/

_ptiddata __cdecl _getptd (
        void
        )
{
        _ptiddata ptd = _getptd_noexit();
        if (!ptd) {
            _amsg_exit(_RT_THREAD); /* write message and die */
        }
        return ptd;
}


/***
*void WINAPI _freefls(void *) - free up a per-fiber data structure
*
*Purpose:
*       Called from _freeptd, as a callback from deleting a fiber, and
*       from deleting an FLS index. This routine frees up the per-fiber
*       buffer associated with a fiber that is going away. The tiddata
*       structure itself is freed, but not until its subordinate buffers
*       are freed.
*
*Entry:
*       pointer to a per-fiber data block (malloc-ed memory)
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP void
WINAPI
_freefls (
    void *data
    )

{

    _ptiddata ptd;
    pthreadlocinfo ptloci;
    pthreadmbcinfo ptmbci;

    /*
     * Free up the _tiddata structure & its malloc-ed buffers.
     */

    ptd = data;
    if (ptd != NULL) {
        if(ptd->_errmsg)
            _free_crt((void *)ptd->_errmsg);

        if(ptd->_namebuf0)
            _free_crt((void *)ptd->_namebuf0);

        if(ptd->_namebuf1)
            _free_crt((void *)ptd->_namebuf1);

        if(ptd->_asctimebuf)
            _free_crt((void *)ptd->_asctimebuf);

        if(ptd->_gmtimebuf)
            _free_crt((void *)ptd->_gmtimebuf);

        if(ptd->_cvtbuf)
            _free_crt((void *)ptd->_cvtbuf);

        if (ptd->_pxcptacttab != _XcptActTab)
            _free_crt((void *)ptd->_pxcptacttab);

        _mlock(_MB_CP_LOCK);
        __try {
            if ( ((ptmbci = ptd->ptmbcinfo) != NULL) &&
                 (InterlockedDecrement(&(ptmbci->refcount)) == 0) &&
                 (ptmbci != &__initialmbcinfo) )
                _free_crt(ptmbci);
        }
        __finally {
            _munlock(_MB_CP_LOCK);
        }

        _mlock(_SETLOCALE_LOCK);

        __try {
            if ( (ptloci = ptd->ptlocinfo) != NULL )
            {
                __removelocaleref(ptloci);
                if ( (ptloci != __ptlocinfo) &&
                     (ptloci != &__initiallocinfo) &&
                     (ptloci->refcount == 0) )
                    __freetlocinfo(ptloci);
            }
        }
        __finally {
            _munlock(_SETLOCALE_LOCK);
        }

        _free_crt((void *)ptd);
    }
    return;
}

/***
*void _freeptd(_ptiddata) - free up a per-thread data structure
*
*Purpose:
*       Called from _endthread and from a DLL thread detach handler,
*       this routine frees up the per-thread buffer associated with a
*       thread that is going away.  The tiddata structure itself is
*       freed, but not until its subordinate buffers are freed.
*
*Entry:
*       pointer to a per-thread data block (malloc-ed memory)
*       If NULL, the pointer for the current thread is fetched.
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _freeptd (
        _ptiddata ptd
        )
{
        /*
         * Do nothing unless per-thread data has been allocated for this module!
         */

        if ( __flsindex != 0xFFFFFFFF ) {

            /*
             * if parameter "ptd" is NULL, get the per-thread data pointer
             * Must NOT call _getptd because it will allocate one if none exists!
             * If FLS_GETVALUE is NULL then ptd could not have been set
             */

            if ( ptd == NULL
#ifndef _M_AMD64
                 && (FLS_GETVALUE != NULL)
#endif  /* _M_AMD64 */
                )
                ptd = FLS_GETVALUE(__flsindex);

            /*
             * Zero out the one pointer to the per-thread data block
             */

            FLS_SETVALUE(__flsindex, (LPVOID)0);

            _freefls(ptd);
        }

        if ( __getvalueindex != 0xFFFFFFFF ) {
            /*
             * Zero out the FlsGetValue pointer
             */
            TlsSetValue(__getvalueindex, (LPVOID)0);
        }
}


/***
*__threadid()     - Returns current thread ID
*__threadhandle() - Returns "pseudo-handle" for current thread
*
*Purpose:
*       The two function are simply do-nothing wrappers for the corresponding
*       Win32 APIs (GetCurrentThreadId and GetCurrentThread, respectively).
*
*Entry:
*       void
*
*Exit:
*       thread ID value
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP unsigned long __cdecl __threadid (
        void
        )
{
    return( GetCurrentThreadId() );
}

_CRTIMP uintptr_t __cdecl __threadhandle(
        void
        )
{
    return( (uintptr_t)GetCurrentThread() );
}

