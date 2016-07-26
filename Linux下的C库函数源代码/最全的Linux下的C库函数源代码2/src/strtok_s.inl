/***
*tcstok_s.inl - implementation of strtok_s
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the algorithm for strtok_s.
*
****/

_FUNC_PROLOGUE
char * __cdecl _FUNC_NAME(char *_String, const char *_Control, char **_Context)
{
    unsigned char *str;
    const unsigned char *ctl = _Control;
    unsigned char map[32];
    int count;

    /* validation section */
    _VALIDATE_POINTER_ERROR_RETURN(_Context, EINVAL, NULL);
    _VALIDATE_POINTER_ERROR_RETURN(_Control, EINVAL, NULL);
    _VALIDATE_CONDITION_ERROR_RETURN(_String != NULL || *_Context != NULL, EINVAL, NULL);

    /* Clear control map */
    for (count = 0; count < 32; count++)
    {
        map[count] = 0;
    }

    /* Set bits in delimiter table */
    do {
        map[*ctl >> 3] |= (1 << (*ctl & 7));
    } while (*ctl++);

    /* If string is NULL, set str to the saved
    * pointer (i.e., continue breaking tokens out of the string
    * from the last strtok call) */
    if (_String != NULL)
    {
        str = _String;
    }
    else
    {
        str = *_Context;
    }

    /* Find beginning of token (skip over leading delimiters). Note that
    * there is no token iff this loop sets str to point to the terminal
    * null (*str == 0) */
    while ((map[*str >> 3] & (1 << (*str & 7))) && *str != 0)
    {
        str++;
    }

    _String = str;

    /* Find the end of the token. If it is not the end of the string,
    * put a null there. */
    for ( ; *str != 0 ; str++ )
    {
        if (map[*str >> 3] & (1 << (*str & 7)))
        {
            *str++ = 0;
            break;
        }
    }

    /* Update context */
    *_Context = str;

    /* Determine if a token has been found. */
    if (_String == str)
    {
        return NULL;
    }
    else
    {
        return _String;
    }
}
