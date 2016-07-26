/***
*muser.cpp - user handlers for managed scenarios
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Handling functions only called by the client programs,
*       not by the C/C++ runtime itself.
*
*******************************************************************************/

#include <stddef.h>
#include <windows.h>
#include <mtdll.h>
#include <stdlib.h>
#include <eh.h>
#include <internal.h>
#include <crtdbg.h>

#pragma warning(disable:4439)   // C4439: function with a managed parameter must have a __clrcall calling convention
#pragma warning(disable:4483)   // disable warning/error about __identifier(<string>)

// Global for storing the __clrcall invalid parameter handler
volatile __declspec(appdomain) _invalid_parameter_handler_m __pParameter_m = (_invalid_parameter_handler_m) _encoded_null();

/***
*__invalid_parameter_handler_thunk
*
*Purpose:
*       This function is a __cdecl function that is called by the
*       CRT when an invalid parameter is encountered. It , in turn, calls
*       the __clrcall handler registered by the user.
*
*******************************************************************************/
void __cdecl __invalid_parameter_handler_thunk(
    const wchar_t* pszExpression,
    const wchar_t* pszFunction,
    const wchar_t* pszFile,
    unsigned int nLine,
    uintptr_t pReserved
)
{
    // Put the value in a local variable to avoid MT synchronization
    // problems using the global.
    _invalid_parameter_handler_m function = (_invalid_parameter_handler_m) _decode_pointer(__pParameter_m);

    if (function)
    {
        function(pszExpression, pszFunction, pszFile, nLine, pReserved);
    }
}

static void __clrcall __invalid_parameter_handler_cleanup(void)
{
    _invalid_parameter_handler handler=_get_invalid_parameter_handler();
    if(handler==__invalid_parameter_handler_thunk && __pParameter_m!=NULL)
    {
        _set_invalid_parameter_handler((_invalid_parameter_handler)NULL);
    }
    __pParameter_m=NULL;
}

/***
*_set_invalid_parameter_handler
*
*Purpose:
*       Overload of _set_invalid_parameter_handler to be used when caller
*       wants to register a __clrcall invalid parameter handler.
*
*******************************************************************************/
_MRTIMP _invalid_parameter_handler_m __cdecl
_set_invalid_parameter_handler( _invalid_parameter_handler_m pNew )
{
    if(_atexit_m_appdomain(__invalid_parameter_handler_cleanup)!=0)
    {
        return NULL;
    }

    _invalid_parameter_handler_m pOld = (_invalid_parameter_handler_m) _decode_pointer(__pParameter_m);

    _set_invalid_parameter_handler(pNew != NULL ? __invalid_parameter_handler_thunk : static_cast<_invalid_parameter_handler>(NULL));

    __pParameter_m = (_invalid_parameter_handler_m) _encode_pointer(pNew);

    return pOld;
}

namespace __identifier("<CrtImplementationDetails>")
{

class Handlers
{
public:
        [System::ThreadStaticAttribute] __declspec(appdomain) static __terminate_function_m __pTerminate_m;
        [System::ThreadStaticAttribute] __declspec(appdomain) static __unexpected_function_m __pUnexpected_m;
        [System::ThreadStaticAttribute] __declspec(appdomain) static _purecall_handler_m __pPurecall_m;
};

__declspec(appdomain) __terminate_function_m Handlers::__pTerminate_m = (__terminate_function_m) _encoded_null();
__declspec(appdomain) __unexpected_function_m Handlers::__pUnexpected_m = (__unexpected_function_m) _encoded_null();
__declspec(appdomain) _purecall_handler_m Handlers::__pPurecall_m = (_purecall_handler_m) _encoded_null();
}

using namespace __identifier("<CrtImplementationDetails>");

#pragma warning(disable:4535)
/***
*__terminate_thunk - calls __clrcall terminate handler
*
*Purpose:
*       Act as a thunk between __cdecl terminate handler and __clrcall
*       terminate handler.
*
*******************************************************************************/
void __cdecl __terminate_thunk()
{
        __terminate_function_m function= (__terminate_function_m) _decode_pointer(Handlers::__pTerminate_m);
    if (function)
    {
        (*Handlers::__pTerminate_m)();
    }
}

static void __clrcall __terminate_cleanup()
{
    terminate_function handler=_get_terminate();
    __terminate_function_m enull = (__terminate_function_m) _encoded_null();
    if(handler==__terminate_thunk && Handlers::__pTerminate_m!=enull)
    {
        set_terminate((terminate_function)NULL);
    }
    Handlers::__pTerminate_m=enull;
}


/***
*__set_terminate_m - sets __clrcall terminate handler
*
*Purpose:
*       Call the appropriate new handler.
*
*Entry:
*       None
*
*Return:
*
*******************************************************************************/
_MRTIMP __terminate_function_m __cdecl
set_terminate( __terminate_function_m pNew )
{
    if(_atexit_m_appdomain(__terminate_cleanup)!=0)
    {
        return NULL;
    }

    __terminate_function_m pOld = (__terminate_function_m) _encoded_null();

#if defined (_DEBUG)
#pragma warning(push)
#pragma warning(disable:4191)

    if ( (pNew != NULL) )

#pragma warning(pop)
#endif  /* defined (_DEBUG) */
    {
        pOld = (__terminate_function_m) _decode_pointer(Handlers::__pTerminate_m);
        Handlers::__pTerminate_m = (__terminate_function_m) _encode_pointer(pNew);
        set_terminate(pNew==NULL ? (terminate_function)NULL : __terminate_thunk);
    }

    return pOld;
}

