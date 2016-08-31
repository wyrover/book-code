
#ifndef __CACTUS_CACTUS_ZLIB_H__
#define __CACTUS_CACTUS_ZLIB_H__

#include <cactus/cactus.h>
#include <cactus/cactus_dll.h>
#include <cactus/cactus_pattern.h>
#include <zlib/zlib.h>

namespace cactus
{

class CACTUS_API CmStaticZipImpl
{
public:
    CmStaticZipImpl(LPCTSTR pszDll = NULL);
    ~CmStaticZipImpl();

    int Compress(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize);

    int Uncompress(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize);

    int Compress2(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize, int nLevel);

    /* for gzip */
    int Compress3(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize, int nLevel);
    int Uncompress3(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize);

    int GzCompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level);
	int GzUncompress (Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);

    ULONG CompressBound(ULONG ulSrcSize);
};

typedef cactus::Singleton<CmStaticZipImpl> CmStaticZip;
} // namespace cactus

#endif // __CACTUS_CACTUS_ZLIB_H__
