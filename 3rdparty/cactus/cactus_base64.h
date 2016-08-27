/*!
* \file cactus_base64.h
* \brief BASE64编码声明
*
* BASE64编码声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/
#ifndef __CACTUS_CACTUS_BASE64_H__
#define __CACTUS_CACTUS_BASE64_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <atlstr.h>         // CString
#include <atltypes.h>       // CSize, CPoint, CRect
#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#include <atlcoll.h>

//------------------------------------------------
// base64.h
namespace cactus
{
/// \brief Base64编码
int CACTUS_API Base64Encode(const char *pcszIn, int nInSize, char *pszOut, int nOutSizeMax, int *pnReturnSize, int nNeedCRLF);

/// \brief Base64解码
int CACTUS_API Base64Decode(const char *pcszIn, int nInSize, char *pszOut, int nOutSizeMax, int *pnReturnSize);

/// \brief 估计Encode的Buffer大小.
int CACTUS_API EstimateEncodeBufferSize(int nInSize, int *pnOutSizeMax);
} // namespace cactus

namespace cactus
{
/// base64工具类
class CACTUS_API CBase64Util
{
public:
    static HRESULT base64_encode(const BYTE* source, int source_length, std::string& dest);

    static string16 base64_encode(const string16& source);

    static string8 base64_encode(const string8& source);

    static HRESULT base64_decode(const char* source, int source_length, std::vector<BYTE>& dest);

    static HRESULT base64_decode(const char* source, int source_length, char** dest, int dest_length);

    static string16 base64_decode(const string16& source);

    static string8 base64_decode(const string8& source);
    
};
} // namespace cactus

#endif // __CACTUS_CACTUS_BASE64_H__
