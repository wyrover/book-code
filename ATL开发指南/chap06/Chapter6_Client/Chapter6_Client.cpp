//
// Chapter6_Client.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

#include <initguid.h>
#include "..\Chapter6_Server\Chapter6_Server_i.c"
#include "..\Chapter6_Server\Chapter6_Server.h"

// For ATL's variant support
#include <atlbase.h>
//#include <atlimpl.cpp>

int main(int argc, char *argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

    // Create the math component and return IUnknown
    IUnknown* pUnk;
    HRESULT hr = CoCreateInstance(CLSID_Math,
                                  NULL,
                                  CLSCTX_SERVER,
                                  IID_IUnknown,
                                  (void**) &pUnk);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to create server instance. HR = " << hr << endl;
        CoUninitialize();
        return -1;
    }

    cout << "Instance created" << endl;
    // Here we demonstrate accessing a
    // dispinterface by first querying for
    // IDispatch (which returns the default)
    // and then using GetIDsOfNames and Invoke
    // to actually call the Add method
    IDispatch* pDispatch;
    pUnk->QueryInterface(IID_IDispatch, (void**) &pDispatch);
    pUnk->Release();

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to create server instance. HR = " << hr << endl;
        CoUninitialize();
        return -1;
    }

    // Get the DISPID
    LPOLESTR lpOleStr = L"Add";
    DISPID dispid;
    hr = pDispatch->GetIDsOfNames(IID_NULL,
                                  &lpOleStr,
                                  1,
                                  LOCALE_SYSTEM_DEFAULT,
                                  &dispid);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "GetIDsOfNames failed. HR = " << hr << endl;
        CoUninitialize();
        return -1;
    }

    // Set up the parameters
    DISPPARAMS dispparms;
    memset(&dispparms, 0, sizeof(DISPPARAMS));
    dispparms.cArgs = 2;
    // allocate memory for parameters
    VARIANTARG* pArg = new VARIANTARG[dispparms.cArgs];
    dispparms.rgvarg = pArg;
    memset(pArg, 0, sizeof(VARIANT) * dispparms.cArgs);
    // The parameters are entered right to left
    // We are adding 123 to 456
    dispparms.rgvarg[0].vt = VT_I4;
    dispparms.rgvarg[0].lVal = 123;
    dispparms.rgvarg[1].vt = VT_I4;
    dispparms.rgvarg[1].lVal = 456;
    // This method returns a value so we need a VARIANT to store it in
    VARIANTARG vaResult;
    VariantInit(&vaResult);
    // Invoke the method in the local server
    hr = pDispatch->Invoke(dispid,
                           IID_NULL,
                           LOCALE_SYSTEM_DEFAULT,
                           DISPATCH_METHOD,
                           &dispparms,
                           &vaResult,
                           0,
                           NULL);
    // Free up our our variantargs
    delete [] pArg;

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Unable to Invoke SetExpression. HR = " << hr << endl;
        CoUninitialize();
        return -1;
    }

    // Display the result
    cout << "123 + 456 = " << vaResult.lVal << endl;
    // Next, we demonstrate using the IMath
    // dual interface. First QI then access
    // the methods through the Vtable interface
    IMath* pMath = 0;
    pDispatch->QueryInterface(IID_IMath, (void**) &pMath);
    pDispatch->Release();
    // We're using variants in this chapter
    CComVariant varResult;
    CComVariant varOp1(9);
    CComVariant varOp2(99);
    pMath->Multiply(varOp1, varOp2, &varResult);
    cout << "9 * 99 = " << varResult.lVal << endl;
    // Because this is a dual interface we
    // can access the IDispatch methods
    // from IMath as well
    lpOleStr = L"Multiply";
    hr = pMath->GetIDsOfNames(IID_NULL,
                              &lpOleStr,
                              1,
                              LOCALE_SYSTEM_DEFAULT,
                              &dispid);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "GetIDsOfNames (IMath) failed. HR = " << hr << endl;
        CoUninitialize();
        return -1;
    }

    cout << "The DISPID for Multiply is " << dispid << endl;
    // Get the IMath2 dual interface
    IMath2* pMath2;
    hr = pMath->QueryInterface(IID_IMath2,
                               (void**) &pMath2);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Unable to QI( IMath2 ). HR = " << hr << endl;
        CoUninitialize();
        return -1;
    }

    // Try calling our Sum method
    // We first have to build a safe array
    // It's an array of 10 longs with
    // the values 0,1,2,3,4...
    SAFEARRAY       *psaArray = 0;
    SAFEARRAYBOUND  rgsabound[1];
    rgsabound[0].lLbound = 0;
    rgsabound[0].cElements = 10;
    psaArray = SafeArrayCreate(VT_I4,
                               1, rgsabound);

    for (int i = 0; i < 10; i++) {
        SafeArrayPutElement(psaArray,
                            (long *) &i, &i);
    }

    VARIANT varArray;
    VariantInit(&varArray);
    V_VT(&varArray) = VT_ARRAY | VT_I4;
    V_ARRAY(&varArray) = psaArray;
    // Finally
    long lResult;
    pMath2->Sum(varArray, &lResult);
    cout << "The sum of 0,1,2,3...9 is " << lResult << endl;
    // Release the interfaces
    pMath->Release();
    pMath2->Release();
    cout << "Shuting down COM" << endl;
    CoUninitialize();
    return 0;
}
