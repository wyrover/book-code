#ifndef directory_h__
#define directory_h__

#include <Windows.h>
#include <string>
#include <list>

namespace cactus
{
// normalize a directory string
std::wstring DirectoryNormalize(const std::wstring& path);
// check whether a directory exists
bool DirectoryExists(const std::string& path);
bool DirectoryExists(const std::wstring& path);
// create a directory with any subdirectories missing, returns the first directory created
std::string DirectoryCreate(const std::string& path);
std::wstring DirectoryCreate(const std::wstring& path);

enum DELETE_DIRECTORY_FLAGS {
    DELETE_DIRECTORY_FILES = 1, //! delete files
    DELETE_DIRECTORY_FOLDERS = 2, //! delete folders
    DELETE_DIRECTORY_SUBDIRECTORIES = 4, //! delete subdirectories
    DELETE_DIRECTORY_EMPTY = DELETE_DIRECTORY_FOLDERS | DELETE_DIRECTORY_SUBDIRECTORIES, //! delete empty (sub)directories only
    DELETE_DIRECTORY_ALL = DELETE_DIRECTORY_FILES | DELETE_DIRECTORY_FOLDERS | DELETE_DIRECTORY_SUBDIRECTORIES //! delete everything
};

// delete a directory
bool DirectoryDelete(const std::string& path, int flags = DELETE_DIRECTORY_ALL);
bool DirectoryDelete(const std::wstring& path, int flags = DELETE_DIRECTORY_ALL);

enum GET_FILES_FLAGS {
    GET_FILES_FILES = 1, //! return files
    GET_FILES_DIRECTORIES = 2, //! return directory names
    GET_FILES_RECURSIVE = 4, //! recurse into subdirectories
    GET_FILES_ALL = GET_FILES_FILES | GET_FILES_DIRECTORIES | GET_FILES_RECURSIVE
};

// get a list of files in a directory structure
std::list<std::wstring> GetFiles(const std::wstring& path, const std::wstring& wildcard = L"*.*", int flags = GET_FILES_FILES);
// get a list of files in a directory structure
std::list<std::wstring> GetFiles(const std::wstring& path_and_wildcard, int flags);
// get a list of file names (without the full path) in a given directory
std::list<std::wstring> GetDirectoryFiles(const std::wstring& path, const std::wstring& wildcard = L"*.*");
}


#endif // directory_h__
