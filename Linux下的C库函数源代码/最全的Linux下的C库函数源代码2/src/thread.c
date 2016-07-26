/***
*thread.c - Begin and end a thread
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This source contains the _beginthread() and _endthread()
*       routines which are used to start and terminate a thread.
*
*******************************************************************************/


#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <mtdll.h>
#include <msdos.h>
#include <malloc.h>
#include <process.h>
#include <stddef.h>
#include <rterr.h>
#include <dbgint.h>
#include <errno.h>
#if defined (_M_CEE) || defined (MRTDLL)
#include <msclr\appdomain.h>
#endif  /* defined (_M_CEE) || defined (MRTDLL) */

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention

/*
 * Startup code for new thread.
 */
static unsigned long WINAPI _threadstart(void *);
static void _callthreadstart(void);

/*
 * declare pointers to per-thread FP initialization and termination routines.
 * Note that this initialization is only done in native, not managed.
 */
#if !defined (MRTDLL) && !defined (CRTDLL)
const _PVFV _FPmtinit;
const _PVFV _FPmtterm;
#endif  /* !defined (MRTDLL) && !defined (CRTDLL) */

#if defined (_M_CEE) || defined (MRTDLL)
static int _getdomain(DWORD *pDomain)
{
    *pDomain=0;
    ICLRRuntimeHost *pClrHost = NULL;

    HRESULT hr = CorBindToRuntimeEx(
        NULL,                       // version of the runtime to request
        NULL,                       // flavor of the runtime to request
        0,                          // runtime startup flags
        CLSID_CLRRuntimeHost,       // clsid of ICLRRuntimeHost
        IID_ICLRRuntimeHost,        // IID of ICLRRuntimeHost
        (PVOID*)&pClrHost);         // a pointer to our punk that we get back

    if (FAILED(hr))
    {
        if (pClrHost != NULL)
        {
            pClrHost->Release();
        }
        return false;
    }

    DWORD domain=0;
    hr=pClrHost->GetCurrentAppDomainId(&domain);
    pClrHost->Release();
    pClrHost=NULL;
    if (FAILED(hr))
    {
        return false;
    }
    *pDomain=domain;
    return true;
}
#endif  /* defined (_M_CEE) || defined (MRTDLL) */

/***
*_beginthread() - Create a child thread
*
*Purpose:
*       Create a child thread.
*
*Entry:
*       initialcode = pointer to thread's startup code address
*       stacksize = size of stack
*       argument = argument to be passed to new thread
*
*Exit:
*       success = handle for new thread if successful
*
*       failure = (unsigned long) -1L in case of error, errno and _doserrno
*                 are set
*
*Exceptions:
*
*******************************************************************************/

_MCRTIMP uintptr_t __cdecl _beginthread (
        void (__CLRCALL_OR_CDECL * initialcode) (void *),
        unsigned stacksize,
        void * argument
        )
{
        _ptiddata ptd;                  /* pointer to per-thread data */
        uintptr_t thdl;                 /* thread handle */
        unsigned long err = 0L;     /* Return from GetLastError() */

        /* validation section */
        _VALIDATE_RETURN(initialcode != NULL, EINVAL, -1);

        /* Initialize FlsGetValue function pointer */
        __set_flsgetvalue();

        /*
         * Allocate and initialize a per-thread data structure for the to-
         * be-created thread.
         */
        if ( (ptd = (_ptiddata)_calloc_crt(1, sizeof(struct _tiddata))) == NULL )
        {
            goto error_return;
        }

        /*
         * Initialize the per-thread data
         */

        _initptd(ptd, _getptd()->ptlocinfo);

        ptd->_initaddr = (void *) initialcode;
        ptd->_initarg = argument;

#if defined (_M_CEE) || defined (MRTDLL)
        if(!_getdomain(&(ptd->__initDomain)))
        {
            goto error_return;
        }
#endif  /* defined (_M_CEE) || defined (MRTDLL) */

        /*
         * Create the new thread. Bring it up in a suspended state so that
         * the _thandle and _tid fields are filled in before execution
         * starts.
         */
        if ( (ptd->_thandle = thdl = (uintptr_t)
              CreateThread( NULL,
                            stacksize,
                            _threadstart,
                            (LPVOID)ptd,
                            CREATE_SUSPENDED,
                            (LPDWORD)&(ptd->_tid) ))
             == (uintptr_t)0 )
        {
                err = GetLastError();
                goto error_return;
        }

        /*
         * Start the new thread executing
         */
        if ( ResumeThread( (HANDLE)thdl ) == (DWORD)(-1) ) {
                err = GetLastError();
                goto error_return;
        }

        /*
         * Good return
         */
        return(thdl);

        /*
         * Error return
         */
error_return:
        /*
         * Either ptd is NULL, or it points to the no-longer-necessary block
         * calloc-ed for the _tiddata struct which should now be freed up.
         */
        _free_crt(ptd);

        /*
         * Map the error, if necessary.
         */
        if ( err != 0L )
                _dosmaperr(err);

        return( (uintptr_t)(-1) );
}


