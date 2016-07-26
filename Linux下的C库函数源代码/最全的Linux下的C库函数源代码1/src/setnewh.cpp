/***
*setnewh.cpp - defines C++ set_new_handler() routine
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines C++ set_new_handler() routine.
*
*       OBSOLETE - the conforming std::set_new_handler can be found in
*       stdhndlr.cpp.  This version remains for backwards-compatibility,
*       and can no longer be referenced using headers new or new.h.
*
*******************************************************************************/

#include <stddef.h>
#include <internal.h>
#include <cruntime.h>
#include <mtdll.h>
#include <process.h>
#include <dbgint.h>

#define set_new_handler set_new_handler_ignore
#include <new.h>
#undef set_new_handler

#define _ASSERT_OK
#include <assert.h>


/***
*new_handler set_new_handler - set the ANSI C++ new handler
*
*Purpose:
*       Set the ANSI C++ per-thread new handler.
*
*Entry:
*       Pointer to the new handler to be installed.
*
*       WARNING: set_new_handler is a stub function that is provided to
*       allow compilation of the Standard Template Library (STL).
*
*       Do NOT use it to register a new handler. Use _set_new_handler instead.
*
*       However, it can be called to remove the current handler:
*
*           set_new_handler(NULL); // calls _set_new_handler(NULL)
*
*Return:
*       Previous ANSI C++ new handler
*
*******************************************************************************/

new_handler __cdecl set_new_handler (
        new_handler new_p
        )
{
        // cannot use stub to register a new handler
        assert(new_p == 0);

        // remove current handler
        _set_new_handler(0);

        return 0;
}



