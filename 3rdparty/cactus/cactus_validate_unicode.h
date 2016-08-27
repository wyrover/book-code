/*!
* \file cactus_validate_unicode.h
* \brief 检测没有bom头的字符串的编码
*
* 检测没有bom头的字符串的编码
*
* \author wangyang
* \date 2015/03/23
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_VALIDATE_UNICODE_H__
#define __CACTUS_CACTUS_VALIDATE_UNICODE_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>

#ifdef _WIN32
    #include <io.h> // For _setmode
    #include <fcntl.h> // For _O_BINARY
#endif



namespace cactus
{

const WORD UNICODE_REPLACEMENT_CHARACTER = 0xfffd;

#define TSTR_INPUTCHARSET _T("ACP|OEM|CP#|UTF8|UTF16<LE|BE>")
#define TSTR_OUTPUTCHARSET _T("ACP|OEM|CP#|UTF8[SIG]|UTF16<LE|BE>[BOM]")

template<typename T> T S7ChLwr(T c)
{
    return c >= 'A' && c <= 'Z' ? (T)(c | 32) : c;
}
template<typename T> T S7ChUpr(T c)
{
    return c >= 'a' && c <= 'z' ? (T)(c - 'a' + 'A') : c;
}
template<typename T> bool S7IsChEqualI(char ch, T cmp)
{
    return S7ChLwr((T)ch) == S7ChLwr(cmp);
}

inline bool IsValidUnicodeCodePoint(UINT32 c, bool StrictUTF32 = false)
{
    // Unicode 6.1: 16.7 Noncharacters
    if ((c & 0xfffe) == 0xfffe) return false; // ..FFFE & ..FFFF is reserved in each plane

    if (c >= 0xfdd0 && c <= 0xfdef) return false; // Reserved in BMP

    if (StrictUTF32 && c > 0x10ffff) return false;

    return true;
}
inline bool IsLeadSurrogateUTF16(unsigned short c)
{
    return c >= 0xd800 && c <= 0xdbff;
}
inline bool IsTrailSurrogateUTF16(unsigned short c)
{
    return c >= 0xdc00 && c <= 0xdfff;
}
inline UINT32 CodePointFromUTF16SurrogatePair(unsigned short lea, unsigned short tra)
{
    const UINT32 surrogate_offset = 0x10000 - (0xD800 << 10) - 0xDC00;
    return ((UINT32)lea << 10) + tra + surrogate_offset;
}

inline bool UTF8_GetTrailCount(unsigned char chFirst, unsigned char &cb)
{
    // This function should only be used to get a rough idea of how large the encoded
    // codepoint is, just because it returns true does not mean that it is valid UTF-8!
    cb = 0;

    if (0xC0 == (0xC0 & chFirst)) {
        ++cb;

        if (0xE0 == (0xE0 & chFirst)) {
            ++cb;

            if (0xF0 == (0xF0 & chFirst)) {
                ++cb;

                if (0xF8 == (0xF8 & chFirst)) {
                    ++cb;

                    if (0xFC == (0xFE & chFirst)) ++cb;
                    else return 0xFC >= (0xFE & chFirst);
                }
            }
        }

        return true;
    }

    return (signed char)chFirst >= 0;
}

inline UINT InlineStrLenUTF16(const void*str)
{
    unsigned short *p = (unsigned short *) str;
    UINT cch = 0;

    for (; p[cch];) ++cch;

    return cch;
}
} // namespace cactus


namespace cactus
{
#define NSISRT_free(p) ( free((void*)(p)) )

char* NSISRT_wctombpath(const wchar_t *Path);
FILE* my_fopen(const TCHAR *path, const char *mode); // from util.h

void UTF16InplaceEndianSwap(void*Buffer, UINT cch);
UINT StrLenUTF16(const void*str);
bool StrSetUTF16LE(tstring&dest, const void*src);

UINT WCFromCodePoint(wchar_t*Dest, UINT cchDest, UINT32 CodPt);
#define DWCFBF_ALLOWOPTIMIZEDRETURN 0x80000000 // DupWCFromBytes can return input Buffer
wchar_t* DupWCFromBytes(void*Buffer, UINT cbBuffer, UINT32 SrcCP);
UINT DetectUTFBOM(void*Buffer, UINT cb);
UINT DetectUTFBOM(FILE*strm);
WORD GetEncodingFromString(const TCHAR*s, bool&BOM);
WORD GetEncodingFromString(const TCHAR*s);
} // namespace cactus


namespace cactus
{
int RunChildProcessRedirected(LPCWSTR cmdprefix, LPCWSTR cmdmain);

inline int RunChildProcessRedirected(LPCWSTR cmd)
{
    return RunChildProcessRedirected(0, cmd);
}
} // namespace cactus

namespace cactus
{
class CharEncConv
{
    char *m_Result;
    WORD m_TE, m_FE;
#ifdef _WIN32
    bool m_AllowOptimizedReturn; // Can Convert() return Src buffer?
    bool m_OptimizedReturn;
#else
    iconvdescriptor m_iconvd;
#endif
protected:
    size_t GuessOutputSize(size_t cbConverted);
    static bool IsWE(WORD Encoding)
    {
        return (WORD) - 1 == Encoding;
    }
    static bool IsWE(UINT32 Encoding)
    {
        return (UINT32) - 1 == Encoding;
    }
public:
    CharEncConv() : m_Result(0)
    {
        SetAllowOptimizedReturn(false);
    }
    ~CharEncConv()
    {
        Close();
    }
    void Close()
    {
#ifdef _WIN32

        if (!m_OptimizedReturn)
#endif
            free(m_Result);

        m_Result = 0;
#ifndef _WIN32
        m_iconvd.Close();
#endif
    }
    void* Detach()
    {
        void *p = m_Result;
        m_Result = 0;
        return p;
    }
    bool Initialize(UINT32 ToEnc, UINT32 FromEnc);
    void* Convert(const void*Src, size_t cbSrc = -1, size_t*cbOut = 0);
#ifdef _WIN32
    void SetAllowOptimizedReturn(bool val = true)
    {
        m_AllowOptimizedReturn = val;
    }
#else
    void SetAllowOptimizedReturn(bool val = false) {}
#endif
    static BOOL IsValidCodePage(UINT cp);
};

class WCToUTF16LEHlpr
{
    unsigned short* m_s;
public:
    WCToUTF16LEHlpr() : m_s(0) {}

    bool Create(const TCHAR*in, unsigned int codepage = CP_ACP)
#if !defined(_WIN32) || !defined(_UNICODE)
    ;
#else
    {
        m_s = (unsigned short*) in;
        return true;
    }
#endif
    void Destroy()
    {
#if !defined(_WIN32) || !defined(_UNICODE)
        free(m_s);
#endif
    }
    const unsigned short* Get() const
    {
        return m_s;
    }
    UINT GetLen() const
    {
        return StrLenUTF16(m_s);
    }
    UINT GetSize() const
    {
        return (GetLen() + 1) * 2;
    }
    unsigned short* Detach()
    {
        unsigned short *r = m_s;
        m_s = 0;
        return r;
    }
    void CopyTo(unsigned short*Dest) const
    {
        memcpy(Dest, Get(), GetSize());
    }
};

class NStreamEncoding
{
protected:
    WORD m_cp;
public:
    enum {
        ACP = CP_ACP,
        OEMCP = 1,
        UTF16LE = 1200,
        UTF16BE = 1201,
        UTF32LE = 12000,
        UTF32BE = 12001,
        UTF8 = CP_UTF8,
        UNKNOWN = (0xffff - 0),
        AUTO = (0xffff - 1),
        CPCOUNT = (0xffff - 2) // Must be less than our other magic numbers
    };

    NStreamEncoding()
    {
        Reset();
    }
    NStreamEncoding(WORD cp)
    {
        Reset(), SetCodepage(cp);
    }
    WORD GetCodepage() const
    {
        return m_cp;
    }
    void SetCodepage(WORD cp)
    {
        m_cp = cp;
    }
    void SafeSetCodepage(WORD cp)
    {
        if (NStreamEncoding::AUTO == cp) cp = GetPlatformDefaultCodepage();

        SetCodepage(cp);
    }
    void Reset()
    {
        SetCodepage(GetPlatformDefaultCodepage());
    }
    WORD GetPlatformDefaultCodepage() const
    {
#ifdef _WIN32
        return ACP;
#else
        return UTF8;
#endif
    }
    bool IsUTF8() const
    {
        return UTF8 == GetCodepage();
    }
    bool IsUTF16() const
    {
        return (UTF16LE | 1) == (GetCodepage() | 1);
    }
    bool IsUTF16LE() const
    {
        return UTF16LE == GetCodepage();
    }
    bool IsUTF16BE() const
    {
        return UTF16BE == GetCodepage();
    }
    bool IsUnicode() const
    {
        return IsUnicodeCodepage(GetCodepage());
    }
    void GetCPDisplayName(TCHAR*Buf)
    {
        GetCPDisplayName(m_cp, Buf);
    }

    static UINT GetCodeUnitSize(WORD cp)
    {
        if ((UTF16LE | 1) == (cp | 1)) return 2;

        if ((UTF32LE | 1) == (cp | 1)) return 4;

        return 1;
    }
    static bool IsUnicodeCodepage(WORD cp)
    {
        return UTF8 == cp || (UTF16LE | 1) == (cp | 1) || (UTF32LE | 1) == (cp | 1);
    }
    static void GetCPDisplayName(WORD CP, TCHAR*Buf);
};

class NStream
{
public:
    enum {
        OK = 0,
        ERR_BUFFEROVERFLOW,
        ERR_IOERROR,
        ERR_INVALIDENCODING,
        ERR_UNSUPPORTEDENCODING,
    };
    static bool IsNewline(wchar_t chW, bool HandleUnicodeNL)
    {
        if (L'\n' == chW || L'\r' == chW) return true;

        if (HandleUnicodeNL) {
            // www.unicode.org/standard/reports/tr13/tr13-5.html#UNICODE NEWLINE GUIDELINES
            if (L'\f' == chW) return true; // FF/Form Feed

            if (L'\v' == chW) return true; // VT/Vertical Tab

            // NOTIMPLEMENTED: NEL/Next Line/U+0085
            // NOTIMPLEMENTED: LS/Line Separator/U+2028
            // NOTIMPLEMENTED: PS/Paragraph Separator/U+2029
        }

        return false;
    }
#ifdef _WIN32
    static bool SetBinaryMode(int fd)
    {
        return -1 != _setmode(fd, _O_BINARY);
    }
    static bool SetBinaryMode(FILE*f)
    {
        return SetBinaryMode(_fileno(f));
    }
#else
    static bool SetBinaryMode(int fd)
    {
        return true;
    }
    static bool SetBinaryMode(FILE*f)
    {
        return true;
    }
#endif
};

class NBaseStream
{
protected:
    FILE* m_hFile;
    NStreamEncoding m_Enc;

public:
    NBaseStream(FILE *hFile = 0) : m_hFile(hFile) {}
    ~NBaseStream()
    {
        Close();
    }

    FILE* GetHandle() const
    {
        return m_hFile;
    }
    NStreamEncoding& StreamEncoding()
    {
        return m_Enc;
    }
    bool IsEOF() const
    {
        return feof(m_hFile) != 0;
    }
    bool IsError() const
    {
        return ferror(m_hFile) != 0;
    }
    bool IsUnicode() const
    {
        return m_Enc.IsUnicode();
    }

    void Close()
    {
        FILE*hF = Detach();

        if (hF) fclose(hF);
    }

    bool OpenFileForReading(const TCHAR* Path, WORD enc = NStreamEncoding::AUTO)
    {
        return Attach(my_fopen(Path, "rb"), enc);
    }
    bool OpenFileForReading(const TCHAR* Path, NStreamEncoding&Enc)
    {
        return OpenFileForReading(Path, Enc.GetCodepage());
    }

    FILE* Detach()
    {
        FILE *hFile = m_hFile;
        m_hFile = 0;
        return hFile;
    }
    bool Attach(FILE*hFile, WORD enc, bool Seek = true);

    UINT ReadOctets(void*Buffer, UINT cbBuf)
    {
        size_t cb = fread(Buffer, 1, cbBuf, m_hFile);
        return (UINT) cb;
    }
    bool ReadOctets(void*Buffer, UINT*pcbBuf)
    {
        UINT cbReq = *pcbBuf, cb = ReadOctets(Buffer, cbReq);
        *pcbBuf = cb;
        return cbReq == cb;
    }
    bool ReadOctet(void*Buffer)
    {
        return 1 == ReadOctets(Buffer, 1);
    }
    bool ReadInt16(void*Buffer)
    {
        return 2 == ReadOctets(Buffer, 2);
    }
};

class NIStream : public NBaseStream
{
public:
    bool OpenStdIn(WORD enc = NStreamEncoding::AUTO)
    {
        return Attach(stdin, enc, false);
    }
    bool OpenStdIn(NStreamEncoding&Enc)
    {
        return OpenStdIn(Enc.GetCodepage());
    }
};

class NOStream : public NBaseStream
{
public:
    NOStream(FILE *hFile = 0) : NBaseStream(hFile) {}

    bool CreateFileForWriting(const TCHAR* Path, WORD enc = NStreamEncoding::AUTO)
    {
        return Attach(my_fopen(Path, "w+b"), enc);
    }
    bool CreateFileForWriting(const TCHAR* Path, NStreamEncoding&Enc)
    {
        return CreateFileForWriting(Path, Enc.GetCodepage());
    }
    bool CreateFileForAppending(const TCHAR* Path, WORD enc = NStreamEncoding::AUTO)
    {
        return Attach(my_fopen(Path, "a+b"), enc);
    }
    bool CreateFileForAppending(const TCHAR* Path, NStreamEncoding&Enc)
    {
        return CreateFileForAppending(Path, Enc.GetCodepage());
    }

    bool WriteOctets(void*Buffer, size_t cbBuf)
    {
        return cbBuf == fwrite(Buffer, 1, cbBuf, m_hFile);
    }
    bool WriteBOM(NStreamEncoding&Enc)
    {
        static const unsigned char u8b[] = {0xEF, 0xBB, 0xBF};
        static const unsigned char u16lb[] = {0xFF, 0xFE}, u16bb[] = {0xFE, 0xFF};

        switch (Enc.GetCodepage()) {
        case NStreamEncoding::UTF8:
            return WriteOctets((void*) u8b, sizeof(u8b));

        case NStreamEncoding::UTF16LE:
            return WriteOctets((void*) u16lb, sizeof(u16lb));

        case NStreamEncoding::UTF16BE:
            return WriteOctets((void*) u16bb, sizeof(u16bb));
        }

        return false;
    }
    bool WriteString(const wchar_t*Str, size_t cch = -1);
    bool WritePlatformNLString(const wchar_t*Str, size_t cch = -1);
};

class NStreamLineReader
{
protected:
    NIStream &m_Strm;
    wchar_t m_PrevNL;

public:
    NStreamLineReader(NIStream &Strm) : m_Strm(Strm), m_PrevNL(0) {}

    NIStream& GetStream()
    {
        return m_Strm;
    }
    NStreamEncoding& StreamEncoding()
    {
        return m_Strm.StreamEncoding();
    }
    bool IsEOF() const
    {
        return m_Strm.IsEOF();
    }
    bool IsUnicode() const
    {
        return m_Strm.IsUnicode();
    }

    UINT ReadLine(wchar_t*Buffer, UINT cchBuf);
    tstring GetErrorMessage(UINT Error, const TCHAR*Filename = 0, UINT Line = 0);

protected:
    bool CompleteLine(wchar_t*&BufWC, UINT cchWC, UINT&cchRemain, bool HandleUnicodeNL)
    {
        const wchar_t chW = *BufWC;
        BufWC += cchWC, cchRemain -= cchWC;

        if (0 == --cchWC) // We only care about code points that fit in a single wchar_t
            return NStream::IsNewline(chW, HandleUnicodeNL);

        return false;
    }
};
} // namespace cactus

namespace cactus
{
class CACTUS_API CValidateUnicode
{
public:

    // Enum type for each Unicode encoding.
    enum FILE_TYPE {
        UTF_8 = 0,
        UTF_16LE,
        UTF_16BE,
        UTF_32LE,
        UTF_32BE,
        UNKNOWN
    };

    // Make sure that the buffer contains valid UTF-8 encoding.
    static int ValidateUTF8(unsigned char* buf, size_t characters);

    // Make sure that the buffer contains valid UTF-16LE encoding.
    static bool ValidateUTF16LE(unsigned char* buf, size_t bytes);

    // Make sure that the buffer contains valid UTF-16BE encoding.
    static bool ValidateUTF16BE(unsigned char* buf, size_t bytes);

    // Make sure that the buffer contains valid UTF-16 encoding.
    static bool ValidateUTF16(unsigned short* buf, size_t characters);

    // Does the buffer have a byte order mark?  And if so, what does it say?
    static FILE_TYPE CheckBOM(unsigned char* buf, size_t bytes);

    // Convert a FILE_TYPE enum to a string.
    static const TCHAR* TypeToName(FILE_TYPE ftype);

protected:

    // Given the initial byte of a UTF-8 character, how many bytes are to
    // follow?
    static int GetBytesToFollow(unsigned char ch);
};
} // namespace cactus


#endif // __CACTUS_CACTUS_VALIDATE_UNICODE_H__
