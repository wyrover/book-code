// console_template.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    std::wcout.imbue(std::locale(""));
    _tprintf_s(_T("%s\n"), _T("hello world. 中文测试"));
    std::wcout << L"中文测试" << std::endl;
    _tsystem(_T("pause"));
    return 0;
}

