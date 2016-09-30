#include <Windows.h>
#include <tchar.h>
#include <iostream>

struct IXyz
{
    virtual int Foo(int n) = 0;
    virtual void Release() = 0;
};

extern "C" IXyz* WINAPI GetXyz();

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
         
    //_tsetlocale(LC_ALL, _T(""));
    //std::wcout.imbue(std::locale(""));

    setlocale(LC_ALL, "");
    std::ios_base::sync_with_stdio(false); // 缺少的话，wcout wchar_t 会漏掉中文
    std::wcin.imbue(std::locale(""));
    std::wcout.imbue(std::locale(""));

    // 1. COM-like usage.
    IXyz* pXyz = GetXyz();

    if(pXyz)
    {
        int retval = pXyz->Foo(42);
        std::cout << retval << std::endl;
        pXyz->Release();
    }

    return 0;
}