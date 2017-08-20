#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
typedef const BYTE *LPCBYTE;
#endif

inline void *heap_xalloc(size_t size)
{
    void *buf = HeapAlloc(GetProcessHeap(), 0, size);
    if (!buf)
    {
        //ERR("Out of memory!\n");
        exit(1);
    }
    return buf;
}

inline void *heap_xrealloc(void *buf, size_t size)
{
    void *new_buf;

    if (buf)
        new_buf = HeapReAlloc(GetProcessHeap(), 0, buf, size);
    else
        new_buf = HeapAlloc(GetProcessHeap(), 0, size);

    if (!new_buf)
    {
        //ERR("Out of memory!\n");
        exit(1);
    }

    return new_buf;
}

inline BOOL heap_free(void *buf)
{
    return HeapFree(GetProcessHeap(), 0, buf);
}


/* code page info common to SBCS and DBCS */
struct cp_info
{
    unsigned int          codepage;          /* codepage id */
    unsigned int          char_size;         /* char size (1 or 2 bytes) */
    WCHAR                 def_char;          /* default char value (can be double-byte) */
    WCHAR                 def_unicode_char;  /* default Unicode char value */
    const char           *name;              /* code page name */
};

struct sbcs_table
{
    struct cp_info        info;
    const WCHAR          *cp2uni;            /* code page -> Unicode map */
    const WCHAR          *cp2uni_glyphs;     /* code page -> Unicode map with glyph chars */
    const unsigned char  *uni2cp_low;        /* Unicode -> code page map */
    const unsigned short *uni2cp_high;
};

struct dbcs_table
{
    struct cp_info        info;
    const WCHAR          *cp2uni;            /* code page -> Unicode map */
    const unsigned char  *cp2uni_leadbytes;
    const unsigned short *uni2cp_low;        /* Unicode -> code page map */
    const unsigned short *uni2cp_high;
    unsigned char         lead_bytes[12];    /* lead bytes ranges */
};

union cptable
{
    struct cp_info    info;
    struct sbcs_table sbcs;
    struct dbcs_table dbcs;
};

extern const union cptable *wine_cp_get_table( unsigned int codepage );
extern const union cptable *wine_cp_enum_table( unsigned int index );

extern int wine_cp_mbstowcs( const union cptable *table, int flags,
                            const char *src, int srclen,
                            WCHAR *dst, int dstlen );
extern int wine_cp_wcstombs( const union cptable *table, int flags,
                            const WCHAR *src, int srclen,
                            char *dst, int dstlen, const char *defchar, int *used );
extern int wine_cpsymbol_mbstowcs( const char *src, int srclen, WCHAR *dst, int dstlen );
extern int wine_cpsymbol_wcstombs( const WCHAR *src, int srclen, char *dst, int dstlen );
extern int wine_utf8_mbstowcs( int flags, const char *src, int srclen, WCHAR *dst, int dstlen );
extern int wine_utf8_wcstombs( int flags, const WCHAR *src, int srclen, char *dst, int dstlen );

extern int wine_compare_string( int flags, const WCHAR *str1, int len1, const WCHAR *str2, int len2 );
extern int wine_get_sortkey( int flags, const WCHAR *src, int srclen, char *dst, int dstlen );
extern int wine_fold_string( int flags, const WCHAR *src, int srclen , WCHAR *dst, int dstlen );

extern int strcmpiW( const WCHAR *str1, const WCHAR *str2 );
extern int strncmpiW( const WCHAR *str1, const WCHAR *str2, int n );
extern int memicmpW( const WCHAR *str1, const WCHAR *str2, int n );
extern WCHAR *strstrW( const WCHAR *str, const WCHAR *sub );
extern long int strtolW( const WCHAR *nptr, WCHAR **endptr, int base );
extern unsigned long int strtoulW( const WCHAR *nptr, WCHAR **endptr, int base );
extern int sprintfW( WCHAR *str, const WCHAR *format, ... );
extern int snprintfW( WCHAR *str, size_t len, const WCHAR *format, ... );
extern int vsprintfW( WCHAR *str, const WCHAR *format, va_list valist );
extern int vsnprintfW( WCHAR *str, size_t len, const WCHAR *format, va_list valist );




