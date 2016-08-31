#include <cactus/cactus_validate_unicode.h>
#include <assert.h>
#include <vector>



namespace cactus
{
FILE *g_output;

#ifdef _WIN32
    #define NPRIs _T("s")
    #define NPRINs _T("hs")
    #define NPRIWs _T("ls") // ws also works, not sure which is most compatible
    #ifndef _WIN64
        #define NPRIp _T(".8x")
        #define NPRINp ".8x"
    #endif
#else  // !_WIN32
    #define NPRINs _T("s")
    #define NPRIWs _T("ls")
    #ifdef _UNICODE
        #define NPRIs _T("ls")
    #else // !_UNICODE
        #define NPRIs _T("s")
    #endif // ~_UNICODE
#endif // ~_WIN32
#ifndef NPRIp
    #define NPRIp _T("p")
    #define NPRINp "p"
#endif


#ifndef __BIG_ENDIAN__
    #define FIX_ENDIAN_INT64(x) (x)
    #define FIX_ENDIAN_INT32_INPLACE(x) ((void)(x))
    #define FIX_ENDIAN_INT32(x) (x)
    #define FIX_ENDIAN_INT16_INPLACE(x) ((void)(x))
    #define FIX_ENDIAN_INT16(x) (x)
#else
    #define FIX_ENDIAN_INT64(x) SWAP_ENDIAN_INT64(x)
    #define FIX_ENDIAN_INT32_INPLACE(x) ((x) = SWAP_ENDIAN_INT32(x))
    #define FIX_ENDIAN_INT32(x) SWAP_ENDIAN_INT32(x)
    #define FIX_ENDIAN_INT16_INPLACE(x) ((x) = SWAP_ENDIAN_INT16(x))
    #define FIX_ENDIAN_INT16(x) SWAP_ENDIAN_INT16(x)
#endif

#define SWAP_ENDIAN_INT64(x) ( \
                               (((x)&0xFF00000000000000) >> 56) | \
                               (((x)&0x00FF000000000000) >> 40) | \
                               (((x)&0x0000FF0000000000) >> 24) | \
                               (((x)&0x000000FF00000000) >>  8) | \
                               (((x)&0x00000000FF000000) <<  8) | \
                               (((x)&0x0000000000FF0000) << 24) | \
                               (((x)&0x000000000000FF00) << 40) | \
                               (((x)&0x00000000000000FF) << 56) )
#define SWAP_ENDIAN_INT32(x) ( \
                               (((x)&0xFF000000) >> 24) | \
                               (((x)&0x00FF0000) >>  8) | \
                               (((x)&0x0000FF00) <<  8) | \
                               (((x)&0x000000FF) << 24) )
#define SWAP_ENDIAN_INT16(x) ( \
                               (((x)&0xFF00) >> 8) | \
                               (((x)&0x00FF) << 8) )


#define FIX_ENDIAN_INT16LETOHOST_INPLACE FIX_ENDIAN_INT16_INPLACE

#ifdef _UNICODE
void PathConvertWinToPosix(char*p)
{
    if ('\"' == *p) ++p; // Skip opening quote if any (For !system)

    size_t len = strlen(p);

    /* Replace drive letter X: by /x */
    if (len >= 2 && ':' == p[1]) p[1] = (char) tolower((int) p[0]), p[0] = '/';

    do if ('\\' == *p) *p = '/';

    while (*(p = CharNextA(p)));
}
#endif
void PathConvertWinToPosix(TCHAR*p)
{
    if (_T('\"') == *p) ++p; // Skip opening quote if any (For !system)

    size_t len = _tcsclen(p);

    /* Replace drive letter X: by /x */
    if (len >= 2 && _T(':') == p[1]) p[1] = (TCHAR) tolower((int) p[0]), p[0] = _T('/');

    do if (_T('\\') == *p) *p = _T('/');

    while (*(p = CharNext(p)));
}

char* NSISRT_wctomb(const wchar_t *Str)
{
    const char *orglocct = setlocale(LC_CTYPE, "");
    char *d = 0;
    const wchar_t *s = Str;
    errno = 0;
    size_t cb = wcsrtombs(0, &s, 0, 0);

    if ((size_t) - 1 != cb && (d = (char*) malloc(++cb))) {
        cb = wcsrtombs(d, &Str, cb, 0);

        if ((size_t) - 1 == cb) {
            NSISRT_free(d);
            d = 0;
        }
    }

    if (!errno) errno = ENOMEM;

    setlocale(LC_CTYPE, orglocct);
    return d;
}

char* NSISRT_wctombpath(const wchar_t *Path)
{
    char *p = NSISRT_wctomb(Path);

    if (p) PathConvertWinToPosix(p);

    return p;
}

FILE* my_fopen(const TCHAR *path, const char *mode)
{
    FILE*f = 0;
#ifndef _UNICODE
    f = fopen(path, mode);
#else
#ifdef _WIN32
    TCHAR tmode[20];

    for (int i = 0; ; ++i) if (0 == (tmode[i] = mode[i])) break;

    f = _wfopen(path, tmode);
#else
    char *nativepath = NSISRT_wctombpath(path);

    if (nativepath) {
        f = fopen(nativepath, mode);
        NSISRT_free(nativepath);
    }

#endif
#endif
    return f;
}

