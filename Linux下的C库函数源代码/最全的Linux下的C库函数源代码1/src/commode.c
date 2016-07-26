/***
*commode.c - set global file commit mode to commit
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets the global file commit mode flag to commit.  Linking with
*       this file sets all files to be opened in commit mode by default.
*
*******************************************************************************/

#define SPECIAL_CRTEXE

#include <cruntime.h>
#include <file2.h>
#include <internal.h>

/* set default file commit mode to commit */
int _commode = _IOCOMMIT;


