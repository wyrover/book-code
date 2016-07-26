/***
*_flswbuf.c - flush buffer and output wide character.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _flswbuf() - flush a file buffer and output a wide character.
*       If no buffer, make one.
*
*******************************************************************************/



#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#include "_flsbuf.c"

