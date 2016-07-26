/***
*mbsncpy_s.inl - general implementation of _mbsncpy_s and _mbsnbcpy_s
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the general algorithm for _mbsncpy_s and _mbsnbcpy_s.
*
*       _COUNT_IN_BYTES defined to 1 implements _mbsnbcpy_s
*       _COUNT_IN_BYTES defined to 0 implements _mbsncpy_s
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
    BOOL fFoundInvalidMBC;
    BOOL fIsLeadPrefix;

    fFoundInvalidMBC = FALSE;

    if (_COUNT == 0 && _DEST == NULL && _SizeInBytes == 0)
    {
        /* this case is allowed; nothing to do */
        _RETURN_NO_ERROR;
    }

    /* validation section */
    _VALIDATE_STRING(_DEST, _SizeInBytes);
    if (_COUNT == 0)
    {
        /* notice that the source string pointer can be NULL in this case */
        _RESET_STRING(_DEST, _SizeInBytes);
        _RETURN_NO_ERROR;
    }
    _VALIDATE_POINTER_RESET_STRING(_SRC, _DEST, _SizeInBytes);

#if _USE_LOCALE_ARG
    _LOCALE_UPDATE;
    if (_LOCALE_SHORTCUT_TEST)
    {
        return _SB_FUNC_NAME((char *)_DEST, _SizeInBytes, (const char *)_SRC, _COUNT);
    }
#endif  /* _USE_LOCALE_ARG */

    p = _DEST;
    available = _SizeInBytes;
    if (_COUNT == _TRUNCATE)
    {
        while ((*p++ = *_SRC++) != 0 && --available > 0)
        {
        }

        /*
         * loop terminates with either:
         * - src, p pointing 1 byte past null, avail includes the null
         * - available == 0, p points 1 past end of dst buffer
         */
    }
    else
    {
        _ASSERT_EXPR((!_CrtGetCheckCount() || _COUNT < _SizeInBytes), L"Buffer is too small");

#if _COUNT_IN_BYTES
        while ((*p++ = *_SRC++) != 0 && --available > 0 && --_COUNT > 0)
        {
        }

        /*
         * loop terminates with either:
         * - p points 1 byte past null, avail includes null, count includes null
         * - available == 0, p points 1 past end of dst buffer (inaccessible)
         * - count == 0, p points 1 past last written byte, space available in dst buffer
         *
         * always p[-1] is written.
         * sometimes p[-1] is null.
         */
#else  /* _COUNT_IN_BYTES */

        /* at this point, avail count be 1. */

        /* Need to track lead-byte context in order to track character count. */
        do
        {
            if (_ISMBBLEAD(*_SRC))
            {
                if (_SRC[1] == 0)
                {
                    /*
                     * Invalid MBC, write null to dst string, we are finished
                     * copying. We know that available is >= 1, so there is
                     * room for the null termination. If we decrement available
                     * then we will incorrectly report BUFFER_TOO_SMALL.
                     */

                    *p++ = 0;
                    fFoundInvalidMBC = TRUE;
                    break;
                }
                if (available <= 2)
                {
                    /* not enough space for a dbc and null */
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
        }
        while (--_COUNT > 0);
#endif  /* _COUNT_IN_BYTES */

        /* If count == 0 then at least one byte was copied and available is still > 0 */
        if (_COUNT == 0)
        {
            *p++ = 0;
            /* Note that available is not decremented here. */
        }
    }

    if (available == 0)
    {
#if _COUNT_IN_BYTES
        /*
         * For COUNT_IN_BYTES, the above loop copied at least one byte so src,p point
         * past a written byte.
         */

        if (*_SRC == 0 || _COUNT == 1)
        {
            fIsLeadPrefix = FALSE;
            _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST, &p[-1]);
            if (fIsLeadPrefix)
            {
                /* the source string ended with a lead byte: we remove it */
                p[-1] = 0;
                _RETURN_MBCS_ERROR;
            }
        }
#endif  /* _COUNT_IN_BYTES */

        if (_COUNT == _TRUNCATE)
        {
            if (fFoundInvalidMBC)
            {
                _SET_MBCS_ERROR;
            }

            if (_SizeInBytes > 1)
            {
                fIsLeadPrefix = FALSE;
                /* Check if 2nd to last copied byte acted as a lead.
                 * Do not set mbcs error because we are truncating.
                 */
                _ISMBBLEADPREFIX(fIsLeadPrefix,_DEST,&_DEST[_SizeInBytes - 2]);
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
    /*
     * COUNT_IN_BYTES copy loop doesn't track lead-byte context, so can't detect
     * invalid mbc. Detect them here.

     * available < _SizeInBytes means that at least one byte was copied so p is >= &dstBuffer[1]
     */

    if ((p - _DEST) >= 2)
    {
        _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST,&p[-2]);
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