inline int wine_is_dbcs_leadbyte( const union cptable *table, unsigned char ch )
{
    return (table->info.char_size == 2) && (table->dbcs.cp2uni_leadbytes[ch]);
}

inline WCHAR tolowerW( WCHAR ch )
{
    extern const WCHAR wine_casemap_lower[];
    return ch + wine_casemap_lower[wine_casemap_lower[ch >> 8] + (ch & 0xff)];
}

inline WCHAR toupperW( WCHAR ch )
{
    extern const WCHAR wine_casemap_upper[];
    return ch + wine_casemap_upper[wine_casemap_upper[ch >> 8] + (ch & 0xff)];
}

/* the character type contains the C1_* flags in the low 12 bits */
/* and the C2_* type in the high 4 bits */
inline unsigned short get_char_typeW( WCHAR ch )
{
    extern const unsigned short wine_wctype_table[];
    return wine_wctype_table[wine_wctype_table[ch >> 8] + (ch & 0xff)];
}

inline int iscntrlW( WCHAR wc )
{
    return get_char_typeW(wc) & C1_CNTRL;
}

inline int ispunctW( WCHAR wc )
{
    return get_char_typeW(wc) & C1_PUNCT;
}

inline int isspaceW( WCHAR wc )
{
    return get_char_typeW(wc) & C1_SPACE;
}

inline int isdigitW( WCHAR wc )
{
    return get_char_typeW(wc) & C1_DIGIT;
}

inline int isxdigitW( WCHAR wc )
{
    return get_char_typeW(wc) & C1_XDIGIT;
}

inline int islowerW( WCHAR wc )
{
    return get_char_typeW(wc) & C1_LOWER;
}

inline int isupperW( WCHAR wc )
{
    return get_char_typeW(wc) & C1_UPPER;
}

inline int isalnumW( WCHAR wc )
{
    return get_char_typeW(wc) & (C1_ALPHA|C1_DIGIT|C1_LOWER|C1_UPPER);
}

inline int isalphaW( WCHAR wc )
{
    return get_char_typeW(wc) & (C1_ALPHA|C1_LOWER|C1_UPPER);
}

inline int isgraphW( WCHAR wc )
{
    return get_char_typeW(wc) & (C1_ALPHA|C1_PUNCT|C1_DIGIT|C1_LOWER|C1_UPPER);
}

inline int isprintW( WCHAR wc )
{
    return get_char_typeW(wc) & (C1_ALPHA|C1_BLANK|C1_PUNCT|C1_DIGIT|C1_LOWER|C1_UPPER);
}

/* some useful string manipulation routines */

inline unsigned int strlenW( const WCHAR *str )
{
    const WCHAR *s = str;
    while (*s) s++;
    return s - str;
}

inline WCHAR *strcpyW( WCHAR *dst, const WCHAR *src )
{
    WCHAR *p = dst;
    while ((*p++ = *src++));
    return dst;
}

/* strncpy doesn't do what you think, don't use it */
#define strncpyW(d,s,n) error do_not_use_strncpyW_use_lstrcpynW_or_memcpy_instead

inline int strcmpW( const WCHAR *str1, const WCHAR *str2 )
{
    while (*str1 && (*str1 == *str2)) { str1++; str2++; }
    return *str1 - *str2;
}

inline int strncmpW( const WCHAR *str1, const WCHAR *str2, int n )
{
    if (n <= 0) return 0;
    while ((--n > 0) && *str1 && (*str1 == *str2)) { str1++; str2++; }
    return *str1 - *str2;
}

