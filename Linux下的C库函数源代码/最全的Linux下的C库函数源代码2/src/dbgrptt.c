/***
*dbgrptt.c - Debug CRT Reporting Functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/

#ifdef _DEBUG

#include <internal.h>
#include <mtdll.h>
#include <malloc.h>
#include <mbstring.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <dbgint.h>
#include <signal.h>
#include <string.h>
#include <awint.h>
#include <windows.h>
#include <errno.h>


int __cdecl __crtMessageWindowA(
    int nRptType,
    const char * szFile,
    const char * szLine,
    const char * szModule,
    const char * szUserMessage
    );

int __cdecl __crtMessageWindowW(
    int nRptType,
    const wchar_t * szFile,
    const wchar_t * szLine,
    const wchar_t * szModule,
    const wchar_t * szUserMessage
    );

_CRT_REPORT_HOOK _pfnReportHook;

ReportHookNode *_pReportHookList;
ReportHookNodeW *_pReportHookListW;

_CRTIMP long _crtAssertBusy = -1;

int _CrtDbgMode[_CRT_ERRCNT] = {
    _CRTDBG_MODE_DEBUG,
    _CRTDBG_MODE_WNDW,
    _CRTDBG_MODE_WNDW
};

_HFILE _CrtDbgFile[_CRT_ERRCNT] = { _CRTDBG_INVALID_HFILE,
                                    _CRTDBG_INVALID_HFILE,
                                    _CRTDBG_INVALID_HFILE
};

#define _CrtInterlockedIncrement InterlockedIncrement
#define _CrtInterlockedDecrement InterlockedDecrement

/***
 *void _CrtDebugBreak - call OS-specific debug function
 *
 *Purpose:
 *       call OS-specific debug function
 *
 *Entry:
 *
 *Exit:
 *
 *Exceptions:
 *
 *******************************************************************************/

#undef _CrtDbgBreak

/* These methods don't need a separate
   wchar version. Hence they need to be compiled only once from
   the original file */

_CRTIMP void _cdecl _CrtDbgBreak(
    void
    )
{
    DebugBreak();
}

/***
 *int _CrtSetReportMode - set the reporting mode for a given report type
 *
 *Purpose:
 *       set the reporting mode for a given report type
 *
 *Entry:
 *       int nRptType    - the report type
 *       int fMode       - new mode for given report type
 *
 *Exit:
 *       previous mode for given report type
 *
 *Exceptions:
 *       Input parameters are validated. Refer to the validation section of the function.
 *
 *******************************************************************************/
_CRTIMP int __cdecl _CrtSetReportMode(
    int nRptType,
    int fMode
    )
{
    int oldMode;

    /* validation section */
    _VALIDATE_RETURN(nRptType >= 0 && nRptType < _CRT_ERRCNT, EINVAL, -1);
    _VALIDATE_RETURN(
        fMode == _CRTDBG_REPORT_MODE ||
        (fMode & ~(_CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW)) == 0,
        EINVAL,
        -1);

    if (fMode == _CRTDBG_REPORT_MODE)
        return _CrtDbgMode[nRptType];

    oldMode = _CrtDbgMode[nRptType];

    _CrtDbgMode[nRptType] = fMode;

    return oldMode;
}

/***
 *int _CrtSetReportFile - set the reporting file for a given report type
 *
 *Purpose:
 *       set the reporting file for a given report type
 *
 *Entry:
 *       int nRptType    - the report type
 *       _HFILE hFile    - new file for given report type
 *
 *Exit:
 *       previous file for given report type
 *
 *Exceptions:
 *       Input parameters are validated. Refer to the validation section of the function.
 *
 *******************************************************************************/
_CRTIMP _HFILE __cdecl _CrtSetReportFile(
    int nRptType,
    _HFILE hFile
    )
{
    _HFILE oldFile;

    /* validation section */
    _VALIDATE_RETURN(nRptType >= 0 && nRptType < _CRT_ERRCNT, EINVAL, _CRTDBG_HFILE_ERROR);

    if (hFile == _CRTDBG_REPORT_FILE)
        return _CrtDbgFile[nRptType];

    oldFile = _CrtDbgFile[nRptType];

    if (_CRTDBG_FILE_STDOUT == hFile)
        _CrtDbgFile[nRptType] = GetStdHandle(STD_OUTPUT_HANDLE);
    else if (_CRTDBG_FILE_STDERR == hFile)
        _CrtDbgFile[nRptType] = GetStdHandle(STD_ERROR_HANDLE);
    else
        _CrtDbgFile[nRptType] = hFile;

    return oldFile;
}

