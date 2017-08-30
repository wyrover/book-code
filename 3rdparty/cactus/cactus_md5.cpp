#include "cactus/cactus_md5.h"
#include "cactus/cactus_exception.h"
#include "cactus/cactus_databuffer.h"
#include <atlfile.h>
#include <winioctl.h>

#ifndef WIN32
    #include <cstring>
#endif

namespace cactus
{
const wchar_t* md5(const wchar_t* str)
{
    KMD5 md5;
    return md5.GetMD5StrByStr(str);
}

const wchar_t* md5_file(const wchar_t* filename)
{
    KMD5 md5;
    return md5.GetMD5Str(filename);
}

}

namespace cactus
{
void SHA::to_hex_le(char *buffer, ubyte32 value, bool uppercase) const
{
    ubyte32 values[4];
    values[0] = (value & 0x000000ff);
    values[1] = ((value & 0x0000ff00) >> 8);
    values[2] = ((value & 0x00ff0000) >> 16);
    values[3] = ((value & 0xff000000) >> 24);
    ubyte32 low = '0';
    ubyte32 high = uppercase ? 'A' : 'a';

    for (int i = 0; i < 4; i++) {
        ubyte32 a = ((values[i] & 0xf0) >> 4);
        ubyte32 b = (values[i] & 0x0f);
        buffer[i * 2 + 0] = (a < 10) ? (low + a) : (high + a - 10);
        buffer[i * 2 + 1] = (b < 10) ? (low + b) : (high + b - 10);
    }
}

void SHA::to_hex_be(char *buffer, ubyte32 value, bool uppercase) const
{
    ubyte32 values[4];
    values[0] = ((value & 0xff000000) >> 24);
    values[1] = ((value & 0x00ff0000) >> 16);
    values[2] = ((value & 0x0000ff00) >> 8);
    values[3] = (value & 0x000000ff);
    ubyte32 low = '0';
    ubyte32 high = uppercase ? 'A' : 'a';

    for (int i = 0; i < 4; i++) {
        ubyte32 a = ((values[i] & 0xf0) >> 4);
        ubyte32 b = (values[i] & 0x0f);
        buffer[i * 2 + 0] = (a < 10) ? (low + a) : (high + a - 10);
        buffer[i * 2 + 1] = (b < 10) ? (low + b) : (high + b - 10);
    }
}

void SHA::to_hex64_be(char *buffer, ubyte64 value, bool uppercase) const
{
    ubyte32 values[8];
    values[0] = ((value & 0xff00000000000000ll) >> 56);
    values[1] = ((value & 0x00ff000000000000ll) >> 48);
    values[2] = ((value & 0x0000ff0000000000ll) >> 40);
    values[3] = ((value & 0x000000ff00000000ll) >> 32);
    values[4] = ((value & 0x00000000ff000000ll) >> 24);
    values[5] = ((value & 0x0000000000ff0000ll) >> 16);
    values[6] = ((value & 0x000000000000ff00ll) >> 8);
    values[7] = (value &  0x00000000000000ffll);
    ubyte32 low = '0';
    ubyte32 high = uppercase ? 'A' : 'a';

    for (int i = 0; i < 8; i++) {
        ubyte32 a = ((values[i] & 0xf0) >> 4);
        ubyte32 b = (values[i] & 0x0f);
        buffer[i * 2 + 0] = (a < 10) ? (low + a) : (high + a - 10);
        buffer[i * 2 + 1] = (b < 10) ? (low + b) : (high + b - 10);
    }
}
}

