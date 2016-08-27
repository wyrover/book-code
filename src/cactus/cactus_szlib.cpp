#include <cactus/cactus_szlib.h>
#include <cactus/cactus_file.h>

namespace cactus
{

#define DEFAULT_WINDOWSIZE 15
#define DEFAULT_MEMLEVEL 8
/*
    m_fnCompress = (LPFN_COMPRESS)this->GetProcAddress("compress");
    m_fnUncompress = (LPFN_UNCOMPRESS)this->GetProcAddress("uncompress");
    m_fnCompress2 = (LPFN_COMPRESS2)this->GetProcAddress("compress2");
    m_fnCompressBound = (LPFN_COMPRESSBOUND)this->GetProcAddress("compressBound");

    m_fnDeflateInit2 = (LPFN_DEFLATEINIT2)this->GetProcAddress("deflateInit2_");
    m_fnDeflate = (LPFN_DEFLATE)this->GetProcAddress("deflate");
    m_fnDeflateEnd = (LPFN_DEFLATEEND)this->GetProcAddress("deflateEnd");

    m_fnInflateInit2 = (LPFN_INFLATEINIT2)this->GetProcAddress("inflateInit2_");
    m_fnInflate = (LPFN_INFLATE)this->GetProcAddress("inflate");
    m_fnInflateEnd = (LPFN_INFLATEEND)this->GetProcAddress("inflateEnd");*/
ULONG CmStaticZipImpl::CompressBound(ULONG ulSrcSize)
{
    ULONG ulResult = 0;

    ulResult = compressBound(ulSrcSize);

    return ulResult;
}

int CmStaticZipImpl::Compress3(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize, int nLevel)
{
    int nResult = Z_ERRNO;

    nResult = GzCompress(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize, nLevel);

    return nResult;
}

int CmStaticZipImpl::Compress2(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize, int nLevel)
{
    int nResult = Z_ERRNO;

    nResult = compress2(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize, nLevel);

    return nResult;
}

int CmStaticZipImpl::Uncompress(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize)
{
    int nResult = Z_ERRNO;

    nResult = uncompress(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize);
    
    return nResult;
}

int CmStaticZipImpl::Compress(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize)
{
    int nResult = Z_ERRNO;
    
    nResult = compress(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize);
    
    return nResult;
}

int CmStaticZipImpl::Uncompress3(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize)
{
	int nResult = Z_ERRNO;

	nResult = GzUncompress(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize);

	return nResult;
}

int CmStaticZipImpl::GzCompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level)
{
    z_stream stream;
    int err;

    stream.next_in = (z_const Bytef *)source;
    stream.avail_in = (uInt)sourceLen;
#ifdef MAXSEG_64K
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;
#endif
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;
    
    // http://www.zlib.net/manual.html#Advanced
    // err = deflateInit2_(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, DEFAULT_WINDOWSIZE, DEFAULT_MEMLEVEL,  Z_DEFAULT_STRATEGY);
    // err = deflateInit(&stream, level);
	// MAX_WBITS + 16 for gzip file encoding
	// DEFAULT_WINDOWSIZE 15
    err = deflateInit2_(&stream, level, Z_DEFLATED, MAX_WBITS + 16, 
        DEFAULT_MEMLEVEL, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (err != Z_OK) return err;

    err = deflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        deflateEnd(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    *destLen = stream.total_out;

    err = deflateEnd(&stream);
    return err;
}


int CmStaticZipImpl::GzUncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)
{
	z_stream stream;
	int err;

	stream.next_in = (z_const Bytef *)source;
	stream.avail_in = (uInt)sourceLen;
	/* Check for source > 64K on 16-bit machine: */
	if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;

	stream.next_out = dest;
	stream.avail_out = (uInt)*destLen;
	if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;
	
	//err = inflateInit2(&stream, ZLIB_VERSION, (int)sizeof(z_stream));
	err = inflateInit2_(&stream, DEFAULT_WINDOWSIZE, ZLIB_VERSION, (int)sizeof(z_stream));
	if (err != Z_OK) return err;

	err = inflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		inflateEnd(&stream);
		if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
			return Z_DATA_ERROR;
		return err;
	}
	*destLen = stream.total_out;

	err = inflateEnd(&stream);
	return err;
}

CmStaticZipImpl::~CmStaticZipImpl()
{

}

CmStaticZipImpl::CmStaticZipImpl(LPCTSTR pszDll /*= NULL*/)
{
   
}



} // namespace cactus