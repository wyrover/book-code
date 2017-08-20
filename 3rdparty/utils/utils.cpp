#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define STRING_MISSING_INTEGER  111
#define STRING_MISSING_HEXDATA  112
#define STRING_UNHANDLED_TYPE   113
#define STRING_INVALID_STRING   123






int strcmpiW( const WCHAR *str1, const WCHAR *str2 )
{
    for (;;)
    {
        int ret = tolowerW(*str1) - tolowerW(*str2);
        if (ret || !*str1) return ret;
        str1++;
        str2++;
    }
}

int strncmpiW( const WCHAR *str1, const WCHAR *str2, int n )
{
    int ret = 0;
    for ( ; n > 0; n--, str1++, str2++)
        if ((ret = tolowerW(*str1) - tolowerW(*str2)) || !*str1) break;
    return ret;
}

int memicmpW( const WCHAR *str1, const WCHAR *str2, int n )
{
    int ret = 0;
    for ( ; n > 0; n--, str1++, str2++)
        if ((ret = tolowerW(*str1) - tolowerW(*str2))) break;
    return ret;
}

WCHAR *strstrW( const WCHAR *str, const WCHAR *sub )
{
    while (*str)
    {
        const WCHAR *p1 = str, *p2 = sub;
        while (*p1 && *p2 && *p1 == *p2) { p1++; p2++; }
        if (!*p2) return (WCHAR *)str;
        str++;
    }
    return NULL;
}

/* strtolW and strtoulW implementation based on the GNU C library code */
/* Copyright (C) 1991,92,94,95,96,97,98,99,2000,2001 Free Software Foundation, Inc. */

long int strtolW( const WCHAR *nptr, WCHAR **endptr, int base )
{
  int negative;
  register unsigned long int cutoff;
  register unsigned int cutlim;
  register unsigned long int i;
  register const WCHAR *s;
  register WCHAR c;
  const WCHAR *save, *end;
  int overflow;

  if (base < 0 || base == 1 || base > 36) return 0;

  save = s = nptr;

  /* Skip white space.  */
  while (isspaceW (*s))
    ++s;
  if (!*s) goto noconv;

  /* Check for a sign.  */
  negative = 0;
  if (*s == '-')
    {
      negative = 1;
      ++s;
    }
  else if (*s == '+')
    ++s;

  /* Recognize number prefix and if BASE is zero, figure it out ourselves.  */
  if (*s == '0')
    {
      if ((base == 0 || base == 16) && toupperW(s[1]) == 'X')
	{
	  s += 2;
	  base = 16;
	}
      else if (base == 0)
	base = 8;
    }
  else if (base == 0)
    base = 10;

  /* Save the pointer so we can check later if anything happened.  */
  save = s;
  end = NULL;

  cutoff = ULONG_MAX / (unsigned long int) base;
  cutlim = ULONG_MAX % (unsigned long int) base;

  overflow = 0;
  i = 0;
  c = *s;
  for (;c != '\0'; c = *++s)
  {
      if (s == end)
          break;
      if (c >= '0' && c <= '9')
          c -= '0';
      else if (isalphaW (c))
          c = toupperW (c) - 'A' + 10;
      else
          break;
      if ((int) c >= base)
          break;
      /* Check for overflow.  */
      if (i > cutoff || (i == cutoff && c > cutlim))
          overflow = 1;
      else
      {
          i *= (unsigned long int) base;
          i += c;
      }
  }

  /* Check if anything actually happened.  */
  if (s == save)
    goto noconv;

  /* Store in ENDPTR the address of one character
     past the last character we converted.  */
  if (endptr != NULL)
    *endptr = (WCHAR *)s;

  /* Check for a value that is within the range of
     `unsigned LONG int', but outside the range of `LONG int'.  */
  if (overflow == 0
      && i > (negative
	      ? -((unsigned long int) (LONG_MIN + 1)) + 1
	      : (unsigned long int) LONG_MAX))
    overflow = 1;

  if (overflow)
    {
      errno = ERANGE;
      return negative ? LONG_MIN : LONG_MAX;
    }

  /* Return the result of the appropriate sign.  */
  return negative ? -i : i;

noconv:
  /* We must handle a special case here: the base is 0 or 16 and the
     first two characters are '0' and 'x', but the rest are not
     hexadecimal digits.  This is no error case.  We return 0 and
     ENDPTR points to the `x`.  */
  if (endptr != NULL)
    {
      if (save - nptr >= 2 && toupperW (save[-1]) == 'X'
	  && save[-2] == '0')
	*endptr = (WCHAR *)&save[-1];
      else
	/*  There was no number to convert.  */
	*endptr = (WCHAR *)nptr;
    }

  return 0L;
}


