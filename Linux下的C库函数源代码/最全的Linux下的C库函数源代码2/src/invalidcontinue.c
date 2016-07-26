/***
* invalidcontinue.c - Set the invalid parameter handler to an empty function.
*
*   Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*
*******************************************************************************/
#include <stdlib.h>
#include <sect_attribs.h>
#include <internal.h>

int __set_emptyinvalidparamhandler(void);

void __empty_invalid_parameter_handler(
    const wchar_t *pszExpression,
    const wchar_t *pszFunction,
    const wchar_t *pszFile,
    unsigned int nLine,
    uintptr_t pReserved
    )
{
    (pszExpression);
    (pszFunction);
    (pszFile);
    (nLine);
    (pReserved);
    return;
}

int __set_emptyinvalidparamhandler(void)
{
    _set_invalid_parameter_handler(__empty_invalid_parameter_handler);
    return 0;
}

#ifndef _M_CEE_PURE
_CRTALLOC(".CRT$XID") static _PIFV pinit = __set_emptyinvalidparamhandler;
#else  /* _M_CEE_PURE */
#pragma warning(disable:4074)
class __set_emptyinvalidparamhandler_class
{
public:
        __set_emptyinvalidparamhandler_class() { __set_emptyinvalidparamhandler(); }
};
#pragma init_seg(compiler)
static __set_emptyinvalidparamhandler_class __set_emptyinvalidparamhandler_instance;
#endif  /* _M_CEE_PURE */