UINT StrLenUTF16(const void*str)
{
    return sizeof(wchar_t) == 2 ? (UINT)wcslen((wchar_t*)str) : InlineStrLenUTF16(str);
}

bool StrSetUTF16LE(tstring&dest, const void*src)
{
#ifndef _WIN32
    CharEncConv cec;

    if (!cec.Initialize(-1, NStreamEncoding::UTF16LE)) return false;

    src = (const void*) cec.Convert(src);

    if (!src) return false;

#endif

    try {
        dest = (wchar_t*) src;
    } catch (...) {
        return false;
    }

    return true;
}

void UTF16InplaceEndianSwap(void*Buffer, UINT cch)
{
    unsigned short *p = (unsigned short *) Buffer;

    while (cch--) p[cch] = SWAP_ENDIAN_INT16(p[cch]);
}

inline UINT UTF8ToWC_Convert(LPCSTR StrU8, UINT cbU8, wchar_t*Buffer, UINT cchBuf)
{
#ifndef MB_ERR_INVALID_CHARS
    const UINT MB_ERR_INVALID_CHARS = 8; // MSDN says this flag is OK for CP_UTF8
#endif
    return (UINT) MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, StrU8, cbU8, Buffer, cchBuf);
}
inline UINT UTF8ToWC_Prepare(LPCSTR StrU8, UINT cbU8)
{
    return UTF8ToWC_Convert(StrU8, cbU8, 0, 0);
}

UINT WCFromCodePoint(wchar_t*Dest, UINT cchDest, UINT32 CodPt)
{
    // Don't allow half surrogate pairs
    if (CodPt >= 0xd800 && CodPt <= 0xdfff) CodPt = UNICODE_REPLACEMENT_CHARACTER;

#ifdef _WIN32

    if (CodPt <= 0xffff && cchDest) {
        *Dest = (wchar_t) CodPt;
        return 1;
    } else if (cchDest >= 2) {
        const UINT32 lead_offset = 0xd800 - (0x10000 >> 10);
        UINT16 lead = lead_offset + (CodPt >> 10), trail = 0xdc00 + (CodPt & 0x3ff);
        Dest[0] = lead, Dest[1] = trail;
        return 2;
    }

    return 0;
#else
    iconvdescriptor iconvd;

    if (!iconvd.Open("wchar_t", iconvdescriptor::GetHostEndianUCS4Code())) return 0;

    size_t inleft = 4;
    UINT cchW = iconvd.Convert(&CodPt, &inleft, Dest, cchDest * sizeof(wchar_t)) / sizeof(wchar_t);
    return !inleft ? cchW : 0;
#endif
}

wchar_t* DupWCFromBytes(void*Buffer, UINT cbBuffer, UINT32 SrcCP)
{
    /*\
    Converts a buffer encoded with SrcCP to a \0 terminated wchar_t malloc'ed buffer.
    Returns 0 on failure.
    \*/
    CharEncConv cec;
    cec.SetAllowOptimizedReturn(!!(SrcCP & DWCFBF_ALLOWOPTIMIZEDRETURN));

    if (!cec.Initialize(-1, SrcCP &= ~DWCFBF_ALLOWOPTIMIZEDRETURN)) return 0;

    wchar_t *pWC = (wchar_t*) cec.Convert(Buffer, cbBuffer);
    return pWC ? (wchar_t*) cec.Detach() : 0;
}
} // namespace cactus