unsigned long int strtoulW( const WCHAR *nptr, WCHAR **endptr, int base )
{
  int negative;
  register unsigned long int cutoff;
  register unsigned int cutlim;
  register unsigned long int i;
  register const WCHAR *s;
  register WCHAR c;
  const WCHAR *save, *end;
  int overflow;

  if (base < 0 || base == 1 || base > 36) return 0;

  save = s = nptr;

  /* Skip white space.  */
  while (isspaceW (*s))
    ++s;
  if (!*s) goto noconv;

  /* Check for a sign.  */
  negative = 0;
  if (*s == '-')
    {
      negative = 1;
      ++s;
    }
  else if (*s == '+')
    ++s;

  /* Recognize number prefix and if BASE is zero, figure it out ourselves.  */
  if (*s == '0')
    {
      if ((base == 0 || base == 16) && toupperW(s[1]) == 'X')
	{
	  s += 2;
	  base = 16;
	}
      else if (base == 0)
	base = 8;
    }
  else if (base == 0)
    base = 10;

  /* Save the pointer so we can check later if anything happened.  */
  save = s;
  end = NULL;

  cutoff = ULONG_MAX / (unsigned long int) base;
  cutlim = ULONG_MAX % (unsigned long int) base;

  overflow = 0;
  i = 0;
  c = *s;
  for (;c != '\0'; c = *++s)
  {
      if (s == end)
          break;
      if (c >= '0' && c <= '9')
          c -= '0';
      else if (isalphaW (c))
          c = toupperW (c) - 'A' + 10;
      else
          break;
      if ((int) c >= base)
          break;
      /* Check for overflow.  */
      if (i > cutoff || (i == cutoff && c > cutlim))
          overflow = 1;
      else
      {
          i *= (unsigned long int) base;
          i += c;
      }
  }

  /* Check if anything actually happened.  */
  if (s == save)
    goto noconv;

  /* Store in ENDPTR the address of one character
     past the last character we converted.  */
  if (endptr != NULL)
    *endptr = (WCHAR *)s;

  if (overflow)
    {
      errno = ERANGE;
      return ULONG_MAX;
    }

  /* Return the result of the appropriate sign.  */
  return negative ? -i : i;

noconv:
  /* We must handle a special case here: the base is 0 or 16 and the
     first two characters are '0' and 'x', but the rest are not
     hexadecimal digits.  This is no error case.  We return 0 and
     ENDPTR points to the `x`.  */
  if (endptr != NULL)
    {
      if (save - nptr >= 2 && toupperW (save[-1]) == 'X'
	  && save[-2] == '0')
	*endptr = (WCHAR *)&save[-1];
      else
	/*  There was no number to convert.  */
	*endptr = (WCHAR *)nptr;
    }

  return 0L;
}


