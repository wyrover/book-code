#ifndef __CACTUS_CACTUS_MD5_H__
#define __CACTUS_CACTUS_MD5_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <atlstr.h>
#include <boost/smart_ptr.hpp>


namespace cactus
{

CACTUS_API const wchar_t* md5(const wchar_t* filename);
CACTUS_API const wchar_t* md5_file(const wchar_t* filename);

}


//------------------------------------------------
// sha.h
namespace cactus
{

class CACTUS_API ubyte128
{
public:
    ubyte128() {}
    ubyte128(const ubyte64 &value) : high(0), low(value) {}
    ubyte128(const ubyte128 &value) : high(value.high), low(value.low) {}

    ubyte128 operator+(const ubyte64 & value)
    {
        ubyte128 sum;
        sum.high = high;
        sum.low = low + value;

        if (sum.low < low)
            ++sum.high;

        return sum;
    }

    ubyte128 operator+(const ubyte128 & rhs)
    {
        ubyte128 sum;
        sum.high = high + rhs.high;
        sum.low = low + rhs.low;

        if (sum.low < low)
            ++sum.high;

        return sum;
    }
    ubyte128 operator-(const ubyte128 & rhs)
    {
        ubyte128 difference;
        difference.high = high - rhs.high;
        difference.low = low - rhs.low;

        if (difference.low > low)
            --difference.high;

        return difference;
    }

    ubyte64 high;
    ubyte64 low;
};

enum cl_sha_type {
    cl_sha_1,
    cl_sha_224,
    cl_sha_256,
    cl_sha_384,
    cl_sha_512,
    cl_sha_512_224,
    cl_sha_512_256
};

// Private class for the SHA classes

class CACTUS_API SHA
{
public:


    inline ubyte32 leftrotate_uint32(ubyte32 value, int shift) const
    {
        return (value << shift) | (value >> (32 - shift));
    }

    inline ubyte32 rightrotate_uint32(ubyte32 value, int shift) const
    {
        return (value >> shift) | (value << (32 - shift));
    }

    inline ubyte64 leftrotate_uint64(ubyte64 value, int shift) const
    {
        return (value << shift) | (value >> (64 - shift));
    }

    inline ubyte64 rightrotate_uint64(ubyte64 value, int shift) const
    {
        return (value >> shift) | (value << (64 - shift));
    }

    void to_hex_be(char *buffer, ubyte32 value, bool uppercase) const;
    void to_hex_le(char *buffer, ubyte32 value, bool uppercase) const;
    void to_hex64_be(char *buffer, ubyte64 value, bool uppercase) const;

};
}

//------------------------------------------------
// cactus_md5.h
namespace cactus
{


class DataBuffer;
class MD5_Impl;

/// \brief MD5 hash function class.
class CACTUS_API MD5
{


public:
    /// \brief Constructs a MD5 hash generator.
    MD5();



public:
    static const int hash_size = 16;

    /// \brief Returns the calculated hash.
    std::string get_hash(bool uppercase = false) const;

    /// \brief Get hash
    ///
    /// \param out_hash = where to write to
    void get_hash(unsigned char out_hash[hash_size]) const;

    /// \}
    /// \name Operations
    /// \{

public:
    /// \brief Resets the hash generator.
    void reset();

    /// \brief Enable a HMAC based calculation
    ///
    /// Call this function before the initial add(). It is reset by reset()
    ///
    /// \param key_data = The HMAC key
    /// \param key_size = The size of the key_data
    void set_hmac(const void *key_data, int key_size);

    /// \brief Adds data to be hashed.
    void add(const void *data, int size);

    /// \brief Add
    ///
    /// \param data = Data Buffer
    void add(const DataBuffer &data);

    /// \brief Finalize hash calculation.
    void calculate();

private:
    boost::shared_ptr<MD5_Impl> impl;

};

}


namespace cactus
{

class MD5_Impl : private SHA
{
    /// \name Construction
    /// \{

public:
    MD5_Impl();


    /// \}
    /// \name Attributes
    /// \{

public:
    std::string get_hash(bool uppercase = false) const;

    void get_hash(unsigned char out_hash[16]) const;

    /// \}
    /// \name Operations
    /// \{

public:
    void reset();

    void add(const void *data, int size);

    void calculate();

    void set_hmac(const void *key_data, int key_size);

    /// \}
    /// \name Implementation
    /// \{

private:
    void process_chunk();

    const static int block_size = 64;

    inline ubyte32 leftrotate_uint32(ubyte32 value, int shift) const
    {
        return (value << shift) | (value >> (32 - shift));
    }

    ubyte32 h0, h1, h2, h3;

    unsigned char chunk[block_size];

    int chunk_filled;

    unsigned int length_message;

    bool calculated;

    bool hmac_enabled;
    unsigned char hmac_key_chunk[block_size];

    /// \}
};

}


namespace cactus
{
class CACTUS_API CMD5Checksum
{
public:
    //interface functions for the RSA MD5 calculation
    static CString GetMD5(BYTE* pBuf, UINT nLength);
    //static CString GetMD5(CFile& File);
    static CString GetMD5(const CString& strFilePath);

protected:
    //constructor/destructor
    CMD5Checksum();
    virtual ~CMD5Checksum() {};

