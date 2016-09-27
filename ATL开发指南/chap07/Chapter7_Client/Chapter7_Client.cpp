//
// Chapter7_Client.cpp
//

#include <windows.h>

// Include ATL
#include <atlbase.h>
CComModule _Module;
#include <atlcom.h>
#include <atlimpl.cpp>

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

#include "..\Chapter7_CPServer\Chapter7_CPServer.h"
#include "..\Chapter7_CPServer\Chapter7_CPServer_i.c"

class COMModule
{
public:
    COMModule()
    {
        CoInitialize(0);
    }
    ~COMModule()
    {
        CoUninitialize();
    }
};

COMModule gModule;

void DisplayMessage(char* szMsg)
{
    MessageBox(0, szMsg, "Chapter7_Client", MB_OK);
}

void HandleError(char*szMsg, HRESULT hr)
{
    char szMessage[128];
    sprintf(szMessage, "%s. HR = %x", szMsg, hr);
    DisplayMessage(szMessage);
}


class CMathEvents :
    public CComObjectRoot,
    public _IMathEvents
{
public:
    CMathEvents()
    {
    }

    BEGIN_COM_MAP(CMathEvents)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(_IMathEvents)
    END_COM_MAP()

// IMathEvents
public:
    STDMETHODIMP GetTypeInfoCount(UINT*)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP GetTypeInfo(UINT iTInfo,
                             LCID lcid,
                             ITypeInfo **ppTInfo)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP GetIDsOfNames(REFIID riid,
                               LPOLESTR *rgszNames,
                               UINT cNames,
                               LCID lcid,
                               DISPID *rgDispId)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP Invoke(DISPID dispIdMember,
                        REFIID riid,
                        LCID lcid,
                        WORD wFlags,
                        DISPPARAMS *pDispParams,
                        VARIANT *pVarResult,
                        EXCEPINFO *pExcepInfo,
                        UINT *puArgErr)
    {
        switch (dispIdMember) {
        case 0x1:

            // Make sure the is just one argument
            if (pDispParams->cArgs != 1)
                return DISP_E_BADPARAMCOUNT;

            // We don't support named arguments
            if (pDispParams->cNamedArgs)
                return DISP_E_NONAMEDARGS;

            // Coerce the argument into a long
            HRESULT hr;
            VARIANTARG var;
            VariantInit(&var);
            hr = VariantChangeTypeEx(&var,
                                     &(pDispParams->rgvarg[0]),
                                     lcid, 0, VT_I4);

            if FAILED(hr)
                return DISP_E_BADVARTYPE;

            ComputationComplete(var.lVal);
            break;

        default:
            DisplayMessage("Error");
            break;
        }

        return S_OK;
    }

    STDMETHODIMP ComputationComplete(long lResult)
    {
        char szMsg[128];
        sprintf(szMsg, "The result is %d", lResult);
        DisplayMessage(szMsg);
        return S_OK;
    }

};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
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

#ifdef NEED_FINAL_CONSTRUCT
    CComObject<CMathEvents>* ptrMathEvents;
    CComObject<CMathEvents>::CreateInstance(&ptrMathEvents);
#else
    CComObject<CMathEvents>* ptrMathEvents = new CComObject<CMathEvents>;
#endif
    CComPtr<IUnknown> ptrEventsUnk = ptrMathEvents;
    // Set up the connection
    DWORD dwCookie;
    hr = AtlAdvise(ptrMath,
                   ptrEventsUnk,
                   DIID__IMathEvents,
                   &dwCookie);

    if (FAILED(hr)) {
        HandleError("Unable to setup the connection for IMathEvents", hr);
        return -1;
    }

    // Access the IMath interface
    long lResult;
    ptrMath->Add(300, 10, &lResult);
    ptrMath->Subtract(300, 10, &lResult);
    ptrMath->Multiply(300, 10, &lResult);
    ptrMath->Divide(300, 10, &lResult);
    // Shut down the event connection
    AtlUnadvise(ptrMath,
                DIID__IMathEvents,
                dwCookie);
    return 0;
}
