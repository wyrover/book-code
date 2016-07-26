/***
*mbccpy_s.inl - general implementation of _mbccpy_s
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the algorithm for _mbccpy_s.
*
****/

_FUNC_PROLOGUE
#if _USE_LOCALE_ARG
errno_t __cdecl _FUNC_NAME(unsigned char *_DEST, size_t _SizeInBytes, int *_PCopied, const unsigned char *_SRC, _LOCALE_ARG_DECL)
#else  /* _USE_LOCALE_ARG */
errno_t __cdecl _FUNC_NAME(unsigned char *_DEST, size_t _SizeInBytes, int *_PCopied, const unsigned char *_SRC)
#endif  /* _USE_LOCALE_ARG */
{
    /* validation section */
    _ASSIGN_IF_NOT_NULL(_PCopied, 0);
    _VALIDATE_STRING(_DEST, _SizeInBytes);
    if (_SRC == NULL)
    {
        *_DEST = '\0';
        _RETURN_EINVAL;
    }

#if _USE_LOCALE_ARG
    _LOCALE_UPDATE;
#endif  /* _USE_LOCALE_ARG */

    /* copy */
    if (_ISMBBLEAD(*_SRC))
    {
        if (_SRC[1] == '\0')
        {
            /* the source string contained a lead byte followed by the null terminator:
               we copy only the null terminator and return EILSEQ to indicate the
               malformed char */
            *_DEST = '\0';
            _ASSIGN_IF_NOT_NULL(_PCopied, 1);
            _RETURN_MBCS_ERROR;
        }
        if (_SizeInBytes < 2)
        {
            *_DEST = '\0';
            _RETURN_BUFFER_TOO_SMALL(_DEST, _SizeInBytes);
        }
        *_DEST++ = *_SRC++;
        *_DEST = *_SRC;
        _ASSIGN_IF_NOT_NULL(_PCopied, 2);
    }
    else
    {
        *_DEST = *_SRC;
        _ASSIGN_IF_NOT_NULL(_PCopied, 1);
    }

    _RETURN_NO_ERROR;
}
