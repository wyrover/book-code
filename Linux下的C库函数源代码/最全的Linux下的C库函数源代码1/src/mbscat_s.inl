/***
*mbscat_s.inl - implementation of _mbscat_s
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the algorithm for _mbscat_s.
*
****/

_FUNC_PROLOGUE
#if _USE_LOCALE_ARG
errno_t __cdecl _FUNC_NAME(unsigned char *_DEST, size_t _SizeInBytes, const unsigned char *_SRC, _LOCALE_ARG_DECL)
#else  /* _USE_LOCALE_ARG */
errno_t __cdecl _FUNC_NAME(unsigned char *_DEST, size_t _SizeInBytes, const unsigned char *_SRC)
#endif  /* _USE_LOCALE_ARG */
{
    unsigned char *p;
    size_t available;
    BOOL fFoundInvalidMBC, fIsLeadPrefix;

    /* validation section */
    _VALIDATE_STRING(_DEST, _SizeInBytes);
    _VALIDATE_POINTER_RESET_STRING(_SRC, _DEST, _SizeInBytes);

#if _USE_LOCALE_ARG
    _LOCALE_UPDATE;
    if (_LOCALE_SHORTCUT_TEST)
    {
        return _SB_FUNC_NAME((char *)_DEST, _SizeInBytes, (const char *)_SRC);
    }
#endif  /* _USE_LOCALE_ARG */

    fFoundInvalidMBC = FALSE;
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

    /*
     * Otherwise we have space available, p points at null that lies
     * within _SizeInBytes, so available > 0. Check if dst ended with
     * an invalid MBC (lead+null), if so then clear that lead byte,
     * move the pointer back one and increase available by one.
     */

    _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST, p-1);
    if (fIsLeadPrefix)
    {
        fFoundInvalidMBC = TRUE;
        p--;
        *p = 0;
        available++;
    }

    /* Append dst to src. */

    while ((*p++ = *_SRC++) != 0 && --available > 0)
    {
    }

    /*
     * We've run out of room in the destination before finding null in the src.
     * It could be that the src was terminated with an invalid mbc (lead+null).
     * In that case its ok to clear the copied lead byte and return mbcs_error.
     */

    if (available == 0)
    {
        if (*_SRC == 0)
        {
            _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST, p-1);
            if (fIsLeadPrefix)
            {
                /* the source string ended with a lead byte: we remove it */
                p[-1] = 0;
                _RETURN_MBCS_ERROR;
            }
        }
        _RESET_STRING(_DEST, _SizeInBytes);
        _RETURN_BUFFER_TOO_SMALL(_DEST, _SizeInBytes);
    }

    /*
     * If the src string ended with an invalid mbc (lead+null) then clear the
     * lead byte.
     */

    _ISMBBLEADPREFIX(fIsLeadPrefix, _DEST, p-2);
    if (fIsLeadPrefix)
    {
        p[-2] = 0;
        available++;
        fFoundInvalidMBC = TRUE;
    }

    _FILL_STRING(_DEST, _SizeInBytes, _SizeInBytes - available + 1);

    if (fFoundInvalidMBC)
    {
        _RETURN_MBCS_ERROR;
    }

    _RETURN_NO_ERROR;
}

