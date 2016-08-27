/*!
* \file cactus_file.h
* \brief 文件路径和文件读写相关操作声明
*
* 文件路径和文件读写相关操作声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/
#ifndef __CACTUS_CACTUS_FILE_H__
#define __CACTUS_CACTUS_FILE_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <atlfile.h>
#include <set>
//------------------------------------------------
// PathUtils.h

namespace cactus
{
const char16 PathDelim  = L'\\';
const char16 DirDelimiter = L'\\';
const char16 DriveDelim = L':';
const char16 PathSep    = L';';

/*! @brief 路径
 *
 * 详细说明
 */
class CACTUS_API Path
{
public:

    static BOOL IsDrive(char16* szDrive);

    static BOOL IsFileUsing(const string16& filename);

    static BOOL IsFreeSpaceAvailable(char16* szDrive);

    static long MyGetDiskFreeSpace(CString strPath);


    /*! @brief 获取当前目录
     *
     * @return 返回当前目录
     */
    static string16 GetCwd();

    /*! @brief 设置当前目录
     *
     * @param[in] dir 目录
     * @return 返回空
     */
    static void SetCwd(const string16& dir);


    /*! @brief 获取路径分隔符
     *
     * @return 返回路径分隔符
     */
    static const char16* GetAllPathDelimiters();
    /*! @brief 获取文件名的类别
     *
     * @param [in] filename 文件名
     * @return 文件名的类别
     */
    static string16 ClassifyFile(const string16& filename);

    /*! @brief 判断路径是否包含路径前缀
     *
     * @param [in] prefix 路径前缀
     * @param [in] path 路径
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL PathIsPrefix(const string16& prefix, const string16& path);


    /*! @brief 路径末尾加上\
     *
     * @param [in] path 参数说明
     * @return 返回新路径
     */
    static string16 PathAddBackslash(const string16& path);

    /*! @brief 判断是否有路径的公共前缀
     *
     * @param [in] srcpath 路径一
     * @param [in] destpath 路径二
     * @param [out] prefixpath 公共路径前缀
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL PathCommonPrefix(const string16& srcpath, const string16& destpath, string16& prefixpath);

    /*! @brief 格式化文件大小字符串
     *
     * @param [in] filelength 文件大小
     * @param [out] deststr 返回字符串
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL StrFormatByteSize(LONGLONG filelength, string16& deststr);

    /*! @brief 获取文件类别，文件展示名，文件属性
     *
     * @param [in] filename 文件名
     * @param [in] type_name 文件类别
     * @param [in] displayname 文件展示名
     * @param [in] attributes 文件属性
     * @return 返回空
     */
    static void SHGetFileInfo2(const string16& filename, string16& type_name, string16& displayname, DWORD& attributes);


    /*! @brief 字符串位置是否为'\'
     *
     * @param [in] S 字符串
     * @param [in] Index 位置
     * @return 返回true表示成功，否则为失败
     */
    static bool IsPathDelimiter(const string16& S, int Index);


    /*! @brief 是否以路径分隔符结尾
     *
     * @param [in] path 路径
     * @return 返回true表示成功，否则为失败
     */
    static bool EndsWithPathDelimiter(const string16& path);


    /*! @brief 是否以路径分隔符开头
     *
     * @param [in] path 路径
     * @return 返回true表示成功，否则为失败
     */
    static bool StartsWithPathDelimiter(const string16& path);


    /*! @brief 字符串末尾添加'\'
     *
     * @param [in] S 字符串
     * @return 返回字符串
     */
    static string16 IncludeTrailingBackslash(const string16& S);

    /*! @brief 字符串末尾添加'\'
     *
     * @param [in] S 字符串
     * @return 返回字符串
     */
    static string16 IncludeTrailingPathDelimiter(const string16& S);

    /*! @brief 字符串末尾去除'\'
     *
     * @param [in] S 字符串
     * @return 返回字符串
     */
    static string16 ExcludeTrailingBackslash(const string16& S);

