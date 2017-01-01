#include "duilib.h"
#include <tracetool/tracetool.h>

typedef wchar_t char16;
typedef std::wstring string16;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

const char16* GetAllPathDelimiters()
{
    return L"\\/";
}

string16 ExtractFilePath(const string16& FileName)
{
    string16 Result = FileName;
    return Result.erase(Result.find_last_of(GetAllPathDelimiters()) + 1);
}

string16 GetAppFileName()
{
    char16 filename[MAX_PATH + 10] = { 0 };
    GetModuleFileNameW((HMODULE)&__ImageBase, filename, MAX_PATH);
    return string16(filename);
}

string16 GetAppDir()
{
    return ExtractFilePath(GetAppFileName());
}




int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    ::CoInitialize(NULL);
    TRACE_CLEAR;
    TRACE_SEND_A("program init");
    CPaintManagerUI::SetInstance(hInstance);
    // 设置 xml 资源路径
    // xml 里图片文件路径是相对于 exe 的路径
#ifdef TRACE_TOOL
    //CPaintManagerUI::SetResourcePath(L"H:\\rover\\rover-self-work\\cpp\\book-code\\bin\\x86\\vs2015\\tools\\");
    CPaintManagerUI::SetResourcePath(GetAppDir().c_str());
#else
    //CPaintManagerUI::SetResourceZip(L"H:\\rover\\rover-self-work\\cpp\\book-code\\bin\\x86\\vs2015\\tools\\example01.zip");
    CPaintManagerUI::SetResourceZip(GetAppDir() + L"example01.zip");
#endif
    std::wstring layout_filename = L"example01/layout/Main_dlg.xml";
    CFrameWnd *pFrame = new CFrameWnd(layout_filename.c_str());
    pFrame->Create(NULL, _T("Redrain菜单Demo"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->ShowModal();
    delete pFrame;
    TRACE_FlUSH;
    ::CoUninitialize();
    return 0;
}
