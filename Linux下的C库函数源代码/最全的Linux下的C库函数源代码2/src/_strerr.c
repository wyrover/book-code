/***
*_strerr.c - routine for indexing into system error list
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   Returns system error message index by errno; conforms to the
*   XENIX standard, much compatibility with 1983 uniforum draft standard.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <errmsg.h>
#include <syserr.h>
#include <string.h>
#include <tchar.h>
#include <malloc.h>
#include <mtdll.h>
#include <dbgint.h>
#include <internal.h>

/* Max length of message = user_string(94)+system_string+2 */
/* [NOTE: The mthread error message buffer is shared by both strerror
   and _strerror so must be the max length of both. */
#define _ERRMSGLEN_ (94+_SYS_MSGMAX+2)

#ifdef _UNICODE
#define _terrmsg    _werrmsg
#else  /* _UNICODE */
#define _terrmsg    _errmsg
#endif  /* _UNICODE */


/***
*char *_strerror(message) - get system error message
*
*Purpose:
*   builds an error message consisting of the users error message
*   (the message parameter), followed by ": ", followed by the system
*   error message (index through errno), followed by a newline.  If
*   message is NULL or a null string, returns a pointer to just
*   the system error message.
*
*Entry:
*   char *message - user's message to prefix system error message
*
*Exit:
*   returns pointer to static memory containing error message.
*   returns NULL if malloc() fails in multi-thread versions.
*
*Exceptions:
*
*******************************************************************************/

#ifdef _UNICODE
wchar_t * __cdecl __wcserror(
#else  /* _UNICODE */
char * __cdecl _strerror (
#endif  /* _UNICODE */
    REG1 const _TCHAR *message
    )
{
    const char *sysErrorMsg = NULL;

    _TCHAR *bldmsg;
    _ptiddata ptd = _getptd_noexit();
    if (!ptd)
        return NULL;




    /* Use per thread buffer area (malloc space, if necessary) */
    /* [NOTE: This buffer is shared between _strerror and streror.] */

    if ( (ptd->_terrmsg == NULL) && ((ptd->_terrmsg =
            _calloc_crt(_ERRMSGLEN_, sizeof(_TCHAR))) == NULL) )
            return(NULL);
    bldmsg = ptd->_terrmsg;


    /* Build the error message */

    bldmsg[0] = '\0';

    if (message && *message) {
        // should leave space for ": \n\0"
        _ERRCHECK(_tcsncat_s( bldmsg, _ERRMSGLEN_, message, _ERRMSGLEN_-4 ));
        _ERRCHECK(_tcscat_s( bldmsg, _ERRMSGLEN_, _T(": ")));
    }

    //  We should have extra space for "\n\0"
    sysErrorMsg = _get_sys_err_msg(errno);

#ifdef _UNICODE
    _ERRCHECK(mbstowcs_s(NULL, bldmsg + wcslen(bldmsg), _ERRMSGLEN_ - wcslen(bldmsg), sysErrorMsg, _ERRMSGLEN_ - wcslen(bldmsg) - 2));
#else  /* _UNICODE */
    _ERRCHECK(strncat_s(bldmsg, _ERRMSGLEN_, sysErrorMsg, _ERRMSGLEN_ - strlen(bldmsg) - 2));
#endif  /* _UNICODE */

    _ERRCHECK(_tcscat_s( bldmsg, _ERRMSGLEN_, _T("\n")));
    return bldmsg;
}

/***
*errno_t _strerror_s(buffer, sizeInTChars, message) - get system error message
*
*Purpose:
*   builds an error message consisting of the users error message
*   (the message parameter), followed by ": ", followed by the system
*   error message (index through errno), followed by a newline.  If
*   message is NULL or a null string, returns a pointer to just
*   the system error message.
*
*Entry:
*   TCHAR * buffer - Destination buffer.
*   size_t sizeInTChars - Size of the destination buffer.
*   TCHAR * message - user's message to prefix system error message
*
*Exit:
*   The error code.
*
*Exceptions:
*   Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

#define _MIN_MSG_LENGTH 5

#ifdef _UNICODE
errno_t __cdecl __wcserror_s(
#else  /* _UNICODE */
errno_t __cdecl _strerror_s(
#endif  /* _UNICODE */
    TCHAR* buffer,
    size_t sizeInTChars,
    REG1 const _TCHAR *message
    )
{
    errno_t e = 0;

    /* validation section */
    _VALIDATE_RETURN_ERRCODE(buffer != NULL, EINVAL);
    _VALIDATE_RETURN_ERRCODE(sizeInTChars > 0, EINVAL);
    buffer[0] = '\0';

    if (message &&
        *message &&
        _tcslen(message) < (sizeInTChars - 2 - _MIN_MSG_LENGTH))
    {
        _ERRCHECK(_tcscpy_s(buffer, sizeInTChars, message));
        _ERRCHECK(_tcscat_s(buffer, sizeInTChars, _T(": ")));
    }

    /* append the error message at the end of the buffer */
    return _tcserror_s(buffer + _tcslen(buffer), sizeInTChars - _tcslen(buffer), errno);
}
