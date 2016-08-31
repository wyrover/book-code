#include "path.h"
#include "string.h"
#include <Shlwapi.h>




std::string cactus::GetFileDirectoryA(const std::string& path)
{
    if (startswith(path, "file://"))
        return "file://" + GetFileDirectoryA(path.substr(ARRAYSIZE("file://") - 1));

    int backslashPos = path.rfind('\\', path.length());

    if (backslashPos == path.npos) backslashPos = path.rfind('/', path.length());

    if (backslashPos == path.npos)
        return "";

    return path.substr(0, backslashPos + 1);
}

std::wstring cactus::GetFileDirectoryW(const std::wstring& path)
{
    if (startswith(path, L"file://"))
        return L"file://" + GetFileDirectoryW(path.substr(ARRAYSIZE(L"file://") - 1));

    std::wstring::size_type backslashPos = path.rfind(L'\\', path.length());

    if (backslashPos == path.npos) backslashPos = path.rfind(L'/', path.length());

    if (backslashPos == path.npos)
        return L"";

    return path.substr(0, backslashPos + 1);
}

std::string cactus::GetFileNameA(const std::string& path)
{
    int backslashPos = path.rfind('\\', path.length());

    if (backslashPos == path.npos) backslashPos = path.rfind('/', path.length());

    if (backslashPos == path.npos)
        return path;

    return path.substr(backslashPos + 1, path.length() - backslashPos - 1);
}

std::wstring cactus::GetFileNameW(const std::wstring& path)
{
    std::wstring::size_type backslashPos = path.rfind(L'\\', path.length());

    if (backslashPos == path.npos) backslashPos = path.rfind(L'/', path.length());

    if (backslashPos == path.npos)
        return path;

    return path.substr(backslashPos + 1, path.length() - backslashPos - 1);
}

std::string cactus::GetModuleDirectoryA()
{
    return GetFileDirectoryA(GetModuleFileNameA());
}

std::wstring cactus::GetModuleDirectoryW()
{
    return GetFileDirectoryW(GetModuleFileNameW());
}

std::string cactus::GetModuleFileNameA(HINSTANCE h)
{
    char moduleFileName[MAX_PATH] = { 0 };
    ::GetModuleFileNameA(h, moduleFileName, MAX_PATH);
    return moduleFileName;
}

std::wstring cactus::GetModuleFileNameW(HINSTANCE h)
{
    wchar_t moduleFileName[MAX_PATH] = { 0 };
    ::GetModuleFileNameW(h, moduleFileName, MAX_PATH);
    return moduleFileName;
}

std::wstring cactus::DirectoryCombine(const std::wstring& dir, const std::wstring& file)
{
    wchar_t buffer[MAX_PATH] = { 0 };
    ::PathCombineW(buffer, dir.length() ? dir.c_str() : NULL, file.length() ? file.c_str() : NULL);
    return buffer;
}

std::string cactus::GetTemporaryDirectoryA()
{
    char td[MAX_PATH] = { 0 };
    ::GetTempPathA(MAX_PATH, td);
    return td;
}

std::wstring cactus::GetTemporaryDirectoryW()
{
    wchar_t td[MAX_PATH] = { 0 };
    ::GetTempPathW(MAX_PATH, td);
    return td;
}

std::string cactus::GetCurrentDirectoryA()
{
    char result[MAX_PATH] = { 0 };
    ::GetCurrentDirectoryA(MAX_PATH, result);
    return result;
}

std::wstring cactus::GetCurrentDirectoryW()
{
    wchar_t result[MAX_PATH];
    ::GetCurrentDirectoryW(MAX_PATH, result);
    return result;
}

std::string cactus::GetSystemDirectoryA()
{
    char td[MAX_PATH];
    ::GetSystemDirectoryA(td, MAX_PATH);
    return td;
}

std::wstring cactus::GetSystemDirectoryW()
{
    wchar_t td[MAX_PATH] = { 0 };
    ::GetSystemDirectoryW(td, MAX_PATH);
    return td;
}

std::string cactus::GetWindowsDirectoryA()
{
    char td[MAX_PATH] = { 0 };
    ::GetWindowsDirectoryA(td, MAX_PATH);
    return td;
}

std::wstring cactus::GetWindowsDirectoryW()
{
    wchar_t td[MAX_PATH] = { 0 };
    ::GetWindowsDirectoryW(td, MAX_PATH);
    return td;
}

std::string cactus::GetSystemWindowsDirectoryA()
{
    return wstring2string(GetSystemWindowsDirectoryW());
}

std::wstring cactus::GetSystemWindowsDirectoryW()
{
    wchar_t td[MAX_PATH] = { 0 };
    ::GetSystemWindowsDirectoryW(td, MAX_PATH);
    return td;
}

std::wstring cactus::GetFullPathName(const std::wstring & path)
{
    LPTSTR szFilePart;
    wchar_t buffer[MAX_PATH + 1];
    ::GetFullPathName(path.c_str(), MAX_PATH, buffer, &szFilePart);
    return std::wstring(buffer);
}

std::wstring cactus::StripPathTerminator(const std::wstring& path)
{
    std::wstring result = trimright(path, L" \t\n\r\\/");

    if (result.length() == 2 && result[1] == L':') result += L"\\";

    return result;
}

