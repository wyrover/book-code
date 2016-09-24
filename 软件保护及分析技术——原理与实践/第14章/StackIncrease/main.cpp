#include <windows.h>
#pragma comment(lib,"Q:/ToolKit/VMProtect/Lib/COFF/VMProtectSDK32.lib")
#include "Q:/ToolKit/VMProtect/Examples/Visual C++/VMProtectSDK.h"
int main()
{
    DebugBreak();   //利用系统断点直接停到代码虚拟化入口
    VMProtectBeginVirtualization("VM Mark");    //VMProtect SDK来做标记
    __asm{  //这部分代码在加密后会被虚拟化
        mov eax,0
 loop_start:
        lea ebx,[eax+0x12340000]
        push ebx                //由于VMProtect是栈机，因此我们可以构建连续压栈的动作
        inc eax                 //来观察VMProtect如何退栈，并且我们将eax寄存器标记为
        cmp eax,0x1000          //比较特殊的值，这样我们在调试时容易区别
        jnz loop_start

    }
    VMProtectEnd();
    DebugBreak();
    return 0;
}

