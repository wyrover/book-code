#include "error.h"
#include "string.h"
#include "format.h"
#include <sstream>

std::string cactus::GetErrorStringA(HRESULT hr, LPCSTR error)
{
    std::stringstream ss;
    ss << "0x" << std::hex << hr;

    if (NULL != error) {
        ss << " - " << error;
    }

    ss << ": " << cactus::FormatMessageFromHRA(hr);
    return ss.str();
}

std::string cactus::GetErrorStringA(DWORD dwErr, LPCSTR error)
{
    return GetErrorStringA(HRESULT_FROM_WIN32(dwErr), error);
}

std::string cactus::GetLastErrorStringA(LPCSTR error)
{
    return GetErrorStringA(GetLastError(), error);
}

std::wstring cactus::GetErrorStringW(HRESULT hr, LPCTSTR error, LPCTSTR dllname)
{
    std::wstringstream ss;
    ss << L"0x" << std::hex << hr;

    if (NULL != error) {
        ss << L" - " << error;
    }

    ss << L": " << cactus::FormatMessageFromHRW(hr, dllname);
    return ss.str();
}

std::wstring cactus::GetErrorStringW(DWORD dwErr, LPCTSTR error, LPCTSTR dllname)
{
    return GetErrorStringW(HRESULT_FROM_WIN32(dwErr), error, dllname);
}

std::wstring cactus::GetLastErrorStringW(LPCTSTR error)
{
    return GetErrorStringW(GetLastError(), error);
}
