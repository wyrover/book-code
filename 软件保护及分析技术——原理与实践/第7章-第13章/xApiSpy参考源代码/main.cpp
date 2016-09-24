#include "xapispy.h"
#include <QApplication>
#include <QProcess>
#include "bookcover.h"

#define _ENABLE_BOOK_MODE

#ifdef _ENABLE_BOOK_MODE
#pragma comment(lib,"Q:/ToolKit/VMProtect/Lib/COFF/VMProtectSDK32.lib")
#include "Q:/ToolKit/VMProtect/Include/C/VMProtectSDK.h"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("xApiSpy");
    app.setApplicationName("xApiSpy");
#ifdef _ENABLE_BOOK_MODE
    VMProtectBeginUltra("VM Mark");    //VMProtect SDK来做标记
    BookCover bkcov;
#endif


    QSettings settings;
    QByteArray st = settings.value("winpos").toByteArray();
    xApiSpy win;
    if (!st.isEmpty())
        win.restoreState(st);
#ifdef _ENABLE_BOOK_MODE
    srand(GetTickCount());
    while(true)
    {
        int idx = rand() % 17;
        int idx2 = rand() % 13;
        wchar_t chaptitles[18][50] =
        {
            L"软件保护技术",
            L"软件保护系统",
            L"软件保护强化",
            L"软件破解工具",
            L"软件破解技术",
            L"软件分析技巧",
            L"打造函数监视器",
            L"打造资源重建工具",
            L"打造重定位修复工具",
            L"打造进程拍照机",
            L"打造函数通用追踪器",
            L"打造预执行调试器",
            L"打造伪调试器",
            L"VMPROTECT虚拟机分析",
            L"WINLICENSE虚拟机分析",
            L"VMPROTECT脱壳",
            L"WINLICENSE脱壳"
        };
        wchar_t chaptitles2[13][50] =
        {
            L"代码虚拟化原理",
            L"模拟虚拟化",
            L"自动化分析原理",
            L"花型替换分析",
            L"打造自动化分析工具",
            L"代码虚拟机结构分析",
            L"OP分支功能性分析",
            L"对比测试分析",
            L"实时监控分析",
            L"OP实时调试分析",
            L"Winlicense 2.3.2 分析",
            L"VMProtect分析",
            L"xVMDebug插件"
        };
            bkcov.setTip(idx+1);
            bkcov.setTip2(idx2+1);
        if (bkcov.exec() != QDialog::Accepted)
            return -1;
        int nbook =bkcov.getBookIndex();
        QString sText = bkcov.inputText(nbook).trimmed();
        if (sText.isEmpty())
            return -1;

        QString sComp = QString::fromWCharArray((nbook==0)?chaptitles[idx]:chaptitles2[idx2]);
        if (sComp.compare(sText,Qt::CaseInsensitive) == 0)
        {
            win.show();
            app.exec();
            break;
        }
    }
    VMProtectEnd();
#else
    win.show();
    app.exec();
#endif
    return 0;
}