namespace cactus
{
BOOL CharEncConv::IsValidCodePage(UINT cp)
{
#ifdef _WIN32

    if (cp <= 1 || NStreamEncoding::IsUnicodeCodepage(cp)) return true; // Allow ACP/OEM/UTF*

#endif
    return cp < NStreamEncoding::CPCOUNT && ::IsValidCodePage(cp);
}
bool CharEncConv::Initialize(UINT32 ToEnc, UINT32 FromEnc)
{
    /*
    ** Initialize() with a Windows codepage or -1 for wchar_t
    */
    const WORD UTF32LE = NStreamEncoding::UTF32LE;
#ifdef _WIN32

    if (NStreamEncoding::UTF16LE == ToEnc) ToEnc = -1;

    if (NStreamEncoding::UTF16LE == FromEnc) FromEnc = -1;

#endif
    m_TE = (WORD) ToEnc, m_FE = (WORD) FromEnc;

    if ((UTF32LE | 1) == (m_FE | 1) || (UTF32LE | 1) == (m_TE | 1)) return false; // UTF-32 is a pain to deal with on Windows

#ifdef _WIN32
    return (IsWE(m_FE) || IsValidCodePage(FromEnc)) && (IsWE(m_TE) || IsValidCodePage(ToEnc));
#else
    char f[50], t[COUNTOF(f)];

    if (IsWE(m_FE)) strcpy(f, "wchar_t");
    else create_code_page_string(f, COUNTOF(f), m_FE);

    if (IsWE(m_TE)) strcpy(t, "wchar_t");
    else create_code_page_string(t, COUNTOF(t), m_TE);

    return m_TE == m_FE || m_iconvd.Open(t, f);
#endif
}
size_t CharEncConv::GuessOutputSize(size_t cbConverted)
{
    UINT cus = IsWE(m_TE) ? sizeof(wchar_t) : NStreamEncoding::GetCodeUnitSize(m_TE);
    size_t zt = 1, cch = cbConverted / cus;

    if (!cch) return 0;

    switch (cus) {
    case 1:
        zt = !!((char*)m_Result)[--cch];
        break;

    case 2:
        zt = !!((WORD*)m_Result)[--cch];
        break;

    case 4:
        zt = !!((UINT32*)m_Result)[--cch];
        break;
    }

    return (cch + (zt ? 0 : 1)) * cus;
}
void* CharEncConv::Convert(const void*Src, size_t cbSrc, size_t*cbOut)
{
    /*
    ** Convert() mallocs a buffer and converts Src (as m_FE) to m_TE.
    ** If cbSrc is -1 the size is calculated. cbOut can be NULL.
    ** Returns a pointer to the buffer on success or 0 on error.
    ** The buffer is valid until you call Close() or Convert().
    */
#ifdef _WIN32
    m_OptimizedReturn = false;
#endif

    if ((size_t) - 1 == cbSrc) {
        UINT cus = IsWE(m_FE) ? sizeof(wchar_t) : NStreamEncoding::GetCodeUnitSize(m_FE);

        switch (cus) {
        case 1:
            cbSrc = strlen((char*)Src);
            break;

        case 2:
            cbSrc = StrLenUTF16(Src);
            break;

        //case 4: // No UTF-32 support...
        default:
            if (sizeof(wchar_t) > 2 && sizeof(wchar_t) == cus) {
                cbSrc = wcslen((wchar_t*)Src);
                break;
            }

            assert(0);
            return 0;
        }

        cbSrc = (cbSrc + 1) * cus;
    }

    if (m_FE == m_TE) {
#ifdef _WIN32

        if (m_AllowOptimizedReturn && IsWE(m_FE)) {
            if (cbOut) {
                cbSrc /= sizeof(wchar_t);

                if (cbSrc && ((WORD*)Src)[--cbSrc]) ++cbSrc;

                *cbOut = cbSrc * sizeof(wchar_t);
            }

            m_OptimizedReturn = true;
            return (void*)(m_Result = (char*) Src);
        }

#endif
        char *p = (char*) realloc(m_Result, cbSrc + sizeof(UINT32));

        if (p) m_Result = p;
        else return 0;

        memcpy(p, Src, cbSrc);
        *((UINT32*)(p + cbSrc)) = 0;

        if (cbOut) *cbOut = GuessOutputSize(cbSrc);

        return m_Result;
    }

#ifdef _WIN32

    if (!IsWE(m_FE) && !IsWE(m_TE) && NStreamEncoding::UTF16BE != m_TE) {
        // We need a middle step: Src -> wchar_t -> Target
        CharEncConv cec;

        if (!cec.Initialize(-1, m_FE)) return 0;

        size_t cbConv;
        char *pWC = (char*) cec.Convert(Src, cbSrc, &cbConv);

        if (!pWC) return 0;

        this->m_FE = -1;
        return this->Convert(pWC, cbConv, cbOut);
    }

    if (IsWE(m_FE)) {
        if (NStreamEncoding::UTF16BE == m_TE) goto l_swapUTF16;

        cbSrc /= sizeof(wchar_t);
        UINT cbDest = WideCharToMultiByte(m_TE, 0, (wchar_t*)Src, (int)cbSrc, 0, 0, 0, 0);
        char *p = (char*) realloc(m_Result, (cbDest + 1) * sizeof(char));

        if (p) m_Result = p;
        else return 0;

        if (!(cbDest = WideCharToMultiByte(m_TE, 0, (wchar_t*)Src, (int)cbSrc, p, (int)cbDest, 0, 0))) return 0;

        if (p[--cbDest]) p[++cbDest] = '\0'; // Always \0 terminate

        if (cbOut) *cbOut = cbDest; // cbOut never includes the \0 terminator
    } else {
        UINT cchDest;

        if (NStreamEncoding::UTF16BE == m_FE) { // UTF16BE -> UTF16LE/wchar_t
l_swapUTF16:
            char *p = (char*) realloc(m_Result, cbSrc + sizeof(wchar_t));

            if (p) m_Result = p;
            else return 0;

            memcpy(p, Src, cbSrc);
            cchDest = (UINT)(cbSrc / sizeof(wchar_t));
            UTF16InplaceEndianSwap(p, cchDest);

            if (!cchDest) *((WORD*)p) = 0, ++cchDest; // For "--cchDest" during \0 termination
        } else {
            cchDest = MultiByteToWideChar(m_FE, 0, (char*)Src, (int)cbSrc, 0, 0);
            char *p = (char*) realloc(m_Result, (cchDest + 1) * sizeof(wchar_t));

            if (p) m_Result = p;
            else return 0;

            if (!(cchDest = MultiByteToWideChar(m_FE, 0, (char*)Src, (int)cbSrc, (LPWSTR)p, (int)cchDest))) return 0;

            if (NStreamEncoding::UTF16BE == m_TE) UTF16InplaceEndianSwap(p, cchDest);
        }

        if (((WORD*)m_Result)[--cchDest])((WORD*)m_Result)[++cchDest] = '\0';

        if (cbOut) *cbOut = cchDest * sizeof(wchar_t);
    }

#else
    char *in = (char*) Src;
    size_t cbConv;

    if (!nsis_iconv_reallociconv(m_iconvd, &in, &cbSrc, &m_Result, cbConv)) return 0;

    if (cbOut) *cbOut = GuessOutputSize(cbConv);

#endif
    return m_Result;
}

#if !defined(_WIN32) || !defined(_UNICODE)
bool WCToUTF16LEHlpr::Create(const TCHAR*in, unsigned int codepage)
{
    CharEncConv cec;

    if (!cec.Initialize(NStreamEncoding::UTF16LE, -1)) return false;

    if (!cec.Convert(in)) return false;

    m_s = (unsigned short*) cec.Detach();
    return true;
}
#endif

UINT DetectUTFBOM(void*Buffer, UINT cb)
{
    unsigned char *b = (unsigned char*) Buffer;

    if (cb >= 3 && 0xef == b[0] && 0xbb == b[1] && 0xbf == b[2])
        return NStreamEncoding::UTF8;

    if (cb >= 2) {
        if (cb >= 4 && !b[0] && !b[1] && 0xfe == b[2] && 0xff == b[3])
            return NStreamEncoding::UTF32BE;

        if (0xff == b[0] && 0xfe == b[1])
            return (cb >= 4 && !b[2] && !b[3]) ? NStreamEncoding::UTF32LE : NStreamEncoding::UTF16LE;

        if (0xfe == b[0] && 0xff == b[1])
            return NStreamEncoding::UTF16BE;
    }

    return 0;
}
UINT DetectUTFBOM(FILE*strm)
{
    /*\
    Tries to detect a BOM at the current position in a stream.
    If a BOM is found it is eaten.
    NOTE: ungetc is only guaranteed to support 1 pushback,
    lets hope no MBCS file starts with parts of a BOM.
    \*/
    const int b1 = fgetc(strm);

    if (EOF == b1) return 0;

    if (0xef == b1) {
        const int b2 = fgetc(strm);

        if (0xbb == b2) {
            const int b3 = fgetc(strm);

            if (0xbf == b3) return NStreamEncoding::UTF8;

            ungetc(b3, strm);
        }

        ungetc(b2, strm);
    }

    if (0xfe == b1 || 0xff == b1 || 0x00 == b1) {
        const int b2 = fgetc(strm), b3 = fgetc(strm);

        if (b1 && (b1 ^ b2) == (0xfe ^ 0xff)) {
            if (0xff == b1 && 0 == b3) {
                const int b4 = fgetc(strm);

                if (0 == b4) return NStreamEncoding::UTF32LE;

                ungetc(b4, strm);
            }

            ungetc(b3, strm);
            return 0xff == b1 ? NStreamEncoding::UTF16LE : NStreamEncoding::UTF16BE;
        }

        if (0 == b1 && 0 == b2) {
            if (0xfe == b3) {
                const int b4 = fgetc(strm);

                if (0xff == b4) return NStreamEncoding::UTF32BE;

                ungetc(b4, strm);
            }
        }

        ungetc(b3, strm);
        ungetc(b2, strm);
    }

    ungetc(b1, strm);
    return 0;
}

WORD GetEncodingFromString(const TCHAR*s, bool&BOM)
{
    BOM = false;

    if (!_tcsicmp(s, _T("ACP"))) return NStreamEncoding::ACP;

    if (!_tcsicmp(s, _T("OEM"))) return NStreamEncoding::OEMCP;

    if (!_tcsicmp(s, _T("UTF8"))) return NStreamEncoding::UTF8;

    if ((!_tcsicmp(s, _T("UTF8SIG")) || !_tcsicmp(s, _T("UTF8BOM"))) && ++BOM)
        return NStreamEncoding::UTF8;

    if (!_tcsicmp(s, _T("UTF16LE")) || (!_tcsicmp(s, _T("UTF16LEBOM")) && ++BOM))
        return NStreamEncoding::UTF16LE;

    if (!_tcsicmp(s, _T("UTF16BE")) || (!_tcsicmp(s, _T("UTF16BEBOM")) && ++BOM))
        return NStreamEncoding::UTF16BE;

    if (S7IsChEqualI('C', *s++) && S7IsChEqualI('P', *s++)) {
        int cp = _tstoi(s);

        if (cp > 0 && cp < NStreamEncoding::CPCOUNT) return (WORD) cp;
    }

    return NStreamEncoding::UNKNOWN;
}
WORD GetEncodingFromString(const TCHAR*s)
{
    bool bom;
    return GetEncodingFromString(s, bom);
}

void NStreamEncoding::GetCPDisplayName(WORD CP, TCHAR*Buf)
{
    TCHAR mybuf[10];
    const TCHAR *p = mybuf;

    switch (CP) {
    case ACP:
        p = _T("ACP");
        break;

    case OEMCP:
        p = _T("OEM");
        break;

    case UTF16LE:
        p = _T("UTF16LE");
        break;

    case UTF16BE:
        p = _T("UTF16BE");
        break;

    case UTF32LE:
        p = _T("UTF32LE");
        break;

    case UTF32BE:
        p = _T("UTF32BE");
        break;

    case UTF8:
        p = _T("UTF8");
        break;

    default:
        _stprintf(mybuf, _T("CP%u"), CP);

        if (CP >= NStreamEncoding::CPCOUNT) p = _T("?");
    }

    _tcscpy(Buf, p);
}

bool NBaseStream::Attach(FILE*hFile, WORD enc, bool Seek /*= true*/)
{
    Close();
    m_hFile = hFile;

    if (!m_hFile) return false;

    if (!NStream::SetBinaryMode(m_hFile) && m_hFile != stdin) return false;

    fpos_t pos;

    if (Seek && !fgetpos(m_hFile, &pos)) rewind(m_hFile);
    else Seek = false;

    WORD cp = DetectUTFBOM(m_hFile);

    if (Seek) {
        fsetpos(m_hFile, &pos);

        if (cp) DetectUTFBOM(m_hFile); // parseScript() etc does not like the BOM, make sure we skip past it
    }

    if (!cp) cp = enc;

    m_Enc.SafeSetCodepage(cp);
    return true;
}

bool NOStream::WriteString(const wchar_t*Str, size_t cch /*= -1*/)
{
    CharEncConv cec;

    if (!cec.Initialize(m_Enc.GetCodepage(), -1)) return false;

    cec.SetAllowOptimizedReturn(true);

    if ((size_t) - 1 != cch) cch *= sizeof(wchar_t); // cec.Convert wants byte count

    size_t cbConv;
    char *p = (char*) cec.Convert(Str, cch, &cbConv);
    return p && WriteOctets(p, cbConv);
}
bool NOStream::WritePlatformNLString(const wchar_t*Str, size_t cch /*= -1*/)
{
#ifdef _WIN32
    size_t cch2 = 0, nlcount = 0;

    for (; cch2 < cch && Str[cch2]; ++cch2) if (L'\n' == Str[cch2]) ++nlcount;

    if (nlcount) {
        cch = cch2 + nlcount;
        wchar_t chPrev = 0, *buf = (wchar_t*) malloc(cch * sizeof(wchar_t));

        if (!buf) return false;

        for (size_t s = 0, d = 0; d < cch; ++s, ++d) {
            if (L'\n' == Str[s]) {
                if (L'\r' != chPrev) buf[d++] = L'\r';
                else --cch;
            }

            buf[d] = chPrev = Str[s];
        }

        bool retval = WriteString(buf, cch);
        free(buf);
        return retval;
    }

#endif
    return WriteString(Str, cch);
}

tstring NStreamLineReader::GetErrorMessage(UINT Error, const TCHAR*Filename, UINT Line)
{
    tstring msg;
    TCHAR buf[40];

    switch (Error) {
    case NStream::ERR_BUFFEROVERFLOW:
        msg = _T("Line too long");
        break;

    case NStream::ERR_IOERROR:
        msg = _T("I/O error"), Filename = 0;
        break;

    case NStream::ERR_UNSUPPORTEDENCODING:
        StreamEncoding().GetCPDisplayName(buf);
        msg = tstring(buf) + _T(" is not supported"), Filename = 0;
        break;

    default:
        msg = _T("Bad text encoding");
        break;
    }

    if (Filename) {
        const TCHAR *filelinesep = *Filename ? _T(":") : _T("");
        _stprintf(buf, _T("%") NPRIs _T("%u"), filelinesep, Line);
        msg = msg + _T(": ") + Filename + buf;
    }

    return msg + _T("\n");
}

UINT NStreamLineReader::ReadLine(wchar_t*Buffer, UINT cchBuf)
{
    /*\
    Reads from the associated stream until it finds a new-line or
    the read fails (I/O error or EOF). It fails with ERR_BUFFEROVERFLOW if
    cchBuf-1 wchar_t's are read without finding the end of the line.
    Buffer MUST be a valid pointer, it will be \0 terminated as long as cchBuf > 0.
    \*/
    if (!cchBuf) return NStream::ERR_BUFFEROVERFLOW;

#ifndef MB_ERR_INVALID_CHARS
    const UINT MB_ERR_INVALID_CHARS = 8;
#endif
    const UINT cchFullBuf = cchBuf;
    NIStream&strm = GetStream();
#ifndef _WIN32
    iconvdescriptor iconvd;
#endif
l_restart:

    if (StreamEncoding().IsUTF8()) {
        for (;;) {
            BYTE cb = 0; // bytes in chU8 -1
            BYTE chU8[6];

            if (!strm.ReadOctet(&chU8[0])) goto l_ioerror;

            UINT cchWC;
#if defined(WIN32) // TODO: Is wchar_t==UTF16LE under cygwin?

            // Fast path if wchar_t == UTF16 and in ASCII range
            if (chU8[0] <= 127 && sizeof(wchar_t) == 2) {
                cchWC = ++cb;

                if (cchBuf <= cchWC) goto l_lineoverflow;

                *Buffer = (wchar_t) chU8[0];
            } else
#endif
            {
                if (!UTF8_GetTrailCount(chU8[0], cb)) goto l_badutf;

                for (BYTE moreU8 = 0; moreU8 < cb;) {
                    BYTE b;

                    if (!strm.ReadOctet(&b)) goto l_ioerror;

                    if (0x80 != (0xC0 & b)) goto l_badutf; // chU8[1..n] must be 0b10xxxxxx

                    chU8[++moreU8] = b;
                }

                ++cb;
                cchWC = UTF8ToWC_Prepare((LPCSTR)chU8, cb);

                if (!cchWC) goto l_badutf;

                if (cchBuf <= cchWC) goto l_lineoverflow;

                cchWC = UTF8ToWC_Convert((LPCSTR)chU8, cb, Buffer, cchWC);
            }

            if (CompleteLine(Buffer, cchWC, cchBuf, true)) goto l_success;
        }
    } else if (StreamEncoding().IsUTF16()) {
#ifndef _WIN32

        if (!iconvd.Open("wchar_t", iconvdescriptor::GetHostEndianUCS4Code())) goto l_unsupportedencoding;

#endif
        const bool utf16be = StreamEncoding().IsUTF16BE();
        unsigned short lead, trail, cchWC;

        for (;;) {
            if (!strm.ReadInt16(&lead)) goto l_ioerror;

            FIX_ENDIAN_INT16LETOHOST_INPLACE(lead);

            if (utf16be) lead = SWAP_ENDIAN_INT16(lead);

            if (IsTrailSurrogateUTF16(lead)) goto l_badutf;

            UINT32 codpt = lead;

            if (cchBuf <= 1) goto l_lineoverflow;

            Buffer[0] = lead, cchWC = 1;

            if (IsLeadSurrogateUTF16(lead)) {
                if (!strm.ReadInt16(&trail)) goto l_ioerror;

                FIX_ENDIAN_INT16LETOHOST_INPLACE(trail);

                if (utf16be) trail = SWAP_ENDIAN_INT16(trail);

                if (!IsTrailSurrogateUTF16(trail)) goto l_badutf;

                codpt = CodePointFromUTF16SurrogatePair(lead, trail);
#ifdef _WIN32

                if (cchBuf <= 2) goto l_lineoverflow;

                Buffer[1] = trail, ++cchWC;
#endif
            }

            if (!IsValidUnicodeCodePoint(codpt)) goto l_badutf;

#ifndef _WIN32
            char tmpdest[8]; // Should be plenty of space to store one UCS4 character as wchar_t(s)
            size_t inleft = 4;
            cchWC = iconvd.Convert(&codpt, &inleft, tmpdest, sizeof(tmpdest)) / sizeof(wchar_t);

            if (!cchWC) goto l_badutf;

            if (cchBuf <= cchWC) goto l_lineoverflow;

            for (UINT i = cchWC; i;) --i, Buffer[i] = ((wchar_t*)tmpdest)[i];

#endif

            if (CompleteLine(Buffer, cchWC, cchBuf, true)) goto l_success;
        }
    } else if (StreamEncoding().IsUnicode()) {
        goto l_unsupportedencoding;
    } else {
        const UINT cp = StreamEncoding().GetCodepage();
        UINT mbtowcflags = (cp < 50220 && cp != 42) ? MB_ERR_INVALID_CHARS : 0;

        for (;;) {
            BYTE bufMB[2];
            BYTE mb = 0;

            if (!strm.ReadOctet(&bufMB[0])) goto l_ioerror;

            if (IsDBCSLeadByteEx(cp, bufMB[0])) {
                if (!strm.ReadOctet(&bufMB[++mb])) goto l_ioerror;
            }

            ++mb;
            UINT cchWC = MultiByteToWideChar(cp, mbtowcflags, (LPCSTR)bufMB, mb, 0, 0);

            if (!cchWC) goto l_badencoding;

            if (cchBuf <= cchWC) goto l_lineoverflow;

            cchWC = MultiByteToWideChar(cp, mbtowcflags, (LPCSTR)bufMB, mb, Buffer, cchWC);

            if (CompleteLine(Buffer, cchWC, cchBuf, false)) goto l_success;
        }
    }

l_ioerror:
    *Buffer = 0;
    return NStream::ERR_IOERROR;
l_lineoverflow:
    *Buffer = 0;
    return NStream::ERR_BUFFEROVERFLOW;
l_badutf:
l_badencoding:
    *Buffer = 0;
    return NStream::ERR_INVALIDENCODING;
l_unsupportedencoding:
    *Buffer = 0;
    return NStream::ERR_UNSUPPORTEDENCODING;
l_success:
    *Buffer = 0;
    // "Foo\r\nBar" is 2 and not 3 lines
    const wchar_t chThisNL = *--Buffer, chPrevNL = m_PrevNL;
    const bool onlyNL = ++cchBuf == cchFullBuf;
    m_PrevNL = chThisNL;

    if (onlyNL && (chPrevNL ^ chThisNL) == ('\r'^'\n')) {
        m_PrevNL = 0;
        goto l_restart; // Previous line was "Foo\r". This line was "\n", ignore it.
    }

    return NStream::OK;
}

} // namespace cactus




