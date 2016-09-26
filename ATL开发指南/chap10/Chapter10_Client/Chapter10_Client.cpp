//
// Chapter10_Client.cpp
//

#include <windows.h>

// Include ATL
#include <atlbase.h>
CComModule _Module;
#include <atlcom.h>
#include <atlimpl.cpp>

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

#include "..\Chapter10_Server\Chapter10_Server.h"
#include "..\Chapter10_Server\Chapter10_Server_i.c"

void DisplayMessage(char* szMsg)
{
    MessageBox(0, szMsg, "Chapter10_Client", MB_OK | MB_TOPMOST);
}

void HandleError(char*szMsg, HRESULT hr)
{
    char szMessage[128];
    wsprintf(szMessage, "%s. HR = %x", szMsg, hr);
    DisplayMessage(szMessage);
    CoUninitialize();
}

class CCallBack :
    public CComObjectRoot,
    ICallBack
{

public:
    CCallBack() {}

    BEGIN_COM_MAP(CCallBack)
    COM_INTERFACE_ENTRY(ICallBack)
    END_COM_MAP()

// ICallBack
public:
    STDMETHODIMP ComputationComplete(long lResult)
    {
        char szMsg[128];
        wsprintf(szMsg, "The result is %d", lResult);
        DisplayMessage(szMsg);

        // Terminate the application when we
        // get the result of the last computation
        if (lResult == 3000)
            PostQuitMessage(0);

        return S_OK;
    }

    STDMETHODIMP Error(BSTR bstrMessage)
    {
        USES_CONVERSION;
        DisplayMessage(W2A(bstrMessage));
        return S_OK;
    }
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    CoInitializeEx(0, COINIT_APARTMENTTHREADED);
    // Initialize the ATL module
    _Module.Init(ObjectMap, hInst);
    // Create an instance of our math component
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

    // Create an instance of our CallBack component
    CComObject<CCallBack>* pCallBack;
    CComObject<CCallBack>::CreateInstance(&pCallBack);
    // QueryInterface for ICallBack and pass
    // it to the component
    CComPtr<ICallBack> ptrCallBack;
    pCallBack->GetUnknown()->QueryInterface(IID_ICallBack, (void**) &ptrCallBack);
    ptrMath->Advise(ptrCallBack);
    // Access the IMath functionality
    // As the computations complete, the callback
    // implementation will display a message box
    ptrMath->Add(300, 10);
    ptrMath->Subtract(300, 10);
    ptrMath->Divide(300, 10);
    ptrMath->Multiply(300, 10);
    // Sit in a message loop until the
    // last notification is fired
    MSG   msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Shut down the connection
    ptrMath->Unadvise();

    // Release our interfaces
    if (ptrMath)
        ptrMath = 0;

    if (ptrCallBack)
        ptrCallBack = 0;

    CoUninitialize();
    return 0;
}
