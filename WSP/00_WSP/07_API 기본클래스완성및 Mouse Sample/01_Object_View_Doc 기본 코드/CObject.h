#include <windows.h>

#ifndef _CObject_
#define _CObject_

class CDocument;		// 전방참조 

class CObject 
{
protected:
    static char szAppName[];
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;

	CDocument *pDoc;

public:
    void InitInstance(HINSTANCE hInstance,PSTR szCmdLine,
                      int iCmdShow);
    void Run();
    WPARAM ExitInstance();
};

#endif