/* format a WCHAR string according to a printf format; helper for vsnprintfW */
static size_t format_string( WCHAR *buffer, size_t len, const char *format, const WCHAR *str, int str_len )
{
    size_t count = 0;
    int i, left_align = 0, width = 0, max = 0;

    assert( *format == '%' );
    format++; /* skip '%' */

    while (*format == '0' || *format == '+' || *format == '-' || *format == ' ' || *format == '#')
    {
        if (*format == '-') left_align = 1;
        format++;
    }

    while (isdigit(*format)) width = width * 10 + *format++ - '0';

    if (str_len == -1) str_len = strlenW( str );
    if (*format == '.')
    {
        format++;
        while (isdigit(*format)) max = max * 10 + *format++ - '0';
        if (max > str_len) max = str_len;
    }
    else max = str_len;

    if (*format == 'h' || *format == 'l') format++;

    assert( *format == 's' );

    if (!left_align && width > max)
    {
        for (i = 0; i < width - max; i++)
        {
            if (count++ < len)
                *buffer++ = ' ';
        }
    }

    if (count < len)
        memcpy( buffer, str, min( max, len - count ) * sizeof(WCHAR) );
    count += max;
    buffer += max;

    if (left_align && width > max)
    {
        for (i = 0; i < width - max; i++)
        {
            if (count++ < len)
                *buffer++ = ' ';
        }
    }
    return count;
}

int vsnprintfW(WCHAR *str, size_t len, const WCHAR *format, va_list valist)
{
    unsigned int written = 0;
    const WCHAR *iter = format;
    char bufa[512], fmtbufa[64], *fmta;

    while (*iter)
    {
        while (*iter && *iter != '%')
        {
            if (written++ < len)
                *str++ = *iter;
            iter++;
        }
        if (*iter == '%')
        {
            if (iter[1] == '%')
            {
                if (written++ < len)
                    *str++ = '%'; /* "%%"->'%' */
                iter += 2;
                continue;
            }

            fmta = fmtbufa;
            *fmta++ = *iter++;
            while (*iter == '0' ||
                   *iter == '+' ||
                   *iter == '-' ||
                   *iter == ' ' ||
                   *iter == '*' ||
                   *iter == '#')
            {
                if (*iter == '*')
                {
                    char *buffiter = bufa;
                    int fieldlen = va_arg(valist, int);
                    sprintf(buffiter, "%d", fieldlen);
                    while (*buffiter)
                        *fmta++ = *buffiter++;
                }
                else
                    *fmta++ = *iter;
                iter++;
            }

            while (isdigit(*iter))
                *fmta++ = *iter++;

            if (*iter == '.')
            {
                *fmta++ = *iter++;
                if (*iter == '*')
                {
                    char *buffiter = bufa;
                    int fieldlen = va_arg(valist, int);
                    sprintf(buffiter, "%d", fieldlen);
                    while (*buffiter)
                        *fmta++ = *buffiter++;
                    iter++;
                }
                else
                    while (isdigit(*iter))
                        *fmta++ = *iter++;
            }
            if (*iter == 'h' || *iter == 'l')
                *fmta++ = *iter++;

            switch (*iter)
            {
            case 's':
            {
                static const WCHAR none[] = { '(','n','u','l','l',')',0 };
                const WCHAR *wstr = va_arg(valist, const WCHAR *);
                size_t remaining = written < len ? len - written : 0;
                size_t count;

                *fmta++ = 's';
                *fmta = 0;
                count = format_string( str, remaining, fmtbufa, wstr ? wstr : none, -1 );
                str += min( count, remaining );
                written += count;
                iter++;
                break;
            }

            case 'c':
            {
                WCHAR wstr;
                size_t remaining = written < len ? len - written : 0;
                size_t count;

                wstr = va_arg(valist, int);
                *fmta++ = 's';
                *fmta = 0;
                count = format_string( str, remaining, fmtbufa, &wstr, 1 );
                str += min( count, remaining );
                written += count;
                iter++;
                break;
            }

            default:
            {
                /* For non wc types, use system sprintf and append to wide char output */
                /* FIXME: for unrecognised types, should ignore % when printing */
                char *bufaiter = bufa;
                if (*iter == 'p')
                    sprintf(bufaiter, "%0*lX", 2 * (int)sizeof(void*),
                            (unsigned long)va_arg(valist, void *));
                else
                {
                    *fmta++ = *iter;
                    *fmta = '\0';
                    if (*iter == 'a' || *iter == 'A' ||
                        *iter == 'e' || *iter == 'E' ||
                        *iter == 'f' || *iter == 'F' || 
                        *iter == 'g' || *iter == 'G')
                        sprintf(bufaiter, fmtbufa, va_arg(valist, double));
                    else
                    {
                        /* FIXME: On 32 bit systems this doesn't handle int 64's. */
                        sprintf(bufaiter, fmtbufa, va_arg(valist, void *));
                    }
                }
                while (*bufaiter)
                {
                    if (written++ < len)
                        *str++ = *bufaiter;
                    bufaiter++;
                }
                iter++;
                break;
            }
            }
        }
    }
    if (len)
    {
        if (written >= len)
            str--;
        *str++ = 0;
    }

    /* FIXME: POSIX [v]snprintf() returns the equivalent of written, not -1, on short buffer. */
    return written < len ? (int)written : -1;
}

