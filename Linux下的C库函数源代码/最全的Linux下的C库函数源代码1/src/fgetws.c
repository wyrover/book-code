/***
*fgetws.c - get wide string from a file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fgetws() - read a wide string from a file
*
*******************************************************************************/


/***
*wchar_t *fgetws(string, count, stream) - input string from a stream
*
*Purpose:
*       get a string, up to count-1 wide chars or L'\n', whichever comes first,
*       append L'\0' and put the whole thing into string. the L'\n' IS included
*       in the string. if count<=1 no input is requested. if WEOF is found
*       immediately, return NULL. if WEOF found after chars read, let WEOF
*       finish the string as L'\n' would.
*
*Entry:
*       wchar_t *string - pointer to place to store string
*       int count - max characters to place at string (include \0)
*       FILE *stream - stream to read from
*
*Exit:
*       returns wide string with text read from file in it.
*       if count <= 0 return NULL
*       if count == 1 put null string in string
*       returns NULL if error or end-of-file found immediately
*
*Exceptions:
*
*******************************************************************************/

#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#include "fgets.c"

