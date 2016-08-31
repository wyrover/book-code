/*!
* \file cactus_file.cpp
* \brief 文件路径和文件读写相关操作
*
* 文件路径和文件读写相关操作
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#include <cactus/cactus_file.h>
#include <cactus/cactus_string.h>
#include <atlfile.h>
#include <atlsecurity.h>
#include <assert.h>
#include <io.h>
#include <fstream>
#include <ShlObj.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "crypt32.lib")
#include <WinCrypt.h>
#include <ShellAPI.h>

namespace cactus
{
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

// Add backslash to string
#define ADDBACKSLASH(s) if(s.Right(1)!=_T('\\')) s+=_T('\\');
#define REMOVEBACKSLASH(s) if(s.Right(1)==_T('\\')) s=s.Left(s.GetLength()-1);
#define READ_FILE_BUFFER_LEGNTH                     1024

enum {
    MAX_PATH_LEN = 32767
};


const LPCWSTR MUSIC_FILE_EXT_NAME[] =
{L".mp3", L".wma", L".aac", L".ogg", L".wav", L".m4a", L".amr", L".ape", L".cue", L".flac", L".wav"};
const LPCWSTR VIDEO_FILE_EXT_NAME[] = {
    L".mp4", L".avi", L".3gp", L".mkv", L".wmv", L".rm", L".rmvb", L".asf", L".f4v", L".flv",
    L".m4v", L".mpg", L".mpeg", L".mov"
};
const LPCWSTR IMAGE_FILE_EXT_NAME[] = {L".jpg", L".jpeg", L".png", L".bmp", L".gif"};
const LPCWSTR EBOOK_FILE_EXT_NAME[] = {L".txt", L".epub", L".pdf"};

struct ClassifyCriteria {
    const LPCWSTR* suffix_array;
    int suffix_array_size;
    LPCWSTR classify_name;
};

ClassifyCriteria classify_criteria[] = {
    {MUSIC_FILE_EXT_NAME, ARRAYSIZE(MUSIC_FILE_EXT_NAME), L"music"},
    {VIDEO_FILE_EXT_NAME, ARRAYSIZE(VIDEO_FILE_EXT_NAME),  L"video"},
    {IMAGE_FILE_EXT_NAME, ARRAYSIZE(IMAGE_FILE_EXT_NAME), L"image"},
    {EBOOK_FILE_EXT_NAME, ARRAYSIZE(EBOOK_FILE_EXT_NAME), L"ebook"}
};

string16 Path::ClassifyFile(const string16& filename)
{
    static string16 DEFAULT_SDCARD_DIR_NAME = L"oter file type";
    // 先转成小写
    char16 name[MAX_PATH];
    StrCpyW(name, filename.c_str());
    CharLowerBuffW(name, lstrlenW(name));

    // 再判断后缀
    for (size_t i = 0; i < ARRAYSIZE(classify_criteria); i++)
        if (PathFindSuffixArrayW(name, classify_criteria[i].suffix_array, classify_criteria[i].suffix_array_size) != NULL)
            return string16(classify_criteria[i].classify_name);

    return DEFAULT_SDCARD_DIR_NAME;
}

string16 Path::GetCwd()
{
    char16 buffer[MAX_PATH + 1];
    ::GetCurrentDirectory(MAX_PATH, buffer);

    if (wcslen(buffer) > 0)
        return string16(buffer);
    else
        return StringUtil::sWEmpty;
}

void SetCwd(const string16& dir)
{
    ::SetCurrentDirectory(dir.c_str());
}


const char16* Path::GetAllPathDelimiters()
{
    return L"\\/";
}

BOOL Path::PathIsPrefix(const string16& prefix, const string16& path)
{
    return ::PathIsPrefixW(prefix.c_str(), path.c_str());
}

string16 Path::PathAddBackslash(const string16& path)
{
    return Path::IncludeTrailingPathDelimiter(path);
}

BOOL Path::PathCommonPrefix(const string16& srcpath, const string16& destpath, string16& prefixpath)
{
    char16 _prefixpath[MAX_PATH];
    prefixpath = StringUtil::sWEmpty;

    if (::PathCommonPrefixW(srcpath.c_str(), destpath.c_str(), _prefixpath)) {
        prefixpath = string16(_prefixpath);
        return TRUE;
    }

    return FALSE;
}

BOOL Path::StrFormatByteSize(LONGLONG filelength, string16& deststr)
{
    char16 _filelength_str[100];
    deststr = StringUtil::sWEmpty;

    if (::StrFormatByteSizeW(filelength, _filelength_str, 100)) {
        deststr = string16(_filelength_str);
        return TRUE;
    }

    return FALSE;
}

void Path::SHGetFileInfo2(const string16& filename, string16& type_name, string16& displayname, DWORD& attributes)
{
    SHFILEINFO info = {0};
    ::SHGetFileInfoW(filename.c_str(), 0, &info, sizeof(info), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME);
    type_name = string16(info.szTypeName);
    ::SHGetFileInfoW(filename.c_str(), 0, &info, sizeof(info), SHGFI_USEFILEATTRIBUTES | SHGFI_DISPLAYNAME);
    displayname = string16(info.szDisplayName);
    ::SHGetFileInfoW(filename.c_str(), 0, &info, sizeof(info), SHGFI_USEFILEATTRIBUTES | SHGFI_ATTRIBUTES);
    attributes = info.dwAttributes;
    //SHGetFileInfoW(filename.c_str(), 0, &info, sizeof(info), SHGFI_USEFILEATTRIBUTES | SHGFI_ICON);
}

bool Path::IsPathDelimiter(const string16& S, int Index)
{
    return (Index > 0) && (Index <= S.length()) && (S[Index] == PathDelim);
}

string16 Path::IncludeTrailingBackslash(const string16& S)
{
    return IncludeTrailingPathDelimiter(S);
}

string16 Path::IncludeTrailingPathDelimiter(const string16& S)
{
    string16 Result = S;

    if (!EndsWithPathDelimiter(Result))
        Result = Result + PathDelim;

    return Result;
}

string16 Path::ExcludeTrailingBackslash(const string16& S)
{
    return ExcludeTrailingPathDelimiter(S);
}

string16 Path::ExcludeTrailingPathDelimiter(const string16& S)
{
    string16 Result = S;

    while (EndsWithPathDelimiter(Result))
        Result.erase(Result.length() - 1);

    return Result;
}

string16 Path::PathRemoveBackslash(const string16& S)
{
    return Path::ExcludeTrailingPathDelimiter(S);
}

string16 Path::ExtractFilePath(const string16& FileName)
{
    string16 Result = FileName;
    return Result.erase(Result.find_last_of(GetAllPathDelimiters()) + 1);
}

string16 Path::ExtractFileName(const string16& FileName)
{
    string16 Result = FileName;
    return Result.erase(0, Result.find_last_of(GetAllPathDelimiters()) + 1);
}

string16 Path::ExtractFileExt(const string16& FileName)
{
    string16 Result = FileName;
    return Result.erase(0, Result.find_last_of(L'.') + 1);
}


string16 Path::ExpandFileName(const string16& FileName)
{
    char16* FName;
    char16 Buffer[MAX_PATH + 1] = {0};
    ::GetFullPathNameW(FileName.c_str(), MAX_PATH,  Buffer, &FName);

    if ((FName != NULL) && (FName != 0))
        return string16(Buffer);
    else
        return StringUtil::sWEmpty;
}

string16 Path::ChangeFileExt(const string16& FileName, const string16& Extension)
{
    string16 Result = FileName;
    return Result.erase(Result.find_last_of(L'.') + 1) + Extension;
}

string16 Path::ChangeFilePath(const string16& FileName, const string16& Path)
{
    string16 Result = Path;
    return IncludeTrailingPathDelimiter(Result) + ExtractFileName(FileName);
}

string16 Path::ExtractShortPathName(const string16& FileName)
{
    char16 Buffer[MAX_PATH + 1] = {0};
    GetShortPathNameW(FileName.c_str(), Buffer, MAX_PATH);
    return string16(Buffer);
}

bool Path::CreateDir(const string16& Dir)
{
    return CreateDirectoryW(Dir.c_str(), NULL) == TRUE;
}

BOOL Path::vmsBuildPathToFile(LPCTSTR ptszFileName)
{
    LPCTSTR ptsz = ptszFileName;

    for (;;) {
        ptsz = _tcschr(ptsz + 1, '\\');

        if (ptsz == NULL)
            break;

        if (ptsz - ptszFileName == 2)
            continue;

        tstring tstr;
        tstr.assign(ptszFileName, ptsz - ptszFileName);
        CreateDirectory(tstr.c_str(), NULL);
    }

    return TRUE;
}

string16 Path::GetCurrentWorkingDir()
{
    char16 Buffer[MAX_PATH + 1] = {0};
    _wgetcwd(Buffer, MAX_PATH);
    return string16(Buffer);
}

string16 Path::GetSysDrv()
{
    static string16 sysdrv;

    if (!sysdrv.empty())
        return sysdrv;

    std::vector<char16> buffer(MAX_PATH + 1);
    GetSystemDirectory(&buffer[0], MAX_PATH);
    sysdrv = string16(&buffer[0]);
    sysdrv = sysdrv.erase(sysdrv.find_first_of(L"\\/") + 1);
    return sysdrv;
}

string16 Path::GetSysDrv2()
{
    static string16 sysdrv;

    if (!sysdrv.empty())
        return sysdrv;

    DWORD size = GetEnvironmentVariable(L"systemdrive", NULL, 0);

    if (size != 0) {
        std::vector<char16> buffer(size + 1);
        GetEnvironmentVariable(L"systemdrive", &buffer[0], size);
        sysdrv = string16(&buffer[0]);
    }

    return sysdrv;
}

string16 Path::GetAppFileName()
{
    char16 filename[MAX_PATH + 10] = {0};
    GetModuleFileNameW((HMODULE)&__ImageBase, filename, MAX_PATH);
    return string16(filename);
}

string16 Path::GetAppDir()
{
    return ExtractFilePath(GetAppFileName());
}


string16 Path::GetAppPath()
{
    string16 filename = GetAppFileName();
    size_t i = filename.find_last_of(L'\\');
    string16 s = filename.substr(0, i);

    if (*(s.end() - 1) != L'\\')
        s.append(L"\\");

    return s;
}

string16 Path::GetAppName()
{
    return Path::ExtractFileName(GetAppFileName());
}

bool Path::isAbsolutePath(const char16* path)
{
    if (IsCharAlphaW(path[0]) && path[1] == ':')
        return true;

    return path[0] == L'/' || path[0] == L'\\';
}

void Path::dividePath(const string16& path, string16& filename, string16& ext)
{
    char16  drive[_MAX_DRIVE],
            dir  [_MAX_DIR],
            fname[_MAX_FNAME],
            fext [_MAX_EXT];
    _wsplitpath(path.c_str(), drive, dir, fname, fext);
    filename = std::wstring(fname);
    ext = std::wstring(fext);
}

string16 Path::GetFilename(const string16& filename)
{
    char16  drive[_MAX_DRIVE],
            dir  [_MAX_DIR],
            fname[_MAX_FNAME],
            fext [_MAX_EXT];
    _wsplitpath(filename.c_str(), drive, dir, fname, fext);
    return std::wstring(fname);
}

string16 Path::GetFileExtension(const string16& filename)
{
    char16  drive[_MAX_DRIVE],
            dir  [_MAX_DIR],
            fname[_MAX_FNAME],
            ext[_MAX_EXT];
    _wsplitpath(filename.c_str(), drive, dir, fname, ext);
    return std::wstring(ext);
}

void Path::SplitPathFileName(const string16& filename, string16& path, string16& basename, string16& extname)
{
    char16  drive[_MAX_DRIVE],
            dir  [_MAX_DIR],
            fname[_MAX_FNAME],
            fext [_MAX_EXT];
    _wsplitpath(filename.c_str(), drive, dir, fname, fext);
    path = string16(drive);
    basename = string16(fname);
    extname = string16(fext);
}

BOOL Path::GetProcessPathNameAndFileName(string16& path, string16& basename, string16& extname)
{
    char16 sz_filename[MAX_PATH];

    if (GetModuleFileNameW(NULL, sz_filename, MAX_PATH) == 0)
        return FALSE;

    SplitPathFileName(string16(sz_filename), path, basename, extname);
    return TRUE;
}

BOOL Path::AdjustProcessCurrentDirectory()
{
    string16 path;
    string16 filename;
    string16 extname;
    GetProcessPathNameAndFileName(path, filename, extname);
    return SetCurrentDirectory(path.c_str());
}

bool Path::FileExist(const string16& filename)
{
    bool bRet = false;
    WIN32_FIND_DATA fd = {0};
    HANDLE hFile = FindFirstFile(filename.c_str(), &fd);

    if (hFile != INVALID_HANDLE_VALUE) {
        FindClose(hFile);

        if (!(fd.dwFileAttributes &  FILE_ATTRIBUTE_DIRECTORY))
            bRet = true;
    }

    return bRet;
}

bool Path::DirExists(const string16& dirname)
{
    return DirectoryExists(dirname);
}

bool Path::DirectoryExists(const string16& dirname)
{
    DWORD attrib;
    attrib = ::GetFileAttributesW(dirname.c_str());
    return ((attrib != INVALID_FILE_ATTRIBUTES) && (FILE_ATTRIBUTE_DIRECTORY & attrib)) == TRUE;
}

bool Path::CopyDir(const string16& source_dir, const string16& dest_dir, bool bOverwrite)
{
    string16 sourcedir = PathRemoveBackslash(Path::GetFullPathName(source_dir));
    string16 destdir = PathRemoveBackslash(Path::GetFullPathName(dest_dir));

    // Ensure source is a directory
    if (DirectoryExists(sourcedir) == false)
        return false;                           // Nope

    // Does the destination dir exist?
    if (DirectoryExists(destdir)) {
        if (bOverwrite == false)
            return false;
    } else {
        if (!CreateDirectoryNested(destdir))
            return false;
    }

    sourcedir.append(L"\\*.*");
    // Setup the struct
    SHFILEOPSTRUCT FileOp = {0};
    FileOp.pFrom = sourcedir.c_str();
    FileOp.pTo = destdir.c_str();
    FileOp.wFunc = FO_COPY;
    FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI; // FOF_NO_UI ("perform the operation with no user input") is not present for in case it would break compatibility somehow, and because the other flags already present seem to make its behavior implicit.  Also, unlike FileMoveDir, FOF_MULTIDESTFILES never seems to be needed.
    // All of the below left set to NULL/FALSE by the struct initializer higher above:
    //FileOp.hNameMappings          = NULL;
    //FileOp.lpszProgressTitle      = NULL;
    //FileOp.fAnyOperationsAborted  = FALSE;
    //FileOp.hwnd                   = NULL;
    // If the source directory contains any saved webpages consisting of a SiteName.htm file and a
    // corresponding directory named SiteName_files, the following may indicate an error even when the
    // copy is successful. Under Windows XP at least, the return value is 7 under these conditions,
    // which according to WinError.h is "ERROR_ARENA_TRASHED: The storage control blocks were destroyed."
    // However, since this error might occur under a variety of circumstances, it probably wouldn't be
    // proper to consider it a non-error.
    // I also checked GetLastError() after calling SHFileOperation(), but it does not appear to be
    // valid/useful in this case (MSDN mentions this fact but isn't clear about it).
    // The issue appears to affect only FileCopyDir, not FileMoveDir or FileRemoveDir.  It also seems
    // unlikely to affect FileCopy/FileMove because they never copy directories.
    return !SHFileOperation(&FileOp);
}


bool Path::MoveDir(const string16& source_dir, const string16& dest_dir, int OverwriteMode)
{
    string16 sourcedir = PathRemoveBackslash(Path::GetFullPathName(source_dir));
    string16 destdir = PathRemoveBackslash(Path::GetFullPathName(dest_dir));

    // Ensure source is a directory
    if (DirectoryExists(sourcedir) == false)
        return false;                           // Nope

    // Does the destination dir exist?
    DWORD attr = GetFileAttributes(destdir.c_str());

    if (attr != 0xFFFFFFFF) { // Destination already exists as a file or directory.
        if (attr & FILE_ATTRIBUTE_DIRECTORY) { // Dest already exists as a directory.
            if (OverwriteMode != 1 && OverwriteMode != 2) // Overwrite Mode is "Never".  Strict validation for safety.
                return false; // For consistency, mode1 actually should move the source-dir *into* the identically name dest dir.  But for backward compatibility, this change hasn't been made.
        } else // Dest already exists as a file.
            return false; // Don't even attempt to overwrite a file with a dir, regardless of mode (I think SHFileOperation refuses to do it anyway).
    }

    if (IsDifferentVolumes(sourcedir, destdir)) {
        // If the source and dest are on different volumes then we must copy rather than move
        // as move in this case only works on some OSes.  Copy and delete (poor man's move).
        if (!CopyDir(sourcedir, destdir, true))
            return false;

        return RemoveDir(sourcedir, true);
    }

    // Setup the struct
    SHFILEOPSTRUCT FileOp = {0};
    FileOp.pFrom = sourcedir.c_str();
    FileOp.pTo = destdir.c_str();
    FileOp.wFunc = FO_MOVE;
    FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI; // Set default. FOF_NO_UI ("perform the operation with no user input") is not present for in case it would break compatibility somehow, and because the other flags already present seem to make its behavior implicit.

    if (OverwriteMode == 2) // v1.0.46.07: Using the FOF_MULTIDESTFILES flag (as hinted by MSDN) overwrites/merges any existing target directory.  This logic supersedes and fixes old logic that didn't work properly when the source dir was being both renamed and moved to overwrite an existing directory.
        FileOp.fFlags |= FOF_MULTIDESTFILES;

    // All of the below left set to NULL/FALSE by the struct initializer higher above:
    //FileOp.hNameMappings          = NULL;
    //FileOp.lpszProgressTitle      = NULL;
    //FileOp.fAnyOperationsAborted  = FALSE;
    //FileOp.hwnd                   = NULL;
    return !SHFileOperation(&FileOp);
}

BOOL Path::MyMoveFile(const string16& source_file, const string16& dest_file)
{
    if (!PathFileExists(source_file.c_str()))
        return FALSE;

    if (PathFileExists(dest_file.c_str()))
        SetFileAttributes(dest_file.c_str(), FILE_ATTRIBUTE_NORMAL);

    DWORD dwFlags = GetFileAttributes(source_file.c_str());
    SetFileAttributes(source_file.c_str(), FILE_ATTRIBUTE_NORMAL);
    BOOL bRet = MoveFileEx(source_file.c_str(), dest_file.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);

    if (bRet)
        SetFileAttributes(dest_file.c_str(), dwFlags);

    return bRet;
}

bool Path::RemoveDir(const string16& source_dir, bool bRecurse)
{
    string16 sourcedir = PathRemoveBackslash(Path::GetFullPathName(source_dir));

    if (DirectoryExists(sourcedir) == false)
        return false;                           // Nope

    // If recursion not on just try a standard delete on the directory (the SHFile function WILL
    // delete a directory even if not empty no matter what flags you give it...)
    if (bRecurse == false) {
        if (!RemoveDirectory(sourcedir.c_str()))
            return false;
        else
            return true;
    }

    SHFILEOPSTRUCT  FileOp;
    FileOp.pFrom                    = sourcedir.c_str();
    FileOp.pTo                      = NULL;
    FileOp.hNameMappings            = NULL;
    FileOp.lpszProgressTitle        = NULL;
    FileOp.fAnyOperationsAborted    = FALSE;
    FileOp.hwnd                     = NULL;
    FileOp.wFunc    = FO_DELETE;
    FileOp.fFlags   = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI;
    return !SHFileOperation(&FileOp);
}

bool Path::IsDifferentVolumes(const string16& path1, const string16& path2)
{
    TCHAR           szP1Drive[_MAX_DRIVE + 1];
    TCHAR           szP2Drive[_MAX_DRIVE + 1];
    TCHAR           szDir[_MAX_DIR + 1];
    TCHAR           szFile[_MAX_FNAME + 1];
    TCHAR           szExt[_MAX_EXT + 1];
    string16 _path1 = Path::GetFullPathName(path1);
    string16 _path2 = Path::GetFullPathName(path2);
    // Split the target into bits
    _tsplitpath(_path1.c_str(), szP1Drive, szDir, szFile, szExt);
    _tsplitpath(_path2.c_str(), szP2Drive, szDir, szFile, szExt);

    if (szP1Drive[0] == '\0' || szP2Drive[0] == '\0')
        return true;
    else
        return _tcsicmp(szP1Drive, szP2Drive);
}


string16 Path::GetuniqueTempName()
{
    //Get the temporary files directory.
    char16 _filepath[MAX_PATH];
    DWORD dwResult = ::GetTempPath(MAX_PATH, _filepath);
    //Create a unique temporary file.
    char16 _tmpfilename[MAX_PATH];
    UINT nResult = GetTempFileNameW(_filepath, L"~ex", 0, _tmpfilename);
    return string16(_tmpfilename);
}

template<size_t nSize>
inline void ModifyPathSpec(TCHAR(&szDst)[nSize], BOOL bAddSpec)
{
    size_t nLen = _tcslen(szDst);
    ATLASSERT(nLen > 0);
    TCHAR  ch  = szDst[nLen - 1];

    if ((ch == _T('\\')) || (ch == _T('/'))) {
        if (!bAddSpec) {
            while (ch == _T('\\') || ch == _T('/')) {
                szDst[nLen - 1] = _T('\0');
                --nLen;
                ch = szDst[nLen - 1];
            }
        }
    } else {
        if (bAddSpec) {
            szDst[nLen] = _T('\\');
            szDst[nLen + 1] = _T('\0');
        }
    }
}

bool Path::CreateDirectoryNested(const string16& dir)
{
    if (::PathIsDirectoryW(dir.c_str()))
        return true;

    if (dir.length() >= MAX_PATH)
        return false;

    char16 szPreDir[MAX_PATH];
    _tcscpy_s(szPreDir, dir.c_str());
    //确保路径末尾没有反斜杠
    ModifyPathSpec(szPreDir, FALSE);
    //获取上级目录
    BOOL bGetPreDir = ::PathRemoveFileSpec(szPreDir);

    if (!bGetPreDir) return false;

    //如果上级目录不存在,则递归创建上级目录
    if (!::PathIsDirectory(szPreDir)) {
        CreateDirectoryNested(szPreDir);
    }

    return SUCCEEDED(::CreateDirectory(dir.c_str(), NULL)) ? true : false;
}

bool Path::CreateDirEx(const char* lpDir)
{
    string16 dir = convert_to<string16>(lpDir);
    return CreateDirectoryNested(dir);
}

bool Path::CreateDirs(const char *lpFileName)
{
    string16 filename = convert_to<string16>(lpFileName);
    string16 dir = Path::ExtractFilePath(filename);
    return Path::CreateDirectoryNested(dir);
}

bool Path::CreateDirs(const string16& filename)
{
    string16 dir = Path::ExtractFilePath(filename);
    return Path::CreateDirectoryNested(dir);
}

bool Path::ForceCreateDirectory(string16 dir)
{
    using namespace std;
    bool bRet = false;

    //确保以"\"结尾，以创建最后一个目录
    if (dir.find_last_of(L"\\") != dir.length() - 1) {
        dir.append(L"\\");
    }

    vector<wstring> dirs;       //存放每一层目录
    wstring temp;           //临时变量，存放目录字符串

    for (size_t i = 0; i != dir.length(); ++i) {
        if (dir[i] != L'\\') {
            temp += dir[i];
        } else {
            dirs.push_back(temp);
            temp += L'\\';
        }
    }

    for (vector<wstring>::const_iterator iter = dirs.begin(); iter != dirs.end(); ++iter) {
        bRet = SUCCEEDED(::CreateDirectory((*iter).c_str(), NULL)) ? true : false;
    }

    return bRet;
}


bool Path::CreateDeepDirectory(const string16& dir)
{
    return Path::CreateDirectoryNested(dir);
}

BOOL Path::file_get_contents(const string16& filename, CStringA &strA)
{
    CAtlFile file;

    if (FAILED(file.Create(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
        return FALSE;

    BOOL bRet = FALSE;

    do {
        ULONGLONG filesize = 0;

        if (FAILED(file.GetSize(filesize)))
            break;

        strA = "";

        if (filesize > 0) {
            file.Read(strA.GetBuffer((int)filesize), (DWORD)filesize);
            strA.ReleaseBuffer((int)filesize);
        }

        bRet = TRUE;
    } while (FALSE);

    file.Close();
    return bRet;
}

BOOL Path::file_get_contents(const string16& filename, CStringW &strW)
{
    CStringA strA;
    BOOL bRet = file_get_contents(filename.c_str(), strA);
    USES_CONVERSION;
    strW = CA2CT(strA);
    return bRet;
}

BOOL Path::file_put_contents(const string16& filename, BYTE* pBuffer, INT nLen)
{
    CAtlFile file;

    if (FAILED(file.Create(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, CREATE_ALWAYS)))
        return FALSE;

    file.Write(pBuffer, nLen);
    file.Close();
    return TRUE;
}

BOOL Path::file_put_contents(const string16& filename, LPCTSTR lpszBuffer)
{
    CStringA strA;
    USES_CONVERSION;
    strA = CT2CA(lpszBuffer);
    return file_put_contents(filename.c_str(), (BYTE*)strA.GetString(), strA.GetLength());
}

BOOL Path::SaveNewFile(const string16& filename, PBYTE lpbData, DWORD dwFileSize)
{
    HANDLE _w_hFile = INVALID_HANDLE_VALUE;
    HANDLE _w_hFileMap = NULL;
    PBYTE _w_lpbFile = NULL;
    _w_hFile = CreateFile(
                   filename.c_str(),//路径
                   GENERIC_WRITE | GENERIC_READ, //写
                   0 ,//共享类型0
                   NULL,//文件的安全特性
                   CREATE_ALWAYS,//创建文件，会改写前一个文件
                   FILE_FLAG_SEQUENTIAL_SCAN,//针对连续访问对文件缓冲进行优化
                   NULL);//如果不为零，则指定一个文件句柄

    if (INVALID_HANDLE_VALUE == _w_hFile) {
        return FALSE;
    }

    _w_hFileMap = CreateFileMapping(
                      _w_hFile,
                      NULL, //安全对象
                      PAGE_READWRITE,//以读写方式打开映射
                      0,
                      dwFileSize,
                      NULL);//指定文件映射对象的名字

    if (NULL == _w_hFileMap) {
        return FALSE;
    }

    _w_lpbFile = (PBYTE) MapViewOfFile(
                     _w_hFileMap,
                     FILE_MAP_WRITE,
                     0,
                     0, //映射的大小
                     0);//零表示允许windows寻找地址

    if (NULL == _w_lpbFile) {
        return FALSE;
    }

    memcpy(_w_lpbFile, lpbData, dwFileSize);
    //这个memcpy导致性能骤减为CFile::Write的水平，郁闷

    if (!FlushViewOfFile(_w_lpbFile, 0)) {
        return FALSE;
    }

    UnmapViewOfFile(_w_lpbFile);
    CloseHandle(_w_hFileMap);
    CloseHandle(_w_hFile);
    _w_hFile = INVALID_HANDLE_VALUE;
    _w_hFileMap = NULL;
    _w_lpbFile = NULL;
    return TRUE;
}

INT64 Path::file_get_size(const string16& filename)
{
    /*
    CAtlFile file;
    if( FAILED( file.Create(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
        return 0;

    ULONGLONG filesize = 0;
    file.GetSize(filesize);
    return filesize;
    */
    std::wfstream f(filename.c_str(), std::ios::binary | std::ios::ate);
    return static_cast<ULONGLONG>(f.tellg());
}

