/***
*wcstok.c - tokenize a wide-character string with given delimiters
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wcstok() - breaks wide-character string into series of token
*       via repeated calls.
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>
#ifdef _SECURE_VERSION
#include <internal.h>
#else  /* _SECURE_VERSION */
#include <mtdll.h>
#endif  /* _SECURE_VERSION */

/***
*wchar_t *wcstok(string, control) - tokenize string with delimiter in control
*       (wide-characters)
*
*Purpose:
*       wcstok considers the string to consist of a sequence of zero or more
*       text tokens separated by spans of one or more control chars. the first
*       call, with string specified, returns a pointer to the first wchar_t of
*       the first token, and will write a null wchar_t into string immediately
*       following the returned token. subsequent calls with zero for the first
*       argument (string) will work thru the string until no tokens remain. the
*       control string may be different from call to call. when no tokens remain
*       in string a NULL pointer is returned. remember the control chars with a
*       bit map, one bit per wchar_t. the null wchar_t is always a control char
*       (wide-characters).
*
*Entry:
*       wchar_t *string - wchar_t string to tokenize, or NULL to get next token
*       wchar_t *control - wchar_t string of characters to use as delimiters
*
*Exit:
*       returns pointer to first token in string, or if string
*       was NULL, to next token
*       returns NULL when no more tokens remain.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

#ifdef _SECURE_VERSION
#define _TOKEN *context
#else  /* _SECURE_VERSION */
#define _TOKEN ptd->_wtoken
#endif  /* _SECURE_VERSION */

#ifdef _SECURE_VERSION
wchar_t * __cdecl wcstok_s (
        wchar_t * string,
        const wchar_t * control,
        wchar_t ** context
        )
#else  /* _SECURE_VERSION */
wchar_t * __cdecl wcstok (
        wchar_t * string,
        const wchar_t * control
        )
#endif  /* _SECURE_VERSION */
{
        wchar_t *token;
        const wchar_t *ctl;

#ifdef _SECURE_VERSION

        /* validation section */
        _VALIDATE_RETURN(context != NULL, EINVAL, NULL);
        _VALIDATE_RETURN(string != NULL || *context != NULL, EINVAL, NULL);
        _VALIDATE_RETURN(control != NULL, EINVAL, NULL);

        /* no static storage is needed for the secure version */

#else  /* _SECURE_VERSION */

        _ptiddata ptd = _getptd();

#endif  /* _SECURE_VERSION */

        /* If string==NULL, continue with previous string */
        if (!string)
                string = _TOKEN;

        /* Find beginning of token (skip over leading delimiters). Note that
         * there is no token iff this loop sets string to point to the terminal
         * null (*string == '\0') */

        while (*string) {
                for (ctl=control; *ctl && *ctl != *string; ctl++)
                        ;
                if (!*ctl) break;
                string++;
        }

        token = string;

        /* Find the end of the token. If it is not the end of the string,
         * put a null there. */
        for ( ; *string ; string++ ) {
                for (ctl=control; *ctl && *ctl != *string; ctl++)
                        ;
                if (*ctl) {
                        *string++ = '\0';
                        break;
                }
        }

        /* Update nextoken (or the corresponding field in the per-thread data
         * structure */
        _TOKEN = string;

        /* Determine if a token has been found. */
        if ( token == string )
                return NULL;
        else
                return token;
}

