#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <tchar.h>
#include <locale.h>

#define VERSION "1.0.2"
#define ARGUMENTS "hvabc:d:f:"

void showHelp();

int _tmain(int argc, TCHAR** argv)
{
    setlocale(LC_CTYPE, "");
    SetConsoleOutputCP(CP_ACP);
    _tprintf(L"÷–Œƒ≤‚ ‘\n");
    static int verbose_flag;
    int c;
    static struct option long_options[] = {
        {_T("verbose"), ARG_NONE, &verbose_flag, 1},
        {_T("brief"),   ARG_NONE, &verbose_flag, 0},
        {_T("add"),     ARG_NONE, 0, _T('a')},
        {_T("append"),  ARG_NONE, 0, _T('b')},
        {_T("delete"),  ARG_REQ,  0, _T('d')},
        {_T("create"),  ARG_REQ,  0, _T('c')},
        {_T("file"),    ARG_REQ, 0 , _T('f')},
        {_T("version"), ARG_REQ, 0 , _T('v')},
        {_T("help"),    ARG_REQ, 0 , _T('h')},
        { ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }
    };

    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, _T(ARGUMENTS), long_options, &option_index);

        // Check for end of operation or error
        if (c == -1 || c == '?')
            break;

        // Handle options
        switch (c) {
        case 0:

            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;

            _tprintf(_T("option %s"), long_options[option_index].name);

            if (optarg)
                _tprintf(_T(" with arg %s"), optarg);

            _tprintf(_T("\n"));
            break;

        case _T('a'):
            _tprintf(_T("option -a\n"));
            break;

        case _T('b'):
            _tprintf(_T("option -b\n"));
            break;

        case _T('c'):
            _tprintf(_T("option -c with value `%s'\n"), optarg);
            break;

        case _T('d'):
            _tprintf(_T("option -d with value `%s'\n"), optarg);
            break;

        case _T('f'):
            _tprintf(_T("option -f with value `%s'\n"), optarg);
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

    if (verbose_flag)
        _tprintf(_T("verbose flag is set\n"));

    if (optind < argc) {
        _tprintf(_T("non-option ARGV-elements: "));

        while (optind < argc) _tprintf(_T("%s "), argv[optind++]);

        _tprintf(_T("\n"));
    }

    return 0;
}

void showHelp()
{
    printf("Snappy Test %s\n\n", VERSION);
    printf("Usage: snappytest <options> <filename>\n\n");
    printf("Options:\n");
    printf("     -c  Compress a file\n");
    printf("     -x  Extract a file\n\n");
    printf("Example:\n");
    printf("  snappytest -c logo.bmp\n");
    exit(0);
	_tcsicmp()
}