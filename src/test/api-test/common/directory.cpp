#include "directory.h"
#include "string.h"
#include "path.h"

std::wstring cactus::DirectoryNormalize(const std::wstring& path)
{
    return cactus::replace(path, L"\\\\", L"\\");
}

bool cactus::DirectoryExists(const std::string& path)
{
    return DirectoryExists(cactus::string2wstring(path));
}

bool cactus::DirectoryExists(const std::wstring& full_path)
{
    std::wstring path = DirectoryNormalize(full_path);

    if (path.empty())
        return false;

    DWORD dwAttr = GetFileAttributesW(path.c_str());

    if (dwAttr == INVALID_FILE_ATTRIBUTES) {
        DWORD dwErr = GetLastError();

        switch (dwErr) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
            return false;

        default:
            return false;
            break;
        }
    } else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    }

    return false;
}

std::string cactus::DirectoryCreate(const std::string& path)
{
    return cactus::wstring2string(
               DirectoryCreate(cactus::string2wstring(path)));
}

std::wstring cactus::DirectoryCreate(const std::wstring& full_path)
{
    std::wstring result;
    std::wstring path = DirectoryNormalize(full_path);
    std::vector<std::wstring> parts = cactus::split(path, L"\\");
    std::wstring current;

    for each(std::wstring part in parts) {
        // avoid double-slashes, bug 4378
        if (part.empty())
            continue;

        if (current.length() > 0) current.append(L"\\");

        current.append(part);

        if (! DirectoryExists(current)) {
            ::CreateDirectoryW(current.c_str(), NULL);

            if (result.empty()) result = current;
        }
    }

    return result;
}

bool cactus::DirectoryDelete(const std::string& path, int flags)
{
    return DirectoryDelete(cactus::string2wstring(path), flags);
}

bool cactus::DirectoryDelete(const std::wstring& full_path, int flags)
{
    std::wstring path = DirectoryNormalize(full_path);
    /*CHECK_BOOL(flags > 0,
        L"Missing flags");*/
    WIN32_FIND_DATAW data = { 0 };
    bool directory_empty = true;
    bool directory_exists = false;
    std::wstring wildcard = cactus::trim(path, L"\\");
    wildcard.append(L"\\*");
    HANDLE hFile = ::FindFirstFileW(wildcard.c_str(), & data); // find the first file

    if (hFile != INVALID_HANDLE_VALUE) {
        directory_exists = true;

        do {
            std::wstring filename = data.cFileName;

            if (filename == L"." || filename == L"..")
                continue;

            std::wstring sub = cactus::trimright(path, L"\\");
            sub.append(L"\\");
            sub.append(filename);

            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if ((flags & DELETE_DIRECTORY_SUBDIRECTORIES) > 0) {
                    directory_empty &= DirectoryDelete(sub, flags);
                }
            } else if ((flags & DELETE_DIRECTORY_FILES) > 0) {
                ::DeleteFileW(sub.c_str());
            } else {
                directory_empty = false;
            }
        } while (FindNextFileW(hFile, & data));
    }

    switch (DWORD rc = ::GetLastError()) {
    case ERROR_PATH_NOT_FOUND: // directory not found in FindFirstFile
    case ERROR_FILE_NOT_FOUND: // file not found in FindFirstFile
    case ERROR_NO_MORE_FILES: // more more files in FindNextFile
        break;

    default:
        break;
        //CHECK_WIN32_DWORD(rc, "Error in FindFirst/NextFile");
    }

    if (hFile)
        ::CloseHandle(hFile);

    if (directory_exists && directory_empty && ((flags & DELETE_DIRECTORY_FOLDERS) > 0)) {
        ::RemoveDirectoryW(path.c_str());
    }

    return directory_empty;
}

