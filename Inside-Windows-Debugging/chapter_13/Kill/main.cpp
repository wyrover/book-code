#include "stdafx.h"

class CMainApp
{
public:
    HRESULT
    MainHR(
        __in int argc,
        __in_ecount(argc) WCHAR* argv[]
        )
    {
        DWORD dwProcessId;

        ChkProlog();

        if (argc == 1)
        {
            wprintf(L"USAGE: kill.exe [PID]\n");
            ChkReturn(E_INVALIDARG);
        }
        else
        {
            ChkHr(ConvertString2Number(
                argv[1],
                &dwProcessId
                ));

            ChkHr(KillProcess(dwProcessId));
        }

        ChkNoCleanup();
    }

private:
    static
    HRESULT
    KillProcess(
        __in DWORD dwProcessId
        )
    {
        CHandle shProcess;

        ChkProlog();

        shProcess.Attach(::OpenProcess(
            PROCESS_TERMINATE,
            FALSE,
            dwProcessId
            ));
        ChkWin32(shProcess);

        ChkWin32(::TerminateProcess(
            shProcess,
            0
            ));

        ChkNoCleanup();
    }

    static
    HRESULT
    ConvertString2Number(
        __in LPCWSTR pwszStr,
        __out DWORD* pnNumber
        )
    {
        BOOL bHex;
        DWORD n, nTmp;
        LPCWSTR pwsz;

        ChkProlog();

        ASSERT(pnNumber);
        ASSERT(pwszStr);

        pwsz = pwszStr;

        //
        // Get to the end of the string
        //
        while (*pwsz++)
            ;

        pwsz--;

        ChkArg(pwsz - pwszStr > 0);

        bHex = FALSE;
        if (pwsz - pwszStr >= 2)
        {
            if (pwszStr[0] == L'0' && (pwszStr[1] == L'x' || pwszStr[1] == L'X'))
            {
                bHex = TRUE;
                pwszStr += 2;
            }
        }

        n = 0;

        if (bHex)
        {
            while (pwszStr < pwsz)
            {
                nTmp = n * 16;
                if (nTmp / 16 != n)
                {
                    ChkReturn(HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW));
                }
                n = nTmp;

                if (*pwszStr >= 'a' && *pwszStr <= 'f')
                {
                    nTmp = n + (*pwszStr - 'a' + 10);
                }
                else if (*pwszStr >= 'A' && *pwszStr <= 'F')
                {
                    nTmp = n + (*pwszStr - 'A' + 10);
                }
                else if (*pwszStr >= '0' && *pwszStr <= '9')
                {
                    nTmp = n + (*pwszStr - '0');
                }
                else
                {
                    ChkReturn(E_INVALIDARG);
                }

                if (nTmp < n)
                {
                    ChkReturn(HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW));
                }
                n = nTmp;

                pwszStr++;
            }
        }
        else
        {
            //
            // Assume decimal
            //
            while (pwszStr < pwsz)
            {
                nTmp = n * 10;
                if (nTmp / 10 != n)
                {
                    ChkReturn(HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW));
                }
                n = nTmp;

                if (*pwszStr >= '0' && *pwszStr <= '9')
                {
                    nTmp = n + (*pwszStr - '0');
                    if (nTmp < n)
                    {
                        ChkReturn(HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW));
                    }
                    n = nTmp;
                }
                else
                {
                    ChkReturn(E_INVALIDARG);
                }

                pwszStr++;
            }
        }

        *pnNumber = n;

        ChkNoCleanup();
    }
};

int
__cdecl
wmain(
    __in int argc,
    __in_ecount(argc) WCHAR* argv[]
    )
{
    HRESULT hr;
    CMainApp app;

    hr = app.MainHR(argc, argv);
    if (FAILED(hr))
    {
        wprintf(L"HRESULT: 0x%08X\n", hr);
    }
    else
    {
        wprintf(L"Success.\n");
    }
    return 0;
}
