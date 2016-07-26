/***
*mbscat_s.inl - general implementation of _mbsncat_s and _mbsnbcat_s
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the general algorithm for _mbsncat_s and _mbsnbcat_s.
*
****/

_FUNC_PROLOGUE
#if _USE_LOCALE_ARG
errno_t __cdecl _FUNC_NAME(unsigned char *_DEST, size_t _SizeInBytes, const unsigned char *_SRC, size_t _COUNT, _LOCALE_ARG_DECL)
#else  /* _USE_LOCALE_ARG */
errno_t __cdecl _FUNC_NAME(unsigned char *_DEST, size_t _SizeInBytes, const unsigned char *_SRC, size_t _COUNT)
#endif  /* _USE_LOCALE_ARG */
{
    unsigned char *p;
    size_t available;
    BOOL fIsLeadPrefix;
    BOOL fFoundInvalidMBC;

    fFoundInvalidMBC = FALSE;

    if (_COUNT == 0 && _DEST == NULL && _SizeInBytes == 0)
    {
        /* this case is allowed; nothing to do */
        _RETURN_NO_ERROR;
    }

    /* validation section */
    _VALIDATE_STRING(_DEST, _SizeInBytes);
    if (_COUNT != 0)
    {
        _VALIDATE_POINTER_RESET_STRING(_SRC, _DEST, _SizeInBytes);
    }

#if _USE_LOCALE_ARG
    _LOCALE_UPDATE;
    if (_LOCALE_SHORTCUT_TEST)
    {
        return _SB_FUNC_NAME((char *)_DEST, _SizeInBytes, (const char *)_SRC, _COUNT);
    }
#endif  /* _USE_LOCALE_ARG */

    p = _DEST;
    available = _SizeInBytes;
    while (available > 0 && *p != 0)
    {
        p++;
        available--;
    }

    /*
     * Ran out of room while looking for end of dst string.
     * p points 1 past end of buffer. We can't look past
     * end of buffer so can't tell if dst ended with an
     * invalid mbc.
     */

    if (available == 0)
    {
        _RESET_STRING(_DEST, _SizeInBytes);
        _RETURN_DEST_NOT_NULL_TERMINATED(_DEST, _SizeInBytes);
    }


    if (available < _SizeInBytes)
    {
        /*
         * Dst may have terminated with an invalid MBCS, in that case we clear
         * the bogus lead byte.
         */
        fIsLeadPrefix = FALSE;
        _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST, &p[-1]);
        if (fIsLeadPrefix) {
            /* the original string ended with a lead byte: we remove it */
            p--;
            *p = 0;
            available++;
            fFoundInvalidMBC = TRUE;
        }
    }

    if (_COUNT == _TRUNCATE)
    {
        while ((*p++ = *_SRC++) != 0 && --available > 0)
        {
        }
    }
    else
    {
        _ASSERT_EXPR((!_CrtGetCheckCount() || _COUNT < available), L"Buffer is too small");

#if _COUNT_IN_BYTES
        while (_COUNT > 0 && (*p++ = *_SRC++) != 0 && --available > 0)
        {
            _COUNT--;
        }
#else  /* _COUNT_IN_BYTES */
        while (_COUNT > 0)
        {
            if (_ISMBBLEAD(*_SRC))
            {
                if (_SRC[1] == 0)
                {
                    /* the source string ended with a lead byte: we remove it */
                    *p = 0;
                    fFoundInvalidMBC = TRUE;
                    break;
                }
                if (available <= 2)
                {
                    /* not enough space */
                    available = 0;
                    break;
                }
                *p++ = *_SRC++;
                *p++ = *_SRC++;
                available -= 2;
            }
            else
            {
                if ((*p++ = *_SRC++) == 0 || --available == 0)
                {
                    break;
                }
            }
            _COUNT--;
        }
#endif  /* _COUNT_IN_BYTES */
        if (_COUNT == 0)
        {
            *p++ = 0;
        }
    }

    if (available == 0)
    {
#if _COUNT_IN_BYTES
        /*
         * defined(_COUNT_IN_BYTES) loop does not track mbc context,
         * so we must iterate backwards to discover character context.
         */
        if (*_SRC == 0 || _COUNT == 1)
        {
            _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST, &p[-1]);
            if (fIsLeadPrefix)
            {
                /* the source string ended with a lead byte: we remove it */
                p[-1] = 0;
                _RETURN_MBCS_ERROR;
            }
        }
#endif  /* _COUNT_IN_BYTES */

        /*
         * _COUNT == _TRUNCATE loop terminated because available became 0.
         * This means that we copied at least one character, and it wasn't
         * a null. If this last character acted as a lead then overwrite
         * it with null. Do not set the mbcs error in this case, due that the
         * user cannot predict this case and he/she's only asking for truncation.
         */
        if (_COUNT == _TRUNCATE)
        {
            if (fFoundInvalidMBC)
            {
                _SET_MBCS_ERROR;
            }

            if (_SizeInBytes > 1)
            {
                fIsLeadPrefix = FALSE;
                _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST, &_DEST[_SizeInBytes - 2]);
                if (fIsLeadPrefix)
                {
                    _DEST[_SizeInBytes - 2] = 0;
                    _FILL_BYTE(_DEST[_SizeInBytes - 1]);
                    _RETURN_TRUNCATE;
                }
            }

            _DEST[_SizeInBytes - 1] = 0;

            _RETURN_TRUNCATE;
        }
        _RESET_STRING(_DEST, _SizeInBytes);
        _RETURN_BUFFER_TOO_SMALL(_DEST, _SizeInBytes);
    }
#if _COUNT_IN_BYTES
    if (available < _SizeInBytes)
    {
        _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST, &p[-2]);
        if (fIsLeadPrefix)
        {
            /* the source string ended with a lead byte: we remove it */
            p[-2] = 0;
            available++;
            fFoundInvalidMBC = TRUE;
        }
    }
#endif  /* _COUNT_IN_BYTES */
    _FILL_STRING(_DEST, _SizeInBytes, _SizeInBytes - available + 1);

    if (fFoundInvalidMBC)
    {
        _RETURN_MBCS_ERROR;
    }

    _RETURN_NO_ERROR;
}

