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