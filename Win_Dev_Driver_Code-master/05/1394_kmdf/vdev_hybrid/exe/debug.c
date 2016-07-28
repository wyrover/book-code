/*++

Copyright (c) 1998  Microsoft Corporation

Module Name: 

    debug.c
--*/

#define _DEBUG_C
#include "pch.h"
#undef _DEBUG_C

void 
DbgPrt(
    __in HANDLE   hWnd,
    __in __nullterminated PCHAR   lpszFormat,
    ... 
    )
{
    char    buf[STRING_SIZE]; // = "WIN1394: ";
    va_list ap;

    va_start(ap, lpszFormat);

//    wvsprintf( &buf[0], lpszFormat, ap );
    StringCbVPrintf( &buf[0] , (STRING_SIZE * sizeof(buf[0])) , lpszFormat, ap ); 
#if defined(DBG)
    OutputDebugStringA(buf);
#endif

    if (hWnd)
        WriteTextToEditControl(hWnd, buf);

    va_end(ap);
}



