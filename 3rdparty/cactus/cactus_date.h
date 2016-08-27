#ifndef __CACTUS_CACTUS_DATE_H__
#define __CACTUS_CACTUS_DATE_H__

// - time_t
// - FILETIME
// - tm
// - ptime (boost)
// - CTime CTimeSpan (ATL)
// http://www.cnblogs.com/yegoufromchongqing/archive/2013/01/24/2875372.html

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <atlstr.h>
#define _WTL_NO_CSTRING
#include <Windows.h>
#include <time.h>

//------------------------------------------------
// DateUtils.h
namespace cactus
{

void CACTUS_API GetCurrentLocalSystemTime(string8& datestr, string8& timestr);
void CACTUS_API FileTimeToLocalSystemTime(FILETIME &ft, string8& datestr, string8& timestr);
void CACTUS_API FormatTimeString(LPTSTR lpszBuf, SYSTEMTIME* pTime);
string16 CACTUS_API GetDateString();
BOOL CACTUS_API ParseDateString(const string16& date, INT &year, INT &month, INT &day);

BOOL CACTUS_API IsToday(SYSTEMTIME * lpTime);
BOOL CACTUS_API IsToday(time_t lTime);
BOOL CACTUS_API GetFileLastWriteTime(const string16& filename, SYSTEMTIME* lpSysTime);
} // namespace cactus

