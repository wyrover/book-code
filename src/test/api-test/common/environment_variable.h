#ifndef environment_variable_h__
#define environment_variable_h__

#include <Windows.h>
#include <string>

bool AppendPathToEnvironment(const std::wstring &path);

std::wstring GetEnvironmentVariable(const std::wstring& name);
std::wstring ExpandEnvironmentVariables(const std::wstring& s_in);
#endif // environment_variable_h__