    /*! @brief 字符串末尾去除'\'
     *
     * @param [in] S 字符串
     * @return 返回字符串
     */
    static string16 ExcludeTrailingPathDelimiter(const string16& S);

    static string16 PathRemoveBackslash(const string16& S);


    /*! @brief 字符串开头去除'\'
     *
     * @param [in] S 字符串
     * @return 返回返回字符串
     */
    static string16 ExcludeLeadingPathDelimiter(const string16& S);


    /*! @brief 合并路径
     *
     * @param [in] part1 路径1
     * @param [in] part2 路径2
     * @return 返回合并后的路径
     */
    static string16 CatPath(const string16& part1, const string16& part2);

    static string16 GetFullPathName(const string16& path);


    /*! @brief 获取完全路径文件名
     *
     * @param [in] dir 目录
     * @param [in] filename 文件名
     * @return 返回完全路径文件名
     */
    static string16 GetFullPathForDirectoryAndFilename(const string16& dir, const string16& filename);

    /*! @brief 连接应用程序根目录加文件名
     *
     * @param [in] filename 参数说明
     * @return 返回0表示成功，否则为错误码
     */
    static string16 CompletePathWithModulePath(const string16& filename);

    /*! @brief 提取文件路径
     *
     * @param [in] FileName 文件名
     * @return 返回文件路径
     */
    static string16 ExtractFilePath(const string16& FileName);

    /*! @brief 提取文件名
     *
     * @param [in] FileName 文件名
     * @return 返回文件名
     */
    static string16 ExtractFileName(const string16& FileName);

    /*! @brief 提取扩展名
     *
     * @param [in] FileName 文件名
     * @return 返回扩展名
     */
    static string16 ExtractFileExt(const string16& FileName);



    /*! @brief 获取完全文件名
     *
     * @param [in] FileName 文件名
     * @return 返回完全文件名
     */
    static string16 ExpandFileName(const string16& FileName);

    /*! @brief 改变文件扩展名
     *
     * @param [in] FileName 文件名
     * @param [in] Extension 扩展名
     * @return 返回改变后的文件名
     */
    static string16 ChangeFileExt(const string16& FileName, const string16& Extension);

    /*! @brief 改变文件路径
     *
     * @param [in] FileName 文件名
     * @param [in] Path 文件名
     * @return 返回改变后的文件名
     */
    static string16 ChangeFilePath(const string16& FileName, const string16& Path);

    /*! @brief 获取文件短路径名
     *
     * @param [in] FileName 文件名
     * @return 返回短路径名
     */
    static string16 ExtractShortPathName(const string16& FileName);


    /*! @brief 获取当前工作目录
     *
     * @return 返回当前工作目录
     */
    static string16 GetCurrentDirectory();


    /*! @brief 获取父目录
     *
     * @param[in] path 文件路径
     * @return 返回父目录
     */
    static string16 GetParentDirectory(const string16& path);

    /*! @brief 创建目录
     *
     * @param [in] Dir 目录
     * @return 返回true表示成功，否则为失败
     */
    static bool CreateDir(const string16& Dir);


    static BOOL vmsBuildPathToFile(LPCTSTR ptszFileName);

    /*! @brief 获取当前工作目录
     *
     * @return 返回当前工作目录
     */
    static string16 GetCurrentWorkingDir();

    /*! @brief 获取操作系统所在的盘符
     *
     * @return 返回操作系统所在的盘符
     */
    static string16 GetSysDrv();

    static string16 GetSysDrv2();

    /*! @brief 获取当前exe文件名
     *
     * @return 返回当前exe文件名
     */
    static string16 GetAppFileName();

    static string16 GetAppDir();

    /*! @brief 获取当前exe路径
     *
     * @return 返回当前exe路径
     */
    static string16 GetAppPath();


    /*! @brief 获取不包含路径的exe文件名
     *
     * @return 返回不包含路径的exe文件名
     */
    static string16 GetAppName();

    /*! @brief 获取桌面路径
     *
     * @return 返回桌面路径
     */
    static string16 GetDesktopDirectory();

    /*! @brief 获取Program files目录
     *
     * @return 返回Program files目录
     */
    static string16 GetProgramfilePath();

