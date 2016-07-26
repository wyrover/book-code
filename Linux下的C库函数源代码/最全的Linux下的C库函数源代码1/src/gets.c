/***
*gets.c - read a line from stdin
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines gets() and getws() - read a line from stdin into buffer
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <mtdll.h>
#include <tchar.h>
#include <limits.h>

#ifdef _UNICODE
#define _getchar_helper _getwchar_nolock
#else  /* _UNICODE */
#define _getchar_helper _getchar_nolock
#endif  /* _UNICODE */

_TCHAR * __cdecl _getts_helper (
    _TCHAR *string,
    size_t bufferSize,
    int earlyOutIfEOFIsFirstChar
)
{
    int ch = 0;
    _TCHAR *pointer = string;
    _TCHAR *retval = string;

    _VALIDATE_RETURN( (string != NULL), EINVAL, NULL);
    _VALIDATE_RETURN( (bufferSize > 0), EINVAL, NULL);

    /* The C Standard states the input buffer should remain
    unchanged if EOF is encountered immediately. Hence we
    do not blank out the input buffer here */

    _lock_str2(0, stdin);
    __try {

#ifndef _UNICODE
    _VALIDATE_STREAM_ANSI_SETRET(stdin, EINVAL, retval, NULL);
    if (retval == NULL)
    {
        goto done;
    }
#endif  /* _UNICODE */

    /* special case: check if the first char is EOF and treat it differently if the user requested so */
    ch = _getchar_helper();
    if (ch == _TEOF)
    {
        retval = NULL;
        if (earlyOutIfEOFIsFirstChar)
            goto done;
    }

    if (bufferSize == (size_t)-1)
    {
        /* insecure case: no buffer size check, no debug filling */
        while (ch != _T('\n') && ch != _TEOF)
        {
            *pointer++ = (_TCHAR)ch;
            ch = _getchar_helper();
        }
        *pointer = 0;
    }
    else
    {
        /* secure case, check buffer size; if buffer overflow, keep on reading until /n or EOF */
        size_t available = bufferSize;
        while (ch != _T('\n') && ch != _TEOF)
        {
            if (available > 0)
            {
                --available;
                *pointer++ = (_TCHAR)ch;
            }
            ch = _getchar_helper();
        }
        if (available == 0)
        {
            _RESET_STRING(string, bufferSize);
            _RETURN_BUFFER_TOO_SMALL_ERROR(string, bufferSize, NULL);
        }
        *pointer = 0;
        _FILL_STRING(string, bufferSize, bufferSize - available + 1);
    }

    /* Common return */
done: ;

    }
    __finally
    {
        _unlock_str2(0, stdin);
    }

    return retval;
}

/***
*errno_t gets_s(string, sz) - read a line from stdin
*
*Purpose:
*
*
*Entry:
*       char *string      - place to store read string.
*       size_t bufferSize - length of dest buffer
*
*Exit:
*       returns string, filled in with the line of input
*       null string if \n found immediately or if EOF found immediately
*Exceptions:
*
*******************************************************************************/

_TCHAR * __cdecl _getts_s (
    _TCHAR *string,
    size_t bufferSize
)
{
    return _getts_helper(string, bufferSize, 0 /* treat first EOF as \n */);
}

/***
*char *gets(string) - read a line from stdin
*
*Purpose:
*       Gets a string from stdin terminated by '\n' or EOF; don't include '\n';
*       append '\0'.
*
*Entry:
*       char *string - place to store read string, assumes enough room.
*
*Exit:
*       returns string, filled in with the line of input
*       null string if \n found immediately
*       NULL if EOF found immediately
*
*Exceptions:
*
*******************************************************************************/

_TCHAR * __cdecl _getts (
    _TCHAR *string
)
{
    return _getts_helper(string, (size_t)-1, 1 /* early out if EOF is first char read */);
}

