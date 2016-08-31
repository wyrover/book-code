
#ifndef __CACTUS_CACTUS_ZLIB_H__
#define __CACTUS_CACTUS_ZLIB_H__

#include <cactus/cactus.h>
#include <cactus/cactus_dll.h>
#include <cactus/cactus_pattern.h>
#include <zlib/zlib.h>

namespace cactus
{
class CACTUS_API CmZipImpl : public vmsDLL
{
private:
    typedef int (*LPFN_COMPRESS)(BYTE *, ULONG *, BYTE *, ULONG);
    typedef int (*LPFN_UNCOMPRESS)(BYTE *, ULONG *, BYTE *, ULONG);
    typedef int (*LPFN_COMPRESS2)(BYTE *, ULONG *, BYTE *, ULONG, int);
    typedef ULONG(*LPFN_COMPRESSBOUND)(ULONG);	

    typedef int (*LPFN_DEFLATEINIT2)(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, char * version, int stream_size);	
    typedef int (*LPFN_DEFLATE)(z_streamp strm, int flush);	
    typedef int (*LPFN_DEFLATEEND)(z_streamp strm);	

	typedef int (*LPFN_INFLATEINIT2)(z_streamp strm, int windowBits, char * version, int stream_size);	
	typedef int (*LPFN_INFLATE)(z_streamp strm, int flush);	
	typedef int (*LPFN_INFLATEEND)(z_streamp strm);	

private:
    LPFN_COMPRESS m_fnCompress;
    LPFN_UNCOMPRESS m_fnUncompress;
    LPFN_COMPRESS2 m_fnCompress2;
    LPFN_COMPRESSBOUND m_fnCompressBound;

	LPFN_DEFLATEINIT2 m_fnDeflateInit2;
	LPFN_DEFLATE m_fnDeflate;
	LPFN_DEFLATEEND m_fnDeflateEnd;

    LPFN_INFLATEINIT2 m_fnInflateInit2;
    LPFN_INFLATE m_fnInflate;
    LPFN_INFLATEEND m_fnInflateEnd;

public:
    CmZipImpl(LPCTSTR pszDll = NULL);
    ~CmZipImpl();

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

typedef cactus::Singleton<CmZipImpl> CmZip;
} // namespace cactus

#endif // __CACTUS_CACTUS_ZLIB_H__