    /*! @brief 获取用户目录
     *
     * @return 返回用户目录
     * C:\ProgramData
     */
    static string16 GetUserHomeDirectory();

    /*! @brief 获取公共AppData目录
     *
     * @return 返回公共AppData目录
     */
    static string16 GetCommonAppDataDirectory();

    /*! @brief 获取用户AppData目录
     *
     * @return 返回用户AppData目录
     */
    static string16 GetUserDataDirectory();


    /*! @brief 获取临时目录
     *
     * @return 返回临时目录
     */
    static string16 GetTempDir();

    /*! @brief 获取windows目录
     *
     * @return 返回windows目录
     */
    static string16 GetWindowsDir();

    /*! @brief 获取System目录
     *
     * @return 返回System目录
     */
    static string16 GetSystemDir();

    /*! @brief 是否为绝对路径
     *
     * @param [in] path 路径字符串
     * @return 返回true表示成功，否则为失败
     */
    static bool isAbsolutePath(const char16* path);


    /*! @brief 获取文件名和文件扩展名
     *
     * @param [in] path 文件路径
     * @param [out] filename 文件名
     * @param [out] ext 扩展名
     * @return 返回空
     */
    static void dividePath(const string16& path, string16& filename, string16& ext);


    /*! @brief 获取不带路径的文件名
     *
     * @param [in] filename 文件名
     * @return 返回不带路径的文件名
     */
    static string16 GetFilename(const string16& filename);


    /*! @brief 获取扩展名
     *
     * @param [in] filename 文件名
     * @return 返回扩展名
     */
    static string16 GetFileExtension(const string16& filename);


    /*! @brief 分割路径字符串
     *
     * @param [in] filename 文件路径
     * @param [in] path 路径
     * @param [in] basename 文件名
     * @param [in] extname 扩展名
     * @return 返回空
     */
    static void SplitPathFileName(const string16& filename, string16& path, string16& basename, string16& extname);

    /*! @brief 获取进程路径，文件名，扩展名
     *
     * @param [in] path 路径
     * @param [in] basename 文件名
     * @param [in] extname 扩展名
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL GetProcessPathNameAndFileName(string16& path, string16& basename, string16& extname);

    /*! @brief 设置工作路径为进程路径
     *
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL AdjustProcessCurrentDirectory();

    /*! @brief 文件是否存在
     *
     * @param [in] filename 文件名
     * @return 返回true表示成功，否则为失败
     */
    static bool FileExist(const string16& filename);


    static bool DirExists(const string16& dirname);

    /*! @brief 目录是否存在
     *
     * @param [in] dirname 目录名
     * @return 返回true表示成功，否则为失败
     */
    static bool DirectoryExists(const string16& dirname);

    static bool CopyDir(const string16& source_dir, const string16& dest_dir, bool bOverwrite);

    static bool MoveDir(const string16& source_dir, const string16& dest_dir, int OverwriteMode);

    static BOOL MyMoveFile(const string16& source_file, const string16& dest_file);

    static bool RemoveDir(const string16& source_dir, bool bRecurse);

    static bool IsDifferentVolumes(const string16& path1, const string16& path2);

    /*! @brief 获取唯一临时文件名
     *
     * @return 返回唯一临时文件名
     */
    static string16 GetuniqueTempName();

    /*! @brief 递归创建目录
     *
     * @param [in] dir 目录
     * @return 返回true表示成功，否则为失败
     */
    static bool CreateDirectoryNested(const string16& dir);

    static bool CreateDirEx(const char* lpDir);

    static bool CreateDirs(const char *lpFileName);

    static bool CreateDirs(const string16& filename);

    /*! @brief 创建级联目录
     *
     * @param [in] dir 目录
     * @return 返回true表示成功，否则为失败
     */
    static bool ForceCreateDirectory(string16 dir);


    static bool CreateDeepDirectory(const string16& dir);

