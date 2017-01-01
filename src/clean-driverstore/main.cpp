// console_template.cpp : Defines the entry point for the console application.
//

#include "targetver.h"
#include "inf_utils.h"

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <vector>
#include <boost/regex.hpp>
#include <tracetool/tracetool.h>


using namespace cactus;



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));

	string16 pnputil_fullpath = get_pnputil_exe_fullpath();
	std::wcout << pnputil_fullpath.c_str() << std::endl;	


	//std::wstring result = L"";
	//DWORD exit_code;

	//if (getRunExeOutput(get_pnputil_exe_fullpath(), L" -e", &exit_code, result)) {
	//	std::wcout << result.c_str() << std::endl;		
	//	boost::wregex r2(L"(oem\\d+\\.inf)");
	//	boost::wsmatch matches;
	//	std::wstring::const_iterator start = result.begin(),
	//		end = result.end();	

	//	while (boost::regex_search(start, end, matches, r2)) {
	//		std::wcout << matches.str() << std::endl;
	//		
	//		start = matches[0].second;
	//	}

	//	
	//}


	std::vector<string16> ivec;
	get_difference_infs2(ivec);

	std::vector<string16>::iterator it2 = ivec.begin();
	for (; it2 != ivec.end(); ++it2) {
		std::wcout << (*it2).c_str() << std::endl;

		char16 args[MAX_PATH] = {0};
		swprintf_s(args, MAX_PATH, L" -d \"%s\"", (*it2).c_str());		
		runExe(pnputil_fullpath, std::wstring(args));		
	}

	

    return 0;
}

