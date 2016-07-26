/***
*_filwbuf.c - fill buffer and get wide character
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _filwbuf() - fill buffer and read first character, allocate
*       buffer if there is none.  Used from getwc().
*
*******************************************************************************/



#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#include "_filbuf.c"

