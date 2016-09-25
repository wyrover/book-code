#include <ntoskrnl.h>
#include <rtlp.h>

#define NDEBUG
#include <debug.h>

size_t __cdecl strlen(const char *str)
{
    const char *s;

    if (str == 0)
        return 0;

    for (s = str; *s; ++s);

    return s - str;
}

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
void *  __cdecl memcpy(void *to, const void *from, size_t count)
{
    register char *f = (char *)from;
    register char *t = (char *)to;
    register int i = count;

    while (i-- > 0)
        *t++ = *f++;

    return to;
}

void *  __cdecl memset(void *src, int val, size_t count)
{
    char *char_src = (char *)src;

    while (count > 0) {
        *char_src = val;
        char_src++;
        count--;
    }

    return src;
}

void *  __cdecl memmove(void *dest, const void *src, size_t count)
{
    char *char_dest = (char *)dest;
    char *char_src = (char *)src;

    if ((char_dest <= char_src) || (char_dest >= (char_src + count))) {
        /*  non-overlapping buffers */
        while (count > 0) {
            *char_dest = *char_src;
            char_dest++;
            char_src++;
            count--;
        }
    } else {
        /* overlaping buffers */
        char_dest = (char *)dest + count - 1;
        char_src = (char *)src + count - 1;

        while (count > 0) {
            *char_dest = *char_src;
            char_dest--;
            char_src--;
            count--;
        }
    }

    return dest;
}

char* strcpy(char *to, const char *from)
{
    char *save = to;

    for (; (*to = *from); ++from, ++to);

    return save;
}

int
_stricmp(const char *s1, const char *s2)
{
    while (toupper(*s1) == toupper(*s2)) {
        if (*s1 == 0)
            return 0;

        s1++;
        s2++;
    }

    return toupper(*(unsigned const char *)s1) - toupper(*(unsigned const char *)(s2));
}

int
_strcmpi(const char *s1, const char *s2)
{
    return _stricmp(s1, s2);
}

#undef strstr
_CRTIMP char *strstr(const char *s, const char *find)
{
    char c, sc;
    size_t len;

    if ((c = *find++) != 0) {
        len = strlen(find);

        do {
            do {
                if ((sc = *s++) == 0)
                    return 0;
            } while (sc != c);
        } while (strncmp(s, find, len) != 0);

        s--;
    }

    return (char *)s;
}

char *
strcat(char *s, const char *append)
{
    char *save = s;

    for (; *s; ++s);

    while ((*s++ = *append++));

    return save;
}



int
strncmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;

    do {
        if (*s1 != *s2++)
            return *(unsigned const char *)s1 - *(unsigned const char *)--s2;

        if (*s1++ == 0)
            break;
    } while (--n != 0);

    return 0;
}

int _strnicmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return 0;

    do {
        if (toupper(*s1) != toupper(*s2++))
            return toupper(*(unsigned const char *)s1) - toupper(*(unsigned const char *)--s2);

        if (*s1++ == 0)
            break;
    } while (--n != 0);

    return 0;
}

char *strchr(const char *s, int c)
{
    char cc = c;

    while (*s) {
        if (*s == cc)
            return (char *)s;

        s++;
    }

    if (cc == 0)
        return (char *)s;

    return 0;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2) {
        if (*s1 == 0)
            return 0;

        s1++;
        s2++;
    }

    return *(unsigned const char *)s1 - *(unsigned const char *)(s2);
}

long
strtol(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    unsigned long acc;
    int c;
    unsigned long cutoff;
    int neg = 0, any, cutlim;

    /*
     * Skip white space and pick up leading +/- sign if any.
     * If base is 0, allow 0x for hex and 0 for octal, else
     * assume decimal; if base is already 16, allow 0x.
     */
    do {
        c = *s++;
    } while (isspace(c));

    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+')
        c = *s++;

    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }

    if (base == 0)
        base = c == '0' ? 8 : 10;

    /*
     * Compute the cutoff value between legal numbers and illegal
     * numbers.  That is the largest legal value, divided by the
     * base.  An input number that is greater than this value, if
     * followed by a legal input character, is too big.  One that
     * is equal to this value may be valid or not; the limit
     * between valid and invalid numbers is then based on the last
     * digit.  For instance, if the range for longs is
     * [-2147483648..2147483647] and the input base is 10,
     * cutoff will be set to 214748364 and cutlim to either
     * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
     * a value > 214748364, or equal but the next digit is > 7 (or 8),
     * the number is too big, and we will return a range error.
     *
     * Set any if any `digits' consumed; make it negative to indicate
     * overflow.
     */
    cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
    cutlim = cutoff % (unsigned long)base;
    cutoff /= (unsigned long)base;

    for (acc = 0, any = 0;; c = *s++) {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;

        if (c >= base)
            break;

        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }

    if (any < 0) {
        acc = neg ? LONG_MIN : LONG_MAX;
    } else if (neg)
        acc = -acc;

    if (endptr != 0)
        *endptr = any ? (char *)s - 1 : (char *)nptr;

    return acc;
}

