#ifndef error_h__
#define error_h__

#include <Windows.h>
#include <string>

namespace cactus
{
//! Exception from GetLastError
std::string GetLastErrorStringA(LPCSTR error = NULL);
//! Exception from an error code
std::string GetErrorStringA(DWORD dwErr, LPCSTR error = NULL);
//! Exception from an HRESULT
std::string GetErrorStringA(HRESULT hr, LPCSTR error = NULL);
//! Exception from GetLastError
std::wstring GetLastErrorStringW(LPCTSTR error = NULL);
//! Exception from an error code
std::wstring GetErrorStringW(DWORD dwErr, LPCTSTR error = NULL, LPCTSTR dllname = NULL);
//! Exception from an HRESULT
std::wstring GetErrorStringW(HRESULT hr, LPCTSTR error = NULL, LPCTSTR dllname = NULL);
}


#endif // error_h__