    //RSA MD5 implementation
    void Transform(BYTE Block[64]);
    void Update(BYTE* Input, ULONG nInputLen);
    CString Final();
    inline DWORD RotateLeft(DWORD x, int n);
    inline void FF(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
    inline void GG(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
    inline void HH(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
    inline void II(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);

    //utility functions
    void DWordToByte(BYTE* Output, DWORD* Input, UINT nLength);
    void ByteToDWord(DWORD* Output, BYTE* Input, UINT nLength);

private:
    BYTE  m_lpszBuffer[64];     //input buffer
    ULONG m_nCount[2];          //number of bits, modulo 2^64 (lsb first)
    ULONG m_lMD5[4];            //MD5 checksum
};

} // namespace cactus

namespace cactus
{
class CACTUS_API MD5_CTX
{
public:
    MD5_CTX();
    ~MD5_CTX();

    void MD5Update(const unsigned char* input, size_t inputLen);
    void MD5Final(unsigned char digest[16]);

private:
    unsigned long int state[4];                 /* state (ABCD) */
    unsigned long int count[2];                 /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];       /* input buffer */
    unsigned char PADDING[64];      /* What? */

private:
    void MD5Init();
    void MD5Transform(unsigned long int state[4], const unsigned char block[64]);
    void MD5_memcpy(unsigned char* output, const unsigned char* input, unsigned int len);
    void Encode(unsigned char *output, const unsigned long int *input, size_t len);	
    void Decode(unsigned long int *output, const unsigned char *input, size_t len);
    void MD5_memset(unsigned char* output, int value, unsigned int len);
};

/*
#ifndef BYTE
    typedef unsigned char BYTE;
#endif
*/

#define NMD5_SIZE 16

class CACTUS_API KMD5
{
public:

    // -------------------------------------------------------------------------
    // 函数     :  KMD5::GetMD5Str
    // 功能     : 获取一个文件的128位MD5串
    // 返回值   : const TCHAR* MD5字符串的指针
    //                  如：_TEXT("C5CE883D2DBDED17F46D24BC906D2A8F")
    // 参数     : const TCHAR *pszFileName 文件名
    // 附注     :
    // -------------------------------------------------------------------------
    const TCHAR* GetMD5Str(const TCHAR* pszFileName);

    // -------------------------------------------------------------------------
    // 函数     :  KMD5::GetMD5ByStr
    // 功能     : 获取一个字符串的128位MD5串
    // 返回值   : const TCHAR* MD5字符串的指针
    //                  如：_TEXT("C5CE883D2DBDED17F46D24BC906D2A8F")
    // 参数     : const TCHAR pszStr 一个以0结尾的字符串
    // 附注     :
    // -------------------------------------------------------------------------
    const TCHAR* GetMD5StrByStr(const TCHAR* pszStr);

    // -------------------------------------------------------------------------
    // 函数     : const unsigned char* KMD5::GetMD5
    // 功能     : 获取一个文件的128位MD5串
    // 返回值   : BYTE MD5串的指针 128位
    // 参数     : const TCHAR *pszFileName  文件名
    // 附注     :
    // -------------------------------------------------------------------------
    const BYTE* GetMD5(const TCHAR* pszFileName);
    const BYTE* GetMD5ByStr(const TCHAR* pszStr);

    //////////////////////////////////////////////////////////////////////////

    // -------------------------------------------------------------------------
    // 函数     : GetFileMd5ForMon
    // 功能     : 获取文件的128位MD5串
    // 返回值   : GetFileMd5ForMon字符串的指针
    //                  如：_TEXT("C5CE883D2DBDED17F46D24BC906D2A8F")
    // 参数     : LPCTSTR pszStr 一个以0结尾的字符串
    // 附注     :
    // ------------------------------------------------------------------------
    const BYTE* GetFileMd5ForMon(const TCHAR* pszFileName);


    // -------------------------------------------------------------------------
    // 函数     : GetFileMd5ForMon
    // 功能     : 获取文件的128位MD5串
    // 返回值   : GetFileMd5ForMon字符串的指针
    //                  如：_TEXT("C5CE883D2DBDED17F46D24BC906D2A8F")
    // 参数     : LPCTSTR pszStr 一个以0结尾的字符串
    // 附注     :
    // ------------------------------------------------------------------------
    const TCHAR* GetFileMd5ForMonStr(const TCHAR* pszFileName);

    //////////////////////////////////////////////////////////////////////////


    KMD5();

    virtual ~KMD5();
    //////////////////////////////////////////////////////////////////////////
protected:

    ULONGLONG* GetFileClusters(
        LPCTSTR lpFileName,
        ULONG ClusterSize,
        ULONG *ClCount,
        ULONG *FileSize);

private:
    BYTE m_pbyMD5[NMD5_SIZE];                       // MD5串
    TCHAR m_pszMD5[(NMD5_SIZE + 1) * 2];            // MD5字符串，以0结尾
    MD5_CTX* m_pAlgoMD5;                            // MD5算法
    BYTE* m_pbyBuf;                                 // 文件缓冲
};

} // namespace cactus


#endif // __CACTUS_CACTUS_MD5_H__
