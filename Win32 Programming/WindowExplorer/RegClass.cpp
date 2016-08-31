#include "stdafx.h"
#include "regclass.h"

typedef ATOM (WINAPI* REGISTERCLASSEXPROC)(const LPWNDCLASSEX lpwcex) ;

//
//  BOOL internalRegisterClass (const LPWNDCLASSEX lpwcex)
//
//  lpwcex          Pointer to WNDCLASSEX structure
//
//  PURPOSE:        Registers the window class using RegisterClassEx
//                  if it is available. If not, registers the class
//                  using RegisterClass.
//
//  COMMENTS:       RegisterClassEx was introduced in Windows 95 and
//                  Windows NT 3.51. An application must register its
//                  window classes using RegisterClassEx in order to
//                  specify the small icons which should be used for
//                  the application.
//
  
ATOM
internalRegisterClass (const LPWNDCLASSEX lpwcex)
{
    WNDCLASS wc ;

    // Get the module handle of the 32-bit USER DLL
    HINSTANCE hModule = GetModuleHandle (TEXT("USER32")) ;
    if (NULL != hModule) {

        // If we're running on a Win32 version supporting RegisterClassEx
        //  get the address of the function so we can call it

#if defined (UNICODE)
    REGISTERCLASSEXPROC proc = 
        (REGISTERCLASSEXPROC) ::GetProcAddress (hModule, "RegisterClassExW") ;
#else
    REGISTERCLASSEXPROC proc =
        (REGISTERCLASSEXPROC) ::GetProcAddress (hModule, "RegisterClassExA") ;
#endif
    if (NULL != proc)

        // RegisterClassEx exists...
        // return RegisterClassEx (&wcex) ;
        return (*proc) (lpwcex) ;
    }

    // Convert the WNDCLASSEX structure to a WNDCLASS structure
    wc.style         = lpwcex->style ;
    wc.lpfnWndProc   = lpwcex->lpfnWndProc ;
    wc.cbClsExtra    = lpwcex->cbClsExtra ;
    wc.cbWndExtra    = lpwcex->cbWndExtra ;
    wc.hInstance     = lpwcex->hInstance ;
    wc.hIcon         = lpwcex->hIcon ;
    wc.hCursor       = lpwcex->hCursor ;
    wc.hbrBackground = lpwcex->hbrBackground ;
    wc.lpszMenuName  = lpwcex->lpszMenuName ;
    wc.lpszClassName = lpwcex->lpszClassName ;

    return RegisterClass (&wc) ;
}


