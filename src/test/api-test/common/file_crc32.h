#ifndef file_crc32_h__
#define file_crc32_h__

#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

bool GetFileCRC32(const TCHAR* pszFileName, unsigned& uCrc32);

bool GetFileCRC32Ex(const TCHAR* pszFileName, unsigned& uCrc32);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif // file_crc32_h__