inline WCHAR *strcatW( WCHAR *dst, const WCHAR *src )
{
    strcpyW( dst + strlenW(dst), src );
    return dst;
}

inline WCHAR *strchrW( const WCHAR *str, WCHAR ch )
{
    do { if (*str == ch) return (WCHAR *)(ULONG_PTR)str; } while (*str++);
    return NULL;
}

inline WCHAR *strrchrW( const WCHAR *str, WCHAR ch )
{
    WCHAR *ret = NULL;
    do { if (*str == ch) ret = (WCHAR *)(ULONG_PTR)str; } while (*str++);
    return ret;
}

inline WCHAR *strpbrkW( const WCHAR *str, const WCHAR *accept )
{
    for ( ; *str; str++) if (strchrW( accept, *str )) return (WCHAR *)(ULONG_PTR)str;
    return NULL;
}

inline size_t strspnW( const WCHAR *str, const WCHAR *accept )
{
    const WCHAR *ptr;
    for (ptr = str; *ptr; ptr++) if (!strchrW( accept, *ptr )) break;
    return ptr - str;
}

inline size_t strcspnW( const WCHAR *str, const WCHAR *reject )
{
    const WCHAR *ptr;
    for (ptr = str; *ptr; ptr++) if (strchrW( reject, *ptr )) break;
    return ptr - str;
}

inline WCHAR *strlwrW( WCHAR *str )
{
    WCHAR *ret;
    for (ret = str; *str; str++) *str = tolowerW(*str);
    return ret;
}

inline WCHAR *struprW( WCHAR *str )
{
    WCHAR *ret;
    for (ret = str; *str; str++) *str = toupperW(*str);
    return ret;
}

inline WCHAR *memchrW( const WCHAR *ptr, WCHAR ch, size_t n )
{
    const WCHAR *end;
    for (end = ptr + n; ptr < end; ptr++) if (*ptr == ch) return (WCHAR *)(ULONG_PTR)ptr;
    return NULL;
}

inline WCHAR *memrchrW( const WCHAR *ptr, WCHAR ch, size_t n )
{
    const WCHAR *end;
    WCHAR *ret = NULL;
    for (end = ptr + n; ptr < end; ptr++) if (*ptr == ch) ret = (WCHAR *)(ULONG_PTR)ptr;
    return ret;
}

inline long int atolW( const WCHAR *str )
{
    return strtolW( str, (WCHAR **)0, 10 );
}

inline int atoiW( const WCHAR *str )
{
    return (int)atolW( str );
}


/* hexchar_to_byte from programs/regedit/hexedit.c */
inline BYTE hexchar_to_byte(WCHAR ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    else if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    else
        return -1;
}

void __cdecl output_message(unsigned int id, ...);


struct CAutoCryptContextHandle
{
    HCRYPTPROV m_h;
    CAutoCryptContextHandle(HCRYPTPROV h = NULL) : m_h(h) { };
    ~CAutoCryptContextHandle() { if( m_h != NULL ) ::CryptReleaseContext(m_h, 0UL); };
    operator HCRYPTPROV() const { return m_h; };
};

struct CAutoCryptHashHandle
{
    HCRYPTHASH m_h;
    CAutoCryptHashHandle(HCRYPTHASH h = NULL) : m_h(h) { };
    ~CAutoCryptHashHandle() { if( m_h != NULL ) ::CryptDestroyHash(m_h); };
    operator HCRYPTHASH() const { return m_h; };
};



inline BOOL MD5Hash(LPCSTR data, LPTSTR pstrResult)
{
    CAutoCryptContextHandle m_hCrypt;
    CAutoCryptHashHandle hHash;
    if( !::CryptAcquireContext(&m_hCrypt.m_h, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET) ) 
        return FALSE;

    if( !::CryptCreateHash(m_hCrypt, CALG_MD5, 0, 0, &hHash.m_h) ) return FALSE;
    SIZE_T len = strlen(data);
    if( !::CryptHashData(hHash, (LPCBYTE) data, (DWORD) len, 0) ) return FALSE;
    BYTE bHash[16];
    DWORD dwHashLen = 16;
    if( !::CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0) ) return FALSE;
    for( int i = 0; i < 16; i++ ) ::wsprintf(pstrResult + (i * 2), _T("%02x"), bHash[i]);
    return TRUE;
}


