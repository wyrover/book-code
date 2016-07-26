/***
*crt0fp.asm - floating point not loaded trap
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       To trap certain cases where certain necessary floating-point
*       software is not loaded.  Two specific cases are when no emulator
*       is linked in but no coprocessor is present, and when floating
*       point i/o conversions are done, but no floating-point variables
*       or expressions are used in the program.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <rterr.h>

/***
*_fptrap - trap for missing floating-point software
*
*Purpose:
*       Catches these cases of incomplete f.p. software linked into a program.
*
*       (1) no coprocessor present, and no emulator linked in
*
*       (2) "%e", "%f", and "%g" i/o conversion formats specified, but
*           not all conversion software has been linked in, because the
*           program did not use any floating-point variables or expressions.
*
*Entry:
*       None.
*
*Exit:
*       Never returns.
*
*Exceptions:
*       Transfers control to _amsg_exit which ...
*       - Writes error message to standard error:  "floating point not loaded"
*       - Terminates the program by calling _exit().
*******************************************************************************/

void __cdecl _fptrap(
        void
        )
{
        _amsg_exit(_RT_FLOAT);
}
