/***
*newmode.c - set new() handler mode to handle malloc failures
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets the global flag which controls whether the new() handler
*       is called on malloc failures.  The default behavior in Visual
*       C++ v2.0 and later is not to, that malloc failures return NULL
*       without calling the new handler.  Linking with this object changes
*       the start-up behavior to call the new handler on malloc failures.
*
*******************************************************************************/


#include <internal.h>

/* enable new handler calls upon malloc failures */

int _newmode = 1;       /* Malloc New Handler MODE */