int vsprintfW( WCHAR *str, const WCHAR *format, va_list valist )
{
    return vsnprintfW( str, INT_MAX, format, valist );
}

int snprintfW( WCHAR *str, size_t len, const WCHAR *format, ...)
{
    int retval;
    va_list valist;
    va_start(valist, format);
    retval = vsnprintfW(str, len, format, valist);
    va_end(valist);
    return retval;
}

int sprintfW( WCHAR *str, const WCHAR *format, ...)
{
    int retval;
    va_list valist;
    va_start(valist, format);
    retval = vsnprintfW(str, INT_MAX, format, valist);
    va_end(valist);
    return retval;
}














LPWSTR strdupW(LPCWSTR str)
{
    int len;
    WCHAR *buf;

    if (str == NULL)
        return NULL;
    len = wcslen(str);
    buf = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(WCHAR)*(len+1));
    if (buf == NULL)
        return NULL;
    wcscpy(buf, str);
    return buf;
}


BOOL check_multisz(LPCWSTR lpMultiSz, DWORD cbSize)
{
    if (cbSize == 0 || (cbSize == sizeof(WCHAR) && lpMultiSz[0] == 0))
        return TRUE;
    if ((cbSize % sizeof(WCHAR)) != 0 || cbSize < 2*sizeof(WCHAR))
        return FALSE;
    if (lpMultiSz[cbSize/2 - 1] || lpMultiSz[cbSize/2 - 2])
        return FALSE;
    return TRUE;
}


/* =========================================================================
 * Routine copied from cmd.exe md command -
 * This works recursively. so creating dir1\dir2\dir3 will create dir1 and
 * dir2 if they do not already exist.
 * ========================================================================= */
BOOL XCOPY_CreateDirectory(const WCHAR* path)
{
    int len;
    WCHAR *new_path;
    BOOL ret = TRUE;

    new_path = (WCHAR*)HeapAlloc(GetProcessHeap(),0, sizeof(WCHAR) * (lstrlenW(path)+1));
    lstrcpyW(new_path,path);

    while ((len = lstrlenW(new_path)) && new_path[len - 1] == '\\')
        new_path[len - 1] = 0;

    while (!CreateDirectoryW(new_path,NULL))
    {
        WCHAR *slash;
        DWORD last_error = GetLastError();
        if (last_error == ERROR_ALREADY_EXISTS)
            break;

        if (last_error != ERROR_PATH_NOT_FOUND)
        {
            ret = FALSE;
            break;
        }

        if (!(slash = wcsrchr(new_path,'\\')) && ! (slash = wcsrchr(new_path,'/')))
        {
            ret = FALSE;
            break;
        }

        len = slash - new_path;
        new_path[len] = 0;
        if (!XCOPY_CreateDirectory(new_path))
        {
            ret = FALSE;
            break;
        }
        new_path[len] = '\\';
    }
    HeapFree(GetProcessHeap(),0,new_path);
    return ret;
}


