/***
*outputformat.c - formatting options used by output functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Option flags used by output functions (like printf).
*       As an example, the user can set the _TWO_DIGIT_EXPONENT flag
*       to force the output of floating point numbers to have a
*       two digits exponent (if possible).
*
*******************************************************************************/

#include <stdio.h>
#include <internal.h>

unsigned int _outputformat = 0;

/***
*unsigned int _set_output_format(unsigned int options) - Set the output format option flags.
*
*Purpose:
*       Set the output format option flags. The only valid value for now is _TWO_DIGIT_EXPONENT.
*
*Entry:
*       unsigned int options    option flags to be set
*
*Exit:
*       Returns the old value of the output format.
*       If the given set of options is invalid, the output format will not be set
*       and errno will contain EINVAL.
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
_CRTIMP unsigned int _set_output_format(unsigned int options)
{
    unsigned int old = _outputformat;

    /* validation section */
    _VALIDATE_RETURN((options & ~_TWO_DIGIT_EXPONENT) == 0, EINVAL, old);

    _outputformat = options;
    return old;
}

/***
*unsigned int _get_output_format() - Get the current output format option flags.
*
*Purpose:
*       Get the current output format option flags.
*
*Entry:
*
*Exit:
*       Returns the current value of the output format.
*
*Exceptions:
*
*******************************************************************************/
_CRTIMP unsigned int _get_output_format()
{
    return _outputformat;
}
