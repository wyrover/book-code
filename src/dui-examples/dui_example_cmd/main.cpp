#include "duilib.h"
#include <tracetool/tracetool.h>
#include "CmdLineParser.h"

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
    CCmdLineParser parser(::GetCommandLine());

    if (!parser.HasKey(L"respath") || !parser.HasKey(L"layout")) {
        return -1;
    }

    ::CoInitialize(NULL);
    TRACE_CLEAR;
    TRACE_SEND_A("program init");
    std::wstring res_path = parser.GetVal(L"respath");
    std::wstring xml_layout_filename = parser.GetVal(L"layout");
    TRACE_SEND_W(res_path.c_str());
    TRACE_SEND_W(xml_layout_filename.c_str());
    CPaintManagerUI::SetInstance(hInstance);
    // 设置 xml 资源路径
    // xml 里图片文件路径是相对于 exe 的路径
#ifdef TRACE_TOOL
    //CPaintManagerUI::SetResourcePath(L"H:\\rover\\rover-self-work\\cpp\\book-code\\bin\\x86\\vs2015\\tools\\");
    CPaintManagerUI::SetResourcePath(res_path.c_str());
#else
    //CPaintManagerUI::SetResourceZip(L"H:\\rover\\rover-self-work\\cpp\\book-code\\bin\\x86\\vs2015\\tools\\example01.zip");
    CPaintManagerUI::SetResourceZip(GetAppDir() + L"example01.zip");
#endif
    //std::wstring layout_filename = L"example01/layout/Main_dlg.xml";
    std::wstring wnd_name = L"template:" + xml_layout_filename;
    SetConsoleTitle(wnd_name.c_str());
    CFrameWnd *pFrame = new CFrameWnd(xml_layout_filename.c_str());
    pFrame->Create(NULL, wnd_name.c_str(), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->ShowModal();
    delete pFrame;
    TRACE_FlUSH;
    ::CoUninitialize();
    return 0;
}