/* Allocate and initialize a WSTR containing the queried value */
LPWSTR GetRegValue(HKEY service_key, const WCHAR *value_name)
{
    DWORD type;
    DWORD reg_size;
    DWORD size;
    LONG ret;
    LPWSTR value;   

    ret = RegQueryValueExW(service_key, value_name, NULL, &type, NULL, &reg_size);
    if (ret != ERROR_SUCCESS)
    {
        return NULL;
    }

    /* Add space for potentially missing NULL terminators in initial alloc.
     * The worst case REG_MULTI_SZ requires two NULL terminators. */
    size = reg_size + (2 * sizeof(WCHAR));
    value = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, size);

    ret = RegQueryValueExW(service_key, value_name, NULL, &type,
            (LPBYTE)value, &reg_size);
    if (ret != ERROR_SUCCESS)
    {
        HeapFree(GetProcessHeap(), 0, value);
        return NULL;
    }

    /* Explicitly NULL terminate the result */
    value[size / sizeof(WCHAR) - 1] = '\0';
    value[size / sizeof(WCHAR) - 2] = '\0';

    return value;

}


DWORD load_reg_string(HKEY hKey, LPCWSTR szValue, BOOL bExpand, LPWSTR *output)
{
    DWORD size, type;
    LPWSTR buf = NULL;
    DWORD err;

    *output = NULL;
    if ((err = RegQueryValueExW(hKey, szValue, 0, &type, NULL, &size)) != 0)
    {
        if (err == ERROR_FILE_NOT_FOUND)
            return ERROR_SUCCESS;
        goto failed;
    }
    if (!(type == REG_SZ || (type == REG_EXPAND_SZ && bExpand)))
    {
        err = ERROR_INVALID_DATATYPE;
        goto failed;
    }
    buf = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, size + sizeof(WCHAR));
    if ((err = RegQueryValueExW(hKey, szValue, 0, &type, (LPBYTE)buf, &size)) != 0)
        goto failed;
    buf[size/sizeof(WCHAR)] = 0;

    if (type == REG_EXPAND_SZ)
    {
        LPWSTR str;
        if (!(size = ExpandEnvironmentStringsW(buf, NULL, 0)))
        {
            err = GetLastError();
            goto failed;
        }
        str = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, size * sizeof(WCHAR));
        ExpandEnvironmentStringsW(buf, str, size);
        HeapFree(GetProcessHeap(), 0, buf);
        *output = str;
    }
    else
        *output = buf;
    return ERROR_SUCCESS;

failed:
    
    HeapFree(GetProcessHeap(), 0, buf);
    return err;
}

DWORD load_reg_multisz(HKEY hKey, LPCWSTR szValue, BOOL bAllowSingle, LPWSTR *output)
{
    DWORD size, type;
    LPWSTR buf = NULL;
    DWORD err;

    *output = NULL;
    if ((err = RegQueryValueExW(hKey, szValue, 0, &type, NULL, &size)) != 0)
    {
        if (err == ERROR_FILE_NOT_FOUND)
        {
            *output = (LPWSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(WCHAR));
            return ERROR_SUCCESS;
        }
        goto failed;
    }
    if (!((type == REG_MULTI_SZ) || ((type == REG_SZ) && bAllowSingle)))
    {
        err = ERROR_INVALID_DATATYPE;
        goto failed;
    }
    buf = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, size + 2*sizeof(WCHAR));
    if ((err = RegQueryValueExW(hKey, szValue, 0, &type, (LPBYTE)buf, &size)) != 0)
        goto failed;
    buf[size/sizeof(WCHAR)] = 0;
    buf[size/sizeof(WCHAR) + 1] = 0;
    *output = buf;
    return ERROR_SUCCESS;

failed:
   
    HeapFree(GetProcessHeap(), 0, buf);
    return err;
}

