/***
*cgets.c - buffered keyboard input
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _cgets() - read a string directly from console
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <mtdll.h>
#include <conio.h>
#include <stdlib.h>
#include <internal.h>
#include <internal_securecrt.h>

/*
 * mask to clear the bits required to be 0 in the handle state passed to
 * DOSSETFHSTATE.
 */
#define FHSTATEMASK 0xffd07888

/*
 * declaration for console handle
 */

extern intptr_t _coninpfh;

/*
 * Use of the following buffer variables is primarily for syncronizing with
 * _cgets_s. _cget_s fills the MBCS buffer and if the user passes in single
 * character buffer and the unicode character is not converted to single byte
 * MBC, then _cget_s should buffer that character so that next call to
 * _cgetws_s can return the same character.
 */
extern wchar_t __console_wchar_buffer;
extern int __console_wchar_buffer_used;

/***
*errno_t _cgets_s(string, sizeInBytes, pSizeRead) - read string from console
*
*Purpose:
*       Reads a string from the console. Always null-terminate the buffer.
*
*Entry:
*       char *string - place to store read string
*       sizeInBytes  - max length to read
*       pSizeRead - sets the max read in this
*
*Exit:
*       returns error code or 0 if no error occurs
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _cgets_s (
        char *string,
        size_t sizeInBytes,
        size_t * pSizeRead
        )
{
        errno_t err = 0;
        size_t available;

        _VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE((string != NULL), EINVAL);
        _VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE((sizeInBytes > 0), EINVAL);
        _RESET_STRING(string, sizeInBytes);

        _VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE((pSizeRead != NULL), EINVAL);

        available = sizeInBytes - 1;

        _mlock(_CONIO_LOCK);            /* lock the console */
        __TRY
            /*
             * The implementation of cgets is slightly tricky. The reason being,
             * the code page for console is different from the CRT code page.
             * What this means is the program may interpret character
             * differently from it's acctual value. To fix this, what we really
             * want to do is read the input as unicode string and then convert
             * it to proper MBCS representation.
             *
             * This fix this we are really converting from Unicode to MBCS.
             * This adds performance problem as we may endup doing this
             * character by character. The basic problem here is that we have
             * no way to know how many UNICODE characters will be needed to fit
             * them in given size of MBCS buffer. To fix this issue we will be
             * converting one Unicode character at a time to MBCS. This makes
             * this slow, but then this is already console input,
             */
            *pSizeRead = 0;
            do {
                wchar_t wchar_buff[2];
                size_t sizeRead=0;
                if ((err = _cgetws_s(wchar_buff, _countof(wchar_buff), &sizeRead)) == 0)
                {
                    int sizeConverted = 0;
                    if (wchar_buff[0] == L'\0')
                    {
                        break;
                    }
                    else if ((err = wctomb_s(&sizeConverted, string, available, wchar_buff[0])) == 0)
                    {
                        string += sizeConverted;
                        (*pSizeRead) += sizeConverted;
                        available -= sizeConverted;
                    }
                    else
                    {
                        /*
                         * Put the wchar back to the buffer so that the
                         * unutilized wchar is still in the stream.
                         */
                        __console_wchar_buffer = wchar_buff[0];
                        __console_wchar_buffer_used = 1;
                        break;
                    }
                }
                else
                {
                    break;
                }
            } while (available > 0);

        __FINALLY
            _munlock(_CONIO_LOCK);          /* unlock the console */
        __END_TRY_FINALLY

        *string++ = '\0';

        if (err != 0)
        {
            errno = err;
        }
        return err;
}

/***
*char *_cgets(string) - read string from console
*
*Purpose:
*       Reads a string from the console via ReadConsole on a cooked console
*       handle.  string[0] must contain the maximum length of the
*       string.  Returns pointer to str[2].
*
*       NOTE: _cgets() does NOT check the pushback character buffer (i.e.,
*       _chbuf).  Thus, _cgets() will not return any character that is
*       pushed back by the _ungetch() call.
*
*Entry:
*       char *string - place to store read string, str[0] = max length.
*
*Exit:
*       returns pointer to str[2], where the string starts.
*       returns NULL if error occurs
*
*Exceptions:
*
*******************************************************************************/
char * __cdecl _cgets (
        char *string)
{
        size_t sizeInBytes;
        size_t sizeRead = 0;
        errno_t err;

        _VALIDATE_CLEAR_OSSERR_RETURN((string != NULL), EINVAL, NULL);

        sizeInBytes = (size_t)string[0];

        err = _cgets_s(string + 2, sizeInBytes, &sizeRead);

        string[1] = (char)(sizeRead);

        if (err != 0)
        {
            return NULL;
        }

        return string + 2;
}
