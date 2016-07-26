/***
*tcstok_s.inl - implementation of _mbstok_s
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the algorithm for _mbstok_s.
*
****/

_FUNC_PROLOGUE
#if _USE_LOCALE_ARG
unsigned char * __cdecl _FUNC_NAME(unsigned char *_String, const unsigned char *_Control, unsigned char **_Context, _LOCALE_ARG_DECL)
#else  /* _USE_LOCALE_ARG */
unsigned char * __cdecl _FUNC_NAME(unsigned char *_String, const unsigned char *_Control, unsigned char **_Context)
#endif  /* _USE_LOCALE_ARG */
{
    unsigned char *token;
    const unsigned char *ctl;
    int dbc;

    /* validation section */
    _VALIDATE_POINTER_ERROR_RETURN(_Context, EINVAL, NULL);
    _VALIDATE_POINTER_ERROR_RETURN(_Control, EINVAL, NULL);
    _VALIDATE_CONDITION_ERROR_RETURN(_String != NULL || *_Context != NULL, EINVAL, NULL);

#if _USE_LOCALE_ARG
    _LOCALE_UPDATE;
    if (_LOCALE_SHORTCUT_TEST)
    {
        return (unsigned char*)_SB_FUNC_NAME((char *)_String, (const char *)_Control, (char **)_Context);
    }
#endif  /* _USE_LOCALE_ARG */

    /* If string==NULL, continue with previous string */
    if (!_String)
    {
        _String = *_Context;
    }

    /* Find beginning of token (skip over leading delimiters). Note that
    * there is no token iff this loop sets string to point to the terminal null. */
    for ( ; *_String != 0; _String++)
    {
        for (ctl = _Control; *ctl != 0; ctl++)
        {
            if (_ISMBBLEAD(*ctl))
            {
                if (ctl[1] == 0)
                {
                    ctl++;
                    _SET_MBCS_ERROR;
                    break;
                }
                if (*ctl == *_String && ctl[1] == _String[1])
                {
                    break;
                }
                ctl++;
            }
            else
            {
                if (*ctl == *_String)
                {
                    break;
                }
            }
        }
        if (*ctl == 0)
        {
            break;
        }
        if (_ISMBBLEAD(*_String))
        {
            _String++;
            if (*_String == 0)
            {
                _SET_MBCS_ERROR;
                break;
            }
        }
    }

    token = _String;

    /* Find the end of the token. If it is not the end of the string,
    * put a null there. */
    for ( ; *_String != 0; _String++)
    {
        for (ctl = _Control, dbc = 0; *ctl != 0; ctl++)
        {
            if (_ISMBBLEAD(*ctl))
            {
                if (ctl[1] == 0)
                {
                    ctl++;
                    break;
                }
                if (ctl[0] == _String[0] && ctl[1] == _String[1])
                {
                    dbc = 1;
                    break;
                }
                ctl++;
            }
            else
            {
                if (*ctl == *_String)
                {
                    break;
                }
            }
        }
        if (*ctl != 0)
        {
            *_String++ = 0;
            if (dbc)
            {
                *_String++ = 0;
            }
            break;
        }
        if (_ISMBBLEAD(_String[0]))
        {
            if (_String[1] == 0)
            {
                *_String = 0;
                break;
            }
            _String++;
        }
    }

    /* Update the context */
    *_Context = _String;

    /* Determine if a token has been found. */
    if (token == _String)
    {
        return NULL;
    }
    else
    {
        return token;
    }
}