namespace cactus
{
int CValidateUnicode::ValidateUTF8(unsigned char* buf, size_t characters)
{
    bool hasNonAscii = false;
    int bytesToFollow = 0;

    for (; characters != 0 ; --characters) {
        unsigned char ch = *buf++;

        if (bytesToFollow != 0) { // in the middle of a multi-byte sequence?
            if ((ch & 0xC0) != 0x80)
                return 0; // we expected a continuation byte

            hasNonAscii = true;
            --bytesToFollow;
        } else if (ch & 0x80) {
            if ((ch & 0xC0) == 0x80)
                return 0; // continuation byte outside multi-byte sequence
            else if ((ch & 0xE0) == 0xC0)
                bytesToFollow = 1;
            else if ((ch & 0xF0) == 0xE0)
                bytesToFollow = 2;
            else if ((ch & 0xF8) == 0xF0)
                bytesToFollow = 3;
            else
                return 0; // byte is invalid UTF-8 (outside RFC 3629)
        } else if (ch == 0 && characters != 1)
            return 0; // NUL character in the middle of the buffer
    }

    if (bytesToFollow != 0)
        return 0; // end of buffer in the middle of a multi-byte sequence

    return hasNonAscii ? 2 : 1;
}

bool CValidateUnicode::ValidateUTF16LE(unsigned char* buf, size_t bytes)
{
    // We need to make sure the endianness matches the processor.
    // Intel x86 is little endian.
    return ValidateUTF16((unsigned short*)(buf), bytes / 2);
}

bool CValidateUnicode::ValidateUTF16BE(unsigned char* buf, size_t bytes)
{
    std::vector<unsigned short> correctedBuf(bytes / 2);

    for (size_t i = 0; i < bytes; i += 2) {
        correctedBuf[i / 2] = buf[i] << 8 | buf[i + 1];
    }

    return ValidateUTF16(&correctedBuf[0], correctedBuf.size());
}

bool CValidateUnicode::ValidateUTF16(unsigned short* buf, size_t characters)
{
    unsigned short ch;
    bool valid = true;

    while (valid && characters > 0) {
        // Last character may be 0.
        if ((ch = *buf) == 0 && characters != 1) {
            valid = false;
        } else if (ch >= 0xd800 && ch <= 0xdbff) {
            unsigned short trailing = *(++buf);
            --characters;

            // Unpaired leading surrogate found?
            if (trailing < 0xdc00 || trailing > 0xdfff) {
                valid = false;
            }
            // Invalid surrogate pairs found?
            else if ((ch == 0xd83f ||
                      ch == 0xd87f ||
                      ch == 0xd8bf ||
                      ch == 0xd8ff ||
                      ch == 0xd93f ||
                      ch == 0xd97f ||
                      ch == 0xd9bf ||
                      ch == 0xd9ff ||
                      ch == 0xda3f ||
                      ch == 0xdA7f ||
                      ch == 0xdabf ||
                      ch == 0xdaff ||
                      ch == 0xdb3f ||
                      ch == 0xdb7f ||
                      ch == 0xdbbf ||
                      ch == 0xdbff)
                     &&
                     (trailing == 0xdffe || trailing == 0xdfff)) {
                valid = false;
            }
        }
        // Unpaired trailing surrogate!
        else if (ch >= 0xdc00 && ch <= 0xdfff) {
            valid = false;
        }
        // Invalid values
        else if (ch == 0xfffe || ch == 0xffff ||
                 (ch >= 0xfdd0 && ch <= 0xfdef)) {
            valid = false;
        }

        ++buf;
        --characters;
    }

    return valid;
}

CValidateUnicode::FILE_TYPE CValidateUnicode::CheckBOM(
    unsigned char* buf,
    size_t         bytes)
{
    FILE_TYPE result = UNKNOWN;

    if (bytes >= 2) {
        if (buf[0] == 0xff && buf[1] == 0xfe) {
            result = UTF_16LE;
        } else if (buf[0] == 0xfe && buf[1] == 0xff) {
            result = UTF_16BE;
        } else if (bytes >= 3 &&
                   buf[0] == 0xef &&
                   buf[1] == 0xbb &&
                   buf[2] == 0xbf) {
            result = UTF_8;
        } else if (bytes >= 4) {
            if (buf[0] == 0 &&
                buf[1] == 0 &&
                buf[2] == 0xfe &&
                buf[3] == 0xff) {
                result = UTF_32BE;
            } else if (buf[0] == 0xff &&
                       buf[1] == 0xfe &&
                       buf[2] == 0 &&
                       buf[3] == 0) {
                result = UTF_32LE;
            }
        }
    }

    return result;
}

const TCHAR* CValidateUnicode::TypeToName(CValidateUnicode::FILE_TYPE ftype)
{
    static const TCHAR* names[] = {
        _T("UTF-8"),
        _T("UTF-16LE"),
        _T("UTF-16BE"),
        _T("UTF-32LE"),
        _T("UTF-32BE"),
        _T("UNKNOWN")
    };
    return names[ftype];
}

} // namespace cactus


