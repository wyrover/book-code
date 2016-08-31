#include "environment_variable.h"
#include <vector>

//  AppendPathToEnvironment(LR"(C:\Program Files\Git\bin)");
bool AppendPathToEnvironment(const std::wstring &path)
{
    WCHAR buffer[32767];
    wcscpy_s(buffer, path.c_str());
    wcscat_s(buffer, L";");
    DWORD size = GetEnvironmentVariableW(L"PATH", buffer + path.size() + 1,
                                         32766 - path.size());

    if (size <= 0)
        return false;

    SetEnvironmentVariableW(L"PATH", buffer);
    return true;
}


std::wstring GetEnvironmentVariable(const std::wstring& name)
{
    DWORD size = ::GetEnvironmentVariableW(name.c_str(), NULL, 0);

    if (! size) {
        DWORD dwErr = ::GetLastError();

        // no such environment variable
        if (dwErr == ERROR_ENVVAR_NOT_FOUND)
            return L"";
    }

    std::vector<wchar_t> value;
    value.resize(size);
    size = ::GetEnvironmentVariableW(name.c_str(), & * value.begin(), value.size());
    return std::wstring(& * value.begin(), value.size() - 1);
}


std::wstring ExpandEnvironmentVariables(const std::wstring& s_in)
{
    std::wstring s(s_in);
    std::wstring::size_type i = 0, j = 0;

    while (((i = s.find(L"%", i)) != s.npos) && ((j = s.find(L"%", i + 1)) != s.npos)) {
        if (i + 1 != j) {
            std::wstring name = s.substr(i + 1, j - i - 1);
            std::wstring value = GetEnvironmentVariable(name);

            // if it's not an environment variable, just ignore it and let the command interpreter handle it
            if (value != L"") {
                s.replace(i, j - i + 1, value);
                i += value.length();
            } else {
                i = j + 1;
            }
        } else {
            i = j + 1;
        }
    }

    return s;
}