namespace cactus
{
// Made available under CodeProject Open Licence
// more information under http://www.codeproject.com/KB/library/timeconvert2.aspx
// =====================================================
//  Public Interface
// -----------------------------------------------------


// ----- time_ref -----
/// Reference to a time. Can be constructed form one of the following formats:
///  - time_t (Unix time stamp)
///  - struct tm
///  - DATE & (OLE Date/Time, stored as double)
///  - SYSTEMTIME &
///  - FILETIME &
/// Note that it does not store the time. It is used mainly as argument adapter, see \c TimeConvert
struct time_ref;


// ----- time_ref -----
/// constant reference to a time.
/// Can be constructed from the same types as \c time_ref, but accepts a <code>const &</code>
/// In addition, a \c time_const_ref can be constructed from a \c time_ref.
struct time_const_ref;

HRESULT TimeConvert(time_const_ref from, time_ref to);

// Utilities

// ----- FileTime64 -----------------
/// converts a FILETIME structure into a 64 bit unsinged integer
inline unsigned __int64 FileTime64(FILETIME ft)
{
    return (((unsigned __int64) ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

// ----- FileTime -----------------
/// Converts a 64 bit unsigned integer into a FILETIME structure
inline FILETIME FileTime(unsigned __int64 ft64)
{
    FILETIME ft = { ft64 & 0xFFFFFFFF, ft64 >> 32 };
    return ft;
}

int GetUTCOffset(time_t * pNow_utc = 0);        //< returns UTC offset in seconds (local time = UTC + UTC Offset)
int FmtUTCOffset(TCHAR * buffer, size_t count, int offsetInSeconds);    //< Formats a UTC offset, e.g. "-8" or "+5:30"

#ifndef PHLIB_NO_CSTRING
    CString FmtUTCOffset(int offsetInSeconds);    //< Formats a UTC offset, e.g. "-8" or "+5:30"
#endif

FILETIME UnpackFileTime(unsigned __int64 ft64, int & utcOffsSec);
unsigned __int64 PackFileTime64(FILETIME ft, int utcOffsSec);

unsigned __int64 GetPackedFileTimeNow();

HRESULT TimeUTCToLocal(time_ref t, int utcOffs);
HRESULT TimeLocalToUTC(time_ref t, int utcOffs);

HRESULT TimeUTCToLocal(time_ref t);
HRESULT TimeLocalToUTC(time_ref t);

// =====================================================
// Implementation details
// -----------------------------------------------------

// ----- ETimeFmt -----------------
/// time formats supported by \ref time_ref
enum ETimeFmt {
    tft_None = 0,
    tftUnix32 = 1,
    tftUnix64 = 2,
    tftTM = 4,
    tftOleDate = 5,
    tftSystemTime = 6,
    tftFileTime = 7,

    tft_Count,

    // Types not covered:
    //  - UDATE
    // When adding a new type:
    //    - add to time_arg (union and constructor)
    //    - add to time_arg_const (union and constructor)
    //    - add to time_arg_instance (struct only_for_sizeof)
    //    - add all conversions from and to other types to Convert
    //      (from X to all, all to X)
};



struct time_ref {
    ETimeFmt fmt;
    union {
        __time32_t * m_time32_t;
        __time64_t * m_time64_t;
        struct tm * m_tm;
        double * m_oleDate;
        SYSTEMTIME * m_systemTime;
        FILETIME * m_fileTime;
    };

    time_ref(__time32_t & m)       : fmt(tftUnix32), m_time32_t(&m) {}
    time_ref(__time64_t & m)       : fmt(tftUnix64), m_time64_t(&m) {}
    time_ref(struct tm & m)    : fmt(tftTM), m_tm(&m) {}
    time_ref(double & m) : fmt(tftOleDate), m_oleDate(&m) {}
    time_ref(SYSTEMTIME & m) : fmt(tftSystemTime), m_systemTime(&m) {}
    time_ref(FILETIME & m) : fmt(tftFileTime), m_fileTime(&m) {}

protected:
    time_ref(ETimeFmt fmt_, void * p) : fmt(fmt_), m_time32_t((__time32_t *)p) {}

};

struct time_const_ref {
    ETimeFmt fmt;
    union {
        const __time32_t * m_time32_t;
        const __time64_t * m_time64_t;
        const struct tm * m_tm;
        const double * m_oleDate;
        const SYSTEMTIME * m_systemTime;
        const FILETIME * m_fileTime;
    };  // anonymous union. yes we can.

    time_const_ref(const __time32_t & m)       : fmt(tftUnix32), m_time32_t(&m) {}
    time_const_ref(const __time64_t & m)       : fmt(tftUnix64), m_time64_t(&m) {}
    time_const_ref(const struct tm & m)    : fmt(tftTM), m_tm(&m) {}
    time_const_ref(const double & m) : fmt(tftOleDate), m_oleDate(&m) {}
    time_const_ref(const SYSTEMTIME & m) : fmt(tftSystemTime), m_systemTime(&m) {}
    time_const_ref(const FILETIME & m) : fmt(tftFileTime), m_fileTime(&m) {}

    time_const_ref(time_ref arg) : fmt(arg.fmt), m_time32_t(arg.m_time32_t)
    {
        // NOTE: assumes all the member pointers are the same size
    }
};


struct time_any_instance : public time_ref {
private:
    struct only_for_sizeof {
        __time32_t m_time32_t;
        __time64_t m_time64_t;
        struct tm m_tm;
        double m_oleDate;
        SYSTEMTIME m_systemTime;
        FILETIME m_fileTime;
    };

    char data[sizeof(only_for_sizeof)];

public:
    time_any_instance(ETimeFmt fmt_) : time_ref(fmt_, data) {}
};

} // namespace cactus

//namespace test
//{
//void TestTimeRef();     // calls all time conversion test routines
//void TestFmtUTCOffset();
//
//// ----------------------------------------------
//
////*
//namespace
//{
//struct RunMe {
//    RunMe()
//    {
//        TestTimeRef();
//        TestFmtUTCOffset();
//        int delta = GetUTCOffset();
//        ATLTRACE(_T("I don't know your UTC offset, but I think it's %s\r\n"), FmtUTCOffset(delta));
//    }
//} runMe;
//}
////*/
//
//// ----------------------------------------------
//
//
//// time-t epoch as SYSTEMTIME
//SYSTEMTIME TTEpochAsST()
//{
//    SYSTEMTIME st = { 0 };
//    st.wYear = 1970;
//    st.wMonth = 1;
//    st.wDayOfWeek = 4;  // thursday
//    st.wDay = 1;
//    return st;
//}
//
//
//void TestTimeRef_tm_time_t()
//{
//    // 1. struct tm --> time_t, tests _mkgmtime, use time_t epoch (1.1.1970)
//    struct tm stm;
//    stm.tm_sec = 0;
//    stm.tm_min = 0;
//    stm.tm_hour = 0;
//    stm.tm_mday = 1;
//    stm.tm_mon = 0; // January == 0
//    stm.tm_year = 70;    // 1900 == 0
//    stm.tm_wday = -1;    // not known, not necessary
//    stm.tm_yday = -1;    // not known - not necessary
//    stm.tm_isdst = -1;   // not known -1 is necessary to avoid correction!
//    time_t result = -1;
//    HRESULT hr = TimeConvert(stm, result);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(result == 0);
//}
//
//
//void TestTimeRef_time_t_2_tm()
//{
//    // 2. time_t --> struct tm, tests gmtime, use time_t epoch (1.1.1970)
//    struct tm tm;
//    time_t epoch = 0;
//    HRESULT hr = TimeConvert(epoch, tm);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(tm.tm_sec == 0);
//    _ASSERTE(tm.tm_min == 0);
//    _ASSERTE(tm.tm_hour == 0);
//    _ASSERTE(tm.tm_mday == 1);
//    _ASSERTE(tm.tm_mon == 0); // January ==== 0
//    _ASSERTE(tm.tm_year == 70);    // 1900 ==== 0
//    _ASSERTE(tm.tm_wday == 4);    // (sunday = 0) 1.1.1970 = thursday, according to e.g. http://kalender-365.de/index.php?yy=1970
//    _ASSERTE(tm.tm_yday == 0);    // 0 == January 1st
//    _ASSERTE(tm.tm_isdst == 0);   // always 0 for "gmtime"
//}
//
//
//void TestTimeRef_st2vt()
//{
//    // SYSTEMTIME --> DATE, testing OLE DATE epoch = 30.12.1899
//    SYSTEMTIME st = { 0 };
//    st.wYear = 1899;
//    st.wMonth = 12;
//    st.wDayOfWeek = -1;  // needed?
//    st.wDay = 30;
//    DATE epoch = -1;
//    HRESULT hr = TimeConvert(st, epoch);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(epoch == 0);
//}
//
//void TestTimeRef_vt2st()
//{
//    // DATE --> SYSTEMTIME , testing OLE DATE epoch = 30.12.1899
//    DATE epoch = 0;
//    SYSTEMTIME st;
//    memset(&st, -1, sizeof(st));
//    HRESULT hr = TimeConvert(epoch, st);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(st.wYear == 1899);
//    _ASSERTE(st.wMonth == 12);
//    _ASSERTE(st.wDayOfWeek == 6);  // a Saturday, according to http://kalender-365.de/index.php?yy=1899
//    _ASSERTE(st.wDay == 30);
//    _ASSERTE(st.wHour == 0);
//    _ASSERTE(st.wMinute == 0);
//    _ASSERTE(st.wSecond == 0);
//    _ASSERTE(st.wMilliseconds == 0);
//}
//
//void TestTimeRef_st2ft()
//{
//    SYSTEMTIME st = { 0 };
//    st.wYear = 1601;
//    st.wMonth = 1;
//    st.wDayOfWeek = -1;  // needed?
//    st.wDay = 1;
//    FILETIME epoch = FileTime(-1);
//    HRESULT hr = TimeConvert(st, epoch);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(FileTime64(epoch) == 0);
//}
//
//void TestTimeRef_ft2st()
//{
//    // DATE --> SYSTEMTIME , testing OLE DATE epoch = 30.12.1899
//    FILETIME epoch = FileTime(0);
//    SYSTEMTIME st;
//    memset(&st, -1, sizeof(st));
//    HRESULT hr = TimeConvert(epoch, st);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(st.wYear == 1601);
//    _ASSERTE(st.wMonth == 1);
//    _ASSERTE(st.wDayOfWeek == 1);  // a Monday, according to... the function itself
//    _ASSERTE(st.wDay == 1);
//    _ASSERTE(st.wHour == 0);
//    _ASSERTE(st.wMinute == 0);
//    _ASSERTE(st.wSecond == 0);
//    _ASSERTE(st.wMilliseconds == 0);
//}
//
//
//void TestTimeRef_tm2od()
//{
//    // Test that 1.1.1900 is indeed OLEDATE==2.0: tm -> SYSTEMTIME -> OLEDATE
//    struct tm tm = { 0 };
//    tm.tm_isdst = -1;
//    tm.tm_mday = 1;
//    tm.tm_mon = 0;
//    tm.tm_year = 0;
//    DATE od = 0.0;
//    HRESULT hr = TimeConvert(tm, od);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(od == 2.0);
//}
//
//void TestTimeRef_Calc_od2tt()
//{
//    // ----- Test Calculation DATE --> time_t @ 1.1.1970 (time_t epoch)
//    SYSTEMTIME st = TTEpochAsST();
//    DATE od = 0;
//    HRESULT hr = TimeConvert(st, od);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(od != 0.0); // od contains time_t epoch
//    time_t tt = -1;
//    hr = TimeConvert(od, tt);     // actual converting calculation
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(tt == 0);
//    // inverse calculation
//    DATE od2 = 0.0;
//    hr = TimeConvert(tt, od2);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(od2 == od);
//    // ----- Test @ 1.1.2002
//    ZeroMemory(&st, sizeof(st));
//    st.wDay = 1;
//    st.wMonth = 1;
//    st.wYear = 2002;
//    hr = TimeConvert(st, od);
//    _ASSERTE(!FAILED(hr));
//    hr = TimeConvert(od, tt);  // the actual conversion
//    _ASSERTE(!FAILED(hr));
//    // Convert to tm so we can check it
//    struct tm tm = { 0 };
//    hr = TimeConvert(tt, tm);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(tm.tm_hour == 0);
//    _ASSERTE(tm.tm_mday == 1);
//    _ASSERTE(tm.tm_min  == 0);
//    _ASSERTE(tm.tm_mon  == 0);
//    _ASSERTE(tm.tm_sec  == 0);
//    _ASSERTE(tm.tm_year == 2002 - 1900);
//    // inverse calculation
//    od2 = 0.0;
//    hr = TimeConvert(tt, od2);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(od2 == od);
//}
//
//
//
//void TestTimeRef_Calc_ft2tt()
//{
//    // ----- Test Calculation FILETIME --> time_t @ 1.1.1970 (time_t epoch)
//    SYSTEMTIME st = TTEpochAsST();
//    FILETIME ft = { 0 };
//    HRESULT hr = TimeConvert(st, ft);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(ft.dwLowDateTime != 0 || ft.dwHighDateTime != 0.0); // od contains time_t epoch
//    time_t tt = -1;
//    hr = TimeConvert(ft, tt);     // actual converting calculation to test
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(tt == 0);
//    // inverse calculation
//    FILETIME ft2 = { 0  };
//    hr = TimeConvert(tt, ft2);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(ft2.dwLowDateTime  == ft.dwLowDateTime &&
//             ft2.dwHighDateTime == ft.dwHighDateTime);
//    // ----- Test @ 1.1.2002
//    ZeroMemory(&st, sizeof(st));
//    st.wDay = 1;
//    st.wMonth = 1;
//    st.wYear = 2002;
//    hr = TimeConvert(st, ft);
//    _ASSERTE(!FAILED(hr));
//    hr = TimeConvert(ft, tt);  // the actual conversion
//    _ASSERTE(!FAILED(hr));
//    // Convert to tm so we can check it
//    struct tm tm = { 0 };
//    hr = TimeConvert(tt, tm);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(tm.tm_hour == 0);
//    _ASSERTE(tm.tm_mday == 1);
//    _ASSERTE(tm.tm_min  == 0);
//    _ASSERTE(tm.tm_mon  == 0);
//    _ASSERTE(tm.tm_sec  == 0);
//    _ASSERTE(tm.tm_year == 2002 - 1900);
//    // inverse calculation
//    ZeroMemory(&ft2, sizeof(ft2));
//    hr = TimeConvert(tt, ft2);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(ft2.dwLowDateTime  == ft.dwLowDateTime &&
//             ft2.dwHighDateTime == ft.dwHighDateTime);
//}
//
//
//void TestTimeRef_Calc_ft2od()
//{
//    // time_t epoch
//    SYSTEMTIME st = TTEpochAsST();
//    FILETIME ft = { 0 };
//    DATE od = 0;
//    HRESULT hr = TimeConvert(st, ft);
//    _ASSERTE(!FAILED(hr));
//    hr = TimeConvert(st, od);
//    _ASSERTE(!FAILED(hr));
//    DATE od2 = 0;
//    hr = TimeConvert(ft, od2);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(od2 == od);
//    FILETIME ft2 = { 0 };
//    hr = TimeConvert(od, ft2);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(ft2.dwHighDateTime == ft.dwHighDateTime &&
//             ft2.dwLowDateTime == ft.dwLowDateTime);
//    // 1.1.2002
//    ZeroMemory(&st, sizeof(st));
//    st.wDay = 1;
//    st.wMonth = 1;
//    st.wYear = 2002;
//    ZeroMemory(&ft, sizeof(ft));
//    od = 0;
//    hr = TimeConvert(st, ft);
//    _ASSERTE(!FAILED(hr));
//    hr = TimeConvert(st, od);
//    _ASSERTE(!FAILED(hr));
//    od2 = 0;;
//    hr = TimeConvert(ft, od2);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(od2 == od);
//    ZeroMemory(&ft2, sizeof(ft2));
//    hr = TimeConvert(od, ft2);
//    _ASSERTE(!FAILED(hr));
//    _ASSERTE(ft2.dwHighDateTime == ft.dwHighDateTime &&
//             ft2.dwLowDateTime == ft.dwLowDateTime);
//}
//
//
//SYSTEMTIME GetSystemTimeDMY(WORD day, WORD month, WORD year)
//{
//    SYSTEMTIME st = { 0 };
//    st.wDay = day;
//    st.wMonth = month;
//    st.wYear = year;
//    return st;
//}
//
//void CalcSignificantTimes()
//{
//    CString result, line;
//    DATE odMin = 0;
//    SYSTEMTIME st = GetSystemTimeDMY(1, 1, 100);
//    BOOL ok = SystemTimeToVariantTime(&st, &odMin);
//    result = _T("Minimum Date (1.1.100):");
//
//    if (!ok)
//        line = _T("(error)");
//    else
//        line.Format(_T("%.16f"), odMin);
//
//    result += line;
//    result += _T("\r\n");
//}
//
//
//void TestTimeRef()
//{
//    // primitive / system-provided conversions
//    TestTimeRef_tm_time_t();
//    TestTimeRef_time_t_2_tm();
//    TestTimeRef_st2vt();
//    TestTimeRef_vt2st();
//    TestTimeRef_st2ft();
//    TestTimeRef_ft2st();
//    TestTimeRef_tm2od();
//    TestTimeRef_Calc_od2tt();
//    TestTimeRef_Calc_ft2tt();
//    TestTimeRef_Calc_ft2od();
//    CalcSignificantTimes();
//}
//
//
//void TestFmtUTCOffset()
//{
//    _ASSERTE(FmtUTCOffset(60 * 60) == _T("+1"));
//    _ASSERTE(FmtUTCOffset(120 * 60) == _T("+2"));
//    _ASSERTE(FmtUTCOffset(-120 * 60) == _T("-2"));
//    _ASSERTE(FmtUTCOffset(-150 * 60) == _T("-2:30"));
//    TCHAR buffer[8];
//    _ASSERTE(FmtUTCOffset(buffer, 8, 60 * 60) > 0);
//    _ASSERTE(_tcscmp(buffer, _T("+1")) == 0);
//    _ASSERTE(FmtUTCOffset(buffer, 8, 195 * 60) > 0);
//    _ASSERTE(_tcscmp(buffer, _T("+3:15")) == 0);
//    _ASSERTE(FmtUTCOffset(buffer, 8, -120 * 60) > 0);
//    _ASSERTE(_tcscmp(buffer, _T("-2")) == 0);
//    _ASSERTE(FmtUTCOffset(buffer, 8, -150 * 60) > 0);
//    _ASSERTE(_tcscmp(buffer, _T("-2:30")) == 0);
//}
//} // namespace test


#endif // __CACTUS_CACTUS_DATE_H__