BOOL Path::DeleteDirectory(const string16& dir, BOOL bContinueWhenFail)
{
    BOOL bRet = FALSE;
    WIN32_FIND_DATA fData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    string16 full_filepath;
    string16 pattern = Path::PathAddBackslash(dir);
    pattern.append(L"*.*");
    hFind = ::FindFirstFile(pattern.c_str(), &fData);

    if (hFind == INVALID_HANDLE_VALUE)
        goto Exit0;

    do {
        full_filepath = Path::PathAddBackslash(dir) + fData.cFileName;

        if (0 == _tcscmp(fData.cFileName, _T(".")) ||
            0 == _tcscmp(fData.cFileName, _T("..")))
            continue;

        if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if ((FALSE == Path::DeleteDirectory(full_filepath, bContinueWhenFail)) && (!bContinueWhenFail))
                goto Exit0;
        } else {
            if ((FALSE == ::DeleteFile(full_filepath.c_str())) && (!bContinueWhenFail))
                goto Exit0;
        }
    } while (::FindNextFile(hFind, &fData) != 0);

    bRet = TRUE;
Exit0:

    if (hFind != INVALID_HANDLE_VALUE)
        ::FindClose(hFind);

    RemoveDirectory(Path::PathRemoveBackslash(dir).c_str());
    return bRet;
}

