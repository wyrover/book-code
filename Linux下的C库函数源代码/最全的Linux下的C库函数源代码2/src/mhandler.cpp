/***
*handler.cpp - defines C++ setHandler routine
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines pure MSIL C++ setHandler routine.
*
*******************************************************************************/


#include <internal.h>
#include <mtdll.h>
#include <new.h>
#include <signal.h>
#include <unknwn.h>
#include <minternal.h>

volatile __declspec(appdomain) __MPNH __mpnhHeap= (__MPNH) _encoded_null();
volatile __declspec(appdomain) _new_handler_m __nhmHeap= (_new_handler_m) _encoded_null();

/***
*int _callnewh - call the appropriate new handler
*
*Purpose:
*       Call the appropriate new handler.
*
*Entry:
*       None
*
*Return:
*       1 for success
*       0 for failure
*       may throw bad_alloc
*
*******************************************************************************/
static int __cdecl _callnewh_thunk(size_t size)
{
    __MPNH pnh = (__MPNH) _decode_pointer(__mpnhHeap);

    if (pnh != NULL)
    {
        return (*pnh)(size);
    }
    else
    {
        return 0;
    }
}

static void __clrcall _callnewh_cleanup(void)
{
    _PNH pnh=_query_new_handler();
    __MPNH enull = (__MPNH) _encoded_null();
    if(pnh==_callnewh_thunk && __mpnhHeap!=enull)
    {
        _set_new_handler((_PNH)NULL);
    }
    __mpnhHeap=enull;
}

static int __cdecl _callnewhandler_thunk
(
    size_t size
)
{
    _new_handler_m pnh = (_new_handler_m) _decode_pointer(__nhmHeap);

    if ( pnh != NULL)
    {
        pnh();
    }
    else
    {
        return 0;
    }

    return 1;
}

static void __clrcall _callnewhandler_cleanup(void)
{
    _PNH pnh=_query_new_handler();
    _new_handler_m enull = (_new_handler_m) _encoded_null();
    if(pnh==_callnewhandler_thunk && __nhmHeap!=enull)
    {
        _set_new_handler((_PNH)NULL);
    }
    __nhmHeap=enull;
}

/***
*__MPNH _set_new_handler(__MPNH pnh) - set the new handler
*
*Purpose:
*       _set_new_handler is different from the ANSI C++ working standard definition
*       of set_new_handler.  Therefore it has a leading underscore in its name.
*
*Entry:
*       Pointer to the new handler to be installed.
*
*Return:
*       Previous new handler
*
*******************************************************************************/
__MPNH _MRTIMP __cdecl _set_new_handler
(
    __MPNH pnh
)
{
    if(_atexit_m_appdomain(_callnewh_cleanup)!=0)
    {
        return NULL;
    }

    __MPNH pnhOld = (__MPNH) _decode_pointer(__mpnhHeap);
    __mpnhHeap = (__MPNH) _encode_pointer(pnh);
    _set_new_handler((_PNH)_callnewh_thunk);

    return(pnhOld);
}

std::_new_handler_m _MRTIMP __cdecl std::set_new_handler
(
    std::_new_handler_m pnh
) throw()
{
    if(_atexit_m_appdomain(_callnewhandler_cleanup)!=0)
    {
        return NULL;
    }

    _new_handler_m pnhOld = (_new_handler_m) _decode_pointer(__nhmHeap);
    __nhmHeap = (_new_handler_m) _encode_pointer(pnh);
    _set_new_handler((_PNH)_callnewhandler_thunk);

    return(pnhOld);
}

/***
*__MPNH __query_new_handler_m(void) - query value of new handler
*
*Purpose:
*       Obtain current new handler value.
*
*Entry:
*       None
*
*       WARNING: This function is OBSOLETE. Use _query_new_ansi_handler instead.
*
*Return:
*       Currently installed new handler
*
*******************************************************************************/
__MPNH __cdecl __query_new_handler_m
(
    void
)
{
    return (__MPNH) _decode_pointer(__mpnhHeap);
}

typedef void (__clrcall *_PHNDLR_m)(int);
volatile __declspec(appdomain) static _PHNDLR_m __psignal_func[NSIG];

class __signal_init
{
public:
        __signal_init()
        {
                _PHNDLR_m enull = (_PHNDLR_m) _encoded_null();
                for(int i=0; i<NSIG; ++i)
                {
                        __psignal_func[i]=enull;
                }
        };
};

