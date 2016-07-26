/***
*vsnprintf.c - "Count" version of vsprintf
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       The _vsnprintf() flavor takes a count argument that is
*       the max number of bytes that should be written to the
*       user's buffer.
*
*******************************************************************************/

#define _COUNT_ 1
#include "vsprintf.c"