std::list<std::wstring> cactus::GetFiles(const std::wstring& path_and_wildcard, int flags)
{
    std::wstring::size_type star_pos = path_and_wildcard.find(L"*");
    std::wstring::size_type question_pos = path_and_wildcard.find(L"?");
    std::wstring::size_type bs_pos = path_and_wildcard.rfind(L"\\");

    if (bs_pos == path_and_wildcard.npos) {
        // the entire path_and_wildcard is a name or a wildcard
        return GetFiles(cactus::GetCurrentDirectoryW(), path_and_wildcard, flags);
    }

    // the only supported wildcards are part of the file name
    // directory wildcards are not supported
    if ((star_pos != path_and_wildcard.npos && star_pos < bs_pos)
        || (question_pos != path_and_wildcard.npos && question_pos < bs_pos)) {
        throw std::exception("wildcard in a directory name");
    }

    // split in 2, path and wildcard
    return GetFiles(
               path_and_wildcard.substr(0, bs_pos),
               path_and_wildcard.substr(bs_pos + 1),
               flags);
}


std::list<std::wstring> cactus::GetDirectoryFiles(const std::wstring& full_path, const std::wstring& wildcard)
{
    std::wstring path = DirectoryNormalize(full_path);
    std::list<std::wstring> result;
    WIN32_FIND_DATA data = { 0 };
    HANDLE hFile = ::FindFirstFileW((path + L"\\" + wildcard).c_str(), & data);

    if (hFile != INVALID_HANDLE_VALUE) {
        do {
            std::wstring filename = data.cFileName;

            if (filename == L"." || filename == L"..")
                continue;

            if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                result.push_back(filename);
            }
        } while (::FindNextFile(hFile, & data));
    }

    switch (DWORD rc = ::GetLastError()) {
    case ERROR_PATH_NOT_FOUND: // directory not found in FindFirstFile
    case ERROR_FILE_NOT_FOUND: // file not found in FindFirstFile
    case ERROR_NO_MORE_FILES: // more more files in FindNextFile
        break;

    default:
        break;
        //CHECK_WIN32_DWORD(rc, "Error in FindFirst/NextFile");
    }

    if (hFile)
        ::CloseHandle(hFile);

    return result;
}

std::list<std::wstring> cactus::GetFiles(const std::wstring& full_path, const std::wstring& wildcard, int flags)
{
    std::wstring path = DirectoryNormalize(full_path);
    std::list<std::wstring> result;
    WIN32_FIND_DATA data = { 0 };
    HANDLE hFile = ::FindFirstFileW((path + L"\\*.*").c_str(), & data);
    DWORD rc = ERROR_SUCCESS;

    if (hFile != INVALID_HANDLE_VALUE) {
        do {
            std::wstring filename = data.cFileName;

            if (filename == L"." || filename == L"..")
                continue;

            filename = DirectoryCombine(path, filename);

            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (flags & GET_FILES_DIRECTORIES)
                    result.push_back(filename);

                if (flags & GET_FILES_RECURSIVE) {
                    std::wstring fullsubpath = DirectoryCombine(path, data.cFileName);
                    std::list<std::wstring> files = GetFiles(fullsubpath, wildcard, flags);
                    result.splice(result.end(), files);
                }
            }
        } while (::FindNextFile(hFile, & data));
    }

    switch (rc = ::GetLastError()) {
    case ERROR_PATH_NOT_FOUND: // directory not found in FindFirstFile
    case ERROR_FILE_NOT_FOUND: // file not found in FindFirstFile
    case ERROR_NO_MORE_FILES: // more more files in FindNextFile
        break;

    default:
        break;
        //CHECK_WIN32_DWORD(rc, "Error in FindFirst/NextFile");
    }

    if (hFile)
        ::CloseHandle(hFile);

    // get files matching wildcard in this directory
    if (flags & GET_FILES_FILES) {
        std::list<std::wstring> files = GetDirectoryFiles(path, wildcard);

        for each(const std::wstring & file in files) {
            result.push_back(DirectoryCombine(path, file));
        }
    }

    return result;
}
