#include <stdio.h>
#include <Windows.h>
#include "ScmDrvCtrl.h"

#pragma comment(lib,"user32.lib")


void GetAppPath(char *szCurFile) //最后带斜杠
{
    GetModuleFileNameA(0, szCurFile, MAX_PATH);

    for (SIZE_T i = strlen(szCurFile) - 1; i >= 0; i--) {
        if (szCurFile[i] == '\\') {
            szCurFile[i + 1] = '\0';
            break;
        }
    }
}

int main()
{
    BOOL b;
    cDrvCtrl dc;
    //设置驱动名称
    char szSysFile[MAX_PATH] = {0};
    char szSvcLnkName[] = "KrnlHW64";;
    GetAppPath(szSysFile);
    strcat(szSysFile, "KrnlHW64.sys");
    //安装并启动驱动
    b = dc.Install(szSysFile, szSvcLnkName, szSvcLnkName);
    b = dc.Start();
    printf("LoadDriver=%d\n", b);
    //“打开”驱动的符号链接
    dc.Open("\\\\.\\KrnlHW64");
    //使用控制码控制驱动（0x800：传入一个数字并返回一个数字）
    DWORD x = 100, y = 0, z = 0;
    dc.IoControl(0x800, &x, sizeof(x), &y, sizeof(y), &z);
    printf("INPUT=%ld\nOUTPUT=%ld\nReturnBytesLength=%ld\n", x, y, z);
    //使用控制码控制驱动（0x801：在DBGVIEW里显示HELLOWORLD）
    dc.IoControl(0x801, 0, 0, 0, 0, 0);
    //关闭符号链接句柄
    CloseHandle(dc.m_hDriver);
    //停止并卸载驱动
    b = dc.Stop();
    b = dc.Remove();
    printf("UnloadDriver=%d\n", b);
    getchar();
    return 0;
}