/***
*cmiscdat.c - miscellaneous C run-time data
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Includes floating point conversion table (for C float output).
*
*       When floating point I/O conversions are done, but no floating-point
*       variables or expressions are used in the C program, we use the
*       _cfltcvt_tab[] to map these cases to the _fptrap entry point,
*       which prints "floating point not loaded" and dies.
*
*       This table is initialized to six copies of _fptrap by default.
*       If floating-point is linked in (_fltused), these table entries
*       are reset (see input.c, output.c, fltused.asm, and fltuseda.asm).
*
*******************************************************************************/

#if !defined (CRTDLL)

#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <fltintrn.h>

/*-
 *  ... table of (model-dependent) code pointers ...
 *
 *  Entries all point to _fptrap by default,
 *  but are changed to point to the appropriate
 *  routine if the _fltused initializer (_cfltcvt_init)
 *  is linked in.
 *
 *  if the _fltused modules are linked in, then the
 *  _cfltcvt_init initializer sets the entries of
 *  _cfltcvt_tab.
-*/

PFV _cfltcvt_tab[10] = {
    _fptrap,    /*  _cfltcvt */
    _fptrap,    /*  _cropzeros */
    _fptrap,    /*  _fassign */
    _fptrap,    /*  _forcdecpt */
    _fptrap,    /*  _positive */
    _fptrap,    /*  _cldcvt */
    _fptrap,    /*  _cfltcvt_l */
    _fptrap,    /*  _fassign_l */
    _fptrap,    /*  _cropzeros_l */
    _fptrap     /*  _forcdecpt_l */
};

void __cdecl _initp_misc_cfltcvt_tab()
{
    int i;
    for (i = 0; i < _countof(_cfltcvt_tab); ++i)
    {
        _cfltcvt_tab[i] = (PFV)_encode_pointer(_cfltcvt_tab[i]);
    }
}

#endif  /* !defined (CRTDLL) */
