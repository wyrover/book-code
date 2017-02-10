#include "stdafx.h"
#include "reftracker.h"

//
// A sample ref-counted object
//
class CRefCountObject : 
    public CIUnknownImpl
{
public:
    ~CRefCountObject(
        VOID
        )
    {
        wprintf(L"Object 0x%p destroyed\n", this);
    }

    HRESULT
    Init(
        VOID
        )
    {
        ChkProlog();

        // TODO: add object initialization code here

        ChkNoCleanup();
    }
};

class CMainApp
{
public:
    static
    HRESULT
    MainHR(
        )
    {
        CComPtr<CRefCountObject> spRefObj;
        CRefCountObject* pRefObj;

        ChkProlog();

        spRefObj.Attach(new CRefCountObject());
        ChkAlloc(spRefObj);

        //
        // Take one explicit reference (for illustration purposes) - 
        // BUG: this reference will be leaked!
        //
        pRefObj = static_cast<CRefCountObject*>(spRefObj);
        pRefObj->AddRef();

        ChkNoCleanup();
    }
};

int
__cdecl
wmain(
    )
{
    HRESULT hr;

    //
    // Initialize the global stack trace database
    //
    CRefCountDatabase_Init();

    hr = CMainApp::MainHR();

    //
    // Look for leaked references before the process exits
    //
    CRefCountDatabase_DetectLeaks();

    if (FAILED(hr))
    {
        wprintf(L"\n");
    }
    else
    {
        wprintf(L"Success.\n");
    }
    return 0;
}