DWORD load_reg_dword(HKEY hKey, LPCWSTR szValue, DWORD *output)
{
    DWORD size, type;
    DWORD err;

    *output = 0;
    size = sizeof(DWORD);
    if ((err = RegQueryValueExW(hKey, szValue, 0, &type, (LPBYTE)output, &size)) != 0)
    {
        if (err == ERROR_FILE_NOT_FOUND)
            return ERROR_SUCCESS;
        goto failed;
    }
    if (type != REG_DWORD || size != sizeof(DWORD))
    {
        err = ERROR_INVALID_DATATYPE;
        goto failed;
    }
    return ERROR_SUCCESS;

failed:    
    return err;
}


static LPBYTE get_regdata(const WCHAR *data, DWORD reg_type, WCHAR separator, DWORD *reg_count)
{
    static const WCHAR empty;
    LPBYTE out_data = NULL;
    *reg_count = 0;

    if (!data) data = &empty;

    switch (reg_type)
    {
    case REG_NONE:
    case REG_SZ:
    case REG_EXPAND_SZ:
        {
            *reg_count = (lstrlenW(data) + 1) * sizeof(WCHAR);
            out_data = (LPBYTE)heap_xalloc(*reg_count);
            lstrcpyW((LPWSTR)out_data,data);
            break;
        }
    case REG_DWORD:
        /* case REG_DWORD_LITTLE_ENDIAN: */
    case REG_DWORD_BIG_ENDIAN: /* Yes, this is correct! */
        {
            LPWSTR rest;
            unsigned long val;
            val = strtoulW(data, &rest, (tolowerW(data[1]) == 'x') ? 16 : 10);
            if (*rest || data[0] == '-' || (val == ~0u && errno == ERANGE) || val > ~0u) {
                output_message(STRING_MISSING_INTEGER);
                break;
            }
            *reg_count = sizeof(DWORD);
            out_data = (LPBYTE)heap_xalloc(*reg_count);
            ((LPDWORD)out_data)[0] = val;
            break;
        }
    case REG_BINARY:
        {
            BYTE hex0, hex1;
            int i = 0, destByteIndex = 0, datalen = lstrlenW(data);
            *reg_count = ((datalen + datalen % 2) / 2) * sizeof(BYTE);
            out_data = (LPBYTE)heap_xalloc(*reg_count);
            if(datalen % 2)
            {
                hex1 = hexchar_to_byte(data[i++]);
                if(hex1 == 0xFF)
                    goto no_hex_data;
                out_data[destByteIndex++] = hex1;
            }
            for(;i + 1 < datalen;i += 2)
            {
                hex0 = hexchar_to_byte(data[i]);
                hex1 = hexchar_to_byte(data[i + 1]);
                if(hex0 == 0xFF || hex1 == 0xFF)
                    goto no_hex_data;
                out_data[destByteIndex++] = (hex0 << 4) | hex1;
            }
            break;
no_hex_data:
            /* cleanup, print error */
            heap_free(out_data);
            output_message(STRING_MISSING_HEXDATA);
            out_data = NULL;
            break;
        }
    case REG_MULTI_SZ:
        {
            int i, destindex, len = strlenW(data);
            WCHAR *buffer = (WCHAR*)heap_xalloc((len + 2) * sizeof(WCHAR));

            for (i = 0, destindex = 0; i < len; i++, destindex++)
            {
                if (!separator && data[i] == '\\' && data[i + 1] == '0')
                {
                    buffer[destindex] = 0;
                    i++;
                }
                else if (data[i] == separator)
                    buffer[destindex] = 0;
                else
                    buffer[destindex] = data[i];

                if (destindex && !buffer[destindex - 1] && (!buffer[destindex] || destindex == 1))
                {
                    heap_free(buffer);
                    output_message(STRING_INVALID_STRING);
                    return NULL;
                }
            }
            buffer[destindex] = 0;
            if (destindex && buffer[destindex - 1])
                buffer[++destindex] = 0;
            *reg_count = (destindex + 1) * sizeof(WCHAR);
            return (BYTE *)buffer;
        }
    default:
        output_message(STRING_UNHANDLED_TYPE, reg_type, data);
    }

    return out_data;
}


