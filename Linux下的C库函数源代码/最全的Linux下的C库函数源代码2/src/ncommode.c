/***
*ncommode.c - set global file commit mode flag to nocommit
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets the global file commit mode flag to nocommit.  This is the default.
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>

/* set default file commit mode to nocommit */
int _commode = 0;
