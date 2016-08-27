#include <cactus/cactus_date.h>

#include <atlstr.h>
#include <time.h>
//#include <atltime.h>

namespace cactus
{
void GetCurrentLocalSystemTime(string8& datestr, string8& timestr)
{
    SYSTEMTIME st = {0};
    char _date[30] = {0};
    char _time[30] = {0};
    GetLocalTime(&st);
    sprintf(_date, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);
    sprintf(_time, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
    datestr = string8(_date);
    timestr = string8(_time);
}

//文件时间转成当地时间
void FileTimeToLocalSystemTime(FILETIME &ft, string8& datestr, string8& timestr)
{
    FILETIME localft;
    SYSTEMTIME st = {0};
    char _date[30] = {0};
    char _time[30] = {0};
    FileTimeToLocalFileTime(&ft, &localft);
    FileTimeToSystemTime(&localft, &st);
    sprintf(_date, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);
    sprintf(_time, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
    datestr = string8(_date);
    timestr = string8(_time);
}

void FormatTimeString(LPTSTR lpszBuf, SYSTEMTIME* pTime)
{
    wsprintf(lpszBuf, _T("%02d:%02d:%02d"), pTime->wHour, pTime->wMinute, pTime->wSecond);
}

string16 GetDateString()
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    CStringW retval;
    retval.Format(L"%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
    return string16((LPCWSTR)retval);
}

BOOL ParseDateString(const string16& date, INT &year, INT &month, INT &day)
{
    return _stscanf(date.c_str(), _T("%u-%u-%u"), &year, &month, &day) == 3;
}

BOOL IsToday(SYSTEMTIME * lpTime)
{
    if (NULL == lpTime)
        return FALSE;

    SYSTEMTIME stSysTime = {0};
    ::GetLocalTime(&stSysTime);

    if (stSysTime.wYear == lpTime->wYear
        && stSysTime.wMonth == lpTime->wMonth
        && stSysTime.wDay == lpTime->wDay)
        return TRUE;
    else
        return FALSE;
}

BOOL IsToday(time_t lTime)
{
    time_t lCurTime;
    struct tm * lpCurTimeInfo, * lpTimeInfo;
    lCurTime = time(NULL);
    lpCurTimeInfo = localtime(&lCurTime);
    lpTimeInfo = localtime(&lTime);

    if (lpCurTimeInfo != NULL && lpTimeInfo != NULL
        && lpCurTimeInfo->tm_year == lpTimeInfo->tm_year
        && lpCurTimeInfo->tm_mon == lpTimeInfo->tm_mon
        && lpCurTimeInfo->tm_mday == lpTimeInfo->tm_mday)
        return TRUE;
    else
        return FALSE;
}

BOOL GetFileLastWriteTime(const string16& filename, SYSTEMTIME* lpSysTime)
{
    BOOL bRet = FALSE;

    if (NULL == lpSysTime)
        return FALSE;

    HANDLE hFile = ::CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        FILETIME stLastWriteTime = {0};
        bRet = ::GetFileTime(hFile, NULL, NULL, &stLastWriteTime);

        if (bRet) {
            FILETIME stLocalFileTime = {0};
            bRet = ::FileTimeToLocalFileTime(&stLastWriteTime, &stLocalFileTime);

            if (bRet)
                bRet = ::FileTimeToSystemTime(&stLocalFileTime, lpSysTime);
        }

        ::CloseHandle(hFile);
    }

    return bRet;
}

} // namespace cactus


namespace cactus
{
#ifndef HRET
    #define HRET do { if (FAILED(hr)) return hr; } while(0)
#endif


// FILETIME from time_t: http://blogs.msdn.com/b/brada/archive/2003/07/30/50205.aspx Brad Adams MSDN blog "Converting to a DateTime from time_t"
// or here: http://msdn.microsoft.com/en-us/library/ms724228(VS.85).aspx
// see also TONS: http://blogs.msdn.com/b/oldnewthing/archive/2003/09/05/54806.aspx  "How to recognize different types of timestamps from quite a long way away"
//                http://blogs.msdn.com/b/oldnewthing/archive/2005/10/28/486194.aspx "How to recognize different types of sentinel timestamps from quite a long way away"
//
//    http://blogs.msdn.com/b/ericlippert/archive/2003/09/16/eric-s-complete-guide-to-vt-date.aspx
//       MSDN Blogs > Fabulous Adventures In Coding > Eric's Complete Guide To VT_DATE  Eric Lippert 16 Sep 2003 2:50 PM
//
// http://blogs.msdn.com/b/joshpoley/archive/2007/12/19/date-time-formats-and-conversions.aspx

// OLE DATE epoch:
// http://blogs.msdn.com/b/ericlippert/archive/2003/09/16/eric-s-complete-guide-to-vt-date.aspx "Eric's Complete Guide To VT_DATE"
// http://blogs.msdn.com/b/ericlippert/archive/2009/06/01/bug-psychology.aspx "Bug Psychology"

// note on _mkgmtime:
// this is a non-standard version of mktime, which does *not* convert from local time to UTC.
// you could implement that yourself by e.g.:
//    - comparing gmtime vs. localtime, and apply the offset (move the "epoch" by current time zone!)
//    - time_t --> (FileTime or VariantTime)--> SystemTime --> tm

HRESULT TimeConvert(time_const_ref from, time_ref to)
{
    HRESULT hr = S_OK;

    switch (to.fmt) {
    case tftUnix32:              // ------ TO time_t (32 bit)
        switch (from.fmt) {
        case tftUnix32         :
            *to.m_time32_t = *from.m_time32_t;
            return S_OK;

        case tftUnix64         :
            if (*from.m_time64_t > 0x7FFFFFFFI64 || *from.m_time64_t < 0xFFFFFFFF80000000)
                return E_INVALIDARG;

            *to.m_time32_t = static_cast<__time32_t>(*from.m_time64_t);
            return S_OK;

        case tftTM           : {
            // Note: DST should be set to -1, otherwise _mkgmtime will adjust the structure
            struct tm cpy = *from.m_tm; // _mkgmtime modifies its argument
            __time32_t result =  _mkgmtime32(&cpy);

            if (result == -1)
                return E_INVALIDARG;

            *to.m_time32_t = result;
            return S_OK;
        }

        case tftOleDate      :
        case tftFileTime     : {
            // convert via time64_t for range check
            __time64_t tt64 = 0;
            hr = TimeConvert(from, tt64);
            HRET;
            hr = TimeConvert(tt64, to);
            HRET;
            return S_OK;
        }

        case tftSystemTime   : {
            FILETIME ft;
            hr = TimeConvert(from, ft);
            HRET;
            hr = TimeConvert(ft, to);
            HRET;
            return S_OK;
        }
        }

    case tftUnix64:              // ------ TO time_t (64 bit)
        switch (from.fmt) {
        case tftUnix32         :
            *to.m_time64_t = *from.m_time32_t;
            return S_OK;

        case tftUnix64         :
            *to.m_time64_t = *from.m_time64_t;
            return S_OK;

        case tftTM           : {
            // Note: DST should be set to -1, otherwise _mkgmtime will adjust the structure
            struct tm cpy = *from.m_tm; // will be modified by _mkgmtime
            *to.m_time64_t = _mkgmtime64(&cpy);
            return (*to.m_time64_t == -1) ? E_INVALIDARG : S_OK;
        }

        case tftOleDate      :
            // no range check since the *possible* range of a 64 bit time_t exceeds the valid range of an OLE DATE.
            // (still, an OLE date might hold a value not representable in _time64_t)
            *to.m_time64_t = static_cast<time_t>((*from.m_oleDate - 25569) * 86400);
            return S_OK;

        case tftSystemTime   : {
            FILETIME ft;
            hr = TimeConvert(from, ft);
            HRET;
            hr = TimeConvert(ft, to);
            HRET;
            return S_OK;
        }

        case tftFileTime     :
            // http://support.microsoft.com/kb/167296/en-us
            // MSDN-KB:"How To Convert a UNIX time_t to a Win32 FILETIME or SYSTEMTIME"
            // no range check since the *possible* range of a 64 bit time_t exceeds the range of a FILETIME
            *to.m_time64_t = FileTime64(*from.m_fileTime) / 10000000 - 11644473600;
            return S_OK;
        }

    case tftTM:                // ------ TO struct tm
        switch (from.fmt) {
        case tftUnix32         : {
            // note: _gmtime will only accept times >= 0, even though negative time_t isn't by definition incorrect
            struct tm result;
            errno_t err = _gmtime32_s(&result, from.m_time32_t);

            if (err != 0)
                return E_INVALIDARG;

            *to.m_tm = result;
            return S_OK;
        }

        case tftUnix64         : {
            // note: _gmtime will only accept times >= 0, even though negative time_t isn't by definition incorrect
            struct tm result;
            errno_t err = _gmtime64_s(&result, from.m_time64_t);

            if (err != 0)
                return E_INVALIDARG;

            *to.m_tm = result;
            return S_OK;
        }

        case tftTM           :
            *to.m_tm = *from.m_tm;
            return S_OK;

        // note: avoid converting via time_t, since this has the worst epoch
        case tftOleDate      : {
            SYSTEMTIME st;
            hr = TimeConvert(from, st);
            HRET; // convert via SYSTEMTIME
            hr = TimeConvert(st, to);
            HRET;          // convert SYSTEMTIME --> TM (copy members + mktime)
            return S_OK;
        }

        case tftSystemTime   : {
            if (from.m_systemTime->wYear < 1900) // can't represent as struct tm
                return E_INVALIDARG;

            to.m_tm->tm_mday  = from.m_systemTime->wDay;
            to.m_tm->tm_wday  = from.m_systemTime->wDayOfWeek;
            to.m_tm->tm_hour  = from.m_systemTime->wHour;
            // drop milliseconds
            to.m_tm->tm_min   = from.m_systemTime->wMinute;
            to.m_tm->tm_mon   = from.m_systemTime->wMonth - 1;
            to.m_tm->tm_sec   = from.m_systemTime->wSecond;
            to.m_tm->tm_year  = from.m_systemTime->wYear - 1900;
            to.m_tm->tm_isdst = -1; // "not known"
            // call _mkgmtime to fill in day of year
            _mkgmtime(to.m_tm);  // ignore return value, since date may be out of range for time_t
            return S_OK;
        }

        case tftFileTime     : {
            SYSTEMTIME st;
            hr = TimeConvert(from, st);
            HRET; // convert via SYSTEMTIME
            return TimeConvert(st, to);
            return S_OK;
        }
        }

    case tftOleDate:           // ------ TO OLE Date/Time
        switch (from.fmt) {
        case tftUnix32       : {
            double result = *from.m_time32_t / 86400.0 + 25569;
            // TODO?: range check: year < 100 || year >= 10000. the entire range is representable as double, but with loss of precision
            *to.m_oleDate = result;
            return S_OK;
        }

        case tftUnix64       : {
            double result = *from.m_time64_t / 86400.0 + 25569;
            // TODO?: range check: year < 100 || year >= 10000. the entire range is representable as double, but with loss of precision
            *to.m_oleDate = result;
            return S_OK;
        }

        case tftTM           : {
            SYSTEMTIME st;
            hr = TimeConvert(from, st);
            HRET;
            return TimeConvert(st, to);
        }

        case tftOleDate      :
            *to.m_oleDate = *from.m_oleDate;
            return S_OK;

        case tftSystemTime   :
            return SystemTimeToVariantTime(
                       const_cast<LPSYSTEMTIME>(from.m_systemTime), to.m_oleDate) ?
                   S_OK : HRESULT_FROM_WIN32(GetLastError());

        case tftFileTime     : {
            const unsigned __int64 offset = 94353120000000000UI64;
            const unsigned __int64 ft64 = FileTime64(*from.m_fileTime);

            if (ft64 < offset)
                return E_INVALIDARG;

            // TODO?: range check: year < 100 || year >= 10000. the entire range is representable as double, but with loss of precision
            *to.m_oleDate = (ft64 - offset) / 86400.0e7;
            return S_OK;
        }
        }

    case tftSystemTime:        // ------ TO SystemTime:
        switch (from.fmt) {
        case tftUnix32         :
        case tftUnix64         : {
            FILETIME ft;
            hr = TimeConvert(from, ft);
            HRET;
            hr = TimeConvert(ft, to);
            HRET;
            return S_OK;
        }

        case tftTM           : {
            to.m_systemTime->wDay            = from.m_tm->tm_mday;
            to.m_systemTime->wDayOfWeek      = from.m_tm->tm_wday;
            to.m_systemTime->wHour           = from.m_tm->tm_hour;
            to.m_systemTime->wMilliseconds   = 0;
            to.m_systemTime->wMinute         = from.m_tm->tm_min;
            to.m_systemTime->wMonth          = from.m_tm->tm_mon + 1; // tm uses 0..11
            to.m_systemTime->wSecond         = from.m_tm->tm_sec;
            to.m_systemTime->wYear           = from.m_tm->tm_year + 1900;
            return S_OK;
        }

        case tftOleDate      :
            return VariantTimeToSystemTime(*from.m_oleDate, to.m_systemTime) ?
                   S_OK : HRESULT_FROM_WIN32(GetLastError());

        case tftSystemTime   :
            *to.m_systemTime = *from.m_systemTime;
            return S_OK;

        case tftFileTime     :
            return FileTimeToSystemTime(from.m_fileTime, to.m_systemTime) ?
                   S_OK : HRESULT_FROM_WIN32(GetLastError());
        }

        break;

    case tftFileTime:          // ------ TO FILETIME:
        switch (from.fmt) {
        case tftUnix32         :
            // entire numeric range representable as FILETIME, no range check necessary
            *to.m_fileTime = FileTime(10000000UI64 * *from.m_time32_t + 116444736000000000UI64);
            return S_OK;

        case tftUnix64         : {
            // range check on numeric limits:
            const unsigned __int64 offset = 116444736000000000UI64;
            const unsigned __int64 scale = 10000000UI64;
            const unsigned __int64 max_time_t = (0xFFFFFFFFFFFFFFFFUI64 - offset) / scale;
            const signed __int64   min_time_t = -static_cast<__int64>(offset) / static_cast<__int64>(scale);

            if (*from.m_time64_t < min_time_t || *from.m_time64_t > max_time_t)
                return E_INVALIDARG;

            *to.m_fileTime = FileTime(scale * *from.m_time64_t + offset);
            return S_OK;
        }

        case tftTM           : {
            SYSTEMTIME st;
            hr = TimeConvert(from, st);
            HRET;
            return TimeConvert(st, to);
        }

        case tftOleDate      : {
            // range check on numeric limits
            const unsigned __int64 scale = 10000000UI64 * 86400UI64;
            const unsigned __int64 offset = 94353120000000000UI64;
            double max_vardate = (0xFFFFFFFFFFFFFFFFUI64 - offset) / scale;
            double min_vardate = -static_cast<double>(offset) / scale;

            if (*from.m_oleDate < min_vardate || *from.m_oleDate > max_vardate)
                return E_INVALIDARG;

            *to.m_fileTime = FileTime(static_cast<unsigned __int64>(*from.m_oleDate * scale + 94353120000000000UI64));
            return S_OK;
        }

        case tftSystemTime   :
            return SystemTimeToFileTime(from.m_systemTime, to.m_fileTime) ?
                   S_OK : HRESULT_FROM_WIN32(GetLastError());

        case tftFileTime     :
            *to.m_fileTime = *from.m_fileTime;
            return S_OK;
        }
    }

    // _ASSERTE(false); // unknown conversion
    return E_INVALIDARG;
}

// ----- GetUTCOffset ----------
/// \param pNow_utc [time_t *]: if not NULL, receives the UTC time for which the calculation was amde
/// \returns [int]: the offset between local time and UTC, in seconds.
///
/// \par Remarks:
/// To guarantee the UTC time in pNow_utc is consistent with the UTC offset, calculating the UTC offset is repeated two or three times.
/// This repetition is not done when pNow_utc is null.
///
/// The values might become inconsistent when the UTC offset changes during the execution of this method, e.g. because
/// of a DST adjustment. To correctly retrieve the UTC timestamp in that situation in a different format than time_t, use the following  algorithm:
///   - get UTC offset (pNow_utc can be set to NULL)
///   - get UTC time
///   - get UTC offset again (pNow_utc can be set to NULL)
///   - repeat these steps if the two UTC offsets don't match
int GetUTCOffset(time_t * pNow_utc)
{
    int prevOffset = -1; // not a valid offset

    // Implementation:
    // There is no standard way to get the UTC offset, but we can use a little trick
    // mktime converts a local time to UTC, if we pass a UTC time, it will be adjusted by the negative UTC offset
    //
    // If pNow_utc is specified, we repeat until we get two identical readings.
    // This is because it's rare but possible for the UTC offset to change between reading UTC time (using time(0))
    // and determining the UTC offset (using mktime), and we want to guarantee consistency between the time and the offset
    //
    while (1) {
        time_t now_utc = time(0);
        struct tm tm;
        int ok = gmtime_s(&tm, &now_utc);
        time_t now_anti_local = mktime(&tm);
        int offset = -static_cast<int>(now_anti_local - now_utc);

        if (prevOffset == offset || !pNow_utc) {
            if (pNow_utc)
                *pNow_utc = now_utc;

            return offset;
        }

        prevOffset = offset;
    }
}


// ----- FmtUTCOffset ----------
/// Always adds a sign, and includes minutes only when the offset is not a full hour.
/// The string is empty when the UTC offset is 0 minutes. Seconds are always ignored.
/// \param buffer [TCHAR *]: buffer receiving the UTC offset as string. Should be at least 7 characters (+xx:xx\0)
/// \param count [size_t]: size of the buffer, in characters
/// \param offsetInSeconds [int]: UTC offset in seconds, as returned by \c GetUTCOffset
/// \returns [int]: -1 if an error occurred, otherwise number of characters written to buffer, not counting the terminating zero
int FmtUTCOffset(TCHAR * buffer, size_t count, int offsetInSeconds)
{
    int totalMinutes = offsetInSeconds / 60; // sub-minute resolution? really, no.

    if (totalMinutes == 0)
        return _stprintf_s(buffer, count, _T(""));

    int minutes = abs(totalMinutes) % 60;
    int hours = totalMinutes / 60;

    if (minutes)
        return _stprintf_s(buffer, count, _T("%+d:%02d"), hours, minutes);
    else
        return _stprintf_s(buffer, count, _T("%+d"), hours);
}


#ifndef PHLIB_NO_CSTRING

// ----- FmtUTCOffset ----------
/// See \ref FmtUTCOffset(TCHAR*,size_t,int) for details
/// \param offsetInSeconds [int]: UTC offset in seconds, as returned by \c GetUTCOffset
/// \returns [CString]: string with the UTC offset (hours and minutes)
CString FmtUTCOffset(int offsetInSeconds)
{
    CString result;
    int totalMinutes = offsetInSeconds / 60; // sub-minute resolution? really, no.

    if (totalMinutes == 0)
        return result;

    int minutes = abs(totalMinutes) % 60;
    int hours = totalMinutes / 60;

    if (minutes)
        result.Format(_T("%+d:%02d"), hours, minutes);
    else
        result.Format(_T("%+d"), hours);

    return result;
}
#endif


// ----- PackFileTime64 -----------------
/// Packs a FILETIME and an UTC offset into an __int64.
/// The UTC offset is rounded to the closest 15 minutes.
/// The FileTime is reduced by a factor of 128 to 12.8 microseconds, and rounded
//  to the nearest representable value.
/// \param ft: a FILETIME to pack. If the rounding to the closest multiple of
/// 12.8 microsecond overflows, the result is -1. (hello year 60000!)
/// \param utcOffsSec: a UTC offset in minutes to pack. This must be a valid value
///                 (currently, time zones exist between -12h..+14h),
//                  otherwise the result is -1.
/// \returns: a packed __int64 containing the filetime and UTC offset
unsigned __int64 PackFileTime64(FILETIME ft, int utcOffsSec)
{
    // the current time zone range is -12h .. +14h.
    // The packing allows a valid range of -15h..+16h, in case islands go   crazy
    // for having the first / last new years party, or earth slows down.
    // We  are prepared!
    const bool validOffset = utcOffsSec >= -15 * 3600 && utcOffsSec <= 16 * 3600;
    _ASSERTE(validOffset);

    if (!validOffset)
        return -1;

    unsigned biasedUtcOffs = (unsigned)(utcOffsSec + 15 * 3600);
    biasedUtcOffs = (biasedUtcOffs + 15 * 60 / 2) / (15 * 60); // seconds --> quarter of an hour, rounded
    _ASSERTE(biasedUtcOffs >= 0 && biasedUtcOffs <= 0x7F); // invalid calculations above?
    unsigned __int64 ft64 = FileTime64(ft);

    if (ft64 + 0x3F < ft64)
        return -1;

    ft64 += 0x3F;
    ft64 &= ~0x7F;
    ft64 |= (biasedUtcOffs & 0x7F);
    return ft64;
}

// ----- UnpackFileTime -----------------
/// extracts a FILETIME and an UTC offset, from an int64 packed by \ref PackFileTime64(FILETIME,int)
/// \param ft64  the value returned from \c FileTime64(FILETIME,int)
/// \param utcOffsSec receives an UTC offset in seconds
/// \returns: FILETIME value (resolution is reduced to ~ 1 microsecond)
FILETIME UnpackFileTime(unsigned __int64 ft64, int & utcOffsSec)
{
    int utcOffsBiased = (int)(ft64 & 0x7F);
    utcOffsSec = (utcOffsBiased * 15 * 60) - 15 * 3600; // 15*60 ist die Aufl鰏ung (15 minuten), 15*3600 der Bias (-15..+16)
    return FileTime(ft64 & ~0x7F);
}

// ----- GetPackedFileTimeNow -----------------
/// Returns the current time and UTC offset as packed int64
/// UTC time and UTC offset can be retrieved by \ref UnpackFileTime.
unsigned __int64 GetPackedFileTimeNow()
{
    int utcOffs = 0;
    FILETIME ft = { 0 };

    while (1) {
        int utcOffs1 = GetUTCOffset(NULL);
        GetSystemTimeAsFileTime(&ft);
        int utcOffs2 = GetUTCOffset(NULL);

        if (utcOffs1 == utcOffs2) {
            utcOffs = utcOffs1;
            break;
        }
    }

    return PackFileTime64(ft, utcOffs);
}


// ----- TimeUTCToLocal -----------------
/**
   Umrechnung von UTC nach Lokaler Zeit mit bekanntem UTC Offset
\param t: [in] UTC-Zeit, [out] lokale Zeit (akzeptiert verschiedene Formate)
\param utcOffs: UTC offset in Sekunden
\returns: S_OK bei Erfolg, sonst Fehlercode.
*/
HRESULT TimeUTCToLocal(time_ref t, int utcOffs)
{
    switch (t.fmt) {
    case tftFileTime:
        *t.m_fileTime = FileTime(FileTime64(*t.m_fileTime) + utcOffs * 1e7);
        return S_OK;

    case tftSystemTime:
    case tftTM: {
        FILETIME ft;
        HRESULT hr = TimeConvert(t, ft);
        HRET;
        hr = TimeUTCToLocal(ft, utcOffs);
        HRET;
        hr = TimeConvert(ft, t);
        HRET;
        return S_OK;
    }

    case tftOleDate:
        *t.m_oleDate += utcOffs / 86400.0;
        return S_OK;

    case tftUnix32:
        *t.m_time32_t += utcOffs;
        return S_OK;

    case tftUnix64:
        *t.m_time64_t += utcOffs;
        return S_OK;

    default:
        _ASSERTE(false);
        return E_INVALIDARG;
    }
}

HRESULT TimeUTCToLocal(time_ref t)
{
    switch (t.fmt) {
    case tftFileTime: {
        FILETIME ftResult = { 0 };
        FileTimeToLocalFileTime(t.m_fileTime, &ftResult);
        *t.m_fileTime = ftResult;
        return S_OK;
    }

    default: {
        FILETIME ftConvert;
        HRESULT hr = TimeConvert(t, ftConvert);
        HRET;
        hr = TimeUTCToLocal(ftConvert);
        HRET;
        hr = TimeConvert(ftConvert, t);
        HRET;
        return S_OK;
    }
    }
}

HRESULT TimeLocalToUTC(time_ref t, int utcOffs)
{
    return TimeUTCToLocal(t, -utcOffs);
}


HRESULT TimeLocalToUTC(time_ref t)
{
    switch (t.fmt) {
    case tftFileTime: {
        FILETIME ftResult = { 0 };
        LocalFileTimeToFileTime(t.m_fileTime, &ftResult);
        *t.m_fileTime = ftResult;
        return S_OK;
    }

    default: {
        FILETIME ftConvert;
        HRESULT hr = TimeConvert(t, ftConvert);
        HRET;
        hr = TimeLocalToUTC(ftConvert);
        HRET;
        hr = TimeConvert(ftConvert, t);
        HRET;
        return S_OK;
    }
    }
}

} // namespace cactus