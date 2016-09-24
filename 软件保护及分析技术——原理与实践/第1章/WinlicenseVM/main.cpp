#include <windows.h>
#include <iostream>

using namespace std;

#include "Q:/ToolKit/WinLicense/WinlicenseSDK/Include/C/winlicensesdk.h"

int main()
{
    DebugBreak();
    VM_START
    __asm jmp $
    cout << "Hello World!" << endl;
    VM_END
    getchar();
    return 0;
}