    /*! @brief 删除级联目录
     *
     * @param [in] dir 目录名
     * @param [in] bContinueWhenFail 失败是否继续
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL DeleteDirectory(const string16& dir, BOOL bContinueWhenFail = TRUE);


    static INT DeletePath(const string16& dir);


    static BOOL DeleteFolder(const string16& dir, BOOL bDeleteDriver);

    static BOOL DeleteAllFiles(const string16& dir, BOOL bDeleteFolderToo = FALSE);


    static string16 GetDriversDir();

    static BOOL GetSubDirs(const string16& dir_path, std::vector<string16>& dir_list, BOOL is_recursion = FALSE);






    /*! @brief 获取文本文件内容
     *
     * @param [in] filename 文件名
     * @param [in] strA 文件内容字符串
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL file_get_contents(const string16& filename, CStringA &strA);

    /*! @brief 获取文本文件内容
     *
     * @param [in] filename 文件名
     * @param [in] strW 文件内容字符串
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL file_get_contents(const string16& filename, CStringW &strW);


    /*! @brief 写文件
     *
     * @param [in] filename 文件名
     * @param [in] pBuffer 字节缓冲区
     * @param [in] nLen 缓冲区长度
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL file_put_contents(const string16& filename, BYTE* pBuffer, INT nLen);

    /*! @brief 写文件
     *
     * @param [in] filename 文件名
     * @param [in] lpszBuffer 字符串缓冲区
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL file_put_contents(const string16& filename, LPCTSTR lpszBuffer);


    /*! @brief 写文件
     *
     * @param [in] filename 文件名
     * @param [in] lpbData 字节缓冲区
     * @param [in] dwFileSize 缓冲区长度
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL SaveNewFile(const string16& filename, PBYTE lpbData, DWORD dwFileSize);


    /*! @brief 获取文件大小
     *
     * @param [in] filename 文件名
     * @return 返回文件大小
     */
    static INT64 file_get_size(const string16& filename);


    /*! @brief 获取大文件大小
     *
     * @param [in] filename 文件名
     * @return 返回文件大小
     */
    static DWORD GetLargeFileSize(const string16& filename);


    /*! @brief 获取AppData目录
     *
     * @return 返回AppData目录
     */
    static string16 GetApplicationDataPath();


    /*! @brief 打印目录下的所有文件
     *
     * @param [in] folderpath 目录
     * @return 返回空
     */
    static void FindAllFile(const string16& folderpath);


    /*! @brief 重命名文件
     *
     * @param[in] source 源文件
     * @param[in] target 目标文件
     * @return 返回空
     */
    static void RenameFile(const string16& source, const string16& target);

    static void CopyFile(const string16& source, const string16& target);

    static BOOL DeleteFile(const string16& filename);


    /*! @brief 移动文件夹
     *
     * @param[in] source 源文件夹
     * @param[in] target 目的文件夹
     * @param[in] bSilent 是否静默
     * @return 返回空
     */
    static void MoveDirectory(const string16& source, const string16& target, BOOL bSilent = TRUE);


    /*! @brief 通过shell打开文件夹并选中
     *
     * @param[in] filename 文件名
     * @return 返回空
     */
    static void OpenFolder(const string16& filename);


    static void ShowItemInFolder(const string16& full_path);

    /*! @brief 获取文件大小
     *
     * @param[in] filename 文件名
     * @return 返回文件大小
     */
    static int64_t GetFileSize(const string16& filename);

    static LPSTREAM LoadResourceToStream(UINT uResId, LPCTSTR lpszResType);


    /*! @brief 桌面图标是否存在
     *
     * @param [in] title 图标名称
     * @return 返回TRUE表示成功，否则为失败
     */
    static BOOL IsExistDesktopIcon(const string16& title);

    static string16 GetSpecialFolderPath(int csidl);


    /*! @brief  Change a DOS path to a Unix path.
     *
     * Replaces '\' by '/', removes any trailing dots on directory/filenames,
     * and changes '*.*' to be equal to '*'
     *
     * @param[in] lpPath path to be modified
     * @return 返回TRUE表示成功，否则为失败
     */
    static void FILEDosToUnixPathA(char* lpPath);