WCHAR *reg_data_to_wchar(DWORD type, const BYTE *src, DWORD size_bytes)
{
    WCHAR *buffer = NULL;
    int i;

    switch (type)
    {
    case REG_SZ:
    case REG_EXPAND_SZ:
        buffer = (WCHAR*)heap_xalloc(size_bytes);
        strcpyW(buffer, (WCHAR *)src);
        break;
    case REG_NONE:
    case REG_BINARY:
        {
            WCHAR *ptr;
            WCHAR fmt[] = {'%','0','2','X',0};

            buffer = (WCHAR*)heap_xalloc((size_bytes * 2 + 1) * sizeof(WCHAR));
            ptr = buffer;
            for (i = 0; i < size_bytes; i++)
                ptr += sprintfW(ptr, fmt, src[i]);
            break;
        }
    case REG_DWORD:
        /* case REG_DWORD_LITTLE_ENDIAN: */
    case REG_DWORD_BIG_ENDIAN:
        {
            const int zero_x_dword = 10;
            WCHAR fmt[] = {'0','x','%','x',0};

            buffer = (WCHAR*)heap_xalloc((zero_x_dword + 1) * sizeof(WCHAR));
            sprintfW(buffer, fmt, *(DWORD *)src);
            break;
        }
    case REG_MULTI_SZ:
        {
            const int two_wchars = 2 * sizeof(WCHAR);
            DWORD tmp_size;
            const WCHAR *tmp = (const WCHAR *)src;
            int len, destindex;

            if (size_bytes <= two_wchars)
            {
                buffer = (WCHAR*)heap_xalloc(sizeof(WCHAR));
                *buffer = 0;
                return buffer;
            }

            tmp_size = size_bytes - two_wchars; /* exclude both null terminators */
            buffer = (WCHAR*)heap_xalloc(tmp_size * 2 + sizeof(WCHAR));
            len = tmp_size / sizeof(WCHAR);

            for (i = 0, destindex = 0; i < len; i++, destindex++)
            {
                if (tmp[i])
                    buffer[destindex] = tmp[i];
                else
                {
                    buffer[destindex++] = '\\';
                    buffer[destindex] = '0';
                }
            }
            buffer[destindex] = 0;
            break;
        }
    }
    return buffer;
}

static const WCHAR type_none[] = {'R','E','G','_','N','O','N','E',0};
static const WCHAR type_sz[] = {'R','E','G','_','S','Z',0};
static const WCHAR type_expand_sz[] = {'R','E','G','_','E','X','P','A','N','D','_','S','Z',0};
static const WCHAR type_binary[] = {'R','E','G','_','B','I','N','A','R','Y',0};
static const WCHAR type_dword[] = {'R','E','G','_','D','W','O','R','D',0};
static const WCHAR type_dword_le[] = {'R','E','G','_','D','W','O','R','D','_','L','I','T','T','L','E','_','E','N','D','I','A','N',0};
static const WCHAR type_dword_be[] = {'R','E','G','_','D','W','O','R','D','_','B','I','G','_','E','N','D','I','A','N',0};
static const WCHAR type_multi_sz[] = {'R','E','G','_','M','U','L','T','I','_','S','Z',0};

static const struct
{
    DWORD type;
    const WCHAR *name;
}
type_rels[] =
{
    {REG_NONE, type_none},
    {REG_SZ, type_sz},
    {REG_EXPAND_SZ, type_expand_sz},
    {REG_BINARY, type_binary},
    {REG_DWORD, type_dword},
    {REG_DWORD_LITTLE_ENDIAN, type_dword_le},
    {REG_DWORD_BIG_ENDIAN, type_dword_be},
    {REG_MULTI_SZ, type_multi_sz},
};

static DWORD wchar_get_type(const WCHAR *type_name)
{
    DWORD i;

    if (!type_name)
        return REG_SZ;

    for (i = 0; i < ARRAY_SIZE(type_rels); i++)
    {
        if (!strcmpiW(type_rels[i].name, type_name))
            return type_rels[i].type;
    }

    return ~0u;
}

