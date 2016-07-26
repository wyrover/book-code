/***
*tcsset_s.inl - general implementation of _tcsset_s
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the general algorithm for _strset_s and its variants.
*
****/

_FUNC_PROLOGUE
errno_t __cdecl _FUNC_NAME(__inout_ecount_z(_SIZE) _CHAR *_DEST, __in size_t _SIZE, __in _CHAR_INT _Value)
{
    _CHAR *p;
    size_t available;

    /* validation section */
    _VALIDATE_STRING(_DEST, _SIZE);

    p = _DEST;
    available = _SIZE;
    while (*p != 0 && --available > 0)
    {
        *p++ = (_CHAR)_Value;
    }

    if (available == 0)
    {
        _RESET_STRING(_DEST, _SIZE);
        _RETURN_DEST_NOT_NULL_TERMINATED(_DEST, _SIZE);
    }
    _FILL_STRING(_DEST, _SIZE, _SIZE - available + 1);
    _RETURN_NO_ERROR;
}

