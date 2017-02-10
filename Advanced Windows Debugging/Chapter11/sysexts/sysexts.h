/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#pragma once 
#include <windows.h>

#define KDEXT_64BIT
#define DBG_COMMAND_EXCEPTION            ((DWORD   )0x40010009L)
#include <wdbgexts.h>
#include <dbgeng.h>

WINDBG_EXTENSION_APIS64 ExtensionApis;