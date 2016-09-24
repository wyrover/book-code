#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#include "../../../xHooklib/xhook_inline.h"
#include "../xdynamic_ollydbg.h"
#include "../resource.h"

#include "VirtualDebug.h"
#include "VirtualDebugApiLayer.h"

CPipeBase*	gPipeSend = NULL;
CPipeBase*	gPipeRecv = NULL;

void EnableVirtualDebug()   //启动伪调试技术
{
    if (gPipeSend)
        return;
    gPipeSend = new CPipeBase();    //初始化通信管道
    gPipeRecv = new CPipeBase();
    gstdbg.m_Invoked = false;
    InstallVirtualDebugLayer(); //安装调试函数模拟层
}


void DisableVirtualDebug()  //关闭伪调试技术
{
    UnInstallVirtualDebugLayer();   //卸载伪调试函数模拟层
    if (!gPipeSend)
        return;
    if (gPipeSend)      //关闭通信管道
    {
        gPipeSend->fpClose();
        delete gPipeSend;
        gPipeSend = 0;
    }
    if (gPipeRecv)
    {
        gPipeRecv->fpClose();
        delete gPipeRecv;
        gPipeRecv = 0;
    }
}