namespace winsparkle
{

/// C++ wrapper for win32 event
class Event
{
public:
    Event()
    {
        m_handle = CreateEvent
                   (
                       NULL,  // default security attributes
                       FALSE, // = is auto-resetting
                       FALSE, // initially non-signaled
                       NULL   // anonymous
                   );
        /*if (!m_handle)
            throw Win32Exception();*/
    }

    ~Event() { CloseHandle(m_handle); }

    /// Signal the event
    void Signal()
    {
        SetEvent(m_handle);
    }

    /// Wait until the event is signalled (true) or timeout ellapses (false)
    bool WaitUntilSignaled(unsigned timeoutMilliseconds = INFINITE)
    {
        return WaitForSingleObject(m_handle, timeoutMilliseconds) == WAIT_OBJECT_0;
    }

    bool CheckIfSignaled()
    {
        return WaitUntilSignaled(0);
    }

private:
    HANDLE m_handle;
};


/**
C++ wrapper for win32 critical section object.
*/
class CriticalSection
{
public:
    CriticalSection() { InitializeCriticalSection(&m_cs); }
    ~CriticalSection() { DeleteCriticalSection(&m_cs); }

    void Enter() { EnterCriticalSection(&m_cs); }
    void Leave() { LeaveCriticalSection(&m_cs); }

private:
    CRITICAL_SECTION m_cs;
};


/**
Locks a critical section as RIIA. Use this instead of manually calling
CriticalSection::Enter() and CriticalSection::Leave().
*/
class CriticalSectionLocker
{
public:
    CriticalSectionLocker(CriticalSection& cs) : m_cs(cs) { cs.Enter(); }
    ~CriticalSectionLocker() { m_cs.Leave(); }

private:
    CriticalSection& m_cs;
};


/**
    Lightweight thread class.

    It's a base class; derived class must implement at least Run().

    Create the thread on heap, then call Start() on it. If the thread is joinable
    (see IsJoinable()), then you must call JoinAndDelete() on it to destroy it.
    Otherwise, it self-destructs.
 */
class Thread
{
public:
    /**
        Creates the thread.

        Note that you must explicitly call Start() to start it.

        @param name Descriptive name of the thread. This is shown in (Visual C++)
                    debugger and should always be set to something meaningful to
                    help identify WinSparkle threads.
     */
    Thread(const char *name);

    virtual ~Thread();

    /**
        Launch the thread.

        Calls Run() in the new thread's context.

        This method doesn't return until Run() calls SignalReady().

        Throws on error.
     */
    void Start();

    /**
        Wait for the thread to terminate.
     */
    void Join();

    /**
        Signal the thread to terminate and call Join().

        @note The thread must support this and call
              CheckShouldTerminate() frequently.
     */
    void TerminateAndJoin();
    
    /// Check if the thread should terminate and throw TerminateThreadException if so.
    void CheckShouldTerminate();

protected:
    /// Signals Start() that the thread is up and ready.
    void SignalReady();

    /**
        Code to run in the thread's context.

        This method @a must call SignalReady() as soon as it is initialized.
     */
    virtual void Run() = 0;

    /// Is the thread joinable?
    virtual bool IsJoinable() const = 0;

    /// This exception is thrown when the thread was terminated.
    struct TerminateThreadException
    {
    };

private:
    static unsigned __stdcall ThreadEntryPoint(void *data);

protected:
    HANDLE m_handle;
    unsigned m_id;
    Event m_signalEvent, m_terminateEvent;
};

} // namespace winsparkle