/***
*__purecall_thunk - calls __clrcall purecall handler
*
*Purpose:
*       Act as a thunk between __cdecl purecall handler and __clrcall
*       purecall handler.
*
*******************************************************************************/
static void __cdecl __purecall_thunk()
{
    _purecall_handler_m handler= (_purecall_handler_m) _decode_pointer(Handlers::__pPurecall_m);

    if (handler)
    {
        (*handler)();
    }
}

static void __clrcall __purecall_cleanup()
{
    _purecall_handler handler=_get_purecall_handler();
    _purecall_handler_m enull = (_purecall_handler_m) _encoded_null();
    if(handler==__purecall_thunk && Handlers::__pPurecall_m!=enull)
    {
        _set_purecall_handler((_purecall_handler)NULL);
    }
    Handlers::__pPurecall_m= enull;
}

/***
*_set_purecall_handler_m - sets __clrcall purecall handler
*
*Purpose:
*       set the appropriate purecall handler.
*
*Entry:
*       None
*
*Return:
*
*******************************************************************************/
_MRTIMP _purecall_handler_m __cdecl
_set_purecall_handler( _purecall_handler_m pNew )
{
    if(_atexit_m_appdomain(__purecall_cleanup)!=0)
    {
        return NULL;
    }

    _purecall_handler_m pOld = (_purecall_handler_m) _encoded_null();

#if defined (_DEBUG)
#pragma warning(push)
#pragma warning(disable:4191)

    if ( (pNew != NULL) )

#pragma warning(pop)
#endif  /* defined (_DEBUG) */
    {
        pOld = (_purecall_handler_m) _decode_pointer(Handlers::__pPurecall_m);
        Handlers::__pPurecall_m = (_purecall_handler_m) _encode_pointer(pNew);
        _set_purecall_handler(pNew==NULL ? (_purecall_handler)NULL : __purecall_thunk);
    }

    return pOld;
}


/***
*__unexpected_thunk - calls __clrcall terminate handler
*
*Purpose:
*       Act as a thunk between __cdecl terminate handler and __clrcall
*       terminate handler.
*
*******************************************************************************/
void __cdecl __unexpected_thunk()
{
    __unexpected_function_m function= (__unexpected_function_m) _decode_pointer(Handlers::__pUnexpected_m);
    if (function)
    {
        (*function)();
    }
}

static void __clrcall __unexpected_cleanup()
{
    unexpected_function handler=_get_unexpected();
    __unexpected_function_m enull = (__unexpected_function_m) _encoded_null();
    if(handler==__unexpected_thunk && Handlers::__pUnexpected_m!=NULL)
    {
        set_unexpected((unexpected_function)NULL);
    }
    Handlers::__pUnexpected_m=enull;
}

/***
*__set_unexpected_m - sets __clrcall terminate handler
*
*Purpose:
*       __clrcall version of set_unexpected
*
*Entry:
*       None
*
*Return:
*       Previous registered Handler.
*
*******************************************************************************/
_MRTIMP __unexpected_function_m __cdecl
set_unexpected( __unexpected_function_m pNew )
{
    if(_atexit_m_appdomain(__unexpected_cleanup)!=0)
    {
        return NULL;
    }

    __unexpected_function_m pOld = (__unexpected_function_m) _encoded_null();

#if defined (_DEBUG)
#pragma warning(push)
#pragma warning(disable:4191)

    if ( (pNew != NULL) )

#pragma warning(pop)
#endif  /* defined (_DEBUG) */
    {
        pOld = (__unexpected_function_m) _decode_pointer(Handlers::__pUnexpected_m);
        Handlers::__pUnexpected_m = (__unexpected_function_m) _encode_pointer(pNew);
        set_unexpected(pNew==NULL ? (unexpected_function)NULL : __unexpected_thunk);
    }

    return pOld;
}

/***
*_set_invalid_parameter_handler
*
*Purpose:
*       Overload of _set_invalid_parameter_handler to be used when caller
*       wants to pass NULL or 0.
*
*******************************************************************************/
_MRTIMP _invalid_parameter_handler __cdecl
_set_invalid_parameter_handler( int pNew )
{
    _VALIDATE_RETURN(pNew == NULL, EINVAL, NULL);

    return _set_invalid_parameter_handler(static_cast<_invalid_parameter_handler>(0));
}

/***
*void _set_purecall_handler(void) -
*
*Purpose:
*       Establish a handler to be called when a pure virtual is called
*       Note that if you link to the crt statically, and replace
*       _purecall, then none of this will happen.
*
*       This function is not thread-safe
*
*Entry:
*       New handler
*
*Exit:
*       Old handler
*
*Exceptions:
*
*******************************************************************************/

_MRTIMP _purecall_handler __cdecl
_set_purecall_handler( int pNew )
{
    _VALIDATE_RETURN(pNew == NULL, EINVAL, NULL);

    return _set_purecall_handler(static_cast<_purecall_handler>(NULL));
}


using namespace System::Reflection;

[assembly: AssemblyTitle("msvcm80")];
[assembly: AssemblyDescription("Microsoft Visual C++ Managed Runtime 8.0")];
[assembly: AssemblyConfiguration("")];
[assembly: AssemblyCompany("Microsoft")];
[assembly: AssemblyProduct("Visual Studio 8.0")];
[assembly: AssemblyCopyright("Microsoft Corporation")];
[assembly: AssemblyTrademark("")];
[assembly: AssemblyCulture("")];

[assembly: AssemblyVersion(_CRT_ASSEMBLY_VERSION)];

[assembly: AssemblyDelaySign(true)];
[assembly: AssemblyKeyName("")];
// the AssemblyKeyFile is added in the linking phase
