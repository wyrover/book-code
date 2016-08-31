#ifndef wow64_h__
#define wow64_h__

#include <Windows.h>
#include <tchar.h>

namespace cactus
{

typedef
BOOL
WINAPI
Fn_Wow64DisableWow64FsRedirectionFunction(
    __out PVOID *OldValue
);

typedef
BOOL
WINAPI
Fn_Wow64RevertWow64FsRedirectionFunction(
    __in PVOID OlValue
);

typedef BOOL
WINAPI
Fn_IsWow64Process(
    __in  HANDLE hProcess,
    __out PBOOL Wow64Process
);
/**
 * @brief 32位、64位兼容处理
 */
class ZLWow64
{
public:
    ZLWow64();
    ~ZLWow64();

public:
    /**
     * @brief 进程是否运行在Wow64下
     * @param[in]  hProcess       进程句柄
     * @param[out] pbWow64Process BOOL指针
     * @return 成功返回TRUE，失败返回FALSE
     * @see IsWow64Process
     */
    static BOOL CheckIsWow64Process(HANDLE hProcess, BOOL* pbWow64Process);
    /**
     * @brief 当前进程是否运行在Wow64下
     * @param[out] pbWow64Process BOOL指针
     * @return 成功返回TRUE，失败返回FALSE
     */
    static BOOL CheckCurrentProcessIsWow64Process(BOOL* pbWow64Process);
    /**
     * @brief 禁用文件系统重定向为调用线程
     * @param[out] ppVoidValue Wow64文件系统重定向值
     * @return 成功返回TRUE，失败返回FALSE
     */
    static BOOL Wow64DisableWow64FsRedirection(PVOID* ppVoidValue);
    /**
     * @brief 恢复文件系统重定向为调用线程
     * @param[in] ppVoidValue Wow64文件系统重定向值
     * @return 成功返回TRUE，失败返回FALSE
     */
    static BOOL Wow64RevertWow64FsRedirection(PVOID pOldValue);
};

class ZLWow64Guard
{
public:
    ZLWow64Guard();

    ~ZLWow64Guard();

private:
    BOOL  bIsWow64_;
    PVOID pVoidValue_;
};

}

#endif // wow64_h__
