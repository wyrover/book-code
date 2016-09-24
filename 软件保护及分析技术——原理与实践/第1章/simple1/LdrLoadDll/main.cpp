#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main(void)
{
    //调用中断函数，直接定位到启动函数位置
    DebugBreak();
    LoadLibrary(_T("c:\\windows\\system32\\shell32.dll"));
    return 0;
}

