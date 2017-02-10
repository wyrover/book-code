#include "stdafx.h"

int 
__cdecl
wmain()
{
    LoadLibraryW(L"advapi32.dll");
    wprintf(L"Success");
}