#pragma warning(push)
#pragma warning(disable: 4074)
#pragma init_seg(compiler)
#pragma warning(pop)
static __signal_init _init;

static void __cdecl __signal_thunk(int i)
{
    _PHNDLR_m local_func = (_PHNDLR_m) _decode_pointer(__psignal_func[i]);
    if (local_func != NULL)
    {
        local_func(i);
    }
}

static void __clrcall _signal_cleanup(void)
{
    _PHNDLR_m enull = (_PHNDLR_m) _encoded_null();
    for(int i=0; i<NSIG; ++i)
    {
        if(__psignal_func[i]!=enull)
        {
            _PHNDLR _handler=signal(i, SIG_GET);
            if(_handler==__signal_thunk)
            {
                signal(i, SIG_DFL);
            }
            __psignal_func[i]=enull;
        }
    }
}

/***
*_PHNDLR_m signal(signum, sigact) - Define a signal handler
*
*Purpose:
*       The signal routine allows the user to define what action should
*       be taken when various signals occur. The Win32/Dosx32 implementation
*       supports seven signals, divided up into three general groups
*
*       1. Signals corresponding to OS exceptions. These are:
*                       SIGFPE
*                       SIGILL
*                       SIGSEGV
*          Signal actions for these signals are installed by altering the
*          XcptAction and SigAction fields for the appropriate entry in the
*          exception-action table (XcptActTab[]).
*
*       2. Signals corresponding to ^C and ^Break. These are:
*                       SIGINT
*                       SIGBREAK
*          Signal actions for these signals are installed by altering the
*          _ctrlc_action and _ctrlbreak_action variables.
*
*       3. Signals which are implemented only in the runtime. That is, they
*          occur only as the result of a call to raise().
*                       SIGABRT
*                       SIGTERM
*
*
*Entry:
*       int signum      signal type. recognized signal types are:
*
*                       SIGABRT         (ANSI)
*                       SIGBREAK
*                       SIGFPE          (ANSI)
*                       SIGILL          (ANSI)
*                       SIGINT          (ANSI)
*                       SIGSEGV         (ANSI)
*                       SIGTERM         (ANSI)
*                       SIGABRT_COMPAT
*
*       _PHNDLR sigact  signal handling function or action code. the action
*                       codes are:
*
*                       SIG_DFL - take the default action, whatever that may
*                       be, upon receipt of this type type of signal.
*
*                       SIG_DIE - *** ILLEGAL ***
*                       special code used in the XcptAction field of an
*                       XcptActTab[] entry to indicate that the runtime is
*                       to terminate the process upon receipt of the exception.
*                       not accepted as a value for sigact.
*
*                       SIG_IGN - ignore this type of signal
*
*                       [function address] - transfer control to this address
*                       when a signal of this type occurs.
*
*Exit:
*       Good return:
*       Signal returns the previous value of the signal handling function
*       (e.g., SIG_DFL, SIG_IGN, etc., or [function address]). This value is
*       returned in DX:AX.
*
*       Error return:
*       Signal returns -1 and errno is set to EINVAL. The error return is
*       generally taken if the user submits bogus input values.
*
*Exceptions:
*       None.
*
*******************************************************************************/
_MRTIMP _PHNDLR_m __cdecl signal(int signum, _PHNDLR_m sigact)
{
    if(signum<0 || signum>=NSIG)
    {
        /* call signal to allow it to do error detection and handling, but without setting us up */
        signal(signum, static_cast<_PHNDLR>(NULL));
        return (void (__clrcall *)(int))-1;
    }

    if(_atexit_m_appdomain(_signal_cleanup)!=0)
    {
        return NULL;
    }

    _PHNDLR_m __psignal_func_old = (_PHNDLR_m) _decode_pointer(__psignal_func[signum]);
    __psignal_func[signum] = (_PHNDLR_m) _encode_pointer(sigact);
    void (__cdecl *pnative_signal_func_old)(int) = signal(signum, (void (__cdecl *)(int))__signal_thunk);
    if (pnative_signal_func_old == __signal_thunk)
    {
        return __psignal_func_old;
    }
    else if (pnative_signal_func_old == NULL)
    {
        return NULL;
    }
    else
    {
        return (void (__clrcall *)(int))-1;
    }
}

_MRTIMP _PHNDLR_m __cdecl signal(int signum, int sigact)
{
    _VALIDATE_RETURN(sigact == NULL, EINVAL, NULL);

    return signal(signum, static_cast<_PHNDLR_m>(NULL));
}
