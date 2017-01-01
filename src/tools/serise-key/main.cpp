// console_template.cpp : Defines the entry point for the console application.
//

#include "buildcfg.h"
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <getopt.h>
#include "SeriseKey.h"


#define VERSION "1.0.2"
#define ARGUMENTS "khv"

void showHelp();


int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T(""));
    std::wstring inFile;
    std::wstring outFile;
    int c;
    static struct option long_options[] = {
        {_T("key"),  ARG_NONE,  0, _T('k')},       
        {_T("help"),    ARG_NONE, 0 , _T('h')},
		{_T("version"), ARG_NONE, 0 , _T('v')},
        { ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }
    };

    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, _T(ARGUMENTS), long_options, &option_index);

        // Check for end of operation or error
		if (c == -1 || c == '?') {            
			break;
		}

        // Handle options
        switch (c) {
        case _T('k'):            
			std::cout << GetSoftSeriseKey().c_str() << std::endl;
            break;        

        case _T('v'):
            _tprintf(_T(VERSION));
            break;

        case _T('h'):
            showHelp();
            break;

        default:
            showHelp();
            break;
        }
    }

    
    return 0;
}

void showHelp()
{
    printf("file2hex Test %s\n\n", VERSION);
    printf("Usage: file2hex <options> <filename> <options> <filename>\n\n");
    printf("Options:\n");
    printf("     -i  input file\n");
    printf("     -o  output file\n\n");
    printf("Example:\n");
    printf("  file2hex -i file1.txt -o file2.txt\n");
    exit(0);
}