/***
 *_CRT_REPORT_HOOK _CrtSetReportHook() - set client report hook
 *
 *Purpose:
 *       set client report hook. This function is provided only in ANSI
 *       for backward compatibility. No Unicode Version of this function exists
 *
 *Entry:
 *       _CRT_REPORT_HOOK pfnNewHook - new report hook
 *
 *Exit:
 *       return previous hook
 *
 *Exceptions:
 *
 *******************************************************************************/

_CRTIMP _CRT_REPORT_HOOK __cdecl _CrtSetReportHook(_CRT_REPORT_HOOK pfnNewHook)
{
    _CRT_REPORT_HOOK pfnOldHook = _pfnReportHook;
    _pfnReportHook = pfnNewHook;
    return pfnOldHook;
}

/***
 *_CRT_REPORT_HOOK _CrtGetReportHook() - get client report hook
 *
 *Purpose:
 *       get client report hook.
 *
 *Entry:
 *
 *Exit:
 *       return current hook
 *
 *Exceptions:
 *
 *******************************************************************************/

_CRTIMP _CRT_REPORT_HOOK __cdecl _CrtGetReportHook(void)
{
    return _pfnReportHook;
}

#define ASSERTINTRO1 "Assertion failed: "
#define ASSERTINTRO2 "Assertion failed!"

/***
 *int _VCrtDbgReportA() - _CrtDbgReport calls into this function
 *
 *Purpose:
 *       See remarks for _CrtDbgReport.
 *
 *Entry:
 *       int             nRptType    - report type
 *       const char *    szFile      - file name
 *       int             nLine       - line number
 *       const char *    szModule    - module name
 *       const char *    szFormat    - format string
 *       va_list         arglist      - var args arglist
 *
 *Exit:
 *       See remarks for _CrtDbgReport
 *
 *Exceptions:
 *
 *******************************************************************************/