    static void FILEDosToUnixPathW(wchar_t* lpPath);

    static void FILEUnixToDosPathA(char* lpPath);

private:
    static BOOL _DeleteFolder(const string16& dir, BOOL bDeleteDriver);

};




namespace io
{

// Used to hold information about a given file path.  See GetFileInfo below.
struct FileInfo {
    // The size of the file in bytes.  Undefined when is_directory is true.
    int64_t size;

    // True if the file corresponds to a directory.
    bool is_directory;

    // Add additional fields here as needed.
};

FILE* OpenFile(const string16& filename, const char* mode);

FILE* OpenFile(const std::string& filename, const char* mode);

int ReadFile(const string16& filename, char* data, int size);

int WriteFile(const string16& filename, const char* data, int size);

bool ReadFileToString(const string16& path, std::string* contents);

bool GetFileSize(const string16& file_path, int64_t* file_size);

bool GetFileInfo(const string16& file_path, FileInfo* results);

bool CloseFile(FILE* file);
} // namespace io


bool CACTUS_API file_get_contents(const std::string& filename, std::vector<char>& buffer);
bool CACTUS_API file_get_contents(const string16& filename, std::vector<char>& buffer);
bool CACTUS_API file_get_contents(const string16& filename, std::vector<string16>& list);
bool CACTUS_API file_get_contents(const string16& filename, std::set<string16>& list);
std::string CACTUS_API file_get_contents(const string16& filename);
bool CACTUS_API file_put_contents(const string16& filename, const string16& data);
INT64 CACTUS_API filesize(const string16& filename);
BOOL CACTUS_API GetFileLargeSize(LPCWSTR lpFilePath, LARGE_INTEGER& liFileSize);
bool CACTUS_API move_file(const string16& from_filename, const string16& to_filename);
ULONG CACTUS_API FileExistCertificate(LPCWSTR lpFilePath);
BOOL CACTUS_API IsPeFile(LPCWSTR lpFilePath);
void CACTUS_API hexDump(FILE* dmpf, const char* name, BYTE* addr, size_t size);


HRESULT CACTUS_API KGetLoadModulePath(HMODULE hModule, string16& filename);
HRESULT CACTUS_API KGetCurrentLoadModulePath(string16& filename);
HRESULT CACTUS_API KGetLoadModuleDirPath(HMODULE hModule, string16& dir);
HRESULT CACTUS_API KGetCurrentLoadModuleDirPath(string16& dir);
HRESULT CACTUS_API KGetCurrentFilePathOnLoadModuleDirPath(const string16& filename, string16& strFilePath, BOOL* pbFileExist = NULL);
HRESULT CACTUS_API KGetCurrentFilePathOnImageDirPath(const string16& filename, string16& strFilePath, BOOL* pbFileExist = NULL);
HRESULT CACTUS_API KSmartGetCurrentFilePath(const string16& filename, string16& strFilePath, BOOL* pbFileExist = NULL);

} // namespace cactus


namespace cactus
{
void CACTUS_API WriteLine2File(string16& filename, LPCWSTR data);
void CACTUS_API WriteFormat2File(string16& filename, LPCWSTR Format, ...);
void CACTUS_API WriteFormat2FileV(string16& filename, LPCWSTR Format, va_list args);

typedef BOOL (*FN_PerString)(const string16& str);
void CACTUS_API EnumByLine(string16& filename, FN_PerString fnPerString);
std::string CACTUS_API SeekToPhrase(const std::string &strPhrase, std::istream &iStream);

int CACTUS_API CountFiles(const string16& dir, BOOL bSubfolders = TRUE);
INT64 CACTUS_API GetFolderSize(const string16& dir, BOOL bSubfolders = TRUE);
LPTSTR CACTUS_API FileAttribToStr(LPTSTR aBuf, DWORD aAttr);


// delphi对应函数
const DWORD fmOpenRead       = 0x0000;
const DWORD fmOpenWrite      = 0x0001;
const DWORD fmOpenReadWrite  = 0x0002;
const DWORD fmShareCompat    = 0x0000; // DOS compatibility mode is not portable
const DWORD fmShareExclusive = 0x0010;
const DWORD fmShareDenyWrite = 0x0020;
const DWORD fmShareDenyRead  = 0x0030; // write-only not supported on all platforms
const DWORD fmShareDenyNone  = 0x0040;

HANDLE CACTUS_API FileOpen(const string16& FileName, DWORD Mode);

HANDLE CACTUS_API FileCreate(const string16& FileName);

DWORD CACTUS_API FileRead(HANDLE handle, BYTE* buffer, DWORD Count);

DWORD CACTUS_API FileWrite(HANDLE handle, const BYTE* buffer, DWORD Count);

DWORD CACTUS_API FileSeek(HANDLE handle, DWORD Offset, DWORD Origin);

INT64 CACTUS_API FileSeek(HANDLE handle, INT64 Offset, DWORD Origin);

void CACTUS_API FileClose(HANDLE handle);

BOOL CACTUS_API CreateDir(const string16& dir);

BOOL CACTUS_API RemoveDir(const string16& dir);

BOOL CACTUS_API ForceDirectories(string16 dir);
}