namespace cactus
{
#ifdef _WIN32
#ifdef _UNICODE
int RunChildProcessRedirected(LPCWSTR cmdprefix, LPCWSTR cmdmain)
{
    // We have to deliver the requested output encoding to our host (if any) and the
    // only way to do that is to convert the pipe content from what we hope is UTF-8.
    // The reason we need a pipe in the first place is because we cannot trust the
    // child to call GetConsoleOutputCP(), and even if we could, UTF-16 is not valid there.
    UINT cp = CP_UTF8, mbtwcf = MB_ERR_INVALID_CHARS, oemcp = GetOEMCP();
    errno = ENOMEM;

    if (!cmdprefix) cmdprefix = L"";

    size_t cch1 = _tcslen(cmdprefix), cch2 = _tcslen(cmdmain);
    WCHAR *cmd = (WCHAR*) malloc((cch1 + cch2 + 1) * sizeof(WCHAR));

    if (!cmd) return -1;

    _tcscpy(cmd, cmdprefix);
    _tcscat(cmd, cmdmain);
    SECURITY_DESCRIPTOR sd = {1, 0, SE_DACL_PRESENT, NULL, };
    SECURITY_ATTRIBUTES sa = {sizeof(sa), &sd, true};
    const UINT orgwinconcp = GetConsoleCP(), orgwinconoutcp = GetConsoleOutputCP();

    if (orgwinconoutcp == oemcp) cp = oemcp, mbtwcf = 0; // Bug #1092: Batch files not a fan of UTF-8

    HANDLE hPipRd, hPipWr;
    PROCESS_INFORMATION pi;
    BOOL ok = CreatePipe(&hPipRd, &hPipWr, &sa, 0);

    if (!ok)
        hPipRd = 0, hPipWr = 0;
    else {
        STARTUPINFO si = {sizeof(si)};
        si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;
        si.hStdOutput = si.hStdError = hPipWr;
        si.hStdInput = INVALID_HANDLE_VALUE;
        errno = ECHILD;
        SetConsoleOutputCP(cp);
        ok = CreateProcess(0, cmd, 0, 0, TRUE, 0, 0, 0, &si, &pi);
        CloseHandle(hPipWr); // We want ERROR_BROKEN_PIPE when the child is done
    }

    free(cmd);
    DWORD childec = -1;

    if (ok) {
        bool utf8 = CP_UTF8 == cp, okt;
        char iobuf[512];
        DWORD cbRead, cbOfs = 0, cchwb = 0;
        WCHAR wbuf[100], wchbuf[2 + 1]; // A surrogate pair + \0

        for (;;) {
            BOOL okr = ReadFile(hPipRd, iobuf + cbOfs, sizeof(iobuf) - cbOfs, &cbRead, 0);
            cbRead += cbOfs, cbOfs = 0;
            unsigned char cbTrail, cch;

            for (DWORD i = 0; i < cbRead;) {
                cch = 0;

                if (utf8) {
                    okt = UTF8_GetTrailCount(iobuf[i], cbTrail);

                    if (!okt) { // Not UTF-8? Switching to a MBCS CP
switchcp:
                        cp = orgwinconoutcp, mbtwcf = 0, utf8 = false;
                        SetConsoleOutputCP(cp = (CP_UTF8 == cp ? CP_ACP : cp));
                        continue;
                    }

                    if (!cbTrail) cch++, wchbuf[0] = iobuf[i]; // ASCII
                } else {
                    cbTrail = !!IsDBCSLeadByteEx(cp, iobuf[i]);
                }

                if (i + cbTrail >= cbRead) { // Read more first?
                    memmove(iobuf, iobuf + i, cbOfs = cbRead - i);

                    if (okr) break;
                    else i = 0;
                }

                if (!cch) {
                    cch = MultiByteToWideChar(cp, mbtwcf, &iobuf[i], 1 + cbTrail, wchbuf, _countof(wchbuf) - 1);

                    if (!cch) {
                        if (utf8) goto switchcp;

                        cch++, wchbuf[0] = UNICODE_REPLACEMENT_CHARACTER;
                    }
                }

                i += 1 + cbTrail;

                if (0xfeff == wchbuf[0] && 1 == cch) cch = 0; // MakeNsisW is not a fan of the BOM, eat it.

                if (!cch) continue;

                wbuf[cchwb++] = wchbuf[0];

                if (--cch) wbuf[cchwb++] = wchbuf[1];

                const bool fullbuf = cchwb + cch >= _countof(wbuf) - 1; // cch is 1 for surrogate pairs

                if (!okr || fullbuf || L'\n' == wchbuf[0]) { // Stop on \n so \r\n conversion has enough context (...\r\n vs ...\n)
#ifdef MAKENSIS
                    extern WINSIO_OSDATA g_osdata_stdout;
                    WinStdIO_OStreamWrite(g_osdata_stdout, wbuf, cchwb); // Faster than _ftprintf
#else
                    wbuf[cchwb] = L'\0';
                    _ftprintf(g_output, _T("%") NPRIs, wbuf);
#endif
                    cchwb = 0;
                }
            }

            if (!okr) break;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &childec);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    SetConsoleCP(orgwinconcp), SetConsoleOutputCP(orgwinconoutcp);
    CloseHandle(hPipRd);
    return childec;
}
#else
int RunChildProcessRedirected(LPCSTR cmd)
{
    STARTUPINFO si = { sizeof(STARTUPINFO), };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        return GetLastError();

    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &si.cb);
    CloseHandle(pi.hThread), CloseHandle(pi.hProcess);
    return (int) si.cb;
}
#endif //~ _UNICODE
#endif //~ _WIN32
} // namespace cactus