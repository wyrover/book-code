#ifndef inf_utils_h__
#define inf_utils_h__

#include <Windows.h>
#include <string>
#include <set>
#include <vector>

typedef wchar_t char16;
typedef std::wstring                        string16;

typedef void (WINAPI *EnumCallback)(const std::wstring& path, void* param);

namespace cactus
{

void enumFile(const std::wstring& searchPath, EnumCallback calllback, void* param);
std::wstring GetFullPathName(const std::wstring & path);
std::wstring get_systemroot_inf_dir();
string16 IncludeTrailingPathDelimiter(const string16& S);
string16 ExtractFileName(const string16& FileName);
string16 ExtractFileExt(const string16& FileName);
BOOL IsWow64();
string16 get_pnputil_exe_fullpath();
BOOL getRunExeOutput(const std::wstring& exe_filename, const std::wstring& args, DWORD* out_exitcode, std::wstring& result);
int runExe(const std::wstring& exe_filename, const std::wstring& args);
void enum_device_oem_infs(std::set<string16>& infset);
void get_difference_infs(std::vector<string16>& ivec);
void get_difference_infs2(std::vector<string16>& ivec);

} // namespace cactus


#endif // inf_utils_h__
