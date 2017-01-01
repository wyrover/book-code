
#include "stdafx.h"
#include "Test.h"
#include "BaseDialog.h"

using namespace DuiLib;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    //设置进程实例句柄
    CPaintManagerUI::SetInstance(hInstance);
    //设置资源路径
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + L"\\Skin5");
    //设置ZIP压缩资源 默认是当前路径下
    //研究从资源中加载皮肤文件
    //CPaintManagerUI::SetResourceZip(_T("skin.zip"));
    //初始化COM
    HRESULT Hr = ::CoInitialize(NULL);

    if (FAILED(Hr)) return 0;

    //创建一个对话框承载容器 用于渲染界面
    BaseDialog* pFrame = new BaseDialog();

    //失败返回
    if (pFrame == NULL) return 0;

    //创建一个3D边框 在任务栏可见的顶层窗口 WS_OVERLAPPEDWINDOW
    pFrame->Create(NULL, _T("T9000"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW);
    //窗口CENTER
    pFrame->CenterWindow();
    //显示显示窗口
    pFrame->ShowWindow(true);
    //开始消息循环
    CPaintManagerUI::MessageLoop();
    //释放COM
    ::CoUninitialize();
    return 0;
}