INT Path::DeletePath(const string16& dir)
{
    SHFILEOPSTRUCT shfileopstruct = {0};
    shfileopstruct.wFunc = FO_DELETE;
    shfileopstruct.pFrom = dir.c_str();
    shfileopstruct.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI;
    return SHFileOperation(&shfileopstruct);
}

BOOL Path::DeleteFolder(const string16& dir, BOOL bDeleteDriver)
{
    BOOL bRet = FALSE;
    BOOL bRetCode = FALSE;
    WIN32_FIND_DATA fData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    string16 full_filepath;
    string16 pattern = Path::PathAddBackslash(dir);
    pattern.append(L"*.*");
    hFind = ::FindFirstFile(pattern.c_str(), &fData);

    if (hFind == INVALID_HANDLE_VALUE) {
        bRet = TRUE;
        goto Exit0;
    }

    do {
        full_filepath = Path::PathAddBackslash(dir) + fData.cFileName;

        if (0 == _tcscmp(fData.cFileName, _T(".")) || 0 == _tcscmp(fData.cFileName, _T("..")))
            continue;

        if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            bRetCode = DeleteFolder(full_filepath, bDeleteDriver);

            if (!bRetCode)
                goto Exit0;
        } else {
            if (bDeleteDriver) {
                int nLen = _tcslen(fData.cFileName);

                if (nLen > 4 && _tcsicmp(&fData.cFileName[nLen - 4], _T(".sys")) == 0) {
                    string16 driver_filename = Path::GetDriversDir() + string16(fData.cFileName);
                    ::SetFileAttributes(driver_filename.c_str(), 0);
                    bRetCode = ::DeleteFile(driver_filename.c_str());

                    if (!bRetCode)
                        ::MoveFileEx(driver_filename.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
                }
            }

            ::SetFileAttributes(full_filepath.c_str(), 0);
            bRetCode = ::DeleteFile(full_filepath.c_str());

            if (!bRetCode)
                ::MoveFileEx(full_filepath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
        }

        bRetCode = ::FindNextFile(hFind, &fData);
    } while (bRetCode);

    bRet = TRUE;
Exit0:

    if (hFind != INVALID_HANDLE_VALUE)
        FindClose(hFind);

    RemoveDirectory(Path::PathRemoveBackslash(dir).c_str());
    return bRet;
}

BOOL Path::DeleteAllFiles(const string16& dir, BOOL bDeleteFolderToo)
{
    BOOL bRet = FALSE;
    WIN32_FIND_DATA fData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    string16 full_filepath;
    string16 pattern = Path::PathAddBackslash(dir);
    pattern.append(L"*.*");
    hFind = ::FindFirstFile(pattern.c_str(), &fData);

    if (hFind == INVALID_HANDLE_VALUE)
        goto Exit0;

    do {
        full_filepath = Path::PathAddBackslash(dir) + fData.cFileName;

        if (0 == _tcscmp(fData.cFileName, _T(".")) ||
            0 == _tcscmp(fData.cFileName, _T("..")))
            continue;

        if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            DeleteAllFiles(full_filepath.c_str(), bDeleteFolderToo);
        } else {
            ::SetFileAttributes(full_filepath.c_str(), 0);
            ::DeleteFile(full_filepath.c_str());
        }
    } while (::FindNextFile(hFind, &fData) != 0);

    bRet = TRUE;
Exit0:

    if (hFind != INVALID_HANDLE_VALUE)
        ::FindClose(hFind);

    if (bDeleteFolderToo) {
        ::RemoveDirectory(Path::PathRemoveBackslash(dir).c_str());
    }

    return bRet;
}

string16 Path::GetDriversDir()
{
    static string16 drivers_dir;

    if (!drivers_dir.empty())
        return drivers_dir;

    string16 systemdir = Path::GetSystemDir();

    if (!systemdir.empty())
        drivers_dir = systemdir + L"drivers\\";

    return drivers_dir;
}

BOOL Path::GetSubDirs(const string16& dir_path, std::vector<string16>& dir_list, BOOL is_recursion)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char16 path[MAX_PATH];
    char16 full_file_path[MAX_PATH];
    string16 dir = Path::PathAddBackslash(dir_path);
    swprintf(path, L"%s*.*", dir.c_str());
    hFind = FindFirstFile(path, &FindFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            swprintf(full_file_path, L"%s%s", dir.c_str(), FindFileData.cFileName);

            if (_tcsicmp(_T("."), FindFileData.cFileName) == 0
                || _tcsicmp(_T(".."), FindFileData.cFileName) == 0) {
                continue;
            }

            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                dir_list.push_back(FindFileData.cFileName);

                if (is_recursion)
                    GetSubDirs(full_file_path, dir_list, is_recursion);
            } else {
            }
        } while (FindNextFile(hFind, &FindFileData) != 0);

        FindClose(hFind);
        return TRUE;
    }

    return FALSE;
}



string16 Path::GetCurrentDirectory()
{
    char16 buffer[MAX_PATH];
    ::GetCurrentDirectoryW(MAX_PATH, buffer);
    return string16(buffer);
}

DWORD Path::GetLargeFileSize(const string16& filename)
{
    DWORD dwSizeHigh;
    DWORD dwSizeLow = GetCompressedFileSizeW(filename.c_str(), &dwSizeHigh);
    return dwSizeHigh;
}

string16 Path::GetApplicationDataPath()
{
    string16 Path;
    char16 Buffer[MAX_PATH] = {0};

    if (!SHGetSpecialFolderPath(NULL, Buffer, CSIDL_APPDATA , 0)) {
        return GetAppPath();
    }

    Path.append(Buffer);
    Path.append(L"\\A Note");
    CreateDirectoryW(Path.c_str(), NULL);
    return Path;
}

void Path::FindAllFile(const string16& folderpath)
{
    _wfinddata_t FileInfo;
    string16 strfind = folderpath + L"\\*";
    long Handle = (long)_wfindfirst(strfind.c_str(), &FileInfo);

    if (Handle == -1L) {
        wprintf(L"can not match the folder path");
        return;
    }

    int count = 0;

    do {
        //判断是否有子目录
        if (FileInfo.attrib & _A_SUBDIR) {
            //这个语句很重要
            if ((_tcscmp(FileInfo.name, L".") != 0) && (_tcscmp(FileInfo.name, L"..") != 0)) {
                string16 newPath = folderpath + L"\\" + FileInfo.name;
                FindAllFile(newPath);
            }
        } else {
            count++;
            wprintf(L"%s\\%s" , folderpath.c_str() , FileInfo.name);
        }
    } while (_wfindnext(Handle, &FileInfo) == 0);

    _findclose(Handle);
}

string16 Path::GetDesktopDirectory()
{
    char16 _path[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, _path)))
        return string16(_path);
    else
        return StringUtil::sWEmpty;
}

string16 Path::GetProgramfilePath()
{
    char16 buffer[MAX_PATH + 1];
    ExpandEnvironmentStrings(L"%ProgramFiles%", buffer, MAX_PATH);
    return string16(buffer);
}

string16 Path::GetUserHomeDirectory()
{
    char16 _path[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, _path))) {
        string16 result(_path);
        result = IncludeTrailingPathDelimiter(result);
        return result;
    } else
        return StringUtil::sWEmpty;
}

string16 Path::GetCommonAppDataDirectory()
{
    char16 _path[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, _path))) {
        string16 result(_path);
        result = IncludeTrailingPathDelimiter(result);
        return result;
    } else
        return StringUtil::sWEmpty;
}

string16 Path::GetUserDataDirectory()
{
    char16 _path[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, _path))) {
        string16 result(_path);
        result = IncludeTrailingPathDelimiter(result);
        return result;
    } else
        return StringUtil::sWEmpty;
}

string16 Path::GetTempDir()
{
    char16 buffer[MAX_PATH_LEN];
    DWORD n = GetTempPathW(MAX_PATH_LEN, buffer);

    if (n > 0) {
        n = GetLongPathNameW(buffer, buffer, static_cast<DWORD>(wcslen(buffer)));

        if (n > 0) {
            return Path::IncludeTrailingPathDelimiter(string16(buffer));
        }
    }

    return StringUtil::sWEmpty;
}

string16 Path::GetWindowsDir()
{
    char16 buffer[MAX_PATH + 1];
    ::GetWindowsDirectory(buffer, MAX_PATH);

    if (wcslen(buffer) > 0)
        return Path::IncludeTrailingPathDelimiter(string16(buffer));
    else
        return StringUtil::sWEmpty;
}

string16 Path::GetSystemDir()
{
    char16 buffer[MAX_PATH_LEN];
    DWORD n = ::GetSystemDirectory(buffer, MAX_PATH_LEN);

    if (n > 0) {
        n = GetLongPathNameW(buffer, buffer, static_cast<DWORD>(wcslen(buffer)));

        if (n > 0) {
            return Path::IncludeTrailingPathDelimiter(string16(buffer));
        }
    }

    return StringUtil::sWEmpty;
}

bool Path::EndsWithPathDelimiter(const string16& path)
{
    return (path.find_last_of(GetAllPathDelimiters()) + 1 == path.length());
}

bool Path::StartsWithPathDelimiter(const string16& path)
{
    return (path.find_first_of(GetAllPathDelimiters()) == 0);
}

string16 Path::ExcludeLeadingPathDelimiter(const string16& S)
{
    string16 result = S;

    while (result.find_first_of(GetAllPathDelimiters()) == 0)
        result.erase(0, 1);

    return result;
}

string16 Path::CatPath(const string16& part1, const string16& part2)
{
    return IncludeTrailingPathDelimiter(part1) + ExcludeLeadingPathDelimiter(part2);
}

string16 Path::GetFullPathName(const string16& path)
{
    LPTSTR szFilePart;
    char16 buffer[MAX_PATH + 1];
    ::GetFullPathName(path.c_str(), MAX_PATH, buffer, &szFilePart);
    return string16(buffer);
}


string16 Path::GetFullPathForDirectoryAndFilename(const string16& dir, const string16& filename)
{
    string16 result = IncludeTrailingPathDelimiter(dir) + ExcludeLeadingPathDelimiter(filename);

    if (!FileExist(result))
        return StringUtil::sWEmpty;

    return result;
}

string16 Path::CompletePathWithModulePath(const string16& filename)
{
    char16 szPath[MAX_PATH] = {0};
    ::GetModuleFileName((HMODULE)&__ImageBase, szPath, MAX_PATH);
    PathAppend(szPath, L"..\\");
    PathAppend(szPath, filename.c_str());
    string16 data = szPath;
    return data;
}

void Path::MoveDirectory(const string16& source, const string16& target, BOOL bSilent /*= TRUE*/)
{
    if (Path::FileExist(source)) {
        if (bSilent) {
            Path::ForceCreateDirectory(target);
            WIN32_FIND_DATAW fData;
            HANDLE hFind;
            hFind = FindFirstFileW((source + L"*").c_str(), &fData);

            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    const string16& name = fData.cFileName;

                    if (name == L"." || name == L"..")
                        continue;

                    if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        MoveDirectory(source + name + PathDelim, target + name + PathDelim);
                    } else {
                        try {
                            Path::RenameFile(source + name, target + name);
                        } catch (.../*const FileException&*/) {
                            // ...
                        }
                    }
                } while (FindNextFile(hFind, &fData));
            }

            FindClose(hFind);
            RemoveDirectory(source.c_str());
        } else {
            SHFILEOPSTRUCT SHFileOp;
            ZeroMemory(&SHFileOp, sizeof(SHFILEOPSTRUCT));
            char16 fromPath[MAX_PATH] = {0};
            char16 toPath[MAX_PATH] = {0};
            lstrcpy(fromPath, (source + L"*").c_str());
            lstrcpy(toPath, (target).c_str());
            SHFileOp.hwnd = NULL;
            SHFileOp.wFunc = FO_MOVE;
            SHFileOp.pFrom = fromPath;
            SHFileOp.pTo = toPath;
            SHFileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;

            if (SHFileOperation(&SHFileOp) == 0) {
                RemoveDirectory(source.c_str());
            }
        }
    }
}

void Path::RenameFile(const string16& source, const string16& target)
{
    if (!::MoveFile(source.c_str(), target.c_str())) {
        // Can't move, try copy/delete...
        Path::CopyFile(source, target);
        Path::DeleteFile(source);
    }
}

void Path::CopyFile(const string16& source, const string16& target)
{
    if (!::CopyFile(source.c_str(), target.c_str(), FALSE)) {
        //throw FileException(Util::translateError(GetLastError()));
    }
}

BOOL Path::DeleteFile(const string16& filename)
{
    return ::DeleteFileW(filename.c_str());
}

void Path::OpenFolder(const string16& filename)
{
    if (Path::GetFileSize(filename) != -1)
        ::ShellExecute(NULL, NULL, L"explorer.exe", (L"/e, /select, \"" + filename + L"\"").c_str(), NULL, SW_SHOWNORMAL);
    else
        ::ShellExecute(NULL, NULL, L"explorer.exe", (L"/e, \"" + Path::ExtractFilePath(filename) + L"\"").c_str(), NULL, SW_SHOWNORMAL);
}

