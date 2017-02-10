#include <windows.h>
#include <stdio.h>
#include <conio.h>

void __cdecl wmain (int argc, wchar_t* pArgs[])
{
    wprintf(L"Waiting for handles...\n");
    wprintf(L"Press any key to exit application...\n");
    _getch();
}

