// CStringTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlstr.h>

//#define ATL_CSTRING_NO_CRT

void MyFunc(BSTR* bstrOut);

int _tmain(int argc, _TCHAR* argv[])
{
    CWin32Heap heap(0, 0, 0);
    CAtlStringMgr strMgr(&heap);
    BSTR bstr;
    MyFunc(&bstr);
    CString strFirst = "John";
    CString strLast = "Doe";
    CString str;
    str.Format("%s, %s: Age = %d", strLast, strFirst, 45);
    return 0;
}

void MyFunc(BSTR* bstrOut)
{
    BSTR bstr1 = ::SysAllocString(OLESTR("Hello"));
    bstr1 = NULL;
    CString str("Fred");
    BSTR bstr2 = str.SetSysString(&bstr1);
    str += "is ";
    str += "a good guy ";
    str += "most of the time";
}