namespace cactus
{

/////////////////////////////////////////////////////////////////////////////
// MD5 Construction:

MD5::MD5()
    : impl(new MD5_Impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// MD5 Attributes:

std::string MD5::get_hash(bool uppercase) const
{
    return impl->get_hash(uppercase);
}

void MD5::get_hash(unsigned char out_hash[16]) const
{
    impl->get_hash(out_hash);
}

/////////////////////////////////////////////////////////////////////////////
// MD5 Operations:

void MD5::reset()
{
    impl->reset();
}

void MD5::add(const void *data, int size)
{
    impl->add(data, size);
}


void MD5::add(const DataBuffer &data)
{
    add(data.get_data(), data.get_size());
}


void MD5::calculate()
{
    impl->calculate();
}

void MD5::set_hmac(const void *key_data, int key_size)
{
    impl->set_hmac(key_data, key_size);
}

/////////////////////////////////////////////////////////////////////////////
// MD5 Implementation:

}

namespace cactus
{

/////////////////////////////////////////////////////////////////////////////
// MD5_Impl Construction:

MD5_Impl::MD5_Impl()
{
    reset();
}

/////////////////////////////////////////////////////////////////////////////
// MD5_Impl Attributes:

std::string MD5_Impl::get_hash(bool uppercase) const
{
    if (calculated == false)
        throw Exception("MD5 hash has not been calculated yet!");

    char digest[33];
    to_hex_le(digest, h0, uppercase);
    to_hex_le(digest + 8, h1, uppercase);
    to_hex_le(digest + 16, h2, uppercase);
    to_hex_le(digest + 24, h3, uppercase);
    digest[32] = 0;
    return digest;
}

void MD5_Impl::get_hash(unsigned char out_hash[16]) const
{
    if (calculated == false)
        throw Exception("MD5 hash has not been calculated yet!");

    out_hash[0] = (unsigned char)(h0 & 0xff);
    out_hash[1] = (unsigned char)((h0 >> 8) & 0xff);
    out_hash[2] = (unsigned char)((h0 >> 16) & 0xff);
    out_hash[3] = (unsigned char)((h0 >> 24) & 0xff);
    out_hash[4] = (unsigned char)(h1 & 0xff);
    out_hash[5] = (unsigned char)((h1 >> 8) & 0xff);
    out_hash[6] = (unsigned char)((h1 >> 16) & 0xff);
    out_hash[7] = (unsigned char)((h1 >> 24) & 0xff);
    out_hash[8] = (unsigned char)(h2 & 0xff);
    out_hash[9] = (unsigned char)((h2 >> 8) & 0xff);
    out_hash[10] = (unsigned char)((h2 >> 16) & 0xff);
    out_hash[11] = (unsigned char)((h2 >> 24) & 0xff);
    out_hash[12] = (unsigned char)(h3 & 0xff);
    out_hash[13] = (unsigned char)((h3 >> 8) & 0xff);
    out_hash[14] = (unsigned char)((h3 >> 16) & 0xff);
    out_hash[15] = (unsigned char)((h3 >> 24) & 0xff);
}

/////////////////////////////////////////////////////////////////////////////
// MD5_Impl Operations:

void MD5_Impl::reset()
{
    //  http://tools.ietf.org/html/rfc1321 section 3.3
    h0 = 0x67452301;
    h1 = 0xEFCDAB89;
    h2 = 0x98BADCFE;
    h3 = 0x10325476;
    memset(chunk, 0, block_size);
    chunk_filled = 0;
    length_message = 0;
    calculated = false;
    hmac_enabled = false;
}

void MD5_Impl::add(const void *_data, int size)
{
    if (calculated)
        reset();

    const unsigned char *data = (const unsigned char *) _data;
    int pos = 0;

    while (pos < size) {
        int data_left = size - pos;
        int buffer_space = block_size - chunk_filled;
        int data_used = min(buffer_space, data_left);
        memcpy(chunk + chunk_filled, data + pos, data_used);
        chunk_filled += data_used;
        pos += data_used;

        if (chunk_filled == block_size) {
            process_chunk();
            chunk_filled = 0;
        }
    }

    length_message += size;
}

void MD5_Impl::set_hmac(const void *key_data, int key_size)
{
    memset(hmac_key_chunk, 0, block_size);
    int key_chunk_filled;

    if (key_size > block_size) {
        MD5 md5;
        md5.add(key_data, key_size);
        md5.calculate();
        key_chunk_filled = MD5::hash_size;
        md5.get_hash(hmac_key_chunk);
    } else {
        memcpy(hmac_key_chunk, key_data, key_size);
        key_chunk_filled = key_size;
    }

    for (int cnt = 0; cnt < block_size; cnt++) { // XOR key with inner pad values
        hmac_key_chunk[cnt] ^= 0x36;
    }

    add(hmac_key_chunk, block_size);    // Add the inner HMAC

    for (int cnt = 0; cnt < block_size; cnt++) { // XOR key with outer pad values
        hmac_key_chunk[cnt] ^= 0x36;    // Undo the inner pad
        hmac_key_chunk[cnt] ^= 0x5c;
    }

    hmac_enabled = true;    // This has to be after the add(), as that function may call reset()
}

void MD5_Impl::calculate()
{
    if (calculated)
        reset();

    // append a single "1" bit to message
    // append "0" bits until message length 鈮?448 鈮?-64 (mod 512)
    // append length of message, in bits as 64-bit little-endian integer to message
    unsigned char end_data[block_size * 2];
    memset(end_data, 0, block_size * 2);
    end_data[0] = 128;
    int size = block_size - chunk_filled;

    if (size < 9)
        size += block_size;

    ubyte64 length_message64 = length_message * (ubyte64) 8;
    unsigned int length_upper = (unsigned int)(length_message64 >> 32);
    unsigned int length_lower = (unsigned int)(length_message64 & 0xffffffff);
    end_data[size - 8] = (length_lower & 0x000000ff);
    end_data[size - 7] = (length_lower & 0x0000ff00) >> 8;
    end_data[size - 6] = (length_lower & 0x00ff0000) >> 16;
    end_data[size - 5] = (length_lower & 0xff000000) >> 24;
    end_data[size - 4] = (length_upper & 0x000000ff);
    end_data[size - 3] = (length_upper & 0x0000ff00) >> 8;
    end_data[size - 2] = (length_upper & 0x00ff0000) >> 16;
    end_data[size - 1] = (length_upper & 0xff000000) >> 24;
    add(end_data, size);

    if (chunk_filled != 0)
        throw Exception("Error in MD5_Impl class. Still chunk data at end of calculate");

    calculated = true;

    if (hmac_enabled) {
        unsigned char temp_hash[MD5::hash_size];
        get_hash(temp_hash);
        reset();
        add(hmac_key_chunk, block_size);    // Add the outer HMAC
        add(temp_hash, MD5::hash_size);
        calculate();
    }
}

/////////////////////////////////////////////////////////////////////////////
// MD5_Impl Implementation:

void MD5_Impl::process_chunk()
{
    int i;
    unsigned int w[16];

    for (i = 0; i < 16; i++) {
        unsigned int b1 = chunk[i * 4 + 0];
        unsigned int b2 = chunk[i * 4 + 1];
        unsigned int b3 = chunk[i * 4 + 2];
        unsigned int b4 = chunk[i * 4 + 3];
        w[i] = (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
    }

    static const unsigned char r[64] = {
        // Round 0
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,

        // Round 1
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,

        // Round 2
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,

        // Round 3
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };
    static const ubyte32 k[64] = {
        // for i from 0 to 63
        //  k[i] := floor(abs(sin(i + 1)) 脳 (2 pow 32))
        // end for
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
    ubyte32 a = h0;
    ubyte32 b = h1;
    ubyte32 c = h2;
    ubyte32 d = h3;

    for (i = 0; i < 64; i++) {
        ubyte32 f, g;

        if (i < 16) {
            f = d ^ (b & (c ^ d));  // f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = c ^ (d & (b ^ c));  // f = (d & b) | ((~d) & c);
            g = (5 * i + 1) & 0xF;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) & 0xF;
        } else {
            f =  c ^ (b | (~d));
            g = (7 * i) & 0xF;
        }

        ubyte32 temp = d;
        d = c;
        c = b;
        b = b + leftrotate_uint32(a + f + k[i] + w[g], r[i]);
        a = temp;
    }

    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
}

}


namespace cactus
{

//MD5ChecksumDefines.h : MD5 Checksum constants

//Magic initialization constants
#define MD5_INIT_STATE_0 0x67452301
#define MD5_INIT_STATE_1 0xefcdab89
#define MD5_INIT_STATE_2 0x98badcfe
#define MD5_INIT_STATE_3 0x10325476

//Constants for Transform routine.
#define MD5_S11  7
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21  5
#define MD5_S22  9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31  4
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41  6
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

//Transformation Constants - Round 1
#define MD5_T01  0xd76aa478 //Transformation Constant 1 
#define MD5_T02  0xe8c7b756 //Transformation Constant 2
#define MD5_T03  0x242070db //Transformation Constant 3
#define MD5_T04  0xc1bdceee //Transformation Constant 4
#define MD5_T05  0xf57c0faf //Transformation Constant 5
#define MD5_T06  0x4787c62a //Transformation Constant 6
#define MD5_T07  0xa8304613 //Transformation Constant 7
#define MD5_T08  0xfd469501 //Transformation Constant 8
#define MD5_T09  0x698098d8 //Transformation Constant 9
#define MD5_T10  0x8b44f7af //Transformation Constant 10
#define MD5_T11  0xffff5bb1 //Transformation Constant 11
#define MD5_T12  0x895cd7be //Transformation Constant 12
#define MD5_T13  0x6b901122 //Transformation Constant 13
#define MD5_T14  0xfd987193 //Transformation Constant 14
#define MD5_T15  0xa679438e //Transformation Constant 15
#define MD5_T16  0x49b40821 //Transformation Constant 16

//Transformation Constants - Round 2
#define MD5_T17  0xf61e2562 //Transformation Constant 17
#define MD5_T18  0xc040b340 //Transformation Constant 18
#define MD5_T19  0x265e5a51 //Transformation Constant 19
#define MD5_T20  0xe9b6c7aa //Transformation Constant 20
#define MD5_T21  0xd62f105d //Transformation Constant 21
#define MD5_T22  0x02441453 //Transformation Constant 22
#define MD5_T23  0xd8a1e681 //Transformation Constant 23
#define MD5_T24  0xe7d3fbc8 //Transformation Constant 24
#define MD5_T25  0x21e1cde6 //Transformation Constant 25
#define MD5_T26  0xc33707d6 //Transformation Constant 26
#define MD5_T27  0xf4d50d87 //Transformation Constant 27
#define MD5_T28  0x455a14ed //Transformation Constant 28
#define MD5_T29  0xa9e3e905 //Transformation Constant 29
#define MD5_T30  0xfcefa3f8 //Transformation Constant 30
#define MD5_T31  0x676f02d9 //Transformation Constant 31
#define MD5_T32  0x8d2a4c8a //Transformation Constant 32

//Transformation Constants - Round 3
#define MD5_T33  0xfffa3942 //Transformation Constant 33
#define MD5_T34  0x8771f681 //Transformation Constant 34
#define MD5_T35  0x6d9d6122 //Transformation Constant 35
#define MD5_T36  0xfde5380c //Transformation Constant 36
#define MD5_T37  0xa4beea44 //Transformation Constant 37
#define MD5_T38  0x4bdecfa9 //Transformation Constant 38
#define MD5_T39  0xf6bb4b60 //Transformation Constant 39
#define MD5_T40  0xbebfbc70 //Transformation Constant 40
#define MD5_T41  0x289b7ec6 //Transformation Constant 41
#define MD5_T42  0xeaa127fa //Transformation Constant 42
#define MD5_T43  0xd4ef3085 //Transformation Constant 43
#define MD5_T44  0x04881d05 //Transformation Constant 44
#define MD5_T45  0xd9d4d039 //Transformation Constant 45
#define MD5_T46  0xe6db99e5 //Transformation Constant 46
#define MD5_T47  0x1fa27cf8 //Transformation Constant 47
#define MD5_T48  0xc4ac5665 //Transformation Constant 48

//Transformation Constants - Round 4
#define MD5_T49  0xf4292244 //Transformation Constant 49
#define MD5_T50  0x432aff97 //Transformation Constant 50
#define MD5_T51  0xab9423a7 //Transformation Constant 51
#define MD5_T52  0xfc93a039 //Transformation Constant 52
#define MD5_T53  0x655b59c3 //Transformation Constant 53
#define MD5_T54  0x8f0ccc92 //Transformation Constant 54
#define MD5_T55  0xffeff47d //Transformation Constant 55
#define MD5_T56  0x85845dd1 //Transformation Constant 56
#define MD5_T57  0x6fa87e4f //Transformation Constant 57
#define MD5_T58  0xfe2ce6e0 //Transformation Constant 58
#define MD5_T59  0xa3014314 //Transformation Constant 59
#define MD5_T60  0x4e0811a1 //Transformation Constant 60
#define MD5_T61  0xf7537e82 //Transformation Constant 61
#define MD5_T62  0xbd3af235 //Transformation Constant 62
#define MD5_T63  0x2ad7d2bb //Transformation Constant 63
#define MD5_T64  0xeb86d391 //Transformation Constant 64


//Null data (except for first BYTE) used to finalise the checksum calculation
static unsigned char PADDING[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};



/*****************************************************************************************
FUNCTION:       CMD5Checksum::GetMD5
DETAILS:        static, public
DESCRIPTION:    Gets the MD5 checksum for a specified file
RETURNS:        CString : the hexadecimal MD5 checksum for the specified file
ARGUMENTS:      CString& strFilePath : the full pathname of the specified file
NOTES:          Provides an interface to the CMD5Checksum class. 'strFilePath' name should
                hold the full pathname of the file, eg C:\My Documents\Arcticle.txt.
                NB. If any problems occur with opening or reading this file, a CFileException
                will be thrown; callers of this function should be ready to catch this
                exception.
*****************************************************************************************/
CString CMD5Checksum::GetMD5(const CString& strFilePath)
{
    //open the file as a binary file in readonly mode, denying write access
    //CFile File(strFilePath, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary);
    if (!PathFileExists(strFilePath))
        return _T("[FILE.NOT.EXISTS]");

    CMD5Checksum MD5Checksum;       //checksum object
    int nLength = 0;                //number of bytes read from the file
    const int nBufferSize = 1024;   //checksum the file in blocks of 1024 bytes
    BYTE Buffer[nBufferSize];       //buffer for data read from the file
    /*
    CFileInStream fs(strFilePath);
    if(fs.Create())
    {
        while(TRUE)
        {
            DWORD dwReaded = 0;
            fs.Read(Buffer, 1024, &dwReaded);
            if(dwReaded==0)
                break;
            MD5Checksum.Update( Buffer, dwReaded );
        }
    }
    */
    CAtlFile file;

    if (SUCCEEDED(file.Create(strFilePath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
        while (TRUE) {
            DWORD dwReaded = 0;
            file.Read(Buffer, 1024, dwReaded);

            if (dwReaded == 0)
                break;

            MD5Checksum.Update(Buffer, dwReaded);
        }

        file.Close();
    }

    return MD5Checksum.Final();
}

/*****************************************************************************************
FUNCTION:       CMD5Checksum::GetMD5
DETAILS:        static, public
DESCRIPTION:    Gets the MD5 checksum for a specified file
RETURNS:        CString : the hexadecimal MD5 checksum for the specified file
ARGUMENTS:      CFile& File : the specified file
NOTES:          Provides an interface to the CMD5Checksum class. 'File' should be open in
                binary readonly mode before calling this function.
                NB. Callers of this function should be ready to catch any CFileException
                thrown by the CFile functions
*****************************************************************************************/
/*
CString CMD5Checksum::GetMD5(CFile& File)
{
    try
    {
        CMD5Checksum MD5Checksum;       //checksum object
        int nLength = 0;                //number of bytes read from the file
        const int nBufferSize = 1024;   //checksum the file in blocks of 1024 bytes
        BYTE Buffer[nBufferSize];       //buffer for data read from the file

        //checksum the file in blocks of 1024 bytes
        while ((nLength = File.Read( Buffer, nBufferSize )) > 0 )
        {
            MD5Checksum.Update( Buffer, nLength );
        }

        //finalise the checksum and return it
        return MD5Checksum.Final();
    }

    //report any file exceptions in debug mode only
    catch (CFileException* e )
    {
        TRACE0("CMD5Checksum::GetMD5: CFileException caught");
        throw e;
    }
}*/

/*****************************************************************************************
FUNCTION:       CMD5Checksum::GetMD5
DETAILS:        static, public
DESCRIPTION:    Gets the MD5 checksum for data in a BYTE array
RETURNS:        CString : the hexadecimal MD5 checksum for the specified data
ARGUMENTS:      BYTE* pBuf  :   pointer to the BYTE array
                UINT nLength :  number of BYTEs of data to be checksumed
NOTES:          Provides an interface to the CMD5Checksum class. Any data that can
                be cast to a BYTE array of known length can be checksummed by this
                function. Typically, CString and char arrays will be checksumed,
                although this function can be used to check the integrity of any BYTE array.
                A buffer of zero length can be checksummed; all buffers of zero length
                will return the same checksum.
*****************************************************************************************/
CString CMD5Checksum::GetMD5(BYTE* pBuf, UINT nLength)
{
    //entry invariants
    //  IsValidAddress(pBuf,nLength,FALSE);
    //calculate and return the checksum
    CMD5Checksum MD5Checksum;
    MD5Checksum.Update(pBuf, nLength);
    return MD5Checksum.Final();
}


/*****************************************************************************************
FUNCTION:       CMD5Checksum::RotateLeft
DETAILS:        private
DESCRIPTION:    Rotates the bits in a 32 bit DWORD left by a specified amount
RETURNS:        The rotated DWORD
ARGUMENTS:      DWORD x : the value to be rotated
                int n   : the number of bits to rotate by
*****************************************************************************************/
DWORD CMD5Checksum::RotateLeft(DWORD x, int n)
{
    //check that DWORD is 4 bytes long - true in Visual C++ 6 and 32 bit Windows
    // sizeof(x);
    //rotate and return x
    return (x << n) | (x >> (32 - n));
}


/*****************************************************************************************
FUNCTION:       CMD5Checksum::FF
DETAILS:        protected
DESCRIPTION:    Implementation of basic MD5 transformation algorithm
RETURNS:        none
ARGUMENTS:      DWORD &A, B, C, D : Current (partial) checksum
                DWORD X           : Input data
                DWORD S           : MD5_SXX Transformation constant
                DWORD T           : MD5_TXX Transformation constant
NOTES:          None
*****************************************************************************************/
void CMD5Checksum::FF(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
    DWORD F = (B & C) | (~B & D);
    A += F + X + T;
    A = RotateLeft(A, S);
    A += B;
}


/*****************************************************************************************
FUNCTION:       CMD5Checksum::GG
DETAILS:        protected
DESCRIPTION:    Implementation of basic MD5 transformation algorithm
RETURNS:        none
ARGUMENTS:      DWORD &A, B, C, D : Current (partial) checksum
                DWORD X           : Input data
                DWORD S           : MD5_SXX Transformation constant
                DWORD T           : MD5_TXX Transformation constant
NOTES:          None
*****************************************************************************************/
void CMD5Checksum::GG(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
    DWORD G = (B & D) | (C & ~D);
    A += G + X + T;
    A = RotateLeft(A, S);
    A += B;
}


/*****************************************************************************************
FUNCTION:       CMD5Checksum::HH
DETAILS:        protected
DESCRIPTION:    Implementation of basic MD5 transformation algorithm
RETURNS:        none
ARGUMENTS:      DWORD &A, B, C, D : Current (partial) checksum
                DWORD X           : Input data
                DWORD S           : MD5_SXX Transformation constant
                DWORD T           : MD5_TXX Transformation constant
NOTES:          None
*****************************************************************************************/
void CMD5Checksum::HH(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
    DWORD H = (B ^ C ^ D);
    A += H + X + T;
    A = RotateLeft(A, S);
    A += B;
}


/*****************************************************************************************
FUNCTION:       CMD5Checksum::II
DETAILS:        protected
DESCRIPTION:    Implementation of basic MD5 transformation algorithm
RETURNS:        none
ARGUMENTS:      DWORD &A, B, C, D : Current (partial) checksum
                DWORD X           : Input data
                DWORD S           : MD5_SXX Transformation constant
                DWORD T           : MD5_TXX Transformation constant
NOTES:          None
*****************************************************************************************/
void CMD5Checksum::II(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T)
{
    DWORD I = (C ^ (B | ~D));
    A += I + X + T;
    A = RotateLeft(A, S);
    A += B;
}


/*****************************************************************************************
FUNCTION:       CMD5Checksum::ByteToDWord
DETAILS:        private
DESCRIPTION:    Transfers the data in an 8 bit array to a 32 bit array
RETURNS:        void
ARGUMENTS:      DWORD* Output : the 32 bit (unsigned long) destination array
                BYTE* Input   : the 8 bit (unsigned char) source array
                UINT nLength  : the number of 8 bit data items in the source array
NOTES:          Four BYTES from the input array are transferred to each DWORD entry
                of the output array. The first BYTE is transferred to the bits (0-7)
                of the output DWORD, the second BYTE to bits 8-15 etc.
                The algorithm assumes that the input array is a multiple of 4 bytes long
                so that there is a perfect fit into the array of 32 bit words.
*****************************************************************************************/
void CMD5Checksum::ByteToDWord(DWORD* Output, BYTE* Input, UINT nLength)
{
    //entry invariants
    //  ASSERT( nLength % 4 == 0 );
    //  ASSERT( AfxIsValidAddress(Output, nLength/4, TRUE) );
    //  ASSERT( AfxIsValidAddress(Input, nLength, FALSE) );
    //initialisations
    UINT i = 0; //index to Output array
    UINT j = 0; //index to Input array

    //transfer the data by shifting and copying
    for (; j < nLength; i++, j += 4) {
        Output[i] = (ULONG)Input[j]         |
                    (ULONG)Input[j + 1] << 8  |
                    (ULONG)Input[j + 2] << 16 |
                    (ULONG)Input[j + 3] << 24;
    }
}

/*****************************************************************************************
FUNCTION:       CMD5Checksum::Transform
DETAILS:        protected
DESCRIPTION:    MD5 basic transformation algorithm;  transforms 'm_lMD5'
RETURNS:        void
ARGUMENTS:      BYTE Block[64]
NOTES:          An MD5 checksum is calculated by four rounds of 'Transformation'.
                The MD5 checksum currently held in m_lMD5 is merged by the
                transformation process with data passed in 'Block'.
*****************************************************************************************/
void CMD5Checksum::Transform(BYTE Block[64])
{
    //initialise local data with current checksum
    ULONG a = m_lMD5[0];
    ULONG b = m_lMD5[1];
    ULONG c = m_lMD5[2];
    ULONG d = m_lMD5[3];
    //copy BYTES from input 'Block' to an array of ULONGS 'X'
    ULONG X[16];
    ByteToDWord(X, Block, 64);
    //Perform Round 1 of the transformation
    FF(a, b, c, d, X[ 0], MD5_S11, MD5_T01);
    FF(d, a, b, c, X[ 1], MD5_S12, MD5_T02);
    FF(c, d, a, b, X[ 2], MD5_S13, MD5_T03);
    FF(b, c, d, a, X[ 3], MD5_S14, MD5_T04);
    FF(a, b, c, d, X[ 4], MD5_S11, MD5_T05);
    FF(d, a, b, c, X[ 5], MD5_S12, MD5_T06);
    FF(c, d, a, b, X[ 6], MD5_S13, MD5_T07);
    FF(b, c, d, a, X[ 7], MD5_S14, MD5_T08);
    FF(a, b, c, d, X[ 8], MD5_S11, MD5_T09);
    FF(d, a, b, c, X[ 9], MD5_S12, MD5_T10);
    FF(c, d, a, b, X[10], MD5_S13, MD5_T11);
    FF(b, c, d, a, X[11], MD5_S14, MD5_T12);
    FF(a, b, c, d, X[12], MD5_S11, MD5_T13);
    FF(d, a, b, c, X[13], MD5_S12, MD5_T14);
    FF(c, d, a, b, X[14], MD5_S13, MD5_T15);
    FF(b, c, d, a, X[15], MD5_S14, MD5_T16);
    //Perform Round 2 of the transformation
    GG(a, b, c, d, X[ 1], MD5_S21, MD5_T17);
    GG(d, a, b, c, X[ 6], MD5_S22, MD5_T18);
    GG(c, d, a, b, X[11], MD5_S23, MD5_T19);
    GG(b, c, d, a, X[ 0], MD5_S24, MD5_T20);
    GG(a, b, c, d, X[ 5], MD5_S21, MD5_T21);
    GG(d, a, b, c, X[10], MD5_S22, MD5_T22);
    GG(c, d, a, b, X[15], MD5_S23, MD5_T23);
    GG(b, c, d, a, X[ 4], MD5_S24, MD5_T24);
    GG(a, b, c, d, X[ 9], MD5_S21, MD5_T25);
    GG(d, a, b, c, X[14], MD5_S22, MD5_T26);
    GG(c, d, a, b, X[ 3], MD5_S23, MD5_T27);
    GG(b, c, d, a, X[ 8], MD5_S24, MD5_T28);
    GG(a, b, c, d, X[13], MD5_S21, MD5_T29);
    GG(d, a, b, c, X[ 2], MD5_S22, MD5_T30);
    GG(c, d, a, b, X[ 7], MD5_S23, MD5_T31);
    GG(b, c, d, a, X[12], MD5_S24, MD5_T32);
    //Perform Round 3 of the transformation
    HH(a, b, c, d, X[ 5], MD5_S31, MD5_T33);
    HH(d, a, b, c, X[ 8], MD5_S32, MD5_T34);
    HH(c, d, a, b, X[11], MD5_S33, MD5_T35);
    HH(b, c, d, a, X[14], MD5_S34, MD5_T36);
    HH(a, b, c, d, X[ 1], MD5_S31, MD5_T37);
    HH(d, a, b, c, X[ 4], MD5_S32, MD5_T38);
    HH(c, d, a, b, X[ 7], MD5_S33, MD5_T39);
    HH(b, c, d, a, X[10], MD5_S34, MD5_T40);
    HH(a, b, c, d, X[13], MD5_S31, MD5_T41);
    HH(d, a, b, c, X[ 0], MD5_S32, MD5_T42);
    HH(c, d, a, b, X[ 3], MD5_S33, MD5_T43);
    HH(b, c, d, a, X[ 6], MD5_S34, MD5_T44);
    HH(a, b, c, d, X[ 9], MD5_S31, MD5_T45);
    HH(d, a, b, c, X[12], MD5_S32, MD5_T46);
    HH(c, d, a, b, X[15], MD5_S33, MD5_T47);
    HH(b, c, d, a, X[ 2], MD5_S34, MD5_T48);
    //Perform Round 4 of the transformation
    II(a, b, c, d, X[ 0], MD5_S41, MD5_T49);
    II(d, a, b, c, X[ 7], MD5_S42, MD5_T50);
    II(c, d, a, b, X[14], MD5_S43, MD5_T51);
    II(b, c, d, a, X[ 5], MD5_S44, MD5_T52);
    II(a, b, c, d, X[12], MD5_S41, MD5_T53);
    II(d, a, b, c, X[ 3], MD5_S42, MD5_T54);
    II(c, d, a, b, X[10], MD5_S43, MD5_T55);
    II(b, c, d, a, X[ 1], MD5_S44, MD5_T56);
    II(a, b, c, d, X[ 8], MD5_S41, MD5_T57);
    II(d, a, b, c, X[15], MD5_S42, MD5_T58);
    II(c, d, a, b, X[ 6], MD5_S43, MD5_T59);
    II(b, c, d, a, X[13], MD5_S44, MD5_T60);
    II(a, b, c, d, X[ 4], MD5_S41, MD5_T61);
    II(d, a, b, c, X[11], MD5_S42, MD5_T62);
    II(c, d, a, b, X[ 2], MD5_S43, MD5_T63);
    II(b, c, d, a, X[ 9], MD5_S44, MD5_T64);
    //add the transformed values to the current checksum
    m_lMD5[0] += a;
    m_lMD5[1] += b;
    m_lMD5[2] += c;
    m_lMD5[3] += d;
}


/*****************************************************************************************
CONSTRUCTOR:    CMD5Checksum
DESCRIPTION:    Initialises member data
ARGUMENTS:      None
NOTES:          None
*****************************************************************************************/
CMD5Checksum::CMD5Checksum()
{
    // zero members
    memset(m_lpszBuffer, 0, 64);
    m_nCount[0] = m_nCount[1] = 0;
    // Load magic state initialization constants
    m_lMD5[0] = MD5_INIT_STATE_0;
    m_lMD5[1] = MD5_INIT_STATE_1;
    m_lMD5[2] = MD5_INIT_STATE_2;
    m_lMD5[3] = MD5_INIT_STATE_3;
}

/*****************************************************************************************
FUNCTION:       CMD5Checksum::DWordToByte
DETAILS:        private
DESCRIPTION:    Transfers the data in an 32 bit array to a 8 bit array
RETURNS:        void
ARGUMENTS:      BYTE* Output  : the 8 bit destination array
                DWORD* Input  : the 32 bit source array
                UINT nLength  : the number of 8 bit data items in the source array
NOTES:          One DWORD from the input array is transferred into four BYTES
                in the output array. The first (0-7) bits of the first DWORD are
                transferred to the first output BYTE, bits bits 8-15 are transferred from
                the second BYTE etc.

                The algorithm assumes that the output array is a multiple of 4 bytes long
                so that there is a perfect fit of 8 bit BYTES into the 32 bit DWORDs.
*****************************************************************************************/
void CMD5Checksum::DWordToByte(BYTE* Output, DWORD* Input, UINT nLength)
{
    //entry invariants
    //  ASSERT( nLength % 4 == 0 );
    //  ASSERT( AfxIsValidAddress(Output, nLength, TRUE) );
    //  ASSERT( AfxIsValidAddress(Input, nLength/4, FALSE) );
    //transfer the data by shifting and copying
    UINT i = 0;
    UINT j = 0;

    for (; j < nLength; i++, j += 4) {
        Output[j] = (UCHAR)(Input[i] & 0xff);
        Output[j + 1] = (UCHAR)((Input[i] >> 8) & 0xff);
        Output[j + 2] = (UCHAR)((Input[i] >> 16) & 0xff);
        Output[j + 3] = (UCHAR)((Input[i] >> 24) & 0xff);
    }
}


/*****************************************************************************************
FUNCTION:       CMD5Checksum::Final
DETAILS:        protected
DESCRIPTION:    Implementation of main MD5 checksum algorithm; ends the checksum calculation.
RETURNS:        CString : the final hexadecimal MD5 checksum result
ARGUMENTS:      None
NOTES:          Performs the final MD5 checksum calculation ('Update' does most of the work,
                this function just finishes the calculation.)
*****************************************************************************************/
CString CMD5Checksum::Final()
{
    //Save number of bits
    BYTE Bits[8];
    DWordToByte(Bits, m_nCount, 8);
    //Pad out to 56 mod 64.
    UINT nIndex = (UINT)((m_nCount[0] >> 3) & 0x3f);
    UINT nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
    Update(PADDING, nPadLen);
    //Append length (before padding)
    Update(Bits, 8);
    //Store final state in 'lpszMD5'
    const int nMD5Size = 16;
    unsigned char lpszMD5[ nMD5Size ];
    DWordToByte(lpszMD5, m_lMD5, nMD5Size);
    //Convert the hexadecimal checksum to a CString
    CString strMD5;

    for (int i = 0; i < nMD5Size; i++) {
        CString Str;
        Str.Format(_T("%02x"), lpszMD5[i]);
        strMD5 += Str;
    }

    //  ASSERT( strMD5.GetLength() == 32 );
    return strMD5;
}


/*****************************************************************************************
FUNCTION:       CMD5Checksum::Update
DETAILS:        protected
DESCRIPTION:    Implementation of main MD5 checksum algorithm
RETURNS:        void
ARGUMENTS:      BYTE* Input    : input block
                UINT nInputLen : length of input block
NOTES:          Computes the partial MD5 checksum for 'nInputLen' bytes of data in 'Input'
*****************************************************************************************/
void CMD5Checksum::Update(BYTE* Input,  ULONG nInputLen)
{
    //Compute number of bytes mod 64
    UINT nIndex = (UINT)((m_nCount[0] >> 3) & 0x3F);

    //Update number of bits
    if ((m_nCount[0] += nInputLen << 3)  < (nInputLen << 3)) {
        m_nCount[1]++;
    }

    m_nCount[1] += (nInputLen >> 29);
    //Transform as many times as possible.
    UINT i = 0;
    UINT nPartLen = 64 - nIndex;

    if (nInputLen >= nPartLen) {
        memcpy(&m_lpszBuffer[nIndex], Input, nPartLen);
        Transform(m_lpszBuffer);

        for (i = nPartLen; i + 63 < nInputLen; i += 64) {
            Transform(&Input[i]);
        }

        nIndex = 0;
    } else {
        i = 0;
    }

    // Buffer remaining input
    memcpy(&m_lpszBuffer[nIndex], &Input[i], nInputLen - i);
}
}


namespace cactus
{
using namespace std;

#pragma warning(disable : 4996)

static const size_t NMAX_BUF = 0xff;

#ifndef _UNICODE
    #define FOPEN fopen
    #define SPRINTF sprintf
#else       // UNICODE
    #define FOPEN _wfopen
    #define SPRINTF swprintf
#endif

#ifndef _INC_STRING
    #include <string.h>
#endif

#define MD5_memcpy(dest, src, len)      memcpy(dest, src, len)
#define MD5_memset(dest, val, len)      memset(dest, val, len)

// -------------------------------------------------------------------------

/* Constants for MD5Transform routine.
*/
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
        (a) += F ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
        (a) = ROTATE_LEFT ((a), (s)); \
        (a) += (b); \
    }
#define GG(a, b, c, d, x, s, ac) { \
        (a) += G ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
        (a) = ROTATE_LEFT ((a), (s)); \
        (a) += (b); \
    }
#define HH(a, b, c, d, x, s, ac) { \
        (a) += H ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
        (a) = ROTATE_LEFT ((a), (s)); \
        (a) += (b); \
    }
#define II(a, b, c, d, x, s, ac) { \
        (a) += I ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
        (a) = ROTATE_LEFT ((a), (s)); \
        (a) += (b); \
    }


/* MD5 initialization. Begins an MD5 operation, writing a new context.
*/

MD5_CTX::MD5_CTX()
{
    MD5Init();
}

MD5_CTX::~MD5_CTX()
{
}

void MD5_CTX::MD5Init()
{
    this->count[0] = this->count[1] = 0;
    /* Load magic initialization constants.*/
    this->state[0] = 0x67452301;
    this->state[1] = 0xefcdab89;
    this->state[2] = 0x98badcfe;
    this->state[3] = 0x10325476;
    /* Add by Liguangyi */
    MD5_memset(PADDING, 0, sizeof(PADDING));
    *PADDING = 0x80;
    //PADDING = {
    //  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    //  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    //  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void MD5_CTX::MD5Update(const unsigned char *input, size_t inputLen)
{
    size_t i, index, partLen;
    /* Compute number of bytes mod 64 */
    index = (size_t)((this->count[0] >> 3) & 0x3F);

    /* Update number of bits */
    if ((this->count[0] += ((unsigned long int)inputLen << 3))
        < ((unsigned long int)inputLen << 3))
        this->count[1]++;

    this->count[1] += ((unsigned long int)inputLen >> 29);
    partLen = 64 - index;

    /* Transform as many times as possible.
    */
    if (inputLen >= partLen) {
        MD5_memcpy((unsigned char*)&this->buffer[index],
                   (unsigned char*)input, partLen);
        MD5Transform(this->state, this->buffer);

        for (i = partLen; i + 63 < inputLen; i += 64)
            MD5Transform(this->state, &input[i]);

        index = 0;
    } else
        i = 0;

    /* Buffer remaining input */
    MD5_memcpy((unsigned char*)&this->buffer[index], (unsigned char*)&input[i], inputLen - i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
the message digest and zeroizing the context.
*/
void MD5_CTX::MD5Final(unsigned char digest[16])
{
    unsigned char bits[8];
    size_t index, padLen;
    /* Save number of bits */
    Encode(bits, this->count, 8);
    /* Pad out to 56 mod 64.
    */
    index = (size_t)((this->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5Update(PADDING, padLen);
    /* Append length (before padding) */
    MD5Update(bits, 8);
    /* Store state in digest */
    Encode(digest, this->state, 16);
    /* Zeroize sensitive information.
    */
    MD5_memset((unsigned char*)this, 0, sizeof(*this));
    this->MD5Init();
}

/* MD5 basic transformation. Transforms state based on block.
*/
void MD5_CTX::MD5Transform(unsigned long int state[4], const unsigned char block[64])
{
    unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    Decode(x, block, 64);
    /* Round 1 */
    FF(a, b, c, d, x[ 0], S11, 0xd76aa478);  /* 1 */
    FF(d, a, b, c, x[ 1], S12, 0xe8c7b756);  /* 2 */
    FF(c, d, a, b, x[ 2], S13, 0x242070db);  /* 3 */
    FF(b, c, d, a, x[ 3], S14, 0xc1bdceee);  /* 4 */
    FF(a, b, c, d, x[ 4], S11, 0xf57c0faf);  /* 5 */
    FF(d, a, b, c, x[ 5], S12, 0x4787c62a);  /* 6 */
    FF(c, d, a, b, x[ 6], S13, 0xa8304613);  /* 7 */
    FF(b, c, d, a, x[ 7], S14, 0xfd469501);  /* 8 */
    FF(a, b, c, d, x[ 8], S11, 0x698098d8);  /* 9 */
    FF(d, a, b, c, x[ 9], S12, 0x8b44f7af);  /* 10 */
    FF(c, d, a, b, x[10], S13, 0xffff5bb1);  /* 11 */
    FF(b, c, d, a, x[11], S14, 0x895cd7be);  /* 12 */
    FF(a, b, c, d, x[12], S11, 0x6b901122);  /* 13 */
    FF(d, a, b, c, x[13], S12, 0xfd987193);  /* 14 */
    FF(c, d, a, b, x[14], S13, 0xa679438e);  /* 15 */
    FF(b, c, d, a, x[15], S14, 0x49b40821);  /* 16 */
    /* Round 2 */
    GG(a, b, c, d, x[ 1], S21, 0xf61e2562);  /* 17 */
    GG(d, a, b, c, x[ 6], S22, 0xc040b340);  /* 18 */
    GG(c, d, a, b, x[11], S23, 0x265e5a51);  /* 19 */
    GG(b, c, d, a, x[ 0], S24, 0xe9b6c7aa);  /* 20 */
    GG(a, b, c, d, x[ 5], S21, 0xd62f105d);  /* 21 */
    GG(d, a, b, c, x[10], S22,  0x2441453);  /* 22 */
    GG(c, d, a, b, x[15], S23, 0xd8a1e681);  /* 23 */
    GG(b, c, d, a, x[ 4], S24, 0xe7d3fbc8);  /* 24 */
    GG(a, b, c, d, x[ 9], S21, 0x21e1cde6);  /* 25 */
    GG(d, a, b, c, x[14], S22, 0xc33707d6);  /* 26 */
    GG(c, d, a, b, x[ 3], S23, 0xf4d50d87);  /* 27 */
    GG(b, c, d, a, x[ 8], S24, 0x455a14ed);  /* 28 */
    GG(a, b, c, d, x[13], S21, 0xa9e3e905);  /* 29 */
    GG(d, a, b, c, x[ 2], S22, 0xfcefa3f8);  /* 30 */
    GG(c, d, a, b, x[ 7], S23, 0x676f02d9);  /* 31 */
    GG(b, c, d, a, x[12], S24, 0x8d2a4c8a);  /* 32 */
    /* Round 3 */
    HH(a, b, c, d, x[ 5], S31, 0xfffa3942);  /* 33 */
    HH(d, a, b, c, x[ 8], S32, 0x8771f681);  /* 34 */
    HH(c, d, a, b, x[11], S33, 0x6d9d6122);  /* 35 */
    HH(b, c, d, a, x[14], S34, 0xfde5380c);  /* 36 */
    HH(a, b, c, d, x[ 1], S31, 0xa4beea44);  /* 37 */
    HH(d, a, b, c, x[ 4], S32, 0x4bdecfa9);  /* 38 */
    HH(c, d, a, b, x[ 7], S33, 0xf6bb4b60);  /* 39 */
    HH(b, c, d, a, x[10], S34, 0xbebfbc70);  /* 40 */
    HH(a, b, c, d, x[13], S31, 0x289b7ec6);  /* 41 */
    HH(d, a, b, c, x[ 0], S32, 0xeaa127fa);  /* 42 */
    HH(c, d, a, b, x[ 3], S33, 0xd4ef3085);  /* 43 */
    HH(b, c, d, a, x[ 6], S34,  0x4881d05);  /* 44 */
    HH(a, b, c, d, x[ 9], S31, 0xd9d4d039);  /* 45 */
    HH(d, a, b, c, x[12], S32, 0xe6db99e5);  /* 46 */
    HH(c, d, a, b, x[15], S33, 0x1fa27cf8);  /* 47 */
    HH(b, c, d, a, x[ 2], S34, 0xc4ac5665);  /* 48 */
    /* Round 4 */
    II(a, b, c, d, x[ 0], S41, 0xf4292244);  /* 49 */
    II(d, a, b, c, x[ 7], S42, 0x432aff97);  /* 50 */
    II(c, d, a, b, x[14], S43, 0xab9423a7);  /* 51 */
    II(b, c, d, a, x[ 5], S44, 0xfc93a039);  /* 52 */
    II(a, b, c, d, x[12], S41, 0x655b59c3);  /* 53 */
    II(d, a, b, c, x[ 3], S42, 0x8f0ccc92);  /* 54 */
    II(c, d, a, b, x[10], S43, 0xffeff47d);  /* 55 */
    II(b, c, d, a, x[ 1], S44, 0x85845dd1);  /* 56 */
    II(a, b, c, d, x[ 8], S41, 0x6fa87e4f);  /* 57 */
    II(d, a, b, c, x[15], S42, 0xfe2ce6e0);  /* 58 */
    II(c, d, a, b, x[ 6], S43, 0xa3014314);  /* 59 */
    II(b, c, d, a, x[13], S44, 0x4e0811a1);  /* 60 */
    II(a, b, c, d, x[ 4], S41, 0xf7537e82);  /* 61 */
    II(d, a, b, c, x[11], S42, 0xbd3af235);  /* 62 */
    II(c, d, a, b, x[ 2], S43, 0x2ad7d2bb);  /* 63 */
    II(b, c, d, a, x[ 9], S44, 0xeb86d391);  /* 64 */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    /* Zeroize sensitive information.
    */
    MD5_memset((unsigned char*)x, 0, sizeof(x));
}

/* Encodes input (unsigned long int) into output (unsigned char). Assumes len is
a multiple of 4.
*/
void MD5_CTX::Encode(unsigned char *output, const unsigned long int *input, size_t len)
{
    size_t i, j;

    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = (unsigned char)(input[i] & 0xff);
        output[j + 1] = (unsigned char)((input[i] >> 8) & 0xff);
        output[j + 2] = (unsigned char)((input[i] >> 16) & 0xff);
        output[j + 3] = (unsigned char)((input[i] >> 24) & 0xff);
    }
}

/* Decodes input (unsigned char) into output (unsigned long int). Assumes len is
a multiple of 4.
*/
void MD5_CTX::Decode(unsigned long int *output, const unsigned char *input, size_t len)
{
    size_t i, j;

    for (i = 0, j = 0; j < len; i++, j += 4)
        output[i] = ((unsigned long int)input[j]) | (((unsigned long int)input[j + 1]) << 8) |
                    (((unsigned long int)input[j + 2]) << 16) | (((unsigned long int)input[j + 3]) << 24);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KMD5::KMD5()
    : m_pbyBuf(NULL)
    , m_pAlgoMD5(NULL)
{
}

KMD5::~KMD5()
{
    delete m_pAlgoMD5;
    m_pAlgoMD5 = NULL;
    delete [] m_pbyBuf;
    m_pbyBuf = NULL;
}

// -------------------------------------------------------------------------
// 函数     : const BYTE* KMD5::GetMD5
// 功能     : 获取一个文件的128位MD5串
// 返回值   : const unsigned MD5串的指针 128位
// 参数     : const TCHAR *pszFileName  文件名
// 附注     :
// -------------------------------------------------------------------------
const BYTE* KMD5::GetMD5(const TCHAR* pszFileName)
{
    if (!m_pAlgoMD5) {
        m_pAlgoMD5 = new MD5_CTX();
    }

    if (!m_pAlgoMD5) {
        assert(0);
        return NULL;
    }

    FILE *pFile;
    pFile = ::FOPEN(pszFileName, _TEXT("rb"));

    if (!pFile) {
        return NULL;
    }

    if (!m_pbyBuf) {
        m_pbyBuf = new BYTE[NMAX_BUF];
    }

    size_t nRead = 0;

    while (nRead = ::fread(m_pbyBuf,  sizeof(BYTE), NMAX_BUF, pFile)) {
        m_pAlgoMD5->MD5Update(m_pbyBuf, nRead);
    }

    ::fclose(pFile);
    m_pAlgoMD5->MD5Final(m_pbyMD5);
    return m_pbyMD5;
}

// -------------------------------------------------------------------------
// 函数     :  KMD5::GetMD5Str
// 功能     : 获取一个文件的128位MD5串
// 返回值   : const TCHAR* MD5字符串的指针
//                  如：_TEXT("C5CE883D2DBDED17F46D24BC906D2A8F")
// 参数     : const TCHAR *pszFileName 文件名
// 附注     :
// -------------------------------------------------------------------------
const TCHAR*  KMD5::GetMD5Str(const TCHAR *pszFileName)
{
    if (!GetMD5(pszFileName)) {
        return NULL;
    }

    TCHAR* pChar = m_pszMD5;

    for (int i = 0; i < 16; ++i) {
        ::SPRINTF(pChar, _TEXT("%02x"), m_pbyMD5[i]);
        pChar += 2;
    }

    return m_pszMD5;
}
//////////////////////////////////////////////////////////////////////////
const BYTE* KMD5::GetMD5ByStr(const TCHAR* pszStr)
{
    if (!pszStr) return NULL;

    while (!m_pAlgoMD5) {
        m_pAlgoMD5 = new MD5_CTX();

        if (!m_pAlgoMD5) {
            assert(0);
            return NULL;
        }
    }

    m_pAlgoMD5->MD5Update((BYTE*)pszStr, ::wcslen(pszStr) * sizeof(TCHAR));
    m_pAlgoMD5->MD5Final(m_pbyMD5);
    return m_pbyMD5;
}

//////////////////////////////////////////////////////////////////////////
const TCHAR* KMD5::GetMD5StrByStr(const TCHAR* pszStr)
{
    if (!GetMD5ByStr(pszStr)) {
        return NULL;
    }

    TCHAR* pChar = m_pszMD5;

    for (int i = 0; i < 16; ++i) {
        ::SPRINTF(pChar, _TEXT("%02x"), m_pbyMD5[i]);
        pChar += 2;
    }

    return m_pszMD5;
}

//////////////////////////////////////////////////////////////////////////
const TCHAR* KMD5::GetFileMd5ForMonStr(const TCHAR* pszFileName)
{
    if (!GetFileMd5ForMon(pszFileName)) {
        return NULL;
    }

    TCHAR* pChar = m_pszMD5;

    for (int i = 0; i < 16; ++i) {
        ::SPRINTF(pChar, _TEXT("%02x"), m_pbyMD5[i]);
        pChar += 2;
    }

    return m_pszMD5;
}

//////////////////////////////////////////////////////////////////////////

const BYTE* KMD5::GetFileMd5ForMon(const TCHAR* lpSrcName)
{
    if (!m_pAlgoMD5) {
        m_pAlgoMD5 = new MD5_CTX();
    }

    ULONG ClusterSize, BlockSize;
    ULONG ClCount, FileSize, Bytes;
    ULONG SecPerCl, BtPerSec, r;
    LARGE_INTEGER Offset;
    HANDLE hDrive = NULL;
    ULONGLONG *Clusters = NULL;
    PVOID Buff = NULL;
    WCHAR Name[7] = {0};
    Name[0] = lpSrcName[0];
    Name[1] = ':';
    Name[2] = 0;
    ::GetDiskFreeSpace(Name, &SecPerCl, &BtPerSec, NULL, NULL);
    ClusterSize = SecPerCl * BtPerSec;
    Clusters = GetFileClusters(lpSrcName, ClusterSize, &ClCount, &FileSize);

    if (Clusters) {
        Name[0] = '\\';
        Name[1] = '\\';
        Name[2] = '.';
        Name[3] = '\\';
        Name[4] = lpSrcName[0];
        Name[5] = ':';
        Name[6] = 0;
        hDrive = ::CreateFile(
                     Name, GENERIC_READ,
                     FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

        if (hDrive != INVALID_HANDLE_VALUE) {
            Buff = malloc(ClusterSize);

            if (Buff) {
                for (r = 0; r < ClCount; r++, FileSize -= BlockSize) {
                    Offset.QuadPart = ClusterSize * Clusters[r];
                    ::SetFilePointer(hDrive, Offset.LowPart, &Offset.HighPart, FILE_BEGIN);
                    ::ReadFile(hDrive, Buff, ClusterSize, &Bytes, NULL);
                    BlockSize = FileSize < ClusterSize ? FileSize : ClusterSize;
                    m_pAlgoMD5->MD5Update((unsigned char*)Buff, BlockSize);
                }

                free(Buff);
            }

            ::CloseHandle(hDrive);
        }

        free(Clusters);
        m_pAlgoMD5->MD5Final(m_pbyMD5);
        return m_pbyMD5;
    }

    return NULL;
}


//////////////////////////////////////////////////////////////////////////
ULONGLONG* KMD5::GetFileClusters(
    LPCTSTR lpFileName,
    ULONG ClusterSize,
    ULONG *ClCount,
    ULONG *FileSize)
{
    HANDLE hFile;
    ULONG OutSize;
    ULONG Bytes, Cls, CnCount, r;
    ULONGLONG* Clusters = NULL;
    BOOLEAN Result = FALSE;
    LARGE_INTEGER PrevVCN, Lcn;
    STARTING_VCN_INPUT_BUFFER InBuf;
    PRETRIEVAL_POINTERS_BUFFER OutBuf;
    hFile = ::CreateFile(
                lpFileName,
                FILE_READ_ATTRIBUTES,
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                NULL,
                OPEN_EXISTING,
                0, 0);

    if (hFile != INVALID_HANDLE_VALUE) {
        *FileSize = ::GetFileSize(hFile, NULL);
        OutSize = sizeof(RETRIEVAL_POINTERS_BUFFER) +
                  (*FileSize / ClusterSize) * sizeof(OutBuf->Extents);
        OutBuf = (PRETRIEVAL_POINTERS_BUFFER) malloc(OutSize);
        InBuf.StartingVcn.QuadPart = 0;

        if (DeviceIoControl(
                hFile,
                FSCTL_GET_RETRIEVAL_POINTERS,
                &InBuf,
                sizeof(InBuf),
                OutBuf,
                OutSize,
                &Bytes,
                NULL)) {
            *ClCount = ((*FileSize + ClusterSize - 1) / ClusterSize) + 1;
            Clusters = (ULONGLONG *) malloc(*ClCount * sizeof(ULONGLONG));
            PrevVCN = OutBuf->StartingVcn;

            for (r = 0, Cls = 0; r < OutBuf->ExtentCount; r++) {
                Lcn = OutBuf->Extents[r].Lcn;

                for (CnCount = (ULONG)(OutBuf->Extents[r].NextVcn.QuadPart - PrevVCN.QuadPart);
                     CnCount; CnCount--, Cls++, Lcn.QuadPart++) {
                    if (Cls >= *ClCount) {
                        break;
                    }

                    Clusters[Cls] = Lcn.QuadPart;
                }

                PrevVCN = OutBuf->Extents[r].NextVcn;
            }
        }

        free(OutBuf);
        ::CloseHandle(hFile);
    }

    return Clusters;
}
} // namespace cactus