//
// Chapter7_CallbackClient.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

// Include ATL
#include <atlbase.h>
CComModule _Module;
#include <atlcom.h>
#include <atlimpl.cpp>

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

#include <initguid.h>
#include "..\Chapter7_CallBackServer\Chapter7_CallBackServer_i.c"
#include "..\Chapter7_CallBackServer\Chapter7_CallBackServer.h"

void DisplayMessage(char* szMsg)
{
    MessageBox(0, szMsg, "Chapter7_CallbackClient", MB_OK);
}

void HandleError(char*szMsg, HRESULT hr)
{
    char szMessage[128];
    sprintf(szMessage, "%s. HR = %x", szMsg, hr);
    DisplayMessage(szMessage);
    CoUninitialize();
}

class CCallBack :
    public ICallBack,
    public CComObjectRoot
{

public:
    CCallBack() {}

    BEGIN_COM_MAP(CCallBack)
    COM_INTERFACE_ENTRY(ICallBack)
    END_COM_MAP()

// ICallback
public:
    STDMETHODIMP ComputationComplete(long lResult)
    {
        char szMsg[128];
        sprintf(szMsg, "The result is %d", lResult);
        DisplayMessage(szMsg);
        return S_OK;
    }
};

int main(int argc, char *argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

    // Initialize the ATL module
    _Module.Init(ObjectMap, 0);
    CComPtr<IMath> ptrMath;
    HRESULT hr;
    hr = CoCreateInstance(CLSID_Math,
                          NULL,
                          CLSCTX_SERVER,
                          IID_IMath,
                          (void**) &ptrMath);

    if (FAILED(hr)) {
        HandleError("Failed to create server instance", hr);
        return -1;
    }

    CComObject<CCallBack>* ptrCallBack;
    CComObject<CCallBack>::CreateInstance(&ptrCallBack);
    CComPtr<IUnknown> ptrEventsUnk = ptrCallBack;
    // Set up the connection
    ptrMath->Advise(ptrCallBack);
    // Use the IMath functionality
    ptrMath->Add(300, 10);
    ptrMath->Subtract(300, 10);
    ptrMath->Multiply(300, 10);
    ptrMath->Divide(300, 10);
    // Shut down the event connection
    ptrMath->UnAdvise();

    // Release all of our interfaces
    if (ptrMath)
        ptrMath = 0;

    if (ptrEventsUnk)
        ptrEventsUnk = 0;

    CoUninitialize();
    return 0;
}