long
atol(const char *str)
{
    return strtol(str, 0, 10);
}


size_t wcslen(const wchar_t * s)
{
    const wchar_t *save;

    if (s == 0)
        return 0;

    for (save = s; *save; ++save);

    return save - s;
}

#undef toupper
int toupper(int c)
{
    if (_isctype(c, _LOWER))
        return (c + ('A' - 'a'));

    return (c);
}

#undef isalpha
int isalpha2(int c)
{
    return _isctype(c, _ALPHA);
}

#undef isdigit
int isdigit(int c)
{
    return _isctype(c, _DIGIT);
}

#undef isspace
int isspace(int c)
{
    return _isctype(c, _SPACE);
}

#undef tolower
int tolower(int c)
{
    if (_isctype(c, _UPPER))
        return (c - ('A' - 'a'));

    return (c);
}

unsigned short _ctype[] = {
    0,          /* <EOF>, 0xFFFF */
    _CONTROL,       /* CTRL+@, 0x00 */
    _CONTROL,       /* CTRL+A, 0x01 */
    _CONTROL,       /* CTRL+B, 0x02 */
    _CONTROL,       /* CTRL+C, 0x03 */
    _CONTROL,       /* CTRL+D, 0x04 */
    _CONTROL,       /* CTRL+E, 0x05 */
    _CONTROL,       /* CTRL+F, 0x06 */
    _CONTROL,       /* CTRL+G, 0x07 */
    _CONTROL,       /* CTRL+H, 0x08 */
    _CONTROL | _SPACE,  /* CTRL+I, 0x09 */
    _CONTROL | _SPACE,  /* CTRL+J, 0x0a */
    _CONTROL | _SPACE,  /* CTRL+K, 0x0b */
    _CONTROL | _SPACE,  /* CTRL+L, 0x0c */
    _CONTROL | _SPACE,  /* CTRL+M, 0x0d */
    _CONTROL,       /* CTRL+N, 0x0e */
    _CONTROL,       /* CTRL+O, 0x0f */
    _CONTROL,       /* CTRL+P, 0x10 */
    _CONTROL,       /* CTRL+Q, 0x11 */
    _CONTROL,       /* CTRL+R, 0x12 */
    _CONTROL,       /* CTRL+S, 0x13 */
    _CONTROL,       /* CTRL+T, 0x14 */
    _CONTROL,       /* CTRL+U, 0x15 */
    _CONTROL,       /* CTRL+V, 0x16 */
    _CONTROL,       /* CTRL+W, 0x17 */
    _CONTROL,       /* CTRL+X, 0x18 */
    _CONTROL,       /* CTRL+Y, 0x19 */
    _CONTROL,       /* CTRL+Z, 0x1a */
    _CONTROL,       /* CTRL+[, 0x1b */
    _CONTROL,       /* CTRL+\, 0x1c */
    _CONTROL,       /* CTRL+], 0x1d */
    _CONTROL,       /* CTRL+^, 0x1e */
    _CONTROL,       /* CTRL+_, 0x1f */
    _SPACE | _BLANK,    /* ` ', 0x20 */
    _PUNCT,         /* `!', 0x21 */
    _PUNCT,         /* 0x22 */
    _PUNCT,         /* `#', 0x23 */
    _PUNCT,         /* `$', 0x24 */
    _PUNCT,         /* `%', 0x25 */
    _PUNCT,         /* `&', 0x26 */
    _PUNCT,         /* 0x27 */
    _PUNCT,         /* `(', 0x28 */
    _PUNCT,         /* `)', 0x29 */
    _PUNCT,         /* `*', 0x2a */
    _PUNCT,         /* `+', 0x2b */
    _PUNCT,         /* `,', 0x2c */
    _PUNCT,         /* `-', 0x2d */
    _PUNCT,         /* `.', 0x2e */
    _PUNCT,         /* `/', 0x2f */
    _DIGIT | _HEX,      /* `0', 0x30 */
    _DIGIT | _HEX,      /* `1', 0x31 */
    _DIGIT | _HEX,      /* `2', 0x32 */
    _DIGIT | _HEX,      /* `3', 0x33 */
    _DIGIT | _HEX,      /* `4', 0x34 */
    _DIGIT | _HEX,      /* `5', 0x35 */
    _DIGIT | _HEX,      /* `6', 0x36 */
    _DIGIT | _HEX,      /* `7', 0x37 */
    _DIGIT | _HEX,      /* `8', 0x38 */
    _DIGIT | _HEX,      /* `9', 0x39 */
    _PUNCT,         /* `:', 0x3a */
    _PUNCT,         /* `;', 0x3b */
    _PUNCT,         /* `<', 0x3c */
    _PUNCT,         /* `=', 0x3d */
    _PUNCT,         /* `>', 0x3e */
    _PUNCT,         /* `?', 0x3f */
    _PUNCT,         /* `@', 0x40 */
    _UPPER | _HEX,      /* `A', 0x41 */
    _UPPER | _HEX,      /* `B', 0x42 */
    _UPPER | _HEX,      /* `C', 0x43 */
    _UPPER | _HEX,      /* `D', 0x44 */
    _UPPER | _HEX,      /* `E', 0x45 */
    _UPPER | _HEX,      /* `F', 0x46 */
    _UPPER,         /* `G', 0x47 */
    _UPPER,         /* `H', 0x48 */
    _UPPER,         /* `I', 0x49 */
    _UPPER,         /* `J', 0x4a */
    _UPPER,         /* `K', 0x4b */
    _UPPER,         /* `L', 0x4c */
    _UPPER,         /* `M', 0x4d */
    _UPPER,         /* `N', 0x4e */
    _UPPER,         /* `O', 0x4f */
    _UPPER,         /* `P', 0x50 */
    _UPPER,         /* `Q', 0x51 */
    _UPPER,         /* `R', 0x52 */
    _UPPER,         /* `S', 0x53 */
    _UPPER,         /* `T', 0x54 */
    _UPPER,         /* `U', 0x55 */
    _UPPER,         /* `V', 0x56 */
    _UPPER,         /* `W', 0x57 */
    _UPPER,         /* `X', 0x58 */
    _UPPER,         /* `Y', 0x59 */
    _UPPER,         /* `Z', 0x5a */
    _PUNCT,         /* `[', 0x5b */
    _PUNCT,         /* 0x5c */
    _PUNCT,         /* `]', 0x5d */
    _PUNCT,         /* `^', 0x5e */
    _PUNCT,         /* `_', 0x5f */
    _PUNCT,         /* 0x60 */
    _LOWER | _HEX,      /* `a', 0x61 */
    _LOWER | _HEX,      /* `b', 0x62 */
    _LOWER | _HEX,      /* `c', 0x63 */
    _LOWER | _HEX,      /* `d', 0x64 */
    _LOWER | _HEX,      /* `e', 0x65 */
    _LOWER | _HEX,      /* `f', 0x66 */
    _LOWER,         /* `g', 0x67 */
    _LOWER,         /* `h', 0x68 */
    _LOWER,         /* `i', 0x69 */
    _LOWER,         /* `j', 0x6a */
    _LOWER,         /* `k', 0x6b */
    _LOWER,         /* `l', 0x6c */
    _LOWER,         /* `m', 0x6d */
    _LOWER,         /* `n', 0x6e */
    _LOWER,         /* `o', 0x6f */
    _LOWER,         /* `p', 0x70 */
    _LOWER,         /* `q', 0x71 */
    _LOWER,         /* `r', 0x72 */
    _LOWER,         /* `s', 0x73 */
    _LOWER,         /* `t', 0x74 */
    _LOWER,         /* `u', 0x75 */
    _LOWER,         /* `v', 0x76 */
    _LOWER,         /* `w', 0x77 */
    _LOWER,         /* `x', 0x78 */
    _LOWER,         /* `y', 0x79 */
    _LOWER,         /* `z', 0x7a */
    _PUNCT,         /* `{', 0x7b */
    _PUNCT,         /* `|', 0x7c */
    _PUNCT,         /* `}', 0x7d */
    _PUNCT,         /* `~', 0x7e */
    _CONTROL,       /* 0x7f */
    0,          /* 0x80 */
    0,          /* 0x81 */
    0,          /* 0x82 */
    0,          /* 0x83 */
    0,          /* 0x84 */
    0,          /* 0x85 */
    0,          /* 0x86 */
    0,          /* 0x87 */
    0,          /* 0x88 */
    0,          /* 0x89 */
    0,          /* 0x8a */
    0,          /* 0x8b */
    0,          /* 0x8c */
    0,          /* 0x8d */
    0,          /* 0x8e */
    0,          /* 0x8f */
    0,          /* 0x90 */
    0,          /* 0x91 */
    0,          /* 0x92 */
    0,          /* 0x93 */
    0,          /* 0x94 */
    0,          /* 0x95 */
    0,          /* 0x96 */
    0,          /* 0x97 */
    0,          /* 0x98 */
    0,          /* 0x99 */
    0,          /* 0x9a */
    0,          /* 0x9b */
    0,          /* 0x9c */
    0,          /* 0x9d */
    0,          /* 0x9e */
    0,          /* 0x9f */
    0,          /* 0xa0 */
    0,          /* 0xa1 */
    0,          /* 0xa2 */
    0,          /* 0xa3 */
    0,          /* 0xa4 */
    0,          /* 0xa5 */
    0,          /* 0xa6 */
    0,          /* 0xa7 */
    0,          /* 0xa8 */
    0,          /* 0xa9 */
    0,          /* 0xaa */
    0,          /* 0xab */
    0,          /* 0xac */
    0,          /* 0xad */
    0,          /* 0xae */
    0,          /* 0xaf */
    0,          /* 0xb0 */
    0,          /* 0xb1 */
    0,          /* 0xb2 */
    0,          /* 0xb3 */
    0,          /* 0xb4 */
    0,          /* 0xb5 */
    0,          /* 0xb6 */
    0,          /* 0xb7 */
    0,          /* 0xb8 */
    0,          /* 0xb9 */
    0,          /* 0xba */
    0,          /* 0xbb */
    0,          /* 0xbc */
    0,          /* 0xbd */
    0,          /* 0xbe */
    0,          /* 0xbf */
    0,          /* 0xc0 */
    0,          /* 0xc1 */
    0,          /* 0xc2 */
    0,          /* 0xc3 */
    0,          /* 0xc4 */
    0,          /* 0xc5 */
    0,          /* 0xc6 */
    0,          /* 0xc7 */
    0,          /* 0xc8 */
    0,          /* 0xc9 */
    0,          /* 0xca */
    0,          /* 0xcb */
    0,          /* 0xcc */
    0,          /* 0xcd */
    0,          /* 0xce */
    0,          /* 0xcf */
    0,          /* 0xd0 */
    0,          /* 0xd1 */
    0,          /* 0xd2 */
    0,          /* 0xd3 */
    0,          /* 0xd4 */
    0,          /* 0xd5 */
    0,          /* 0xd6 */
    0,          /* 0xd7 */
    0,          /* 0xd8 */
    0,          /* 0xd9 */
    0,          /* 0xda */
    0,          /* 0xdb */
    0,          /* 0xdc */
    0,          /* 0xdd */
    0,          /* 0xde */
    0,          /* 0xdf */
    0,          /* 0xe0 */
    0,          /* 0xe1 */
    0,          /* 0xe2 */
    0,          /* 0xe3 */
    0,          /* 0xe4 */
    0,          /* 0xe5 */
    0,          /* 0xe6 */
    0,          /* 0xe7 */
    0,          /* 0xe8 */
    0,          /* 0xe9 */
    0,          /* 0xea */
    0,          /* 0xeb */
    0,          /* 0xec */
    0,          /* 0xed */
    0,          /* 0xee */
    0,          /* 0xef */
    0,          /* 0xf0 */
    0,          /* 0xf1 */
    0,          /* 0xf2 */
    0,          /* 0xf3 */
    0,          /* 0xf4 */
    0,          /* 0xf5 */
    0,          /* 0xf6 */
    0,          /* 0xf7 */
    0,          /* 0xf8 */
    0,          /* 0xf9 */
    0,          /* 0xfa */
    0,          /* 0xfb */
    0,          /* 0xfc */
    0,          /* 0xfd */
    0,          /* 0xfe */
    0           /* 0xff */
};