namespace cactus
{
class CNoCaseCompareTraits: public CElementTraitsBase<CString>
{
public:
    typedef const CString& INARGTYPE;
    typedef CString& OUTARGTYPE;

    static bool CompareElements(INARGTYPE element1, INARGTYPE element2)
    {
        return 0 == element1.CompareNoCase(element2);
    }

    static int CompareElementsOrdered(INARGTYPE element1, INARGTYPE element2)
    {
        return element1.CompareNoCase(element2);
    }
};




class KeyValue;
class Section;
class Document;



class KeyValue
{
public:
    KeyValue();
    KeyValue(Document* pDoc, POSITION posSection, POSITION posKeyValue);

    LPCWSTR     GetKey();
    LPCWSTR     GetValueString();
    DWORD       GetValueDWORD();
    ULONGLONG   GetValueULONGLONG();

    BOOL        IsNull();

    KeyValue    FindNextKeyValue();

protected:

    Document* m_pDoc;
    POSITION  m_posSection;
    POSITION  m_posKeyValue;
};



class Section
{
public:
    Section();
    Section(Document* pDoc, POSITION posSection);

    BOOL        HasKey(LPCWSTR lpszKey);
    size_t      GetKeyCount();

    KeyValue    operator[](LPCWSTR lpszKey);
    KeyValue    SetAt(LPCWSTR lpszKey, LPCWSTR lpszValue);

    BOOL        IsNull();
    Section     FindNextSection();

    LPCWSTR     GetName();

protected:

    Document* m_pDoc;
    POSITION  m_posSection;
};



class Document
{
public:
    Document();
    virtual ~Document();

    HRESULT     LoadFile(LPCWSTR lpszFilePath);
    HRESULT     LoadBuffer(const BYTE* pBuffer, DWORD dwBytes);

    BOOL        HasSection(LPCWSTR lpszKey);
    size_t      GetSectionCount();

    Section     operator[](LPCWSTR lpszKey);

    Section     FindFirstSection();

protected:
    Document(Document&);
    Document& operator=(Document&);

    friend class KeyValue;
    friend class Section;

    typedef CRBMap<CString, CString, CNoCaseCompareTraits>        SectionNode;
    typedef CRBMap<CString, SectionNode*, CNoCaseCompareTraits>   DocRoot;

    DocRoot m_docMap;
};


}

//------------------------------------------------
// 内存映射文件
namespace cactus
{
/*! @brief 内存映射文件
 *
 * 详细说明
 */
class CACTUS_API CMappedInFile
{
public:

    CMappedInFile(const string16& fileName);
    virtual ~CMappedInFile();

    const BYTE* GetBuffer() const;
    size_t GetSize() const;

private:
    void MapToMemory(const string16& fileName);
    void UnMap();

private:
    HANDLE          file;
    HANDLE          mapping;
    const BYTE*     buffer;
    size_t          size;

};

inline const BYTE* CMappedInFile::GetBuffer() const
{
    return buffer;
}

inline size_t CMappedInFile::GetSize() const
{
    return size;
}
}


