/***
*vsnwprnt.c - "Count" version of vswprintf
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       The _vsnwprintf() flavor takes a count argument that is
*       the max number of wide characters that should be written to the
*       user's buffer.
*
*******************************************************************************/


#define _COUNT_ 1
#include "vswprint.c"