unsigned short *my_pctype = _ctype + 1;

int
__cdecl
_isctype(
    _In_ int _C,
    _In_ int _Type)
{
    return (my_pctype[(unsigned char)(_C & 0xFF)] & _Type);
}

#undef isupper
int isupper(int c)
{
    return _isctype(c, _UPPER);
}

#define upalpha ('A' - 'a')

// The wctype table is 144 KB, too much for poor freeldr
int iswctype(wint_t wc, wctype_t wctypeFlags)
{
    return _isctype((char)wc, wctypeFlags);
}

/*
 * @implemented
 */
wint_t towupper(wint_t c)
{
    if (iswctype(c, _LOWER))
        return (c + upalpha);

    return (c);
}

#undef wcscmp
int wcscmp(const wchar_t* cs, const wchar_t * ct)
{
    while (*cs == *ct) {
        if (*cs == 0)
            return 0;

        cs++;
        ct++;
    }

    return *cs - *ct;
}

int _wcsnicmp(const wchar_t *cs, const wchar_t *ct, size_t count)
{
    if (count == 0)
        return 0;

    do {
        if (towupper(*cs) != towupper(*ct++))
            return towupper(*cs) - towupper(*--ct);

        if (*cs++ == 0)
            break;
    } while (--count != 0);

    return 0;
}


