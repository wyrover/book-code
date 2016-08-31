#include <cactus/cactus_zlib.h>
#include <cactus/cactus_file.h>

namespace cactus
{

#define DEFAULT_WINDOWSIZE 15
#define DEFAULT_MEMLEVEL 8

ULONG CmZipImpl::CompressBound(ULONG ulSrcSize)
{
    ULONG ulResult = 0;

    if (m_fnCompressBound != NULL) {
        ulResult = m_fnCompressBound(ulSrcSize);
    }

    return ulResult;
}

int CmZipImpl::Compress3(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize, int nLevel)
{
    int nResult = Z_ERRNO;

    if (m_fnDeflateInit2 != NULL) {
        nResult = GzCompress(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize, nLevel);
    }

    return nResult;
}

int CmZipImpl::Compress2(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize, int nLevel)
{
    int nResult = Z_ERRNO;

    if (m_fnCompress2 != NULL) {
        nResult = m_fnCompress2(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize, nLevel);
    }

    return nResult;
}

int CmZipImpl::Uncompress(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize)
{
    int nResult = Z_ERRNO;

    if (m_fnUncompress != NULL) {
        nResult = m_fnUncompress(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize);
    }

    return nResult;
}

int CmZipImpl::Compress(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize)
{
    int nResult = Z_ERRNO;

    if (m_fnCompress != NULL) {
        nResult = m_fnCompress(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize);
    }

    return nResult;
}

int CmZipImpl::Uncompress3(BYTE *pDstBuf, ULONG *pulDstSize, BYTE *pSrcBuf, ULONG ulSrcSize)
{
	int nResult = Z_ERRNO;

	if (m_fnInflateInit2 != NULL) {
		nResult = GzUncompress(pDstBuf, pulDstSize, pSrcBuf, ulSrcSize);
	}

	return nResult;
}

int CmZipImpl::GzCompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level)
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
    err = m_fnDeflateInit2(&stream, level, Z_DEFLATED, MAX_WBITS + 16, 
        DEFAULT_MEMLEVEL, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (err != Z_OK) return err;

    err = m_fnDeflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        m_fnDeflateEnd(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    *destLen = stream.total_out;

    err = m_fnDeflateEnd(&stream);
    return err;
}


int CmZipImpl::GzUncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)
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
	err = m_fnInflateInit2(&stream, DEFAULT_WINDOWSIZE, ZLIB_VERSION, (int)sizeof(z_stream));
	if (err != Z_OK) return err;

	err = m_fnInflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		m_fnInflateEnd(&stream);
		if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
			return Z_DATA_ERROR;
		return err;
	}
	*destLen = stream.total_out;

	err = m_fnInflateEnd(&stream);
	return err;
}

CmZipImpl::~CmZipImpl()
{
    __super::Free();
}

CmZipImpl::CmZipImpl(LPCTSTR pszDll /*= NULL*/)
{
    string16 filename;

    if (pszDll)
        filename = pszDll;
    else
        filename = Path::GetAppDir() + L"zlib.dll";

    if (__super::Load(filename.c_str())) {
        m_fnCompress = (LPFN_COMPRESS)this->GetProcAddress("compress");
        m_fnUncompress = (LPFN_UNCOMPRESS)this->GetProcAddress("uncompress");
        m_fnCompress2 = (LPFN_COMPRESS2)this->GetProcAddress("compress2");
        m_fnCompressBound = (LPFN_COMPRESSBOUND)this->GetProcAddress("compressBound");

        m_fnDeflateInit2 = (LPFN_DEFLATEINIT2)this->GetProcAddress("deflateInit2_");
        m_fnDeflate = (LPFN_DEFLATE)this->GetProcAddress("deflate");
        m_fnDeflateEnd = (LPFN_DEFLATEEND)this->GetProcAddress("deflateEnd");

		m_fnInflateInit2 = (LPFN_INFLATEINIT2)this->GetProcAddress("inflateInit2_");
		m_fnInflate = (LPFN_INFLATE)this->GetProcAddress("inflate");
		m_fnInflateEnd = (LPFN_INFLATEEND)this->GetProcAddress("inflateEnd");
    } else {
        m_fnCompress = NULL;
        m_fnUncompress = NULL;
        m_fnCompress2 = NULL;
        m_fnCompressBound = NULL;

        m_fnDeflateInit2 = NULL;
        m_fnDeflate = NULL;
        m_fnDeflateEnd = NULL;

		m_fnInflateInit2 = NULL;
		m_fnInflate = NULL;
		m_fnInflateEnd = NULL;
    }
}



} // namespace cactus