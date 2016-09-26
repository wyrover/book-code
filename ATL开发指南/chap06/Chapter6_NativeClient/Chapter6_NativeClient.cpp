//
// Chapter6_NativeClient.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>

#import "..\Chapter6_Server\Debug\Chapter6_Server.dll" no_namespace named_guids

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
        _variant_t result;
        _variant_t Op1(long(100));
        _variant_t Op2(long(200));
        result = ptrMath->Add(Op1, Op2);
        cout << "100 + 200 = " << long(result) << endl;
    } catch (_com_error& e) {
        cout << "Error accessing IMath" << endl;
        cout << "HRESULT message is " << e.ErrorMessage() << endl;

        if (e.ErrorInfo())
            cout << e.Description() << endl;
    }

    // We have to release the pointer here
    // because if we call CoUninitialize before
    // it goes out of scope, we'll get an exception
    ptrMath = 0;
    CoUninitialize();
    return 0;
}
