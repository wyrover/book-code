/***
*strerror.c - Contains the strerror C runtime.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The strerror runtime accepts an error number as input
*       and returns the corresponding error string.
*
*       NOTE: The "old" strerror C runtime resides in file _strerr.c
*       and is now called _strerror.  The new strerror runtime
*       conforms to the ANSI standard.
*
*******************************************************************************/

#include <cruntime.h>
#include <errmsg.h>
#include <stdlib.h>
#include <syserr.h>
#include <string.h>
#include <mtdll.h>
#include <tchar.h>
#include <malloc.h>
#include <stddef.h>
#include <dbgint.h>
#include <internal.h>

/* [NOTE: The _MT error message buffer is shared by both strerror
   and _strerror so must be the max length of both. */
/* Max length of message = user_string(94)+system_string+2 */
#define _ERRMSGLEN_ (94+_SYS_MSGMAX+2)

#ifdef _UNICODE
#define _terrmsg    _werrmsg
#else  /* _UNICODE */
#define _terrmsg    _errmsg
#endif  /* _UNICODE */

/***
*char *strerror(errnum) - Map error number to error message string.
*
*Purpose:
*       The strerror runtime takes an error number for input and
*       returns the corresponding error message string.  This routine
*       conforms to the ANSI standard interface.
*
*Entry:
*       int errnum - Integer error number (corresponding to an errno value).
*
*Exit:
*       char * - Strerror returns a pointer to the error message string.
*       This string is internal to the strerror routine (i.e., not supplied
*       by the user).
*
*Exceptions:
*       None.
*
*******************************************************************************/

#ifdef _UNICODE
wchar_t * cdecl _wcserror(
#else  /* _UNICODE */
char * __cdecl strerror (
#endif  /* _UNICODE */
        int errnum
        )
{

        _TCHAR *errmsg;
        _ptiddata ptd = _getptd_noexit();
        if (!ptd)
                return _T("Visual C++ CRT: Not enough memory to complete call to strerror.");



        if ( (ptd->_terrmsg == NULL) && ((ptd->_terrmsg =
                        _calloc_crt(_ERRMSGLEN_, sizeof(_TCHAR)))
                        == NULL) )
                return _T("Visual C++ CRT: Not enough memory to complete call to strerror.");
        else
                errmsg = ptd->_terrmsg;


#ifdef _UNICODE
        _ERRCHECK(mbstowcs_s(NULL, errmsg, _ERRMSGLEN_, _get_sys_err_msg(errnum), _ERRMSGLEN_ - 1));
#else  /* _UNICODE */
        _ERRCHECK(strcpy_s(errmsg, _ERRMSGLEN_, _get_sys_err_msg(errnum)));
#endif  /* _UNICODE */

        return(errmsg);
}

/***
*errno_t strerror_s(buffer, sizeInTChars, errnum) - Map error number to error message string.
*
*Purpose:
*       The strerror_s runtime takes an error number for input and
*       copies the corresponding error message string in the destination
*       buffer. If the buffer is too small, the message is truncated.
*
*Entry:
*       TCHAR * buffer - Destination buffer.
*       size_t sizeInTChars - Size of the destination buffer.
*       int errnum - Integer error number (corresponding to an errno value).
*
*Exit:
*       The error code.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

#ifdef _UNICODE
errno_t __cdecl _wcserror_s(
#else  /* _UNICODE */
errno_t __cdecl strerror_s(
#endif  /* _UNICODE */
        TCHAR* buffer,
        size_t sizeInTChars,
        int errnum
        )
{
        errno_t e = 0;

        /* validation section */
        _VALIDATE_RETURN_ERRCODE(buffer != NULL, EINVAL);
        _VALIDATE_RETURN_ERRCODE(sizeInTChars > 0, EINVAL);

        /* we use mbstowcs_s or strncpy_s because we want to truncate the error string
         * if the destination is not big enough
         */
#ifdef _UNICODE
        e = _ERRCHECK_EINVAL_ERANGE(mbstowcs_s(NULL, buffer, sizeInTChars, _get_sys_err_msg(errnum), _TRUNCATE));
        /* ignore the truncate information */
        if (e == STRUNCATE)
        {
                e = 0;
        }
#else  /* _UNICODE */
        _ERRCHECK(strncpy_s(buffer, sizeInTChars, _get_sys_err_msg(errnum), sizeInTChars - 1));
#endif  /* _UNICODE */
    return e;
}
