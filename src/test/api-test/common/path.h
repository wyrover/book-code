#ifndef path_h__
#define path_h__

#include <Windows.h>
#include <string>

namespace cactus
{
// get the directory portion of a full path
std::string GetFileDirectoryA(const std::string& path);
std::wstring GetFileDirectoryW(const std::wstring& path);
// get the filename portion of a full path
std::string GetFileNameA(const std::string& path);
std::wstring GetFileNameW(const std::wstring& path);
// get the full path (directory) of the module's filename
std::string GetModuleDirectoryA();
std::wstring GetModuleDirectoryW();
// get the full path to the module's filename
std::string GetModuleFileNameA(HINSTANCE h = NULL);
std::wstring GetModuleFileNameW(HINSTANCE h = NULL);
// combine two directory paths
std::wstring DirectoryCombine(const std::wstring& dir, const std::wstring& file);
// strip trailing backslash except for drive:
std::wstring StripPathTerminator(const std::wstring& path);
// get the location of the temporary directory
std::string GetTemporaryDirectoryA();
std::wstring GetTemporaryDirectoryW();
//  get the current directory
std::string GetCurrentDirectoryA();
std::wstring GetCurrentDirectoryW();
// get the system directory
std::string GetSystemDirectoryA();
std::wstring GetSystemDirectoryW();
// get the windows directory
std::string GetWindowsDirectoryA();
std::wstring GetWindowsDirectoryW();
// get the windows directory (system)
std::string GetSystemWindowsDirectoryA();
std::wstring GetSystemWindowsDirectoryW();

std::wstring GetFullPathName(const std::wstring & path);

}

#endif // path_h__
