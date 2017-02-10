#include "stdafx.h"

int 
__cdecl
wmain()
{
    BYTE* pMem = NULL;
    pMem[0] = 5;
    wprintf(L"Value at memory address is 0x%x\n", pMem[0]);
    return 0;
}

