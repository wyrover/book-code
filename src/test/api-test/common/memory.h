#ifndef memory_h__
#define memory_h__

#include <Windows.h>

namespace cactus
{

/**
 * @brief 获取内存大小
 */
class ZLMemory
{
public:
    /**
     * @brief 获取内存大小，以MB为单位
     * @return 成功返回内存大小(MB)
     */
    static ULONG GetMemorySize();

    /**
     * @brief 获取已使用内存大小，以MB为单位
     * @return 成功返回内存大小(MB)
     */
    static ULONGLONG GetUsedMem();
};


}

#endif // memory_h__