namespace cactus
{
/// \brief 获取Shell Folders
///
/// Example:
/// CAtlArray<CString> arrPath;
///
/// BkEnvDir::Get(CSIDL_APPDATA, arrPath);
/// 详细信息
class CACTUS_API BkEnvDir
{
public:
    BkEnvDir();

    ~BkEnvDir();

    void Init();

    BOOL Get(DWORD dwCSIDL, CAtlArray<CString> &arrPathRet);

protected:

    struct __CsidlKey {
        __CsidlKey(LPCTSTR _lpszValueName = NULL, BOOL _bCommon = FALSE);
        LPCTSTR lpszValueName;
        BOOL    bCommon;
    };

    CAtlMap<DWORD, __CsidlKey> m_mapCsidl;

    void _FillCSidlMap();


    BOOL _GetPath(HKEY hRootKey, LPCWSTR lpszValueName, CString& strPathRet);
};
}


namespace cactus
{
class CACTUS_API CBkFileUtil
{
public:
    static BOOL CreateFullPath(LPCTSTR lpszFileName);

    static BOOL DenyExecute(LPCTSTR lpszFileName);

    static BOOL FixFilePrivilege(LPCTSTR lpszFileName);

    static HRESULT CreateFile(LPCTSTR lpszFileName, ATL::CAtlFile& theFile);

    static HRESULT FileExist(LPCTSTR lpszFileName);

private:

    static BOOL _IsVistaOrLater();
};
}

namespace cactus
{
/*! @brief 内存文件映射
 *
 * LPCTSTR szFilePath = TEXT("E:\xxx.zip");
 * SharedMemory sharedMemory(szFilePath, SharedMemory::AM_READ);
 * char* pFileBegin = sharedMemory.begin();
 */
class CACTUS_API SharedMemory
{
public:
    /// 访问模式
    enum AccessMode {
        AM_READ = 0,    ///< 读
        AM_WRITE        ///< 写
    };

public:
    SharedMemory(const string16& name, std::size_t size, AccessMode mode, const void* addrHint = 0, bool server = true);
    SharedMemory(const string16& fileName, SharedMemory::AccessMode mode, const void* addrHint = 0);
    ~SharedMemory();

public:
    /// 共享内存的起始指针
    char* begin() const;

    /// 共享内存的尾部指针
    char* end() const;

    /// 共享内存的名字
    string16 name() const;

protected:
    void map();

    void unmap();

    void close();

private:
    SharedMemory();
    SharedMemory(const SharedMemory&);
    SharedMemory& operator = (const SharedMemory&);

    string16    m_name;
    HANDLE        m_memHandle;
    HANDLE        m_fileHandle;
    DWORD        m_size;
    DWORD        m_mode;
    char*        m_address;
};
}

namespace cactus
{

class CACTUS_API CAtlFile_CT : public ATL::CAtlFile
{
public:
    CAtlFile_CT() throw()
    {
    }

    CAtlFile_CT(__in cactus::CAtlFile_CT& file) throw()
        : ATL::CAtlFile(file)    // Transfers ownership
    {
    }

    explicit CAtlFile_CT(__in HANDLE hFile) throw()
        : ATL::CAtlFile(hFile)    // Takes ownership
    {
    }
    virtual ~CAtlFile_CT() {};
public:
    DWORD get_Pos();
    void set_Pos(DWORD dwPos, DWORD dwMethod);
    UINT64 get_Size();

    void WriteString(LPCSTR psz);
    void ReadString(string8& str);
    void WriteDouble(double f);
    void ReadDouble(double &f);
    void WriteInt64(__int64 i);
    void ReadInt64(__int64 &i);
    void WriteInt(int i);
    void ReadInt(int &i);
private:
    static BOOL _SaveStrToFile(LPCSTR pszStr, HANDLE hFile);
    static BOOL _ReadStrFromFile(LPSTR *ppszStr, HANDLE hFile);


};
}

#endif // __CACTUS_CACTUS_FILE_H__
