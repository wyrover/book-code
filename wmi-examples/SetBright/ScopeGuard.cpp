// see `file_header_text.txt` for licensing & contact info. If you can't find that file, then assume you're NOT allowed to do whatever you wanted to do.

#include "ScopeGuard.h"
#include <strsafe.h>
#include <exception>

#ifndef SCOPEGUARD_CPP_INCLUDED
#define SCOPEGUARD_CPP_INCLUDED

//intentionally NOT defined as part of ScopeGuard, to reduce code duplication. //Also, produces cleaner `TRACE` output.
#ifdef DEBUG
void trace_out(_In_z_ PCSTR const func_expr, _In_z_ PCSTR const file_name, _In_z_ PCSTR const func_name, _In_ _In_range_(0, INT_MAX) const int line_number)
{
#ifdef SCOPE_GUARD_DEBUGGING
    const rsize_t bufferSize = 512u;
    wchar_t outputBuffer[ bufferSize ] = { 0 };
    const HRESULT fmt_res = StringCchPrintfW(
                                outputBuffer,
                                bufferSize,
                                //L"NAME_OF_YOUR_APP_HERE:\t"
                                L"Scope guard triggered!"
                                L"\r\n\t\tScope guard initialized in file: `%S`,"
                                L"\r\n\t\tfunction:                        `%S`,"
                                L"\r\n\t\tline:                            `%i`,"
                                L"\r\n\t\tActual expression:"
                                L"\r\n%S\r\n",
                                file_name,
                                func_name,
                                line_number,
                                func_expr);

    if (SUCCEEDED(fmt_res)) {
        OutputDebugStringW(outputBuffer);
        return;
    }

    if (fmt_res == STRSAFE_E_INSUFFICIENT_BUFFER) {
        OutputDebugStringA("Failed to trace ScopeGuard information - buffer was too small!\r\n");
        return;
    }

    if (fmt_res == STRSAFE_E_INVALID_PARAMETER) {
        OutputDebugStringA("Failed to trace ScopeGuard information - INVALID PARAMETER!!\r\n");
        std::terminate();
    }

    OutputDebugStringA("Failed to trace ScopeGuard information - Unexpected error!\r\n");
    std::terminate();
    //TRACE( L"Scope guard triggered!"
    //      L"\r\n\t\tScope guard initialized in file: `%S`,"
    //      L"\r\n\t\tfunction:                        `%S`,"
    //      L"\r\n\t\tline:                            `%i`\r\n", file_name, func_name, line_number );
#else
    UNREFERENCED_PARAMETER(file_name);
    UNREFERENCED_PARAMETER(func_name);
    UNREFERENCED_PARAMETER(line_number);
    UNREFERENCED_PARAMETER(func_expr);
#endif
}
#endif

#else

#endif