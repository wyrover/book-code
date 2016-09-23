#include "file.h"
#include "path.h"
#include "string.h"
#include "format.h"
#include <Shlwapi.h>
#include <string>
#include <sstream>

BOOL CreateDeepDirectory(const wchar_t* dir_path)
{
    BOOL bRetCode = FALSE;
    wchar_t path[MAX_PATH] = { 0 };
    //StringCchCopy(path, MAX_PATH, dir_path);
    wcscpy_s(path, MAX_PATH, dir_path);

    do {
        if (GetFileAttributes(dir_path) != INVALID_FILE_ATTRIBUTES) {
            bRetCode = TRUE;
            break;
        }

        bRetCode = ::CreateDirectory(dir_path, NULL);

        if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS) {
            PathRemoveFileSpecW(path);

            if (wcslen(path) == 0) {
                bRetCode = FALSE;
                break;
            }

            bRetCode = CreateDeepDirectory(path);

            if (!bRetCode) {
                bRetCode = FALSE;
                break;
            }

            bRetCode = ::CreateDirectory(dir_path, NULL);

            if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS) {
                bRetCode = FALSE;
                break;
            }
        }
    } while (0);

    return bRetCode;
}


std::wstring GetAppDir()
{
    wchar_t filename[MAX_PATH] = { 0 };
    ::GetModuleFileName(NULL, filename, _countof(filename));
    ::PathRemoveFileSpec(filename);
    ::PathAddBackslash(filename);
    return filename;
}

void write_utf8_text_file(const std::wstring& filename, std::string data)
{
    FILE * f = _wfopen(filename.c_str(), L"wb+");

    if (f) {
        unsigned char UTF8_BOM[3] = { 0xEF, 0xBB, 0xBF };
        fwrite(UTF8_BOM, 1, 3, f);
        fwrite(data.c_str(), 1, data.length(), f);
        fclose(f);
    }
}


bool cactus::FileExists(const std::string& filename)
{
    return FileExists(string2wstring(filename));
}

bool cactus::FileExists(const std::wstring& filename)
{
    DWORD dwAttr = GetFileAttributesW(filename.c_str());

    if (dwAttr == INVALID_FILE_ATTRIBUTES) {
        DWORD dwErr = GetLastError();

        switch (dwErr) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
            return false;

        default:
            return false;
        }
    } else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
        return false;
    }

    return true;
}

std::string cactus::GetTemporaryFileNameA()
{
    char tf[MAX_PATH];
    GetTempFileNameA(GetTemporaryDirectoryA().c_str(), "DV", 0, tf);
    return tf;
}

std::wstring cactus::GetTemporaryFileNameW()
{
    wchar_t tf[MAX_PATH];
    GetTempFileNameW(GetTemporaryDirectoryW().c_str(), L"DV", 0, tf);
    return tf;
}

void cactus::FileDelete(const std::string& filename)
{
    ::DeleteFileA(filename.c_str());
}

void cactus::FileDelete(const std::wstring& filename)
{
    ::DeleteFileW(filename.c_str());
}

void cactus::FileCopy(const std::string& from, const std::string& to, bool overwrite)
{
    ::CopyFileA(from.c_str(), to.c_str(), overwrite);
}

void cactus::FileCopy(const std::wstring& from, const std::wstring& to, bool overwrite)
{
    ::CopyFileW(from.c_str(), to.c_str(), overwrite);
}

void cactus::FileMove(const std::string& from, const std::string& to)
{
    if (FileExists(to))
        FileDelete(to);

    ::MoveFileA(from.c_str(), to.c_str());
}

void cactus::FileMove(const std::wstring& from, const std::wstring& to)
{
    if (FileExists(to))
        FileDelete(to);

    ::MoveFileW(from.c_str(), to.c_str());
}

