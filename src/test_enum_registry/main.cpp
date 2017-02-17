// console_template.cpp : Defines the entry point for the console application.
//

#include "targetver.h"
#include <cactus/cactus_registry.h>
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
    std::vector<string16> subkeys;
    std::wstring rootkey = L"SYSTEM\\CurrentControlSet\\Control\\CriticalDeviceDatabase";
    LONG lRes = cactus::CRegKey::get_subkey_names(HKEY_LOCAL_MACHINE, rootkey, subkeys);

    if (lRes == ERROR_SUCCESS) {
        std::vector<string16>::iterator it = subkeys.begin();

        for (; it != subkeys.end(); ++it) {
            //std::wcout << (*it) << std::endl;
            std::vector<string16> values;
            std::wstring subkey = rootkey + L"\\" + (*it);
            //std::wcout << subkey.c_str() << std::endl;
            lRes = cactus::CRegKey::get_value_names(HKEY_LOCAL_MACHINE, subkey, values);

            if (lRes == ERROR_SUCCESS) {
                std::vector<string16>::iterator it2 = values.begin();

                for (; it2 != values.end(); ++it2) {
                    if (wcsicmp((*it2).c_str(), L"DriverPackageId") == 0) {
                        //std::wcout << (*it2) << std::endl;


						string16 orgine_inf_dir_name;
						LONG lRes2 = cactus::CRegKey::ReadStringValue(HKEY_LOCAL_MACHINE, subkey, L"DriverPackageId", orgine_inf_dir_name);
						if (lRes2 == ERROR_SUCCESS) {
							std::wcout << orgine_inf_dir_name.c_str() << std::endl;
						}
                    }
                }
            }
        }
    }

    _tsystem(_T("pause"));
    return 0;
}

