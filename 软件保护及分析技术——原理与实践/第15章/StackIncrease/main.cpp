#include <windows.h>
#pragma comment(lib,"Q:/ToolKit/WinLicense/WinlicenseSDK/Lib/COFF/WinlicenseSDK.lib")
#include "Q:/ToolKit/WinLicense/WinlicenseSDK/Include/C/winlicensesdk.h"
int main()
{
    DebugBreak();   //利用系统断点直接停到代码虚拟化入口
    VM_START    //虚拟代码开始标记
    __asm{  //这部分代码在加密后会被虚拟化
        mov eax,0
 loop_start:
        lea ebx,[eax+0x12340000]
        push ebx
        inc eax
        cmp eax,0x1000
        jnz loop_start
    }
    VM_END
    DebugBreak();
    return 0;
}