long cactus::GetFileSize(const std::wstring& filename)
{
    WIN32_FILE_ATTRIBUTE_DATA attr = { 0 };
    GetFileAttributesExW(filename.c_str(), GetFileExInfoStandard, & attr);
    /*CHECK_BOOL(0 == attr.nFileSizeHigh,
        L"File " << filename << L" is > 2GB (" << attr.nFileSizeHigh << ")");*/
    return (long) attr.nFileSizeLow;
}

long cactus::GetFileSize(const std::string& filename)
{
    return GetFileSize(cactus::string2wstring(filename));
}

std::vector<char> cactus::FileReadToEnd(const std::wstring& filename)
{
    std::vector<char> data;
    long size = GetFileSize(filename);

    if (size > 0) {
        HANDLE hFile = ::CreateFile(filename.c_str(), GENERIC_READ, 0,
                                    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        data.resize(size);
        DWORD dwRead = 0;
        ::ReadFile(hFile, & * data.begin(), size, & dwRead, NULL);

        if (hFile)
            ::CloseHandle(hFile);
    }

    return data;
}

void cactus::FileWrite(
    const std::wstring& filename,
    const std::vector<char>& data,
    DWORD dwShareMode, // GENERIC_READ | GENERIC_WRITE
    DWORD dwCreationDisposition, // CREATE_ALWAYS
    DWORD dwFlagsAndAttributes) // FILE_ATTRIBUTE_NORMAL)
{
    HANDLE hFile = ::CreateFile(filename.c_str(), dwShareMode, 0,
                                NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);

    if (data.size() > 0) { // empty files are ok
        DWORD dwWritten = 0;
        ::WriteFile(hFile, & * data.begin(), data.size(), & dwWritten, NULL);
    }

    if (hFile)
        ::CloseHandle(hFile);
}

void cactus::FileCreate(
    const std::wstring& filename,
    DWORD dwDesiredAccess, // GENERIC_READ | GENERIC_WRITE
    DWORD dwShareMode, // 0
    DWORD dwCreationDisposition, // CREATE_ALWAYS
    DWORD dwFlagsAndAttributes) // FILE_ATTRIBUTE_NORMAL)
{
    HANDLE hFile = ::CreateFile(filename.c_str(), dwDesiredAccess, dwShareMode,
                                NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);

    if (hFile)
        ::CloseHandle(hFile);
}

cactus::FileVersionInfo cactus::GetFileVersionInfo(const std::wstring& filename)
{
    cactus::FileVersionInfo result = { 0 };
    DWORD dwVerHnd = 0;
    DWORD dwVerInfoSize = ::GetFileVersionInfoSize(filename.c_str(), & dwVerHnd);
    /*CHECK_WIN32_BOOL(dwVerInfoSize != 0,
        L"GetFileVersionInfoSize(" << filename << L")");*/
    std::vector<byte> versioninfo_data;
    versioninfo_data.resize(dwVerInfoSize);
    dwVerInfoSize = versioninfo_data.size();
    ::GetFileVersionInfo(filename.c_str(), dwVerHnd, dwVerInfoSize, & * versioninfo_data.begin());
    // VS_FIXEDFILEINFO
    UINT fixed_len = 0;
    VS_FIXEDFILEINFO * lpvi = NULL;
    ::VerQueryValueW(& * versioninfo_data.begin(), L"\\", reinterpret_cast<LPVOID *>(& lpvi), & fixed_len);
    result.fixed_info = * lpvi;
    // translation info
    UINT translation_len = 0;
    TranslationInfo * lpti = NULL;
    ::VerQueryValueW(& * versioninfo_data.begin(), L"\\VarFileInfo\\Translation", reinterpret_cast<LPVOID *>(& lpti), & translation_len);
    /*CHECK_BOOL(translation_len <= sizeof(TranslationInfo),
        L"VerQueryValue(" << filename << L", \"\\VarFileInfo\\Translation\"): invalid size");*/
    result.translation_info = * lpti;
    return result;
}

std::wstring cactus::GetFileVersion(const std::wstring& filename)
{
    cactus::FileVersionInfo versioninfo = cactus::GetFileVersionInfo(filename);
    std::wstringstream version;
    version << HIWORD(versioninfo.fixed_info.dwFileVersionMS) << L"."
            << LOWORD(versioninfo.fixed_info.dwFileVersionMS) << L"."
            << HIWORD(versioninfo.fixed_info.dwFileVersionLS) << L"."
            << LOWORD(versioninfo.fixed_info.dwFileVersionLS);
    return version.str();
}

bool cactus::ResourceExists(HMODULE h, const std::wstring& resource, const std::wstring& type)
{
    return (NULL != ::FindResource(h, resource.c_str(), type.c_str()));
}

cactus::FileVersion cactus::wstring2fileversion(const std::wstring& version)
{
    FileVersion result = { 0, 0, 0, 0 };

    for (size_t i = 0; i < version.length(); i++) {
        /*CHECK_BOOL(version[i] == '.' || isdigit(version[i]),
            L"Invalid version format: '" << version << L"'");*/
    }

    if (! version.empty()) {
        std::vector<std::wstring> parts_l = cactus::split(version, L".", 4);
        result.major = ((parts_l.size() >= 1) ? cactus::wstring2long(parts_l[0]) : 0);
        result.minor = ((parts_l.size() >= 2) ? cactus::wstring2long(parts_l[1]) : 0);
        result.build = ((parts_l.size() >= 3) ? cactus::wstring2long(parts_l[2]) : 0);
        result.rev = ((parts_l.size() == 4) ? cactus::wstring2long(parts_l[3]) : 0);
    }

    return result;
}

std::wstring cactus::fileversion2wstring(const FileVersion& version)
{
    return cactus::FormatMessageW(L"%d.%d.%d.%d",
                                  version.major, version.minor, version.build, version.rev);
}

int cactus::CompareVersion(const std::wstring& l, const std::wstring& r)
{
    FileVersion l_v = wstring2fileversion(l);
    FileVersion r_v = wstring2fileversion(r);

    if (l_v.major < r_v.major)
        return -1;
    else if (l_v.major > r_v.major)
        return 1;

    if (l_v.minor < r_v.minor)
        return -1;
    else if (l_v.minor > r_v.minor)
        return 1;

    if (l_v.build < r_v.build)
        return -1;
    else if (l_v.build > r_v.build)
        return 1;

    if (l_v.rev < r_v.rev)
        return -1;
    else if (l_v.rev > r_v.rev)
        return 1;

    return 0;
}

void cactus::enumFile(const std::wstring& searchPath, cactus::EnumCallback callback, void* param)
{
    WIN32_FIND_DATA fd;
    HANDLE findFile = ::FindFirstFile((searchPath + L"*").c_str(), &fd);

    if (findFile == INVALID_HANDLE_VALUE) {
        ::FindClose(findFile);
        return;
    }

    std::wstring fullpath = GetFullPathName(searchPath);

    while (true) {
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            //file
            callback(fullpath + fd.cFileName, param);
        } else if (wcscmp(fd.cFileName, L".") != 0 && wcscmp(fd.cFileName, L"..")  != 0) {
            //folder
            enumFile(searchPath + fd.cFileName + L"/", callback, param);
        }

        if (!FindNextFile(findFile, &fd)) {
            ::FindClose(findFile);
            return;
        }
    }

    ::FindClose(findFile);
}

std::vector<BYTE> cactus::ReadFileToMem( LPCWSTR name )
{
	FILE* fp;
	std::vector<unsigned char> ret;

	if (_wfopen_s(&fp, name, L"rb") == 0)
	{
		fseek(fp, 0, SEEK_END);

		ret.resize(ftell(fp));
		fseek(fp, 0, SEEK_SET);

		fread(&ret[0], 1, ret.size(), fp);

		fclose(fp);
	}

	return ret;
}
