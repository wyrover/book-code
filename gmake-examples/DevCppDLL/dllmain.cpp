/* Replace "dll.h" with the name of your header */
#include "dll.h"
#include <windows.h>
#include <stdio.h>

/* --- DLL Èë¿Ú ---*/ 

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ ,
                       DWORD reason        /* Reason this function is being called. */ ,
                       LPVOID reserved     /* Not used. */ )
{
    switch (reason)
    {
      case DLL_PROCESS_ATTACH:
        break;

      case DLL_PROCESS_DETACH:
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}

/* -------------------------------------------------------------------------- */

DllClass::DllClass()
{

}


DllClass::~DllClass ()
{

}

class XyzImpl : public IXyz
{
    int Foo(int n);
    void Release();
};

int XyzImpl::Foo(int n)
{
    return n * n;
}

void XyzImpl::Release()
{
    delete this;
}


extern "C" DLLIMPORT void WINAPI Hello1()
{
	printf("dddd");
}

extern "C" DLLIMPORT IXyz* WINAPI GetXyz()
{
    return new XyzImpl;
}