//
// Chapter9_CollectionClient.cpp
//

#include <windows.h>
#include <tchar.h>
#include <iostream.h>
#include <initguid.h>

#include <atlbase.h>

#include "..\Chapter9_CollectionServer\Chapter9_CollectionServer_i.c"
#include "..\Chapter9_CollectionServer\Chapter9_CollectionServer.h"

int main(int argc, char *argv[])
{
    cout << "Initializing COM" << endl;

    if (FAILED(CoInitialize(NULL))) {
        cout << "Unable to initialize COM" << endl;
        return -1;
    }

    // Get the class factory
    IApplication* pApp;
    HRESULT hr = CoCreateInstance(CLSID_Application,
                                  NULL,
                                  CLSCTX_SERVER,
                                  IID_IApplication,
                                  (void**) &pApp);

    if (FAILED(hr)) {
        cout.setf(ios::hex, ios::basefield);
        cout << "Failed to create server instance. HR = " << hr << endl;
        return -1;
    }

    cout << "Instance created" << endl;
    IMovieCollection *pMovies;
    pApp->get_Movies((IDispatch**) &pMovies);
    long lCount;
    pMovies->get_Count(&lCount);

    for (int i = 0; i < lCount; i++) {
        USES_CONVERSION;
        IMovie* pMovie = 0;
        pMovies->get_Item(i + 1, &pMovie);
        BSTR bstr = 0;
        pMovie->get_Title(&bstr);
        cout << W2A(bstr) << " - ";
        SysFreeString(bstr);
        pMovie->get_Director(&bstr);
        cout << W2A(bstr) << endl;
        SysFreeString(bstr);
        pMovie->Release();
    }

    pMovies->Release();
    cout << "Shuting down COM" << endl;
    CoUninitialize();
    return 0;
}
