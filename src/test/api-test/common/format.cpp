#include "format.h"
#include "string.h"
#include <vector>
#include <sstream>
#include <time.h>

namespace cactus
{
std::string FormatMessageFromHRA(HRESULT hr, LPCSTR dllname)
{
    std::string result;
    DWORD rc = 0;
    HMODULE dllhandle = NULL;

    if (dllname != NULL) {
        dllhandle = ::LoadLibraryA(dllname);
    }

    std::vector<char> msgBuffer(1024);
    rc = ::FormatMessageA(
             FORMAT_MESSAGE_ALLOCATE_BUFFER |
             ((dllhandle != NULL) ? FORMAT_MESSAGE_FROM_HMODULE : FORMAT_MESSAGE_FROM_SYSTEM),
             dllhandle,
             hr,
             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
             reinterpret_cast<LPSTR>(&msgBuffer[0]),
             0,
             NULL);

    if (rc != 0) {
        result = trim(reinterpret_cast<LPCSTR>(&msgBuffer[0]));
    } else {
        std::stringstream result_s;
        result_s << "0x" << std::hex << hr;
        result = result_s.str();
    }

    if (dllhandle != NULL) {
        ::FreeLibrary(dllhandle);
    }

    return result;
}

std::wstring FormatMessageFromHRW(HRESULT hr, LPCWSTR dllname)
{
    std::wstring result;
    DWORD rc = 0;
    HMODULE dllhandle = NULL;

    if (dllname != NULL) {
        dllhandle = ::LoadLibraryW(dllname);
    }

    std::vector<wchar_t> msgBuffer(1024);
    rc = ::FormatMessageW(
             FORMAT_MESSAGE_ALLOCATE_BUFFER |
             ((dllhandle != NULL) ? FORMAT_MESSAGE_FROM_HMODULE : FORMAT_MESSAGE_FROM_SYSTEM),
             dllhandle,
             hr,
             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
             reinterpret_cast<LPWSTR>(&msgBuffer[0]),
             0,
             NULL);

    if (rc != 0) {
        result = trim(reinterpret_cast<LPCWSTR>(&msgBuffer[0]));
    } else {
        std::wstringstream result_s;
        result_s << L"0x" << std::hex << hr;
        result = result_s.str();
    }

    if (dllhandle != NULL) {
        ::FreeLibrary(dllhandle);
    }

    return result;
}

std::wstring FormatMessageFromLastErrorCodeW(const DWORD errorCode)
{
    return FormatMessageFromHRW(HRESULT_FROM_WIN32(errorCode));
}

std::string FormatMessageFromLastErrorCodeA(const DWORD errorCode)
{
    return FormatMessageFromHRA(HRESULT_FROM_WIN32(errorCode));
}

std::string FormatMessageFromVArgs(char * fmt, va_list& args)
{
    std::string result;
    int len = 0;
    result.resize(24);

    while ((len = vsnprintf_s(& * result.begin(), result.size(), result.size() - 1, fmt, args)) < 0) {
        result.resize(result.size() * 2);
    }

    result.resize(len);
    return result;
}

std::wstring FormatMessageFromVArgs(wchar_t * fmt, va_list& args)
{
    std::wstring result;
    int len = 0;
    result.resize(24);

    while ((len = _vsnwprintf_s(& * result.begin(), result.size(), result.size() - 1, fmt, args)) < 0) {
        result.resize(result.size() * 2);
    }

    result.resize(len);
    return result;
}

std::string FormatMessage(char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string result = FormatMessageFromVArgs(fmt, args);
    va_end(args);
    return result;
}

std::wstring FormatMessage(wchar_t * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::wstring result = FormatMessageFromVArgs(fmt, args);
    va_end(args);
    return result;
}

std::string FormatCurrentDateTimeA(LPCSTR fmt)
{
    __time64_t tt;
    _time64(& tt);
    return FormatDateTimeA(tt, fmt);
}

std::wstring FormatCurrentDateTimeW(LPCWSTR fmt)
{
    __time64_t tt;
    _time64(& tt);
    return FormatDateTimeW(tt, fmt);
}

std::string FormatDateTimeA(__time64_t tt, LPCSTR fmt)
{
    struct tm tm = { 0 };
    _localtime64_s(& tm, & tt);
    char buffer[128];
    strftime(buffer, ARRAYSIZE(buffer), fmt, & tm);
    return buffer;
}

std::wstring FormatDateTimeW(__time64_t tt, LPCWSTR fmt)
{
    struct tm tm = { 0 };
    _localtime64_s(& tm, & tt);
    wchar_t buffer[128];
    wcsftime(buffer, ARRAYSIZE(buffer), fmt, & tm);
    return buffer;
}

// same as StrFormatByteSize (which is not supported on Windows 95)
std::string FormatBytesA(ULONG bytes)
{
    if (bytes == 1) // bytes
        return FormatMessage("%lu byte", bytes);
    else if (bytes < 1024) // bytes
        return FormatMessage("%lu bytes", bytes);
    else if (bytes < 1048576 && bytes % 1024 == 0) // Kb
        return FormatMessage("%.0fKB", (double) bytes / 1024);
    else if (bytes < 1048576) // Kb
        return FormatMessage("%.1fKB", (double) bytes / 1024);
    else if (bytes % 1048576 == 0 && bytes < 1073741824) // Mb
        return FormatMessage("%.0fMB", (double) bytes / 1048576);
    else if (bytes < 1073741824) // Mb
        return FormatMessage("%.1fMB", (double) bytes / 1048576);
    else if (bytes % 1073741824 == 0 && bytes < 1099511627776)  // GB
        return FormatMessage("%.0fGB", (double) bytes / 1073741824);
    else if (bytes < 1099511627776)  // GB
        return FormatMessage("%.1fGB", (double) bytes / 1073741824);
    else
        return FormatMessage("%lu bytes", bytes);
}

std::wstring FormatBytesW(ULONG bytes)
{
    if (bytes == 1) // bytes
        return FormatMessage(L"%lu byte", bytes);
    else if (bytes < 1024) // bytes
        return FormatMessage(L"%lu bytes", bytes);
    else if (bytes < 1048576 && bytes % 1024 == 0) // Kb
        return FormatMessage(L"%.0fKB", (double) bytes / 1024);
    else if (bytes < 1048576) // Kb
        return FormatMessage(L"%.1fKB", (double) bytes / 1024);
    else if (bytes % 1048576 == 0 && bytes < 1073741824) // Mb
        return FormatMessage(L"%.0fMB", (double) bytes / 1048576);
    else if (bytes < 1073741824) // Mb
        return FormatMessage(L"%.1fMB", (double) bytes / 1048576);
    else if (bytes % 1073741824 == 0 && bytes < 1099511627776)  // GB
        return FormatMessage(L"%.0fGB", (double) bytes / 1073741824);
    else if (bytes < 1099511627776)  // GB
        return FormatMessage(L"%.1fGB", (double) bytes / 1073741824);
    else
        return FormatMessage(L"%lu bytes", bytes);
}

}