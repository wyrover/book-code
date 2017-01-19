// console_template.cpp : Defines the entry point for the console application.
//

#include "targetver.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/thread.hpp>

using namespace boost::interprocess;

int num = 0;
mapped_region *mp_r;

void funs()
{
    while (1) {
        num ++;
        memcpy(mp_r->get_address(), &num, sizeof(int));
        mp_r->get_address();
        ::Sleep(500);
    }
}

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T(""));
    shared_memory_object share_obj(create_only, "named_obj", read_write);
    share_obj.truncate(sizeof(int));
    mp_r = new mapped_region(share_obj, read_write);
    boost::thread th(funs);
    th.detach();
    getchar();
    return 0;
}