/***
*_threadstart() - New thread begins here
*
*Purpose:
*       The new thread begins execution here.  This routine, in turn,
*       passes control to the user's code.
*
*Entry:
*       void *ptd       = pointer to _tiddata structure for this thread
*
*Exit:
*       Never returns - terminates thread!
*
*Exceptions:
*
*******************************************************************************/

static unsigned long WINAPI _threadstart (
        void * ptd
        )
{
        _ptiddata _ptd;                  /* pointer to per-thread data */

        /* Initialize FlsGetValue function pointer */
        __set_flsgetvalue();

        /*
         * Check if ptd is initialised during THREAD_ATTACH call to dll mains
         */
        if ( (_ptd = (_ptiddata)__fls_getvalue(__get_flsindex())) == NULL)
        {
            /*
             * Stash the pointer to the per-thread data stucture in TLS
             */
            if ( !__fls_setvalue(__get_flsindex(), ptd) )
            {
                ExitThread(GetLastError());
            }
        }
        else
        {
            _ptd->_initaddr = ((_ptiddata) ptd)->_initaddr;
            _ptd->_initarg =  ((_ptiddata) ptd)->_initarg;
            _ptd->_thandle =  ((_ptiddata) ptd)->_thandle;
#if defined (_M_CEE) || defined (MRTDLL)
            _ptd->__initDomain=((_ptiddata) ptd)->__initDomain;
#endif  /* defined (_M_CEE) || defined (MRTDLL) */
            _freefls(ptd);
            ptd = _ptd;
        }

        /*
         * Call fp initialization, if necessary
         */
#ifndef MRTDLL
#ifdef CRTDLL
        _fpclear();
#else  /* CRTDLL */
        if (_FPmtinit != NULL &&
            _IsNonwritableInCurrentImage((PBYTE)&_FPmtinit))
        {
            (*_FPmtinit)();
        }
#endif  /* CRTDLL */
#endif  /* MRTDLL */

#if defined (_M_CEE) || defined (MRTDLL)
        DWORD domain=0;
        if(!_getdomain(&domain))
        {
            ExitThread(0);
        }
        if(domain!=_ptd->__initDomain)
        {
            /* need to transition to caller's domain and startup there*/
            ::msclr::call_in_appdomain(_ptd->__initDomain, _callthreadstart);

            return 0L;
        }
#endif  /* defined (_M_CEE) || defined (MRTDLL) */

        _callthreadstart();

        return(0L);
}

static void _callthreadstart(void)
{
    _ptiddata ptd;           /* pointer to thread's _tiddata struct */

    /* must always exist at this point */
    ptd = _getptd();
    /*
     * Guard call to user code with a _try - _except statement to
     * implement runtime errors and signal support
     */
    __try
    {
        ( (void(__CLRCALL_OR_CDECL *)(void *))(((_ptiddata)ptd)->_initaddr) )
            ( ((_ptiddata)ptd)->_initarg );

        _endthread();
    }
    __except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
    {
            /*
                * Should never reach here
                */
            _exit( GetExceptionCode() );

    } /* end of _try - _except */
}



#ifndef MRTDLL

/***
*_endthread() - Terminate the calling thread
*
*Purpose:
*
*Entry:
*       void
*
*Exit:
*       Never returns!
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _endthread (
        void
        )
{
        _ptiddata ptd;           /* pointer to thread's _tiddata struct */

        /*
         * Call fp termination, if necessary
         */
#ifdef CRTDLL
        _fpclear();
#else  /* CRTDLL */
        if (_FPmtterm != NULL &&
            _IsNonwritableInCurrentImage((PBYTE)&_FPmtterm))
        {
            (*_FPmtterm)();
        }
#endif  /* CRTDLL */

        ptd = _getptd_noexit();
        if (ptd) {
            /*
             * Close the thread handle (if there was one)
             */
            if ( ptd->_thandle != (uintptr_t)(-1) )
                    (void) CloseHandle( (HANDLE)(ptd->_thandle) );

            /*
             * Free up the _tiddata structure & its subordinate buffers
             *      _freeptd() will also clear the value for this thread
             *      of the FLS variable __flsindex.
             */
            _freeptd(ptd);
        }

        /*
         * Terminate the thread
         */
        ExitThread(0);

}

#endif  /* MRTDLL */

