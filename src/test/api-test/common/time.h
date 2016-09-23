#ifndef time_h__
#define time_h__

#include <Windows.h>

namespace cactus
{

/**
 * @brief 时间相关操作
 */
class ZLTime
{
public:
    /**
     * @brief __time64_t到FileTime的转化
     * @param[in] tm __time64_t
     * @return 成功返回FILETIME
     */
    static FILETIME Time642FileTime(const __time64_t& tm);
    /**
     * @brief FileTime到__time64_t的转化
     * @param[in] pft FILETIME结构体指针
     * @return 返回__time64_t
     */
    static __time64_t FileTime2Time64(const FILETIME& pft);

    static __time64_t SystemTime2Time64(const SYSTEMTIME& st);

    static BOOL SystemTime2FileTime(const SYSTEMTIME& syst, FILETIME& filet);

    static BOOL FileTime2SystemTime(const FILETIME& filet, SYSTEMTIME& syst);

    static SYSTEMTIME GetLocalTime();

    static SYSTEMTIME GetSystemTime();
    /**
     * @brief 计算TimeInterval
     * @param[in]  llTimeBegin    开始时间
     * @param[out] dfTimeInterval TimeInterval
     * @return 成功返回TRUE，失败返回FALSE
     */
    static BOOL Calc(LARGE_INTEGER& llTimeBegin, double& dfTimeInterval);

    static BOOL GetTime(LARGE_INTEGER& llCurrentTime);
};

void PrintSystemTime(PCHAR lpszBuf, DWORD cbBuf);


}


#endif // time_h__