#pragma warning(push)
#pragma warning(disable:6262)
// prefast (6262): This func uses lots of stack because we want to tolerate very large reports, and we can't use malloc here.
_CRTIMP int __cdecl _VCrtDbgReportA
(
    int nRptType,
    const char * szFile,
    int nLine,
    const char * szModule,
    const char * szFormat,
    va_list arglist
    )
{
    int retval=0;
    int handled=FALSE;
    char szLineMessage[DBGRPT_MAX_MSG] = {0};
    char szOutMessage[DBGRPT_MAX_MSG] = {0};
    wchar_t szOutMessage2[DBGRPT_MAX_MSG] = {0};
    char szUserMessage[DBGRPT_MAX_MSG] = {0};

    if (nRptType < 0 || nRptType >= _CRT_ERRCNT)
        return -1;

    /*
     * handle the (hopefully rare) case of
     *
     * 1) ASSERT while already dealing with an ASSERT
     *  If it's a recursive assert, the mlock(_DEBUG_LOCK) will not
     *  block(because we are in the same thread) & _crtAssertBusy value will
     *  cause DebugBreak to be called.
     *      or
     * 2) two threads asserting at the same time - here
     *  mlock(_DEBUG_LOCK) will cause the asserts to be serialized
     */

    _mlock(_DEBUG_LOCK);
    __try
    {

        if (_CRT_ASSERT == nRptType && _CrtInterlockedIncrement(&_crtAssertBusy) > 0)
        {
            /* use only 'safe' functions -- must not assert in here! */

            _ERRCHECK(_itoa_s(nLine, szLineMessage, DBGRPT_MAX_MSG, 10));

            OutputDebugStringA("Second Chance Assertion Failed: File ");
            OutputDebugStringA(szFile ? szFile : "<file unknown>");
            OutputDebugStringA(", Line ");
            OutputDebugStringA(szLineMessage);
            OutputDebugStringA("\n");

            _CrtDbgBreak();
            retval=-1;
        }
        else
        {

            // Leave space for ASSERTINTRO1 and "\r\n"
            if (szFormat)
            {
                int szlen = 0;
                _ERRCHECK_SPRINTF(szlen = _vsnprintf_s(szUserMessage, DBGRPT_MAX_MSG,
                                                       DBGRPT_MAX_MSG - 2- max(sizeof(ASSERTINTRO1),sizeof(ASSERTINTRO2)),
                                                       szFormat, arglist));
                if (szlen < 0)
                {
                    _ERRCHECK(strcpy_s(szUserMessage, DBGRPT_MAX_MSG, DBGRPT_TOOLONGMSG));
                }
            }

            if (_CRT_ASSERT == nRptType)
                _ERRCHECK(strcpy_s(szLineMessage, DBGRPT_MAX_MSG, szFormat ? ASSERTINTRO1 : ASSERTINTRO2));

            _ERRCHECK(strcat_s(szLineMessage, DBGRPT_MAX_MSG, szUserMessage));

            if (_CRT_ASSERT == nRptType)
            {
                if (_CrtDbgMode[nRptType] & _CRTDBG_MODE_FILE)
                    _ERRCHECK(strcat_s(szLineMessage, DBGRPT_MAX_MSG, "\r"));
                _ERRCHECK(strcat_s(szLineMessage, DBGRPT_MAX_MSG, "\n"));
            }

            if (szFile)
            {
                int szlen = 0;
                _ERRCHECK_SPRINTF(szlen = _snprintf_s(szOutMessage, DBGRPT_MAX_MSG, DBGRPT_MAX_MSG - 1, "%s(%d) : %s",
                                                      szFile, nLine, szLineMessage));
                if (szlen < 0)
                {
                    _ERRCHECK(strcpy_s(szOutMessage, DBGRPT_MAX_MSG, DBGRPT_TOOLONGMSG));
                }
            }
            else
                _ERRCHECK(strcpy_s(szOutMessage, DBGRPT_MAX_MSG, szLineMessage));

            {
                size_t ret = 0;
                errno_t e = 0;
                _ERRCHECK_EINVAL_ERANGE(e = mbstowcs_s(&ret, szOutMessage2, DBGRPT_MAX_MSG, szOutMessage, _TRUNCATE));
                if(e != 0)
                    _ERRCHECK(wcscpy_s(szOutMessage2, DBGRPT_MAX_MSG, _CRT_WIDE(DBGRPT_INVALIDMSG)));

            }

            /* User hook may handle report.
               We have to check the ANSI Hook2 List & then the UNICODE Hook2 List.
               Then we have check any ANSI individual Hook set through
               SetReportHook */

            if (_pReportHookList || _pReportHookListW)
            {
                ReportHookNode *pnode=NULL;
                ReportHookNodeW *pnodeW=NULL;

                for (pnode = _pReportHookList; pnode; pnode = pnode->next)
                {
                    int hook_retval=0;
                    if ((*pnode->pfnHookFunc)(nRptType, szOutMessage, &hook_retval))
                    {
                        handled=TRUE;
                        retval=hook_retval;
                        break;
                    }
                }

                if(!handled)
                {
                    for (pnodeW = _pReportHookListW; pnodeW; pnodeW = pnodeW->next)
                    {
                        int hook_retval=0;
                        if ((*pnodeW->pfnHookFunc)(nRptType, szOutMessage2, &hook_retval))
                        {
                            handled=TRUE;
                            retval=hook_retval;
                            break;
                        }
                    }
                }
            }
            if(!handled)
            {
                if(_pfnReportHook)
                {
                    int hook_retval=0;
                    if(((*_pfnReportHook)(nRptType, szOutMessage, &hook_retval)))
                    {
                        handled=TRUE;
                        retval=hook_retval;
                    }
                }

                if (!handled)
                {
                    if (_CrtDbgMode[nRptType] & _CRTDBG_MODE_FILE)
                    {
                        if (_CrtDbgFile[nRptType] != _CRTDBG_INVALID_HFILE)
                        {
                            DWORD written;
                            WriteFile(_CrtDbgFile[nRptType], szOutMessage, (unsigned long)strlen(szOutMessage), &written, NULL);
                        }
                    }

                    if (_CrtDbgMode[nRptType] & _CRTDBG_MODE_DEBUG)
                    {
                        OutputDebugStringA(szOutMessage);
                    }

                    if (_CrtDbgMode[nRptType] & _CRTDBG_MODE_WNDW)
                    {
                        szLineMessage[0] = 0;
                        if (nLine)
                        {
                            _ERRCHECK(_itoa_s(nLine, szLineMessage, DBGRPT_MAX_MSG, 10));
                        }
                        retval = __crtMessageWindowA(nRptType, szFile, (nLine ? szLineMessage : NULL), szModule, szUserMessage);
                    }
                }
            }
        }
    }
    __finally
    {
        if (_CRT_ASSERT == nRptType)
        {
            _CrtInterlockedDecrement(&_crtAssertBusy);
        }
        _munlock(_DEBUG_LOCK);
    }

    return retval;
}
#pragma warning(pop)

