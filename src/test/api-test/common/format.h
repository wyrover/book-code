#ifndef format_h__
#define format_h__

#include <Windows.h>
#include <string>

namespace cactus
{
// format bytes
std::string FormatBytesA(ULONG bytes);
std::wstring FormatBytesW(ULONG bytes);
// format current date/time
std::string FormatCurrentDateTimeA(LPCSTR fmt = "%Y-%m-%d %H:%M:%S");
std::wstring FormatCurrentDateTimeW(LPCWSTR fmt = L"%Y-%m-%d %H:%M:%S");
// format a date/time
std::string FormatDateTimeA(__time64_t tt, LPCSTR fmt = "%Y-%m-%d %H:%M:%S");
std::wstring FormatDateTimeW(__time64_t tt, LPCWSTR fmt = L"%Y-%m-%d %H:%M:%S");
// format an ANSI message from an HRESULT and a specific DLL
std::string FormatMessageFromHRA(HRESULT hr, LPCSTR dllname = NULL);
// format an ANSI message from a Win32 error code
std::string FormatMessageFromLastErrorCodeA(DWORD);
// format a UNICODE message from an HRESULT and a specific DLL
std::wstring FormatMessageFromHRW(HRESULT hr, LPCWSTR dllname = NULL);
// format a UNICODE message from a Win32 error code
std::wstring FormatMessageFromLastErrorCodeW(DWORD);
// format a message with a list of arguments (printf-style)
std::string FormatMessageFromVArgs(char *, va_list&);
// format a message with a list of arguments (printf-style)
std::wstring FormatMessageFromVArgs(wchar_t *, va_list&);
// format a message with a list of arguments (printf-style)
std::string FormatMessage(char *, ...);
// format a message with a list of arguments (printf-style)
std::wstring FormatMessage(wchar_t *, ...);
}


#endif // format_h__