/* Allocate and initialize a WSTR containing the expanded string */
LPWSTR ExpandEnv(LPWSTR string)
{
    DWORD size;
    LPWSTR expanded_string;   

    size = 0;
    size = ExpandEnvironmentStringsW(string, NULL, size);
    if (size == 0)
    {
       
        return NULL;
    }
    expanded_string = (LPWSTR)HeapAlloc(GetProcessHeap(), 0,
        (size + 1) * sizeof(WCHAR));
    if (ExpandEnvironmentStringsW(string, expanded_string, size) == 0)
    {
        
        HeapFree(GetProcessHeap(), 0, expanded_string);
        return NULL;
    }
    return expanded_string;
}


LPWSTR get_parent_dir(WCHAR* path)
{
    WCHAR *last_slash;
    WCHAR *result;
    int len;

    last_slash = wcsrchr( path, '\\' );
    if (last_slash == NULL)
        len = 1;
    else
        len = last_slash - path + 1;

    result = (WCHAR*)HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    CopyMemory(result, path, (len-1)*sizeof(WCHAR));
    result[len-1] = '\0';

    return result;
}

void output(const WCHAR *message)
{
	DWORD count;
	DWORD   res;   

	int    wlen = wcslen(message);

	if (!wlen) return;

	res = WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), message, wlen, &count, NULL);

	/* If writing to console fails, assume it's file
         * i/o so convert to OEM codepage and output
         */
	if (!res)
	{
		DWORD len;
		char  *mesA;
		/* Convert to OEM, then output */
		len = WideCharToMultiByte( GetConsoleOutputCP(), 0, message, wlen, NULL, 0, NULL, NULL );
		mesA = (char*)HeapAlloc(GetProcessHeap(), 0, len*sizeof(char));
		if (!mesA) return;
		WideCharToMultiByte( GetConsoleOutputCP(), 0, message, wlen, mesA, len, NULL, NULL );
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), mesA, len, &count, FALSE);
		HeapFree(GetProcessHeap(), 0, mesA);
	}
}

void output_writeconsole(const WCHAR *str, DWORD wlen)
{
    DWORD count, ret;

    ret = WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), str, wlen, &count, NULL);
    if (!ret)
    {
        DWORD len;
        char  *msgA;

        /* On Windows WriteConsoleW() fails if the output is redirected. So fall
         * back to WriteFile(), assuming the console encoding is still the right
         * one in that case.
         */
        len = WideCharToMultiByte(GetConsoleOutputCP(), 0, str, wlen, NULL, 0, NULL, NULL);
        msgA = (char*)heap_xalloc(len);

        WideCharToMultiByte(GetConsoleOutputCP(), 0, str, wlen, msgA, len, NULL, NULL);
        WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), msgA, len, &count, FALSE);
        heap_free(msgA);
    }
}

void output_formatstring(const WCHAR *fmt, va_list va_args)
{
    WCHAR *str;
    DWORD len;

    SetLastError(NO_ERROR);
    len = FormatMessageW(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
        fmt, 0, 0, (WCHAR *)&str, 0, &va_args);
    if (len == 0 && GetLastError() != NO_ERROR)
    {
        
        return;
    }
    output_writeconsole(str, len);
    LocalFree(str);
}

void __cdecl output_string(const WCHAR *fmt, ...)
{
    va_list va_args;

    va_start(va_args, fmt);
    output_formatstring(fmt, va_args);
    va_end(va_args);
}

void __cdecl output_message(unsigned int id, ...)
{
    WCHAR fmt[1024];
    va_list va_args;

    if (!LoadStringW(GetModuleHandleW(NULL), id, fmt, ARRAY_SIZE(fmt)))
    {
        //WINE_FIXME("LoadString failed with %d\n", GetLastError());
        return;
    }
    va_start(va_args, id);
    output_formatstring(fmt, va_args);
    va_end(va_args);
}

