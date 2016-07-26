/***
*purevirt.c - stub to trap pure virtual function calls
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _purecall() -
*
*******************************************************************************/


#include <cruntime.h>
#include <internal.h>
#include <rterr.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
//
// The global variable:
//

extern _purecall_handler __pPurecall;

/***
*void _purecall(void) -
*
*Purpose:
*       The compiler calls this if a pure virtual happens
*
*Entry:
*       No arguments
*
*Exit:
*       Never returns
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _purecall(
        void
        )
{
    _purecall_handler purecall = (_purecall_handler) _decode_pointer(__pPurecall);
    if(purecall != NULL)
    {
        purecall();

        /*  shouldn't return, but if it does, we drop back to
            default behaviour
        */
    }

    _NMSG_WRITE(_RT_PUREVIRT);
    /* do not write the abort message */
    _set_abort_behavior(0, _WRITE_ABORT_MSG);
    abort();
}

/***
*void _set_purecall_handler(void) -
*
*Purpose:
*       Establish a handler to be called when a pure virtual is called
*       Note that if you link to the crt statically, and replace
*       _purecall, then none of this will happen.
*
*       This function is not thread-safe
*
*Entry:
*       New handler
*
*Exit:
*       Old handler
*
*Exceptions:
*
*******************************************************************************/

_CRTIMP _purecall_handler __cdecl
_set_purecall_handler( _purecall_handler pNew )
{
    _purecall_handler pOld = NULL;

    pOld = (_purecall_handler) _decode_pointer(__pPurecall);
    __pPurecall = (_purecall_handler) _encode_pointer(pNew);

    return pOld;
}

_CRTIMP _purecall_handler __cdecl _get_purecall_handler(void)
{
    return (_purecall_handler) _decode_pointer(__pPurecall);
}

