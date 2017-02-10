#include "stdafx.h"

WCHAR* g_pwszMessage = L"Hello World";

int 
__cdecl
wmain()
{
    // BUG! writing data to read-only memory.
    g_pwszMessage[0] = L'a';
    return 0;
}

