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
using namespace std;

mapped_region *mp_r;

void fung()
{
    while (1) {
        int num = 0;
        memcpy(&num, static_cast<char*>(mp_r->get_address()), sizeof(int));
        cout << num << endl;
        Sleep(500);
    }
}

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T(""));
    //open shared memory object
    shared_memory_object share_obj(open_only, "named_obj", read_only);//map the shared memory object to current process
    mp_r = new mapped_region(share_obj, read_only);
    boost::thread th(fung);
    th.detach();
    getchar();
    //remove shared memory object
    shared_memory_object::remove("named_obj");
    return 0;
}

