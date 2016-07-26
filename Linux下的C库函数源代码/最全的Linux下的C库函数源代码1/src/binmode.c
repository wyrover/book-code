/***
*binmode.c - set global file mode to binary
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets the global file mode flag to binary.  Linking with this file
*       sets all files to be opened in binary mode.
*
*******************************************************************************/

#define SPECIAL_CRTEXE

#include <cruntime.h>
#include <fcntl.h>
#include <stdlib.h>

/* set default file mode */
int _fmode = _O_BINARY;