void Path::ShowItemInFolder(const string16& full_path)
{
    string16 dir = Path::ExtractFilePath(full_path);

    // ParseDisplayName will fail if the directory is "C:", it must be "C:\\".
    if (dir.c_str() == L"")
        return;

    typedef HRESULT(WINAPI * SHOpenFolderAndSelectItemsFuncPtr)(
        PCIDLIST_ABSOLUTE pidl_Folder,
        UINT cidl,
        PCUITEMID_CHILD_ARRAY pidls,
        DWORD flags);
    static SHOpenFolderAndSelectItemsFuncPtr open_folder_and_select_itemsPtr = NULL;
    static bool initialize_open_folder_proc = true;

    if (initialize_open_folder_proc) {
        initialize_open_folder_proc = false;
        // The SHOpenFolderAndSelectItems API is exposed by shell32 version 6
        // and does not exist in Win2K. We attempt to retrieve this function export
        // from shell32 and if it does not exist, we just invoke ShellExecute to
        // open the folder thus losing the functionality to select the item in
        // the process.
        HMODULE shell32_base = GetModuleHandle(L"shell32.dll");

        if (!shell32_base) {
            NOTREACHED();
            return;
        }

        open_folder_and_select_itemsPtr =
            reinterpret_cast<SHOpenFolderAndSelectItemsFuncPtr>
            (GetProcAddress(shell32_base, "SHOpenFolderAndSelectItems"));
    }

    if (!open_folder_and_select_itemsPtr) {
        ShellExecute(NULL, _T("open"), dir.c_str(), NULL, NULL, SW_SHOW);
        return;
    }

    CComPtr<IShellFolder> desktop;
    HRESULT hr = SHGetDesktopFolder(&desktop);

    if (FAILED(hr))
        return;

    CoMemReleaser<ITEMIDLIST> dir_item;
    hr = desktop->ParseDisplayName(NULL, NULL,
                                   const_cast<wchar_t *>(dir.c_str()),
                                   NULL, &dir_item, NULL);

    if (FAILED(hr))
        return;

    CoMemReleaser<ITEMIDLIST> file_item;
    hr = desktop->ParseDisplayName(NULL, NULL,
                                   const_cast<wchar_t *>(full_path.c_str()),
                                   NULL, &file_item, NULL);

    if (FAILED(hr))
        return;

    const ITEMIDLIST* highlight[] = {
        {file_item},
    };
    (*open_folder_and_select_itemsPtr)(dir_item, arraysize(highlight),
                                       highlight, NULL);
}

int64_t Path::GetFileSize(const string16& filename)
{
    WIN32_FIND_DATA fd;
    HANDLE hFind;
    hFind = FindFirstFile(filename.c_str(), &fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        return -1;
    } else {
        FindClose(hFind);
        return ((int64_t)fd.nFileSizeHigh << 32 | (int64_t)fd.nFileSizeLow);
    }
}

string16 Path::GetParentDirectory(const string16& path)
{
    string16::size_type pos = path.find_last_of(L'\\');

    if (pos != string16::npos) {
        string16 sPath = path.substr(0, pos);
        return sPath;
    }

    return path;
}

LPSTREAM Path::LoadResourceToStream(UINT uResId, LPCTSTR lpszResType)
{
    LPSTREAM pStream = NULL;
    // 查找资源.
    HRSRC hRes = FindResource((HMODULE)&__ImageBase,
                              MAKEINTRESOURCE(uResId), lpszResType);

    if (hRes) {
        // 加载资源.
        HGLOBAL hGlobal = LoadResource((HMODULE)&__ImageBase, hRes);

        if (hGlobal) {
            // 锁定资源.
            LPVOID pData = LockResource(hGlobal);

            do {
                if (pData) {
                    // 获取资源大小.
                    DWORD dwSize = SizeofResource((HMODULE)&__ImageBase, hRes);
                    // 分配内存.
                    HGLOBAL hGlobalMem = GlobalAlloc(GMEM_MOVEABLE, dwSize);
                    // 锁定内存.
                    LPVOID pData = GlobalLock(hGlobalMem);
                    // 复制数据到分配的内存.
                    CopyMemory(pData, hGlobal, dwSize);
                    // 解锁内存.
                    GlobalUnlock(hGlobalMem);

                    // 创建流对象.
                    if (CreateStreamOnHGlobal(hGlobalMem, TRUE, &pStream) != S_OK)
                        break;

                    // 因为CreateStreamOnHGlobal的参数2为TRUE, 所以不需要手工释放hGlobalMem.
                }
            } while (false);

            // 释放资源.
            if (hGlobal != NULL) {
                UnlockResource(hGlobal);
                FreeResource(hGlobal);
            }
        }
    }

    return pStream;
}

