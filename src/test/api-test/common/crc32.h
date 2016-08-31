#ifndef _CRC_32_
#define _CRC_32_

#include <Windows.h>
#include <tchar.h>


//////////////////////////////////////////////////////////////////////////
extern "C"
{
    unsigned int CRC32(unsigned int CRC, void const *pvBuf, unsigned int uLen);
    unsigned int CRC32_file(unsigned int CRC, LPCTSTR lpFileName);
};

//////////////////////////////////////////////////////////////////////////

#endif  // _CRC_32