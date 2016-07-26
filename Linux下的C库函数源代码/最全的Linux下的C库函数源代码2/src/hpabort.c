/***
*hpabort.c - Abort process due to fatal heap error
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <rterr.h>


/***
* _heap_abort() - Abort process due to fatal heap error
*
*Purpose:
*       Terminate the process and output a heap error message
*
*Entry:
*       Void
*
*Exit:
*       Never returns
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _heap_abort (
        void
        )
{
        _amsg_exit(_RT_HEAP);           /* heap error */
        /*** PROCESS TERMINATED ***/
}