/***
 *int _VCrtDbgReportW() - _CrtDbgReportW calls into this function
 *
 *Purpose:
 *       See remarks for _CrtDbgReport.
 *
 *Entry:
 *       int             nRptType    - report type
 *       const wchar_t * szFile      - file name
 *       int             nLine       - line number
 *       const wchar_t * szModule    - module name
 *       const wchar_t * szFormat    - format string
 *       va_list         arglist      - var args arglist
 *
 *Exit:
 *       See remarks for _CrtDbgReport
 *
 *Exceptions:
 *
 *******************************************************************************/

#pragma warning(push)
#pragma warning(disable:6262)
// prefast(6262): This func uses lots of stack because we want to tolerate very large reports, and we can't use malloc here.
_CRTIMP int __cdecl _VCrtDbgReportW
(
    int nRptType,
    const wchar_t * szFile,
    int nLine,
    const wchar_t * szModule,
    const wchar_t * szFormat,
    va_list arglist
    )
{
    int retval=0;
    int handled=FALSE;
    wchar_t szLineMessage[DBGRPT_MAX_MSG] = {0};
    wchar_t szOutMessage[DBGRPT_MAX_MSG] = {0};
    char szOutMessage2[DBGRPT_MAX_MSG] = {0};
    wchar_t szUserMessage[DBGRPT_MAX_MSG] = {0};

    if (nRptType < 0 || nRptType >= _CRT_ERRCNT)
        return -1;

    /*
     * handle the (hopefully rare) case of
     *
     * 1) ASSERT while already dealing with an ASSERT
     *  If it's a recursive assert, the mlock(_DEBUG_LOCK) will not
     *  block(because we are in the same thread) & _crtAssertBusy value will
     *  cause DebugBreak to be called.
     *      or
     * 2) two threads asserting at the same time - here
     *  mlock(_DEBUG_LOCK) will cause the asserts to be serialized
     */

    _mlock(_DEBUG_LOCK);
    __try
    {
        if (_CRT_ASSERT == nRptType && _CrtInterlockedIncrement(&_crtAssertBusy) > 0)
        {
            /* use only 'safe' functions -- must not assert in here! */

            _ERRCHECK(_itow_s(nLine, szLineMessage, DBGRPT_MAX_MSG, 10));

            OutputDebugStringW(L"Second Chance Assertion Failed: File ");
            OutputDebugStringW(szFile ? szFile : L"<file unknown>");
            OutputDebugStringW(L", Line ");
            OutputDebugStringW(szLineMessage);
            OutputDebugStringW(L"\n");

            _CrtDbgBreak();
        }
        else
        {

            // Leave space for ASSERTINTRO1 and "\r\n"
            if (szFormat)
            {
                int szlen;
                _ERRCHECK_SPRINTF(szlen = _vsnwprintf_s(szUserMessage, DBGRPT_MAX_MSG,
                                                        DBGRPT_MAX_MSG - 2 -
                                                        (max(sizeof(_CRT_WIDE(ASSERTINTRO1)),sizeof(_CRT_WIDE(ASSERTINTRO2)))/sizeof(wchar_t)),
                                                        szFormat,
                                                        arglist));
                if (szlen < 0)
                {
                    _ERRCHECK(wcscpy_s(szUserMessage, DBGRPT_MAX_MSG, _CRT_WIDE(DBGRPT_TOOLONGMSG)));
                }
            }

            if (_CRT_ASSERT == nRptType)
                _ERRCHECK(wcscpy_s(szLineMessage, DBGRPT_MAX_MSG, szFormat ? _CRT_WIDE(ASSERTINTRO1) : _CRT_WIDE(ASSERTINTRO2)));

            _ERRCHECK(wcscat_s(szLineMessage, DBGRPT_MAX_MSG, szUserMessage));

            if (_CRT_ASSERT == nRptType)
            {
                if (_CrtDbgMode[nRptType] & _CRTDBG_MODE_FILE)
                    _ERRCHECK(wcscat_s(szLineMessage, DBGRPT_MAX_MSG, L"\r"));
                {
                    _ERRCHECK(wcscat_s(szLineMessage, DBGRPT_MAX_MSG, L"\n"));
                }
            }

            if (szFile)
            {
                int szlen = 0;
                _ERRCHECK_SPRINTF(szlen = _snwprintf_s(szOutMessage, DBGRPT_MAX_MSG, DBGRPT_MAX_MSG, L"%s(%d) : %s",
                                                       szFile, nLine, szLineMessage));
                if (szlen < 0)
                    _ERRCHECK(wcscpy_s(szOutMessage, DBGRPT_MAX_MSG, _CRT_WIDE(DBGRPT_TOOLONGMSG)));
            }
            else
            {
                _ERRCHECK(wcscpy_s(szOutMessage, DBGRPT_MAX_MSG, szLineMessage));
            }

            /* scope */
            {
                errno_t e = 0;
                e = _ERRCHECK_EINVAL_ERANGE(wcstombs_s(NULL, szOutMessage2, DBGRPT_MAX_MSG, szOutMessage, _TRUNCATE));
                if(e != 0)
                    _ERRCHECK(strcpy_s(szOutMessage2, DBGRPT_MAX_MSG, DBGRPT_INVALIDMSG));
            }

            /* User hook may handle report.
               We have to check the ANSI Hook2 List & then the UNICODE Hook2 List.
               Then we have check any ANSI individual Hook set through
               SetReportHook */

            if (_pReportHookList || _pReportHookListW)
            {
                ReportHookNode *pnode=NULL;
                ReportHookNodeW *pnodeW=NULL;

                for (pnode = _pReportHookList; pnode; pnode = pnode->next)
                {
                    int hook_retval=0;
                    if ((*pnode->pfnHookFunc)(nRptType, szOutMessage2, &hook_retval))
                    {
                        retval=hook_retval;
                        handled=TRUE;
                        break;
                    }
                }

                if(!handled)
                {
                    for (pnodeW = _pReportHookListW; pnodeW; pnodeW = pnodeW->next)
                    {
                        int hook_retval=0;
                        if ((*pnodeW->pfnHookFunc)(nRptType, szOutMessage, &hook_retval))
                        {
                            retval=hook_retval;
                            handled=TRUE;
                            break;
                        }
                    }
                }
            }
            if(!handled)
            {
                if(_pfnReportHook)
                {
                    int hook_retval=0;
                    if(((*_pfnReportHook)(nRptType, szOutMessage2, &hook_retval)))
                    {
                        retval=hook_retval;
                        handled=TRUE;
                    }
                }

                if (!handled)
                {
                    if (_CrtDbgMode[nRptType] & _CRTDBG_MODE_FILE)
                    {
                        if (_CrtDbgFile[nRptType] != _CRTDBG_INVALID_HFILE)
                        {
                            /* Use WriteConsole for Consoles, WriteFile otherwise */
                            DWORD written;
                            switch(GetFileType(_CrtDbgFile[nRptType]))
                            {
                            case FILE_TYPE_CHAR:
                                if(WriteConsoleW(_CrtDbgFile[nRptType], szOutMessage, (unsigned long)wcslen(szOutMessage), &written, NULL))
                                    break;

                                /* If WriteConsole fails & LastError is ERROR_INVALID_VALUE,
                                   then the console is redirected */
                                if(GetLastError() != ERROR_INVALID_HANDLE)
                                    break;

                            default:
                            {
                                char szaOutMessage[DBGRPT_MAX_MSG];
                                size_t ret = 0;
                                errno_t e = _ERRCHECK_EINVAL_ERANGE(wcstombs_s(&ret, szaOutMessage, DBGRPT_MAX_MSG, szOutMessage, _TRUNCATE));

                                if(e != 0 && e != STRUNCATE)
                                {
                                    WriteFile(_CrtDbgFile[nRptType], szOutMessage, (unsigned long)wcslen(szOutMessage) * 2, &written, NULL);
                                }
                                else
                                {
                                    /* ret counts for the null terminator as well */
                                    if (ret > 0)
                                    {
                                        --ret;
                                    }
                                    WriteFile(_CrtDbgFile[nRptType], szaOutMessage, (unsigned long)ret, &written, NULL);
                                }
                            }
                            }
                        }
                    }

                    if (_CrtDbgMode[nRptType] & _CRTDBG_MODE_DEBUG)
                    {
                        OutputDebugStringW(szOutMessage);
                    }

                    if (_CrtDbgMode[nRptType] & _CRTDBG_MODE_WNDW)
                    {
                        szLineMessage[0] = 0;
                        if (nLine)
                        {
                            _ERRCHECK(_itow_s(nLine, szLineMessage, DBGRPT_MAX_MSG, 10));
                        }
                        retval = __crtMessageWindowW(nRptType, szFile, (nLine ? szLineMessage : NULL), szModule, szUserMessage);
                    }
                }
            }
        }

    }
    __finally
    {
        if (_CRT_ASSERT == nRptType)
        {
            _CrtInterlockedDecrement(&_crtAssertBusy);
        }
        _munlock(_DEBUG_LOCK);
    }

    return retval;
}
#pragma warning(pop)

#endif  /* _DEBUG */

