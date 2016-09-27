//
// Chapter4_NativeClient.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

#import "..\Chapter4_Server\Debug\Chapter4_Server.exe" no_namespace named_guids

int main(int argc, char *argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

    IMathPtr ptrMath;

    // Create an instance of the server
    try {
        HRESULT hr;
        hr = ptrMath.CreateInstance(CLSID_Math);

        if (FAILED(hr))
            _com_issue_error(hr);
    } catch (_com_error& e) {
        cout << "Error creating instance" << endl;
        cout << "HRESULT message is " << e.ErrorMessage() << endl;

        if (e.ErrorInfo())
            cout << e.Description() << endl;

        return -1;
    }

    // Access the IMath interface
    try {
        long lResult;
        lResult = ptrMath->Add(134, 353);
        cout << "134 + 353 = " << lResult << endl;
        // Try to divide by zero
        lResult = ptrMath->Divide(0, 0);
    } catch (_com_error& e) {
        cout << "Error accessing IMath" << endl;
        cout << "HRESULT message is " << e.ErrorMessage() << endl;

        if (e.ErrorInfo())
            cout << e.Description() << endl;

        // Don't return, we forced the error with 0/0
    }

    // Access IMath2
    try {
        IMath2Ptr ptrMath2(ptrMath);
        long lResult;
        lResult = ptrMath2->Compute(mathAdd,
                                    100,
                                    200);
        cout << "Compute( 100 + 200 ) = " << lResult << endl;
        // Sum an array
        short sArray[3] = { 3, 4, 5 };
        lResult = ptrMath2->Sum(3, sArray);
        cout << "Sum( 3,4,5 ) = " << lResult << endl;
    } catch (_com_error& e) {
        cout << "Error accessing IMath2" << endl;
        cout << "HRESULT message is " << e.ErrorMessage() << endl;

        if (e.ErrorInfo())
            cout << e.Description() << endl;

        return -1;
    }

    // Access IAdvancedMath
    try {
        IAdvancedMathPtr ptrAdvancedMath(ptrMath);

        if (ptrAdvancedMath) {
            long lResult;
            lResult = ptrAdvancedMath->Factorial(12);
            cout << "12! = " << lResult << endl;
            lResult = ptrAdvancedMath->Fibonacci(12);
            cout << "The Fibonacci of 12 = " << lResult << endl;
        }
    } catch (_com_error& e) {
        cout << "Error accessing IAdvancedMath" << endl;
        cout << "HRESULT message is " << e.ErrorMessage() << endl;
        return -1;
    }

    // Access IComponentInfo
    try {
        IComponentInfoPtr ptrInfo(ptrMath);

        if (ptrInfo) {
            COMPONENT_INFO* pInfo = 0;
            ptrInfo->get_Info(&pInfo);
            cout << "Component author is " << pInfo->pstrAuthor << endl;
            cout << "Component version is " << pInfo->sMajor << "." << pInfo->sMinor << endl;
            _bstr_t name(pInfo->bstrName);
            cout << "Component name is " << name << endl;

            if (pInfo->pstrAuthor)
                CoTaskMemFree(pInfo->pstrAuthor);

            if (pInfo->bstrName)
                SysFreeString(pInfo->bstrName);

            if (pInfo)
                CoTaskMemFree(pInfo);
        }
    } catch (_com_error& e) {
        cout << "Error accessing IComponentInfo" << endl;
        cout << "HRESULT message is " << e.ErrorMessage() << endl;
        return -1;
    }

    // We have to release the pointer here
    // because if we call CoUninitialize before
    // it goes out of scope, we'll get an exception
    ptrMath = 0;
    CoUninitialize();
    return 0;
}