wchar_t* wcschr(const wchar_t* str, wchar_t ch)
{
    while ((*str) != 0) {
        if ((*str) == ch) {
            return ((wchar_t *)str);
        }

        str++;
    }

    return (NULL);
}

char *_strupr(char *x)
{
    char  *y = x;

    while (*y) {
        *y = toupper(*y);
        y++;
    }

    return x;
}

wchar_t* wcsrchr(const wchar_t* str, wchar_t ch)
{
    wchar_t *sp = (wchar_t *)0;

    while (*str != 0) {
        if (*str == ch)
            sp = (wchar_t *)str;

        str++;
    }

    if (ch == 0)
        sp = (wchar_t *)str;

    return sp;
}

wchar_t * wcscpy(wchar_t * str1, const wchar_t * str2)
{
    wchar_t *save = str1;

    for (; (*str1 = *str2); ++str2, ++str1);

    return save;
}
wchar_t * wcscat(wchar_t * dest, const wchar_t * src)
{
    wchar_t *d = dest;

    for (; *dest != 0; dest++);

    while (*src != 0) {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = 0;
    return d;
}
int _wcsicmp(const wchar_t* cs, const wchar_t * ct)
{
    while (towlower(*cs) == towlower(*ct)) {
        if (*cs == 0)
            return 0;

        cs++;
        ct++;
    }

    return towlower(*cs) - towlower(*ct);
}
int wcsncmp(const wchar_t * cs, const wchar_t * ct, size_t count)
{
    while ((*cs) == (*ct) && count > 0) {
        if (*cs == 0)
            return 0;

        cs++;
        ct++;
        count--;
    }

    return (*cs) - (*ct);
}
/* Write formatted output into S, according to the format
   string FORMAT, writing no more than MAXLEN characters.  */
/* VARARGS3 */
int
_snprintf(char *s, size_t maxlen, const char *format, ...)
{
    va_list arg;
    int done;
    va_start(arg, format);
    done = _vsnprintf(s, maxlen, format, arg);
    va_end(arg);
    return done;
}

wchar_t * wcsncpy(wchar_t * dest, const wchar_t *src, size_t count)
{
    int i;

    for (i = 0; i < count; i++) {
        dest[i] = src[i];

        if (src[i] == 0) {
            return (dest);
        }
    }

    dest[i] = 0;
    return (dest);
}

#undef iswdigit
int iswdigit(wint_t c)
{
    return iswctype(c, _DIGIT);
}

#undef isalpha
int isalpha(int c)
{
    return _isctype(c, _ALPHA);
}

#undef iswalpha
int iswalpha(wint_t c)
{
    return iswctype(c, _ALPHA);
}

#undef towlower
wchar_t towlower(wchar_t c)
{
    if (iswctype(c, _UPPER))
        return (c - (L'A' - L'a'));

    return (c);
}

char *
strncpy(char *dst, const char *src, size_t n)
{
    if (n != 0) {
        char *d = dst;
        const char *s = src;

        do {
            if ((*d++ = *s++) == 0) {
                while (--n != 0)
                    *d++ = 0;

                break;
            }
        } while (--n != 0);
    }

    return dst;
}

char *
strrchr(const char *s, int c)
{
    char cc = c;
    const char *sp = (char *)0;

    while (*s) {
        if (*s == cc)
            sp = s;

        s++;
    }

    if (cc == 0)
        sp = s;

    return (char *)sp;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    if (n != 0) {
        const unsigned char *p1 = s1, *p2 = s2;

        do {
            if (*p1++ != *p2++)
                return (*--p1 - *--p2);
        } while (--n != 0);
    }

    return 0;
}

wchar_t *wcsstr(const wchar_t *s, const wchar_t *b)
{
    wchar_t *x;
    wchar_t *y;
    wchar_t *c;
    x = (wchar_t *)s;

    while (*x) {
        if (*x == *b) {
            y = x;
            c = (wchar_t *)b;

            while (*y && *c && *y == *c) {
                c++;
                y++;
            }

            if (!*c)
                return x;
        }

        x++;
    }

    return NULL;
}