BOOL Path::IsExistDesktopIcon(const string16& title)
{
    TCHAR dstPath[MAX_PATH] = {0};
    ::SHGetSpecialFolderPath(NULL, dstPath, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
    ::PathAddBackslash(dstPath);
    wcscat_s(dstPath, MAX_PATH, title.c_str());
    wcscat_s(dstPath, MAX_PATH, L".lnk");

    if (PathFileExists(dstPath))
        return TRUE;

    return FALSE;
}

string16 Path::GetSpecialFolderPath(int csidl)
{
    TCHAR dstPath[MAX_PATH] = {0};
    ::SHGetSpecialFolderPath(NULL, dstPath, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
    ::PathAddBackslash(dstPath);
    return string16(dstPath);
}

BOOL Path::IsDrive(char16* szDrive)
{
    return (DRIVE_FIXED == ::GetDriveType(szDrive)) ? TRUE : FALSE;
}

BOOL Path::IsFileUsing(const string16& filename)
{
    DWORD dwFlags = GetFileAttributes(filename.c_str());
    SetFileAttributes(filename.c_str(), FILE_ATTRIBUTE_NORMAL);
    HANDLE hfile = CreateFile(filename.c_str(), GENERIC_ALL, 0, 0, OPEN_EXISTING, 0, 0);
    BOOL bRet = TRUE;

    if (hfile != INVALID_HANDLE_VALUE) {
        CloseHandle(hfile);
        bRet = FALSE;
    }

    SetFileAttributes(filename.c_str(), dwFlags);
    return bRet;
}

BOOL Path::IsFreeSpaceAvailable(char16* szDrive)
{
    BOOL bReturn = FALSE;

    do {
        if (IsDrive(szDrive)) {
            ULARGE_INTEGER i64FreeBytes = { 0 };

            if (!GetDiskFreeSpaceEx(szDrive, NULL, NULL, &i64FreeBytes))
                break;

            double dFreeDiskM = (double)i64FreeBytes.QuadPart / (1024 * 1024);
            double dFreeDiskG = dFreeDiskM / 1024;

            if (dFreeDiskG >= 1) {
                bReturn = TRUE;
            }
        }
    } while (0);

    return bReturn;
}

long Path::MyGetDiskFreeSpace(CString strPath)
{
    double  dFreeSpace = -1;
    ULARGE_INTEGER a1 , a2 , a3;
    CString strRoot;

    do {
        if (strPath.IsEmpty())
            break;

        //if (!PathFileExists(strPath))
        //  goto Exit0;
        strRoot = strPath.GetAt(0);
        strRoot += L":";

        if (GetDiskFreeSpaceEx(strRoot.GetBuffer(), (PULARGE_INTEGER)&a1, (PULARGE_INTEGER)&a2, (PULARGE_INTEGER)&a3) != 0) {
            dFreeSpace = (double)a3.QuadPart / 1024 / 1024;
        }
    } while (0);

    return (long)dFreeSpace;
}

void Path::FILEDosToUnixPathA(char* lpPath)
{
    LPSTR p;
    LPSTR pPointAtDot = NULL;
    char charBeforeFirstDot = '\0';
    TRACE("Original DOS path = [%s]\n", lpPath);

    if (!lpPath) {
        return;
    }

    for (p = lpPath; *p; p++) {
        /* Make the \\ to / switch first */
        if (*p == '\\') {
            /* Replace \ with / */
            *p = '/';
        }

        if (pPointAtDot) {
            /* If pPointAtDot is not NULL, it is pointing at the first encountered
               dot.  If we encountered a \, that means it could be a trailing dot */
            if (*p == '/') {
                /* If char before the first dot is a '\' or '.' (special case if the
                   dot is the first char in the path) , then we leave it alone,
                   because it is either . or .., otherwise it is a trailing dot
                   pattern and will be truncated */
                if (charBeforeFirstDot != '.' && charBeforeFirstDot != '/') {
                    memmove(pPointAtDot, p, (strlen(p)*sizeof(char)) + 1);
                    p = pPointAtDot;
                }

                pPointAtDot = NULL; /* Need to reset this */
            } else if (*p == '*') {
                /* Check our size before doing anything with our pointers */
                if ((p - lpPath) >= 3) {
                    /* At this point, we know that there is 1 or more dots and
                       then a star.  AND we know the size of our string at this
                       point is at least 3 (so we can go backwards from our pointer
                       safely AND there could possilby be two characters back)
                       So lets check if there is a '*' and a '.' before, if there
                       is, replace just a '*'.  Otherwise, reset pPointAtDot to NULL
                       and do nothing */
                    if (p[-2] == '*' &&
                        p[-1] == '.' &&
                        p[0] == '*') {
                        memmove(&(p[-2]), p, (strlen(p)*sizeof(char)) + 1);
                    }

                    pPointAtDot  = NULL;
                }
            } else if (*p != '.') {
                /* If we are here, that means that this is NOT a trailing dot,
                   some other character is here, so forget our pointer */
                pPointAtDot = NULL;
            }
        } else {
            if (*p == '.') {
                /* If pPointAtDot is NULL, and we encounter a dot, save the pointer */
                pPointAtDot = p;

                if (pPointAtDot != lpPath) {
                    charBeforeFirstDot = p[-1];
                } else {
                    charBeforeFirstDot = lpPath[0];
                }
            }
        }
    }

    /* If pPointAtDot still points at anything, then we still have trailing dots.
       Truncate at pPointAtDot, unless the dots are path specifiers (. or ..) */
    if (pPointAtDot) {
        /* make sure the trailing dots don't follow a '/', and that they aren't
           the only thing in the name */
        if (pPointAtDot != lpPath && *(pPointAtDot - 1) != '/') {
            *pPointAtDot = '\0';
        }
    }

    TRACE("Resulting Unix path = [%s]\n", lpPath);
}

void Path::FILEDosToUnixPathW(wchar_t* lpPath)
{
    LPWSTR p;
    LPWSTR pPointAtDot = NULL;
    WCHAR charBeforeFirstDot = '\0';
    TRACE("Original DOS path = [%S]\n", lpPath);

    if (!lpPath) {
        return;
    }

    for (p = lpPath; *p; p++) {
        /* Make the \\ to / switch first */
        if (*p == '\\') {
            /* Replace \ with / */
            *p = '/';
        }

        if (pPointAtDot) {
            /* If pPointAtDot is not NULL, it is pointing at the first encountered
               dot.  If we encountered a \, that means it could be a trailing dot */
            if (*p == '/') {
                /* If char before the first dot is a '\' or '.' (special case if the
                   dot is the first char in the path) , then we leave it alone,
                   because it is either . or .., otherwise it is a trailing dot
                   pattern and will be truncated */
                if (charBeforeFirstDot != '.' && charBeforeFirstDot != '/') {
                    memmove(pPointAtDot, p, ((wcslen(p) + 1)*sizeof(WCHAR)));
                    p = pPointAtDot;
                }

                pPointAtDot = NULL; /* Need to reset this */
            } else if (*p == '*') {
                /* Check our size before doing anything with our pointers */
                if ((p - lpPath) >= 3) {
                    /* At this point, we know that there is 1 or more dots and
                       then a star.  AND we know the size of our string at this
                       point is at least 3 (so we can go backwards from our pointer
                       safely AND there could possilby be two characters back)
                       So lets check if there is a '*' and a '.' before, if there
                       is, replace just a '*'.  Otherwise, reset pPointAtDot to NULL
                       and do nothing */
                    if (p[-2] == '*' &&
                        p[-1] == '.' &&
                        p[0] == '*') {
                        memmove(&(p[-2]), p, (wcslen(p)*sizeof(WCHAR)));
                    }

                    pPointAtDot  = NULL;
                }
            } else if (*p != '.') {
                /* If we are here, that means that this is NOT a trailing dot,
                   some other character is here, so forget our pointer */
                pPointAtDot = NULL;
            }
        } else {
            if (*p == '.') {
                /* If pPointAtDot is NULL, and we encounter a dot, save the pointer */
                pPointAtDot = p;

                if (pPointAtDot != lpPath) {
                    charBeforeFirstDot = p[-1];
                } else {
                    charBeforeFirstDot = lpPath[0];
                }
            }
        }
    }

    /* If pPointAtDot still points at anything, then we still have trailing dots.
       Truncate at pPointAtDot, unless the dots are path specifiers (. or ..) */
    if (pPointAtDot) {
        /* make sure the trailing dots don't follow a '/', and that they aren't
           the only thing in the name */
        if (pPointAtDot != lpPath && *(pPointAtDot - 1) != '/') {
            *pPointAtDot = '\0';
        }
    }

    TRACE("Resulting Unix path = [%S]\n", lpPath);
}

void Path::FILEUnixToDosPathA(char* lpPath)
{
    LPSTR p;
    TRACE("Original Unix path = [%s]\n", lpPath);

    if (!lpPath)
        return;

    for (p = lpPath; *p; p++) {
        if (*p == '/')
            *p = '\\';
    }

    TRACE("Resulting DOS path = [%s]\n", lpPath);
}

std::string file_get_contents(const string16& filename)
{
    std::string result;
    CAtlFile file;

    if (FAILED(file.Create(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
        return StringUtil::sEmpty;

    do {
        ULONGLONG filesize = 0;

        if (FAILED(file.GetSize(filesize)))
            break;

        if (filesize > 0) {
            std::vector<char> buf((DWORD)++filesize);
            file.Read(&buf[0], (DWORD)filesize);
            result = std::string(&buf[0]);
        }
    } while (FALSE);

    file.Close();
    return result;
}

bool file_get_contents(const std::string& filename, std::vector<char>& buffer)
{
    std::ifstream file(filename.c_str(), std::ios_base::binary);

    if (file) {
        file.seekg(0, std::ios_base::end);
        std::streamsize size = file.tellg();

        if (size > 0) {
            file.seekg(0, std::ios_base::beg);
            buffer.resize(static_cast<std::size_t>(size));
            file.read(&buffer[0], size);
        }

        buffer.push_back('\0');
        return true;
    } else {
        return false;
    }
}

bool file_get_contents(const string16& filename, std::vector<char>& buffer)
{
    std::ifstream file(filename.c_str(), std::ios_base::binary);

    if (file) {
        file.seekg(0, std::ios_base::end);
        std::streamsize size = file.tellg();

        if (size > 0) {
            file.seekg(0, std::ios_base::beg);
            buffer.resize(static_cast<std::size_t>(size));
            file.read(&buffer[0], size);
        }

        buffer.push_back('\0');
        return true;
    } else {
        return false;
    }
}

bool file_get_contents(const string16& filename, std::vector<string16>& list)
{
    std::fstream fin(filename.c_str());

    if (!fin)
        return false;

    std::string s;

    while (getline(fin, s)) {
        if (!s.empty()) {
            list.push_back(convert_to<string16>(trim(s)));
        }
    }

    return true;
}

bool file_get_contents(const string16& filename, std::set<string16>& list)
{
    std::fstream fin(filename.c_str());

    if (!fin)
        return false;

    std::string s;

    while (getline(fin, s)) {
        if (!s.empty()) {
            list.insert(convert_to<string16>(trim(s)));
        }
    }

    return true;
}

bool file_put_contents(const string16& filename, const string16& data)
{
    CAtlFile file;

    if (FAILED(file.Create(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, CREATE_ALWAYS)))
        return FALSE;

    string8 content = convert_to<string8>(data);
    file.Write(content.c_str(), (DWORD)content.length());
    file.Close();
    return true;
}

INT64 filesize(const string16& filename)
{
    CAtlFile file;

    if (FAILED(file.Create(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
        return 0;

    ULONGLONG filesize = 0;
    file.GetSize(filesize);
    return filesize;
}

BOOL GetFileLargeSize(LPCWSTR lpFilePath,
                      LARGE_INTEGER& liFileSize)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    LARGE_INTEGER larFileSize;

    if (!lpFilePath) {
        liFileSize.LowPart = 0xFFFFFFFE;
        return FALSE;
    }

    larFileSize.LowPart = 0;
    larFileSize.HighPart = 0;
    hFile = ::CreateFile(
                lpFilePath,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
        liFileSize.LowPart = ::GetLastError();
        return FALSE;
    }

    if (::GetFileSizeEx(hFile, &larFileSize)) {
        liFileSize.LowPart = larFileSize.LowPart;
        liFileSize.HighPart = larFileSize.HighPart;
    } else {
        liFileSize.LowPart = ::GetLastError();
    }

    ::CloseHandle(hFile);
    hFile = NULL;
    return TRUE;
}

bool move_file(const string16& from_filename, const string16& to_filename)
{
    if (!PathFileExistsW(from_filename.c_str()))
        return false;

    if (PathFileExistsW(to_filename.c_str()))
        SetFileAttributesW(to_filename.c_str(), FILE_ATTRIBUTE_NORMAL);

    DWORD dwFlags = GetFileAttributesW(from_filename.c_str());
    SetFileAttributesW(from_filename.c_str(), FILE_ATTRIBUTE_NORMAL);
    BOOL bRet = MoveFileExW(from_filename.c_str(), to_filename.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);

    if (bRet)
        SetFileAttributesW(to_filename.c_str(), dwFlags);

    return (bRet == TRUE);
}

ULONG FileExistCertificate(LPCWSTR lpFilePath)
{
    ULONG ulResult = -1;
    WCHAR szFilePath[MAX_PATH] = {0};
    ULONG ulNumberOfConverted = 0;
    HCERTSTORE hStore = NULL;
    HCRYPTMSG hMsg = NULL;
    BOOL bRetCode = FALSE;
    ULONG ulEncoding = 0, ulContentType = 0, ulFormatType = 0;
    ULONG ulSignerInfo = 0;

    __try {
        wcsncpy_s(szFilePath, MAX_PATH, lpFilePath, MAX_PATH - 1);
        bRetCode = ::CryptQueryObject(
                       CERT_QUERY_OBJECT_FILE,
                       szFilePath,
                       CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                       CERT_QUERY_FORMAT_FLAG_BINARY,
                       0,
                       &ulEncoding,
                       &ulContentType,
                       &ulFormatType,
                       &hStore,
                       &hMsg,
                       NULL);

        if (!bRetCode) {
            __leave;
        }

        bRetCode = ::CryptMsgGetParam(
                       hMsg,
                       CMSG_SIGNER_INFO_PARAM,
                       0,
                       NULL,
                       &ulSignerInfo);

        if (!bRetCode) {
            __leave;
        }
    } __finally {
        ulResult = ::GetLastError();

        if (hStore) {
            ::CertCloseStore(hStore, 0);
            hStore = NULL;
        }

        if (hMsg) {
            ::CryptMsgClose(hMsg);
            hMsg = NULL;
        }
    }

    return ulResult;
}

/*! @brief 测试函数说明
 *
 * @param[in] lpFilePath 参数说明
 * @return 返回TRUE表示成功，否则为失败
 */
BOOL IsPeFile(LPCWSTR lpFilePath)
{
    BOOL bResult = FALSE;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    PBYTE pFileHeader = NULL;
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNtHeader = NULL;
    DWORD dwFileSize = 0;
    DWORD dwNumberOfBytesRead = 0;

    if (!lpFilePath || !::PathFileExists(lpFilePath)) {
        goto _abort;
    }

    hFile = ::CreateFile(
                lpFilePath,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
        goto _abort;
    }

    dwFileSize = ::GetFileSize(hFile, NULL);

    if (0 >= dwFileSize) {
        goto _abort;
    }

    if (dwFileSize > READ_FILE_BUFFER_LEGNTH) {
        dwFileSize = READ_FILE_BUFFER_LEGNTH;
    }

    pFileHeader = new BYTE[dwFileSize];

    if (!pFileHeader) {
        goto _abort;
    }

    ::RtlZeroMemory(pFileHeader, dwFileSize);

    if (!::ReadFile(hFile, pFileHeader, dwFileSize, &dwNumberOfBytesRead, NULL)) {
        goto _abort;
    }

    pDosHeader = (PIMAGE_DOS_HEADER)pFileHeader;

    if (IMAGE_DOS_SIGNATURE != pDosHeader->e_magic ||
        0 > pDosHeader->e_lfanew ||
        pDosHeader->e_lfanew >= READ_FILE_BUFFER_LEGNTH - 0x3C) {
        goto _abort;
    }

    pNtHeader = (PIMAGE_NT_HEADERS)(pFileHeader + pDosHeader->e_lfanew);

    if (0 != IsBadReadPtr(pNtHeader, sizeof(DWORD)) || IMAGE_NT_SIGNATURE != pNtHeader->Signature) {
        goto _abort;
    }

    bResult = TRUE;
_abort:

    if (pFileHeader) {
        delete[] pFileHeader;
        pFileHeader = NULL;
    }

    if (INVALID_HANDLE_VALUE != hFile) {
        ::CloseHandle(hFile);
        hFile = NULL;
    }

    return bResult;
}


void hexDump(FILE* dmpf, const char* name, BYTE* addr, size_t size)
{
    if (!size)
        return;

    assert(addr);
    fprintf(dmpf, "Hex dump of %s:\n", name);

    for (unsigned i = 0; i < size; i++) {
        if ((i % 16) == 0) {
            fprintf(dmpf, "\n    %04X: ", i);
        }

        fprintf(dmpf, "%02X ", *addr++);
    }

    fprintf(dmpf, "\n\n");
}


HRESULT KGetLoadModulePath(HMODULE hModule, string16& filename)
{
    HRESULT hr      = E_FAIL;
    wchar_t* pBuf   = NULL;
    DWORD nAllocSize = 256; //2的8次方
    DWORD nBufferSize = 0;
    DWORD dwLen     = 0;

    //2的15次方是32768,路径最长不会超过这个数
    for (; nAllocSize <= 32768; nAllocSize *= 2) {
        if (pBuf != NULL) {
            delete[] pBuf;
            pBuf = NULL;
        }

        pBuf = new wchar_t[nAllocSize];
        nBufferSize = nAllocSize - 1;
        dwLen = GetModuleFileNameW(hModule, pBuf, nBufferSize);

        if (0 == dwLen) {
            break;//执行失败
        }

        //dwLen 至少应该比nBufferSize小1
        if (dwLen < nBufferSize) {
            hr = S_OK;
            filename = pBuf;
            break;
        }
    }

    if (pBuf) {
        delete[] pBuf;
        pBuf = NULL;
    }

    return hr;
}

HRESULT KGetCurrentLoadModulePath(string16& filename)
{
    return KGetLoadModulePath((HMODULE)&__ImageBase, filename);
}

HRESULT KGetLoadModuleDirPath(HMODULE hModule, string16& dir)
{
    string16 path;
    HRESULT hr = KGetLoadModulePath(hModule, path);
    ::PathRemoveFileSpecW(&path[0]);
    dir = (const wchar_t*)(&path[0]);
    return hr;
}

HRESULT KGetCurrentLoadModuleDirPath(string16& dir)
{
    return KGetLoadModuleDirPath((HMODULE)&__ImageBase, dir);
}

HRESULT KGetCurrentFilePathOnLoadModuleDirPath(const string16& filename, string16& strFilePath, BOOL* pbFileExist)
{
    KGetCurrentLoadModuleDirPath(strFilePath);
    strFilePath += L"\\";
    strFilePath += filename;

    if (NULL != pbFileExist) {
        *pbFileExist = Path::FileExist(strFilePath) ? TRUE : FALSE;
    }

    return S_OK;
}

HRESULT KGetCurrentFilePathOnImageDirPath(const string16& filename, string16& strFilePath, BOOL* pbFileExist)
{
    KGetLoadModuleDirPath(NULL, strFilePath);
    strFilePath += L"\\";
    strFilePath += filename;

    if (NULL != pbFileExist) {
        *pbFileExist = Path::FileExist(strFilePath) ? TRUE : FALSE;
    }

    return S_OK;
}

HRESULT KSmartGetCurrentFilePath(const string16& filename, string16& strFilePath, BOOL* pbFileExist)
{
    BOOL bFileExist = FALSE;
    string16 strPath;
    KGetLoadModuleDirPath(NULL, strPath);
    strPath += L"\\";
    strPath += filename;

    if (!Path::FileExist(strPath)) {
        strPath = L"";
        KGetCurrentLoadModuleDirPath(strPath);
        strPath += L"\\";
        strPath += filename;
        bFileExist = Path::FileExist(strPath);
    } else {
        bFileExist = TRUE;
    }

    strFilePath = strPath;

    if (pbFileExist)
        *pbFileExist = bFileExist;

    return S_OK;
}



typedef BOOL WINAPI FN_CreateProcessWithTokenW(
    __in         HANDLE hToken,
    __in         DWORD dwLogonFlags,
    __in_opt     LPCWSTR lpApplicationName,
    __inout_opt  LPWSTR lpCommandLine,
    __in         DWORD dwCreationFlags,
    __in_opt     LPVOID lpEnvironment,
    __in_opt     LPCWSTR lpCurrentDirectory,
    __in         LPSTARTUPINFOW lpStartupInfo,
    __out        LPPROCESS_INFORMATION lpProcessInfo
);





BOOL ModifyServiceStartType(LPCWSTR pszServiceName, DWORD dwStartType)
{
    BOOL bRetCode = FALSE;
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService   = NULL;
    QUERY_SERVICE_CONFIG svrConfig;
    schSCManager = OpenSCManager(
                       NULL,
                       NULL,
                       SC_MANAGER_ALL_ACCESS
                   );

    if (schSCManager == NULL)
        goto Exit0;

    schService = OpenService(
                     schSCManager,
                     pszServiceName,
                     SERVICE_CHANGE_CONFIG);

    if (schService == NULL)
        goto Exit0;

    DWORD dwErrCode = 0;
    BOOL bNeedChange = TRUE;

    if (QueryServiceConfig(schService, &svrConfig, sizeof(QUERY_SERVICE_CONFIG), &dwErrCode)) {
        if (svrConfig.dwStartType == dwStartType) {
            bNeedChange = FALSE;
        }
    }

    if (bNeedChange) {
        bRetCode = ChangeServiceConfig(
                       schService,
                       SERVICE_NO_CHANGE,
                       dwStartType,
                       SERVICE_NO_CHANGE,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL);
    }

Exit0:

    if (schService != NULL) {
        CloseServiceHandle(schService);
    }

    if (schSCManager != NULL) {
        CloseServiceHandle(schSCManager);
    }

    return bRetCode;
}







namespace io
{

inline bool AbsolutePath(string16* path)
{
    wchar_t file_path_buf[MAX_PATH];

    if (!_wfullpath(file_path_buf, path->c_str(), MAX_PATH))
        return false;

    *path = string16(file_path_buf);
    return true;
}


FILE* OpenFile(const string16& filename, const char* mode)
{
    string16 w_mode = convert_to<string16>(mode);
    FILE* file;

    if (_wfopen_s(&file, filename.c_str(), w_mode.c_str()) != 0) {
        return NULL;
    }

    return file;
}

FILE* OpenFile(const std::string& filename, const char* mode)
{
    FILE* file;

    if (fopen_s(&file, filename.c_str(), mode) != 0) {
        return NULL;
    }

    return file;
}


int ReadFile(const string16& filename, char* data, int size)
{
    ScopedHandle file(CreateFile(filename.c_str(),
                                 GENERIC_READ,
                                 FILE_SHARE_READ | FILE_SHARE_WRITE,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_FLAG_SEQUENTIAL_SCAN,
                                 NULL));

    if (file == INVALID_HANDLE_VALUE)
        return -1;

    int ret_value;
    DWORD read;

    if (::ReadFile(file, data, size, &read, NULL) && read == size) {
        ret_value = static_cast<int>(read);
    } else {
        ret_value = -1;
    }

    return ret_value;
}

int WriteFile(const string16& filename, const char* data, int size)
{
    ScopedHandle file(CreateFile(filename.c_str(),
                                 GENERIC_WRITE,
                                 0,
                                 NULL,
                                 CREATE_ALWAYS,
                                 0,
                                 NULL));

    if (file == INVALID_HANDLE_VALUE) {
        std::wcout << L"CreateFile failed for path " << filename.c_str() <<
                   L" error code=" << GetLastError() <<
                   L" error text=" << FormatLastWin32Error() << std::endl;
        return -1;
    }

    DWORD written;
    BOOL result = ::WriteFile(file, data, size, &written, NULL);

    if (result && written == size)
        return static_cast<int>(written);

    if (!result) {
        // WriteFile failed.
        std::wcout << L"writing file " << filename.c_str() <<
                   L" failed, error code=" << GetLastError() <<
                   L" description=" << FormatLastWin32Error() << std::endl;
    } else {
        // Didn't write all the bytes.
        std::wcout << L"wrote" << written << L" bytes to " <<
                   filename.c_str() << L" expected " << size << std::endl;
    }

    return -1;
}


bool ReadFileToString(const string16& path, std::string* contents)
{
    FILE* file = OpenFile(path, "rb");

    if (!file) {
        return false;
    }

    char buf[1 << 16];
    size_t len;

    while ((len = fread(buf, 1, sizeof(buf), file)) > 0) {
        contents->append(buf, len);
    }

    CloseFile(file);
    return true;
}

bool GetFileSize(const string16& file_path, int64_t* file_size)
{
    FileInfo info;

    if (!GetFileInfo(file_path, &info))
        return false;

    *file_size = info.size;
    return true;
}

bool GetFileInfo(const string16& file_path, FileInfo* results)
{
    WIN32_FILE_ATTRIBUTE_DATA attr;

    if (!GetFileAttributesEx(file_path.c_str(),
                             GetFileExInfoStandard, &attr)) {
        return false;
    }

    ULARGE_INTEGER size;
    size.HighPart = attr.nFileSizeHigh;
    size.LowPart = attr.nFileSizeLow;
    results->size = size.QuadPart;
    results->is_directory =
        (attr.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    return true;
}

bool CloseFile(FILE* file)
{
    if (file == NULL)
        return true;

    return fclose(file) == 0;
}

bool CopyFile(const string16& from_path, const string16& to_path)
{
    if (from_path.length() >= MAX_PATH ||
        to_path.length() >= MAX_PATH) {
        return false;
    }

    return (::CopyFile(from_path.c_str(), to_path.c_str(), false) != 0);
}







} // namespace io






} // namespace cactus


namespace cactus
{
void WriteLine2File(string16& filename, LPCWSTR data)
{
    std::fstream f;
    f.open(filename.c_str(), std::ios::app);
    string8 str = convert_to<string8>(data);

    if (f.bad())
        return ;

    f << str.c_str() << std::endl;
    f.close();
}

void WriteFormat2File(string16& filename, LPCWSTR Format, ...)
{
    va_list argList;
    va_start(argList, Format);
    WriteFormat2FileV(filename, Format, argList);
    va_end(argList);
}

void WriteFormat2FileV(string16& filename, LPCWSTR Format, va_list args)
{
    CString str;
    str.FormatV(Format, args);
    WriteLine2File(filename, (LPCWSTR)str);
}

void EnumByLine(string16& filename, FN_PerString fnPerString)
{
    if (!Path::FileExist(filename))
        return;

    std::fstream f;
    f.open(filename.c_str(), std::ios_base::in);

    if (f.bad())
        return ;

    char cLine[ 1024 ];
    BOOL bRet = TRUE;

    while (!f.eof() && bRet) {
        ZeroMemory(cLine, 1024);
        f.getline(cLine, 1024);
        cLine[ 1023 ] = '\0';
        string16 data = convert_to<string16>((const char*)cLine);
        bRet = fnPerString(data);
    }

    f.close();
}

std::string SeekToPhrase(const std::string &strPhrase, std::istream &iStream)
{
    std::string strLine;
    std::getline(iStream, strLine);

    while (strLine.find(strPhrase) == std::string::npos && !iStream.eof()) {
        std::getline(iStream, strLine);
    };

    return strLine;
}

int CountFiles(const string16& dir, BOOL bSubfolders)
{
    int iCount = 0;
    HANDLE hFile;
    WIN32_FIND_DATA FileInformation;
    string16 full_filepath;
    string16 pattern = dir;
    pattern.append(L"\\*.*");
    hFile = ::FindFirstFile(pattern.c_str(), &FileInformation);

    if (hFile != INVALID_HANDLE_VALUE) {
        do {
            // Is it something else then current or top folder?
            if ((FileInformation.cFileName[0] != '.') && (FileInformation.cFileName[0] != '..')) {
                // Set up path
                full_filepath = Path::IncludeTrailingBackslash(dir) + FileInformation.cFileName;

                // Is it a folder?
                if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    // Should we search subdirectories too?
                    if (bSubfolders) {
                        // Search subdirectory
                        int iRC = CountFiles(full_filepath, bSubfolders);
                        // Check result of function
                        iCount += iRC;
                    }
                } else {
                    // Increase counter
                    ++iCount;
                }
            }
        } while (::FindNextFile(hFile, &FileInformation) == TRUE);

        ::FindClose(hFile);
    }

    return iCount;
}

INT64 GetFolderSize(const string16& dir, BOOL bSubfolders)
{
    INT64 iSize = 0;
    HANDLE hFile;
    WIN32_FIND_DATA FileInformation;
    string16 full_filepath;
    string16 pattern = dir;
    pattern.append(L"\\*.*");
    hFile = ::FindFirstFile(pattern.c_str(), &FileInformation);

    if (hFile != INVALID_HANDLE_VALUE) {
        do {
            if ((FileInformation.cFileName[0] != '.') && (FileInformation.cFileName[0] != '..')) {
                full_filepath = Path::IncludeTrailingBackslash(dir) + FileInformation.cFileName;

                if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    // Should we search subdirectories too?
                    if (bSubfolders) {
                        // Search subdirectory
                        INT64 iRS = GetFolderSize(full_filepath, bSubfolders);
                        // Check result of function
                        iSize += iRS;
                    }
                } else {
                    // Increase size
                    if (FileInformation.nFileSizeHigh > 0) {
                        iSize += (FileInformation.nFileSizeHigh * MAXDWORD) +
                                 FileInformation.nFileSizeLow;
                    } else {
                        iSize += FileInformation.nFileSizeLow;
                    }
                }
            }
        } while (::FindNextFile(hFile, &FileInformation) == TRUE);

        ::FindClose(hFile);
    }

    return iSize;
}


LPTSTR FileAttribToStr(LPTSTR aBuf, DWORD aAttr)
// Caller must ensure that aAttr is valid (i.e. that it's not 0xFFFFFFFF).
{
    if (!aBuf) return aBuf;

    int length = 0;

    if (aAttr & FILE_ATTRIBUTE_READONLY)
        aBuf[length++] = 'R';

    if (aAttr & FILE_ATTRIBUTE_ARCHIVE)
        aBuf[length++] = 'A';

    if (aAttr & FILE_ATTRIBUTE_SYSTEM)
        aBuf[length++] = 'S';

    if (aAttr & FILE_ATTRIBUTE_HIDDEN)
        aBuf[length++] = 'H';

    if (aAttr & FILE_ATTRIBUTE_NORMAL)
        aBuf[length++] = 'N';

    if (aAttr & FILE_ATTRIBUTE_DIRECTORY)
        aBuf[length++] = 'D';

    if (aAttr & FILE_ATTRIBUTE_OFFLINE)
        aBuf[length++] = 'O';

    if (aAttr & FILE_ATTRIBUTE_COMPRESSED)
        aBuf[length++] = 'C';

    if (aAttr & FILE_ATTRIBUTE_TEMPORARY)
        aBuf[length++] = 'T';

    aBuf[length] = '\0';  // Perform the final termination.
    return aBuf;
}



HANDLE FileOpen(const string16& FileName, DWORD Mode)
{
    DWORD AccessMode[3] = {GENERIC_READ, GENERIC_WRITE, GENERIC_READ | GENERIC_WRITE};
    DWORD ShareMode[5] = {0, 0, FILE_SHARE_READ, FILE_SHARE_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE};
    HANDLE Result = INVALID_HANDLE_VALUE;

    if (((Mode & 3) <= fmOpenReadWrite) && ((Mode & 0xF0) <= fmShareDenyNone)) {
        Result = ::CreateFile(FileName.c_str(), AccessMode[Mode & 3], ShareMode[(Mode & 0xF0) >> 4],                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    }

    return Result;
}

HANDLE FileCreate(const string16& FileName)
{
    return ::CreateFile(FileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
}

DWORD FileRead(HANDLE handle, BYTE* buffer, DWORD Count)
{
    DWORD dwSize;

    if (!::ReadFile(handle, buffer, Count, &dwSize, NULL))
        return -1;

    return dwSize;
}

DWORD FileWrite(HANDLE handle, const BYTE* buffer, DWORD Count)
{
    DWORD dwSize;

    if (::WriteFile(handle, buffer, Count, &dwSize, NULL))
        return -1;

    return dwSize;
}

DWORD FileSeek(HANDLE handle, DWORD Offset, DWORD Origin)
{
    return ::SetFilePointer(handle, Offset, NULL, Origin);
}

INT64 FileSeek(HANDLE handle, INT64 Offset, DWORD Origin)
{
    INT64 result = Offset;
    PLARGE_INTEGER pdata = (LARGE_INTEGER *)&result;
    pdata->LowPart = ::SetFilePointer(handle, pdata->LowPart, &(pdata->HighPart), Origin);

    if ((pdata->LowPart == 0xFFFFFFFF) && (GetLastError != 0))
        pdata->HighPart = 0xFFFFFFFF;

    return result;
}

void FileClose(HANDLE handle)
{
    ::CloseHandle(handle);
}

BOOL CreateDir(const string16& dir)
{
    return ::CreateDirectory(dir.c_str(), NULL);
}

BOOL RemoveDir(const string16& dir)
{
    return ::RemoveDirectory(dir.c_str());
}

BOOL ForceDirectories(string16 dir)
{
    if (dir.empty())
        return FALSE;

    dir = Path::ExcludeTrailingPathDelimiter(dir);

    if ((dir.length() < 3) || Path::DirectoryExists(dir) || (Path::ExtractFilePath(dir) == dir))
        return TRUE;

    return ForceDirectories(Path::ExtractFilePath(dir)) && CreateDir(dir);
}


}



namespace cactus
{
struct BOMFlagType {
    DWORD   dwBOMSize;
    LPCSTR  pszBOM;
    BOOL    bIsMutibyte;
    UINT    uMultiByteCodePage;
};
static const BOMFlagType BOMFlags[] = {
    { 3, "\xef\xbb\xbf", TRUE,  CP_UTF8 },
    { 2, "\xff\xfe",     FALSE, CP_ACP },
    { 0, NULL,           TRUE,  CP_THREAD_ACP },
};




class Parser
{
public:
    typedef wchar_t*                CharPtr;
    typedef const wchar_t*          ConstCharPtr;



    static HRESULT LoadText(Document& doc, ConstCharPtr pText, DWORD dwBytes)
    {
        if (!pText)
            return E_POINTER;

        HRESULT hr = ParseDoc(doc, pText, dwBytes);

        if (FAILED(hr))
            return hr;

        return S_OK;
    }



    static HRESULT ParseDoc(Document& doc, ConstCharPtr& pText, DWORD& dwBytes)
    {
        if (IsEndOfIni(pText, dwBytes))
            return S_OK;

        while (!IsEndOfIni(pText, dwBytes)) {
            TrimSpace(pText, dwBytes);

            if (IsEndOfIni(pText, dwBytes))
                return S_OK;

            // find section [ at line being
            if (L'[' != *pText) {
                FindNextLine(pText, dwBytes);
                continue;
            }

            // name begin
            ConstCharPtr pSectionNameBegin = MoveToNextChar(pText, dwBytes);

            if (!FindInLine(pText, dwBytes, L']')) {
                FindNextLine(pText, dwBytes);
                continue;
            }

            CString strSectionName;
            strSectionName.SetString(pSectionNameBegin, (int)(pText - pSectionNameBegin));
            strSectionName.Trim();

            if (strSectionName.IsEmpty()) {
                FindNextLine(pText, dwBytes);
                continue;
            }

            Section newSection = doc[strSectionName];

            if (newSection.IsNull())
                return E_OUTOFMEMORY;

            // skip line of section name
            FindNextLine(pText, dwBytes);

            if (IsEndOfIni(pText, dwBytes))
                return S_OK;

            ParseSection(newSection, pText, dwBytes);
        }

        return S_OK;
    }





    static HRESULT ParseSection(Section& section, ConstCharPtr& pText, DWORD& dwBytes)
    {
        // find key-value in section
        for (NULL; !IsEndOfIni(pText, dwBytes); FindNextLine(pText, dwBytes)) {
            TrimSpace(pText, dwBytes);

            if (IsEndOfIni(pText, dwBytes))
                return S_FALSE;

            // 结束section的解析
            if (L'[' == *pText)
                return S_OK;

            // 跳过注释行
            if (L';' == *pText)
                continue;

            // find key name
            CString strKeyName;

            if (!FindKeyName(pText, dwBytes, strKeyName))
                continue;

            // find value
            CString strValue;
            FindValue(pText, dwBytes, strValue);
            // add key-value
            section.SetAt(strKeyName, strValue);
        }

        return S_FALSE;
    }


    static BOOL FindKeyName(ConstCharPtr& pText, DWORD& dwBytes, CString& strKeyName)
    {
        ConstCharPtr pKeyNameBegin = pText;

        if (IsEndOfIni(pText, dwBytes))
            return FALSE;

        // find '=' in line
        if (!FindInLine(pText, dwBytes, L'='))
            return FALSE;

        strKeyName.SetString(pKeyNameBegin, (int)(pText - pKeyNameBegin));
        strKeyName.Trim();

        if (strKeyName.IsEmpty())
            return FALSE;

        return TRUE;
    }


    static void FindValue(ConstCharPtr& pText, DWORD& dwBytes, CString& strValue)
    {
        strValue.Empty();

        if (IsEndOfIni(pText, dwBytes))
            return;

        if (!FindInLine(pText, dwBytes, L'='))
            return;

        ConstCharPtr pValueBegin = MoveToNextChar(pText, dwBytes);
        FindEndOfLine(pText, dwBytes);

        if (pText == pValueBegin)
            return;

        strValue.SetString(pValueBegin, (int)(pText - pValueBegin));
        strValue.Trim();
    }


    static BOOL FindInLine(ConstCharPtr& pText, DWORD& dwBytes, wchar_t chFind)
    {
        // find line break
        for (NULL; !IsEndOfIni(pText, dwBytes); MoveToNextChar(pText, dwBytes)) {
            wchar_t ch = *pText;

            if (L'\r' == ch || L'\n' == ch)
                return FALSE;

            if (chFind == ch)
                return TRUE;
        }

        return FALSE;
    }


    static void FindEndOfLine(ConstCharPtr& pText, DWORD& dwBytes)
    {
        // find line break
        for (NULL; !IsEndOfIni(pText, dwBytes); MoveToNextChar(pText, dwBytes)) {
            wchar_t ch = *pText;

            if (L'\r' == ch || L'\n' == ch)
                break;
        }
    }


    static void FindNextLine(ConstCharPtr& pText, DWORD& dwBytes)
    {
        // find line break
        FindEndOfLine(pText, dwBytes);

        // find next line
        for (NULL; !IsEndOfIni(pText, dwBytes); MoveToNextChar(pText, dwBytes)) {
            wchar_t ch = *pText;

            if (L'\r' != ch || L'\n' != ch)
                break;
        }
    }


    static void TrimSpace(ConstCharPtr& pText, DWORD& dwBytes)
    {
        // trim line break and white space
        for (NULL; !IsEndOfIni(pText, dwBytes); MoveToNextChar(pText, dwBytes)) {
            wchar_t ch = *pText;

            if (!iswspace(ch))
                break;
        }
    }


    static ConstCharPtr MoveToNextChar(ConstCharPtr& pText, DWORD& dwBytes)
    {
        if (!IsEndOfIni(pText, dwBytes)) {
            ++pText;
            --dwBytes;
        }

        return pText;
    }


    static BOOL IsEndOfIni(ConstCharPtr pText, DWORD& dwBytes)
    {
        assert(pText);
        return !dwBytes || !*pText;
    }
};









KeyValue::KeyValue()
    : m_pDoc(NULL)
    , m_posSection(NULL)
    , m_posKeyValue(NULL)
{
}

KeyValue::KeyValue(Document* pDoc, POSITION posSection, POSITION posKeyValue)
    : m_pDoc(pDoc)
    , m_posSection(posSection)
    , m_posKeyValue(posKeyValue)
{
}

LPCWSTR KeyValue::GetKey()
{
    if (IsNull())
        return L"";

    Document::SectionNode* pSectionNode = m_pDoc->m_docMap.GetValueAt(m_posSection);
    assert(pSectionNode);

    if (!pSectionNode)
        return L"";

    return pSectionNode->GetKeyAt(m_posKeyValue);
}

LPCWSTR KeyValue::GetValueString()
{
    if (IsNull())
        return L"";

    Document::SectionNode* pSectionNode = m_pDoc->m_docMap.GetValueAt(m_posSection);
    assert(pSectionNode);

    if (!pSectionNode)
        return L"";

    return pSectionNode->GetValueAt(m_posKeyValue);
}

DWORD KeyValue::GetValueDWORD()
{
    if (IsNull())
        return 0;

    LPCWSTR lpszValue = GetValueString();

    if (!lpszValue)
        return 0;

    wchar_t* lpszEnd = NULL;
    return wcstoul(lpszValue, &lpszEnd, 10);
}

ULONGLONG KeyValue::GetValueULONGLONG()
{
    if (IsNull())
        return 0;

    LPCWSTR lpszValue = GetValueString();

    if (!lpszValue)
        return 0;

    wchar_t* lpszEnd = NULL;
    return _wcstoui64(lpszValue, &lpszEnd, 10);
}

BOOL KeyValue::IsNull()
{
    return !m_pDoc || !m_posSection || !m_posKeyValue;
}

KeyValue KeyValue::FindNextKeyValue()
{
    if (IsNull())
        return KeyValue();

    Document::SectionNode* pSectionNode = m_pDoc->m_docMap.GetValueAt(m_posSection);
    assert(pSectionNode);

    if (!pSectionNode)
        return KeyValue();

    POSITION posNextKeyValue = m_posKeyValue;
    pSectionNode->GetNext(posNextKeyValue);

    if (!posNextKeyValue)
        return KeyValue();

    return KeyValue(m_pDoc, m_posSection, posNextKeyValue);
}
















Section::Section()
    : m_pDoc(NULL)
    , m_posSection(NULL)
{
}

Section::Section(Document* pDoc, POSITION posSection)
    : m_pDoc(pDoc)
    , m_posSection(posSection)
{
}

BOOL Section::HasKey(LPCWSTR lpszKey)
{
    if (IsNull())
        return FALSE;

    Document::SectionNode* pNode = m_pDoc->m_docMap.GetValueAt(m_posSection);

    if (!pNode)
        return FALSE;

    return !!pNode->Lookup(lpszKey);
}

size_t Section::GetKeyCount()
{
    Document::SectionNode* pNode = m_pDoc->m_docMap.GetValueAt(m_posSection);

    if (!pNode)
        return FALSE;

    return !!pNode->GetCount();
}

KeyValue Section::operator[](LPCWSTR lpszKey)
{
    assert(lpszKey);
    assert(*lpszKey);

    if (IsNull() || !lpszKey || !*lpszKey)
        return KeyValue();

    Document::SectionNode* pNode = m_pDoc->m_docMap.GetValueAt(m_posSection);

    if (!pNode)
        return KeyValue();

    POSITION posKeyValue = pNode->Lookup(lpszKey);

    if (!posKeyValue) {
        posKeyValue = pNode->SetAt(lpszKey, L"");
    }

    return KeyValue(m_pDoc, m_posSection, posKeyValue);
}

KeyValue Section::SetAt(LPCWSTR lpszKey, LPCWSTR lpszValue)
{
    assert(lpszKey);
    assert(*lpszKey);

    if (IsNull() || !lpszKey || !*lpszKey)
        return KeyValue();

    Document::SectionNode* pNode = m_pDoc->m_docMap.GetValueAt(m_posSection);

    if (!pNode)
        return KeyValue();

    POSITION posKeyValue = pNode->SetAt(lpszKey, lpszValue);
    return KeyValue(m_pDoc, m_posSection, posKeyValue);
}

BOOL Section::IsNull()
{
    return !m_pDoc || !m_posSection;
}

Section Section::FindNextSection()
{
    if (IsNull())
        return Section();

    POSITION posNextSection = m_posSection;
    m_pDoc->m_docMap.GetNext(posNextSection);

    if (!posNextSection)
        return Section();

    return Section(m_pDoc, posNextSection);
}

LPCWSTR Section::GetName()
{
    if (IsNull())
        return L"";

    return m_pDoc->m_docMap.GetKeyAt(m_posSection);
}




















Document::Document()
{
}

Document::~Document()
{
    POSITION posSection = m_docMap.GetHeadPosition();

    for (NULL; posSection; m_docMap.GetNext(posSection)) {
        Document::SectionNode* pNode = m_docMap.GetValueAt(posSection);

        if (pNode)
            delete pNode;
    }

    m_docMap.RemoveAll();
}

BOOL Document::HasSection(LPCWSTR lpszKey)
{
    assert(lpszKey);
    assert(*lpszKey);

    if (!lpszKey || !*lpszKey)
        return FALSE;

    return !!m_docMap.Lookup(lpszKey);
}

size_t Document::GetSectionCount()
{
    return m_docMap.GetCount();
}

Section Document::operator[](LPCWSTR lpszKey)
{
    assert(lpszKey);
    assert(*lpszKey);

    if (!lpszKey || !*lpszKey)
        return Section();

    POSITION posSection = m_docMap.Lookup(lpszKey);

    if (!posSection) {
        SectionNode* pNewSection = new SectionNode();

        if (!pNewSection)
            return Section();

        posSection = m_docMap.SetAt(lpszKey, pNewSection);
    }

    return Section(this, posSection);
}

Section Document::FindFirstSection()
{
    POSITION posFirst = m_docMap.GetHeadPosition();

    if (!posFirst)
        return Section(NULL, NULL);

    return Section(this, posFirst);
}


HRESULT Document::LoadFile(LPCWSTR lpszFilePath)
{
    if (!lpszFilePath)
        return E_POINTER;

    CAtlFile hFile;
    HRESULT hr = hFile.Create(
                     lpszFilePath,
                     GENERIC_READ,
                     FILE_SHARE_READ | FILE_SHARE_DELETE,
                     OPEN_EXISTING);

    if (FAILED(hr))
        return hr;

    ULONGLONG uFileSize = 0;
    hr = hFile.GetSize(uFileSize);

    if (FAILED(hr))
        return hr;

    // 对长度作简单限制
    if (uFileSize >= ULONG_MAX)
        return hr;

    CAtlArray<BYTE> byBuf;
    byBuf.SetCount((DWORD)uFileSize);

    if (uFileSize != byBuf.GetCount())
        return E_OUTOFMEMORY;

    hr = hFile.Read(byBuf.GetData(), (DWORD)byBuf.GetCount());

    if (FAILED(hr))
        return hr;

    return LoadBuffer(byBuf.GetData(), (DWORD)byBuf.GetCount());
}

HRESULT Document::LoadBuffer(const BYTE* pBuffer, DWORD dwBytes)
{
    m_docMap.RemoveAll();
    int nBOMType;

    for (nBOMType = 0; nBOMType < (_countof(BOMFlags) - 1); nBOMType++) {
        if (dwBytes < BOMFlags[nBOMType].dwBOMSize)
            continue;

        if (0 == memcmp(BOMFlags[nBOMType].pszBOM, pBuffer, BOMFlags[nBOMType].dwBOMSize))
            break;
    }

    pBuffer += BOMFlags[nBOMType].dwBOMSize;
    dwBytes -= BOMFlags[nBOMType].dwBOMSize;

    if (CP_UTF8 == BOMFlags[nBOMType].uMultiByteCodePage) {
        // utf8
        CStringA strUTF8;
        strUTF8.SetString((LPCSTR)pBuffer, dwBytes);
        CString strWide = CA2W(strUTF8, CP_UTF8);
        return Parser::LoadText(*this, (LPCWSTR)strWide, strWide.GetLength());
    } else if (BOMFlags[nBOMType].bIsMutibyte) {
        // ansi
        CStringA strAnsi;
        strAnsi.SetString((LPCSTR)pBuffer, dwBytes);
        CString strWide = CA2W(strAnsi);
        return Parser::LoadText(*this, (LPCWSTR)strWide, strWide.GetLength());
    } else {
        // wide char
        return Parser::LoadText(*this, (LPCWSTR)pBuffer, dwBytes / sizeof(WCHAR));
    }
}


}


namespace cactus
{
void CMappedInFile::MapToMemory(const string16& fileName)
{
    file = CreateFile(fileName.c_str()
                      , GENERIC_READ
                      , FILE_SHARE_READ
                      , NULL
                      , OPEN_EXISTING
                      , FILE_ATTRIBUTE_NORMAL
                      , NULL);

    if (file == INVALID_HANDLE_VALUE)
        return;

    LARGE_INTEGER fileSize;
    fileSize.QuadPart = 0;
    GetFileSizeEx(file, &fileSize);
    size = (size_t)fileSize.QuadPart;
    mapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);

    if (mapping == NULL) {
        UnMap();
        return;
    }

    LPVOID address = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);

    if (address == NULL) {
        UnMap();
        return;
    }

    buffer = reinterpret_cast<const unsigned char*>(address);
}

void CMappedInFile::UnMap()
{
    if (buffer != NULL)
        UnmapViewOfFile(buffer);

    if (mapping != INVALID_HANDLE_VALUE)
        CloseHandle(mapping);

    if (file != INVALID_HANDLE_VALUE)
        CloseHandle(file);
}

CMappedInFile::CMappedInFile(const string16& fileName)
    : file(INVALID_HANDLE_VALUE)
    , mapping(INVALID_HANDLE_VALUE)
    , buffer(NULL)
    , size(0)
{
    MapToMemory(fileName);
}

CMappedInFile::~CMappedInFile()
{
    UnMap();
}
}


namespace cactus
{
#define SHELL_FOLDER_KEY_NAME   L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"

BOOL BkEnvDir::_GetPath(HKEY hRootKey, LPCWSTR lpszValueName, CString& strPathRet)
{
    CRegKey reg;
    LONG lRet = reg.Open(hRootKey, SHELL_FOLDER_KEY_NAME, KEY_READ);

    if (ERROR_SUCCESS != lRet)
        return FALSE;

    DWORD dwLen = 0;
    reg.QueryStringValue(lpszValueName, NULL, &dwLen);
    lRet = reg.QueryStringValue(lpszValueName, strPathRet.GetBuffer(dwLen + 1), &dwLen);
    strPathRet.ReleaseBuffer(dwLen);

    if (ERROR_SUCCESS != lRet)
        return FALSE;

    return TRUE;
}

void BkEnvDir::_FillCSidlMap()
{
    m_mapCsidl[CSIDL_COMMON_ADMINTOOLS]         = __CsidlKey(L"Common Administrative Tools", TRUE);
    m_mapCsidl[CSIDL_COMMON_APPDATA]            = __CsidlKey(L"Common AppData", TRUE);
    m_mapCsidl[CSIDL_COMMON_DESKTOPDIRECTORY]   = __CsidlKey(L"Common Desktop", TRUE);
    m_mapCsidl[CSIDL_COMMON_DOCUMENTS]          = __CsidlKey(L"Common Documents", TRUE);
    m_mapCsidl[CSIDL_COMMON_PROGRAMS]           = __CsidlKey(L"Common Programs", TRUE);
    m_mapCsidl[CSIDL_COMMON_STARTMENU]          = __CsidlKey(L"Common Start Menu", TRUE);
    m_mapCsidl[CSIDL_COMMON_STARTUP]            = __CsidlKey(L"Common Startup", TRUE);
    m_mapCsidl[CSIDL_ADMINTOOLS]                = __CsidlKey(L"Common Templates", TRUE);
    m_mapCsidl[CSIDL_COMMON_MUSIC]              = __CsidlKey(L"CommonMusic", TRUE);
    m_mapCsidl[CSIDL_COMMON_PICTURES]           = __CsidlKey(L"CommonPictures", TRUE);
    m_mapCsidl[CSIDL_COMMON_VIDEO]              = __CsidlKey(L"CommonVideo", TRUE);
    // Not used in win7
    m_mapCsidl[CSIDL_COMMON_FAVORITES]          = __CsidlKey(L"Common Favorites", TRUE);
    m_mapCsidl[CSIDL_APPDATA]                   = __CsidlKey(L"AppData", FALSE);
    m_mapCsidl[CSIDL_INTERNET_CACHE]            = __CsidlKey(L"Cache", FALSE);
    m_mapCsidl[CSIDL_COOKIES]                   = __CsidlKey(L"Cookies", FALSE);
    m_mapCsidl[CSIDL_DESKTOPDIRECTORY]          = __CsidlKey(L"Desktop", FALSE);
    m_mapCsidl[CSIDL_FAVORITES]                 = __CsidlKey(L"Favorites", FALSE);
    m_mapCsidl[CSIDL_FONTS]                     = __CsidlKey(L"Fonts", FALSE);
    m_mapCsidl[CSIDL_HISTORY]                   = __CsidlKey(L"History", FALSE);
    m_mapCsidl[CSIDL_LOCAL_APPDATA]             = __CsidlKey(L"Local AppData", FALSE);
    m_mapCsidl[CSIDL_PERSONAL]                  = __CsidlKey(L"Personal", FALSE);
    m_mapCsidl[CSIDL_PROGRAMS]                  = __CsidlKey(L"Programs", FALSE);
    m_mapCsidl[CSIDL_RECENT]                    = __CsidlKey(L"Recent", FALSE);
    m_mapCsidl[CSIDL_SENDTO]                    = __CsidlKey(L"SendTo", FALSE);
    m_mapCsidl[CSIDL_STARTMENU]                 = __CsidlKey(L"Start Menu", FALSE);
    m_mapCsidl[CSIDL_STARTUP]                   = __CsidlKey(L"Startup", FALSE);
    m_mapCsidl[CSIDL_TEMPLATES]                 = __CsidlKey(L"Templates", FALSE);
    m_mapCsidl[CSIDL_CDBURN_AREA]               = __CsidlKey(L"CD Burning", FALSE);
    m_mapCsidl[CSIDL_MYMUSIC]                   = __CsidlKey(L"My Music", FALSE);
    m_mapCsidl[CSIDL_MYPICTURES]                = __CsidlKey(L"My Pictures", FALSE);
    m_mapCsidl[CSIDL_MYVIDEO]                   = __CsidlKey(L"My Video", FALSE);
    m_mapCsidl[CSIDL_NETHOOD]                   = __CsidlKey(L"NetHood", FALSE);
    m_mapCsidl[CSIDL_PRINTHOOD]                 = __CsidlKey(L"PrintHood", FALSE);
    // Not used in win7
    m_mapCsidl[CSIDL_PROFILE]                   = __CsidlKey(L"Local Settings", FALSE);
}

BOOL BkEnvDir::Get(DWORD dwCSIDL, CAtlArray<CString> &arrPathRet)
{
    arrPathRet.RemoveAll();
    const CAtlMap<DWORD, __CsidlKey>::CPair *pPair = m_mapCsidl.Lookup(dwCSIDL);

    if (!pPair)
        return FALSE;

    if (pPair->m_value.bCommon) {
        CString strPath;
        BOOL bRet = _GetPath(HKEY_LOCAL_MACHINE, pPair->m_value.lpszValueName, strPath);

        if (!bRet)
            return FALSE;

        arrPathRet.Add(strPath);
    } else {
        CRegKey reg(HKEY_USERS);
        LONG lRet = ERROR_SUCCESS;
        DWORD dwIndex = 0, dwLen = 0, dwMaxLen = 0;
        CString strSID;
        lRet = ::RegQueryInfoKey(reg, NULL, NULL, NULL, NULL, &dwMaxLen, NULL, NULL, NULL, NULL, NULL, NULL);

        while (TRUE) {
            dwLen = dwMaxLen;
            lRet = reg.EnumKey(dwIndex, strSID.GetBuffer(dwLen + 1), &dwLen, NULL);
            strSID.ReleaseBuffer(dwLen);

            if (ERROR_NO_MORE_ITEMS == lRet)
                break;

            if (ERROR_SUCCESS == lRet) {
                CString strPath;
                CRegKey subkey;
                subkey.Open(reg, strSID, KEY_READ);

                if (NULL != subkey.m_hKey) {
                    BOOL bRet = _GetPath(subkey, pPair->m_value.lpszValueName, strPath);

                    if (bRet && (FALSE == strPath.IsEmpty())) {
                        arrPathRet.Add(strPath);
                    }

                    subkey.Close();
                }
            }

            dwIndex ++;
        }
    }

    return TRUE;
}

void BkEnvDir::Init()
{
    _FillCSidlMap();
}

BkEnvDir::~BkEnvDir()
{
}

BkEnvDir::BkEnvDir()
{
}


BkEnvDir::__CsidlKey::__CsidlKey(LPCTSTR _lpszValueName /*= NULL*/, BOOL _bCommon /*= FALSE*/) : lpszValueName(_lpszValueName)
    , bCommon(_bCommon)
{
}
}

namespace cactus
{
BOOL CBkFileUtil::_IsVistaOrLater()
{
    OSVERSIONINFOEX vi = { sizeof(OSVERSIONINFOEX) };
    ::GetVersionEx((LPOSVERSIONINFO)&vi);

    if (vi.dwMajorVersion >= 6)
        return TRUE;
    else
        return FALSE;
}

HRESULT CBkFileUtil::FileExist(LPCTSTR lpszFileName)
{
    DWORD dwRet = ::GetFileAttributes(lpszFileName);

    if (INVALID_FILE_ATTRIBUTES == dwRet) {
        dwRet = ::GetLastError();

        if (ERROR_FILE_NOT_FOUND == dwRet || ERROR_PATH_NOT_FOUND == dwRet)
            return S_FALSE;

        return ::AtlHresultFromWin32(dwRet);
    }

    return S_OK;
}

HRESULT CBkFileUtil::CreateFile(LPCTSTR lpszFileName, ATL::CAtlFile& theFile)
{
    if (!CreateFullPath(lpszFileName))
        return ::AtlHresultFromLastError();

    HRESULT hRet = theFile.Create(lpszFileName, FILE_GENERIC_READ | FILE_GENERIC_WRITE, 0, CREATE_ALWAYS);

    if (FAILED(hRet))
        return hRet;

    FixFilePrivilege(lpszFileName);
    //         if (!FixFilePrivilege(lpszFileName))
    //             return BKERR_FROM_LASTERROR();
    return S_OK;
}

BOOL CBkFileUtil::FixFilePrivilege(LPCTSTR lpszFileName)
{
    if (!_IsVistaOrLater())
        return TRUE;

    BOOL bRet = FALSE;
    CDacl dacl;
    bRet = ::AtlGetDacl(lpszFileName, SE_FILE_OBJECT, &dacl);

    if (!bRet)
        goto Exit0;

    bRet = dacl.AddAllowedAce(Sids::Users(), FILE_GENERIC_READ | FILE_GENERIC_EXECUTE);

    if (!bRet)
        goto Exit0;

    bRet = ::AtlSetDacl(lpszFileName, SE_FILE_OBJECT, dacl);

    if (!bRet)
        goto Exit0;

Exit0:
    return bRet;
}

BOOL CBkFileUtil::DenyExecute(LPCTSTR lpszFileName)
{
    BOOL bRet = FALSE;
    CDacl dacl;
    bRet = ::AtlGetDacl(lpszFileName, SE_FILE_OBJECT, &dacl);

    if (!bRet)
        goto Exit0;

    dacl.RemoveAllAces();
    bRet = dacl.AddAllowedAce(Sids::World(), FILE_GENERIC_READ | FILE_GENERIC_WRITE);
    bRet = dacl.AddDeniedAce(Sids::World(), FILE_GENERIC_EXECUTE);

    if (!bRet)
        goto Exit0;

    bRet = ::AtlSetDacl(lpszFileName, SE_FILE_OBJECT, dacl);

    if (!bRet)
        goto Exit0;

Exit0:
    return bRet;
}

BOOL CBkFileUtil::CreateFullPath(LPCTSTR lpszFileName)
{
    BOOL bRet = FALSE;
    CString strPath;
    int nPos = 3;
    DWORD dwFileAttrib = 0;
    CString strFileName = lpszFileName;

    while (TRUE) {
        nPos = strFileName.Find(_T('\\'), nPos) + 1;

        if (0 == nPos) {
            bRet = TRUE;
            break;
        }

        strPath = strFileName.Left(nPos);
        dwFileAttrib = ::GetFileAttributes(strPath);

        if (INVALID_FILE_ATTRIBUTES == dwFileAttrib) {
            bRet = ::CreateDirectory(strPath, NULL);

            if (!bRet)
                break;

            /*bRet = */FixFilePrivilege(strPath);
            //                 if (!bRet)
            //                     break;
        }
    }

    return bRet;
}
}

namespace cactus
{
char* SharedMemory::begin() const
{
    return m_address;
}

char* SharedMemory::end() const
{
    return m_address + m_size;
}

string16 SharedMemory::name() const
{
    return m_name;
}


SharedMemory::SharedMemory(const string16& name,
                           std::size_t size,
                           SharedMemory::AccessMode mode,
                           const void* addrHint,
                           bool)
    : m_name(name)
    , m_memHandle(INVALID_HANDLE_VALUE)
    , m_fileHandle(INVALID_HANDLE_VALUE)
    , m_size(static_cast<DWORD>(size))
    , m_mode(PAGE_READONLY)
    , m_address(0)
{
    if (mode == SharedMemory::AM_WRITE) {
        m_mode = PAGE_READWRITE;
    }

    m_memHandle = ::CreateFileMapping(INVALID_HANDLE_VALUE,
                                      NULL,
                                      m_mode,
                                      0,
                                      m_size,
                                      m_name.c_str());

    if (!m_memHandle) {
        //throw SystemException("Cannot create shared memory object", m_name);
    }

    map();
}

SharedMemory::SharedMemory(const string16& fileName, AccessMode mode, const void* addrHint)
    : m_name(fileName)
    , m_memHandle(INVALID_HANDLE_VALUE)
    , m_fileHandle(INVALID_HANDLE_VALUE)
    , m_size(0)
    , m_mode(PAGE_READONLY)
    , m_address(0)
{
    DWORD shareMode = FILE_SHARE_READ;
    DWORD fileMode  = GENERIC_READ;

    if (mode == SharedMemory::AM_WRITE) {
        m_mode = PAGE_READWRITE;
        fileMode |= GENERIC_WRITE;
    }

    m_fileHandle = ::CreateFile(m_name.c_str(),
                                fileMode,
                                shareMode,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

    if (m_fileHandle == INVALID_HANDLE_VALUE) {
        //throw OpenFileException("Cannot open memory mapped file", m_name);
        return;
    }

    m_size = ::GetFileSize(m_fileHandle, NULL);
    m_memHandle = ::CreateFileMapping(m_fileHandle, NULL, m_mode, 0, 0, NULL);

    if (!m_memHandle) {
        ::CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
        //throw SystemException("Cannot map file into shared memory", m_name);
    }

    map();
}

SharedMemory::~SharedMemory()
{
    unmap();
    close();
}

void SharedMemory::map()
{
    DWORD access = FILE_MAP_READ;

    if (m_mode == PAGE_READWRITE) {
        access = FILE_MAP_WRITE;
    }

    LPVOID addr = ::MapViewOfFile(m_memHandle, access, 0, 0, m_size);

    if (!addr) {
        //throw SystemException("Cannot map shared memory object", m_name);
    }

    m_address = static_cast<char*>(addr);
}

void SharedMemory::unmap()
{
    if (m_address) {
        ::UnmapViewOfFile(m_address);
        m_address = 0;
        return;
    }
}

void SharedMemory::close()
{
    if (m_memHandle != INVALID_HANDLE_VALUE) {
        ::CloseHandle(m_memHandle);
        m_memHandle = INVALID_HANDLE_VALUE;
    }

    if (m_fileHandle != INVALID_HANDLE_VALUE) {
        ::CloseHandle(m_fileHandle);
        m_fileHandle = INVALID_HANDLE_VALUE;
    }
}
}


namespace cactus
{
BOOL CAtlFile_CT::_SaveStrToFile(LPCSTR pszStr, HANDLE hFile)
{
    int len;

    if (pszStr == NULL)
        len = -1;
    else
        len = strlen(pszStr);

    DWORD dw;

    if (!WriteFile(hFile, &len, sizeof(len), &dw, NULL))
        return FALSE;

    if (len != -1)
        return WriteFile(hFile, pszStr, len, &dw, NULL);

    return TRUE;
}

BOOL CAtlFile_CT::_ReadStrFromFile(LPSTR *ppszStr, HANDLE hFile)
{
    int len;
    DWORD dw;

    if (!ReadFile(hFile, &len, sizeof(len), &dw, NULL) || dw != sizeof(len))
        return FALSE;

    if (len != -1) {
        if (UINT(len) > 100000)
            return FALSE;

        *ppszStr = new char [len + 1];

        if (!ReadFile(hFile, *ppszStr, len, &dw, NULL))
            return FALSE;

        if (len != int (dw))
            return FALSE;

        (*ppszStr) [len] = 0;
    } else {
        *ppszStr = NULL;
    }

    return TRUE;
}

void CAtlFile_CT::WriteString(LPCSTR psz)
{
    if (FALSE == _SaveStrToFile(psz, m_h))
        throw GetLastError();
}

void CAtlFile_CT::ReadString(string8& str)
{
    LPSTR psz;

    if (FALSE == _ReadStrFromFile(&psz, m_h))
        throw GetLastError();

    str  = psz;
    delete [] psz;
}

void CAtlFile_CT::WriteDouble(double f)
{
    Write(&f, sizeof(f));
}

void CAtlFile_CT::ReadDouble(double &f)
{
    Read(&f, sizeof(f));
}

void CAtlFile_CT::WriteInt64(__int64 i)
{
    Write(&i, sizeof(i));
}

void CAtlFile_CT::ReadInt64(__int64 &i)
{
    Read(&i, sizeof(i));
}

void CAtlFile_CT::WriteInt(int i)
{
    Write(&i, sizeof(i));
}

void CAtlFile_CT::ReadInt(int &i)
{
    Read(&i, sizeof(i));
}

DWORD CAtlFile_CT::get_Pos()
{
    return SetFilePointer(m_h, 0, NULL, FILE_CURRENT);
}

void CAtlFile_CT::set_Pos(DWORD dwPos, DWORD dwMethod)
{
    SetFilePointer(m_h, dwPos, NULL, dwMethod);
}

UINT64 CAtlFile_CT::get_Size()
{
    DWORD dwSizeHigh = 0;
    DWORD dwSizeLow;
    dwSizeLow = GetFileSize(m_h, &dwSizeHigh);
    ULARGE_INTEGER uli;
    uli.LowPart = dwSizeLow;
    uli.HighPart = dwSizeHigh;
    return uli.QuadPart;
}



}