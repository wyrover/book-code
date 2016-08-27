/*!
* \file cactus_string.h
* \brief 常用字符串函数声明
*
* 常用字符串函数声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_STRING_H__
#define __CACTUS_CACTUS_STRING_H__

#include "cactus/cactus_types.h"
#include "cactus/icu_utf.h"
#include <algorithm>
#include <atlstr.h>


namespace cactus
{
#define WHITESPACE_UNICODE \
    0x0009, /* <control-0009> to <control-000D> */ \
    0x000A,                                        \
    0x000B,                                        \
    0x000C,                                        \
    0x000D,                                        \
    0x0020, /* Space */                            \
    0x0085, /* <control-0085> */                   \
    0x00A0, /* No-Break Space */                   \
    0x1680, /* Ogham Space Mark */                 \
    0x180E, /* Mongolian Vowel Separator */        \
    0x2000, /* En Quad to Hair Space */            \
    0x2001,                                        \
    0x2002,                                        \
    0x2003,                                        \
    0x2004,                                        \
    0x2005,                                        \
    0x2006,                                        \
    0x2007,                                        \
    0x2008,                                        \
    0x2009,                                        \
    0x200A,                                        \
    0x200C, /* Zero Width Non-Joiner */            \
    0x2028, /* Line Separator */                   \
    0x2029, /* Paragraph Separator */              \
    0x202F, /* Narrow No-Break Space */            \
    0x205F, /* Medium Mathematical Space */        \
    0x3000, /* Ideographic Space */                \
    0


const wchar_t kWhitespaceWide[] = {
    WHITESPACE_UNICODE
};


const char16 kWhitespaceUTF16[] = {
    WHITESPACE_UNICODE
};

const char kWhitespaceASCII[] = {
    0x09, // <control-0009> to <control-000D>
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x20, // Space
    0
};

const char kUtf8ByteOrderMark[] = "\xEF\xBB\xBF";

// Locale independent ctype (applied to the ASCII characters only)
// isctype/iswctype affects the some non-ASCII characters.
typedef WCHAR TBYTE , *PTBYTE ;
inline int cisctype(TBYTE c, int type)
{
    return (c & (~0x7F)) ? 0 : _isctype(c, type);
}

#define cisalpha(c)     cisctype(c, _ALPHA)
#define cisalnum(c)     cisctype(c, _ALPHA | _DIGIT)
#define cisdigit(c)     cisctype(c, _DIGIT)
#define cisxdigit(c)    cisctype(c, _HEX)
#define cisupper(c)     cisctype(c, _UPPER)
#define cislower(c)     cisctype(c, _LOWER)
#define cisprint(c)     cisctype(c, _ALPHA | _BLANK | _DIGIT | _PUNCT)
#define cisspace(c)     cisctype(c, _SPACE)

#define FILE_PATH_LITERAL(x)    L ## x
#define PRFilePath              "ls"
#define PRFilePathLiteral       L"%ls"
#define WCHAR_T_IS_UTF16
#define snprintf _snprintf
#define snwprintf _snwprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define wcscasecmp _wcsicmp
#define wcsncasecmp _wcsnicmp
#define stristr     StrStrIA
#define wcsistr     StrStrIW


// The results of toupper/tolower are implementations dependent (see below), though the test results are OK in VS2008's CRT.
// MDSN: In order for toupper to give the expected results, __isascii and islower must both return nonzero.
// Linux (man page): The value returned is that of the converted letter, or c if the conversion was not possible. (CONFORMING TO C89, C99, 4.3BSD.)
inline TCHAR ctoupper(TBYTE c)
{
    return cislower(c) ? (c & ~0x20) : c;
}
inline TCHAR ctolower(TBYTE c)
{
    return cisupper(c) ? (c | 0x20) : c;
}

#if !defined NOCASE_WCSCMP_FUNC && !defined wcscasecmp
inline int wcscasecmp(const wchar_t *s1, const wchar_t *s2)
{
    std::wint_t a, b;

    for (; a = std::towlower(*s1), b = std::towlower(*s2), a && a == b; ++s1, ++s2);

    return a - b;
}
#endif

#if !defined NOCASE_WCSNCMP_FUNC && !defined wcsncasecmp
inline int wcsncasecmp(const wchar_t *s1, const wchar_t *s2, std::size_t n)
{
    std::wint_t a = 0, b = 0;

    for (; n && (a = std::towlower(*s1), b = std::towlower(*s2), a && a == b); ++s1, ++s2, --n);

    return a - b;
}
#endif


inline int stricmp(const std::string& a, const std::string& b)
{
    return strcasecmp(a.c_str(), b.c_str());
}
inline int strnicmp(const std::string& a, const std::string& b, size_t n)
{
    return strncasecmp(a.c_str(), b.c_str(), n);
}
inline int wcsicmp(const std::wstring& a, const std::wstring& b)
{
    return wcscasecmp(a.c_str(), b.c_str());
}
inline int wcsnicmp(const std::wstring& a, const std::wstring& b, size_t n)
{
    return wcsncasecmp(a.c_str(), b.c_str(), n);
}

inline int strncmp16(const char16* s1, const char16* s2, size_t count)
{
    return ::wcsncmp(s1, s2, count);
}

template <typename CHAR>
size_t lcpyT(CHAR* dst, const CHAR* src, size_t dst_size)
{
    for (size_t i = 0; i < dst_size; ++i) {
        if ((dst[i] = src[i]) == 0)  // We hit and copied the terminating NULL.
            return i;
    }

    // We were left off at dst_size.  We over copied 1 byte.  Null terminate.
    if (dst_size != 0)
        dst[dst_size - 1] = 0;

    // Count the rest of the |src|, and return it's length in characters.
    while (src[dst_size]) ++dst_size;

    return dst_size;
}

inline size_t strlcpy(char* dst, const char* src, size_t dst_size)
{
    return lcpyT<char>(dst, src, dst_size);
}

inline size_t wcslcpy(wchar_t* dst, const wchar_t* src, size_t dst_size)
{
    return lcpyT<wchar_t>(dst, src, dst_size);
}

// 一种节省string拷贝的办法, 解决写数据wchar_t[]到std::wstring中的问题.
// 预留足够的空间并设置字符长度为正确的值保证.length()属性正常.
//
// reserve()分配空间需要在末尾多加一个空结束符. 这样做是因为resize()不能
// 保证预留空结束符. 调用resize()只是为了改变string的'length'成员.
//
// 性能: 调用wide.resize()是线性时间复杂度, 因为需要用空字符填充缓冲区.
// 这样做是因为需要改变字符串的实际长度(直接写缓冲区不会改变实际长度).
// 或许有常量时间复杂度改变string长度的办法.
template<class string_type>
inline typename string_type::value_type* WriteInto(string_type* str,
        size_t length_with_null)
{
    str->reserve(length_with_null);
    str->resize(length_with_null - 1);
    return &((*str)[0]);
}


// Hack to convert any char-like type to its unsigned counterpart.
// For example, it will convert char, signed char and unsigned char to unsigned
// char.
template<typename T>
struct ToUnsigned {
    typedef T Unsigned;
};

template<>
struct ToUnsigned<char> {
    typedef unsigned char Unsigned;
};

template<>
struct ToUnsigned<signed char> {
    typedef unsigned char Unsigned;
};

template<>
struct ToUnsigned<wchar_t> {
#if defined(WCHAR_T_IS_UTF16)
    typedef unsigned short Unsigned;
#elif defined(WCHAR_T_IS_UTF32)
    typedef uint32_t Unsigned;
#endif
};

template<>
struct ToUnsigned<short> {
    typedef unsigned short Unsigned;
};


template<class STR>
static bool DoIsStringASCII(const STR& str)
{
    for (size_t i = 0; i < str.length(); i++) {
        typename ToUnsigned<typename STR::value_type>::Unsigned c = str[i];

        if (c > 0x7F)
            return false;
    }

    return true;
}

inline bool IsValidCodepoint(uint32 code_point)
{
    // 不包含高代理码点([0xD800, 0xDFFF])和大于0x10FFFF(允许的最大码点)
    // 的码点.
    // 非字符和未指派码点是允许的.
    return code_point < 0xD800u ||
           (code_point >= 0xE000u && code_point <= 0x10FFFFu);
}

inline bool IsValidCharacter(uint32 code_point)
{
    // 合法码点集合中排除所有非字符(U+FDD0..U+FDEF, 和所有
    // 以0xFFFE结尾的码点).
    return code_point < 0xD800u || (code_point >= 0xE000u &&
                                    code_point < 0xFDD0u) || (code_point > 0xFDEFu &&
                                            code_point <= 0x10FFFFu && (code_point & 0xFFFEu) != 0xFFFEu);
}

inline bool IsStringASCII(const string16& str)
{
    return DoIsStringASCII(str);
}

inline bool IsStringASCII(const std::string& str)
{
    return DoIsStringASCII(str);
}

inline bool IsStringUTF8(const std::string& str)
{
    const char* src = str.data();
    int32 src_len = static_cast<int32>(str.length());
    int32 char_index = 0;

    while (char_index < src_len) {
        int32 code_point;
        CBU8_NEXT(src, char_index, src_len, code_point);

        if (!IsValidCharacter(code_point)) {
            return false;
        }
    }

    return true;
}


// ASCII特定的tolower.
template<class Char>
inline Char ToLowerASCII(Char c)
{
    return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
}

template<class str>
inline void StringToLowerASCII(str* s)
{
    for (typename str::iterator i = s->begin(); i != s->end(); ++i) {
        *i = ToLowerASCII(*i);
    }
}

template<class str>
inline str StringToLowerASCII(const str& s)
{
    // 针对std::string和std::wstring
    str output(s);
    StringToLowerASCII(&output);
    return output;
}

inline std::string WideToASCII(const std::wstring& wide)
{
    return std::string(wide.begin(), wide.end());
}

inline std::wstring ASCIIToWide(const std::string& ascii)
{
    return std::wstring(ascii.begin(), ascii.end());
}


inline string8 toLower(const string8& str)
{
    string8 Result = str;
    std::transform(Result.begin(), Result.end(), Result.begin(), ::tolower);
    return Result;
}

inline string16 toLower(const string16& str)
{
    string16 Result = str;
    std::transform(Result.begin(), Result.end(), Result.begin(), ::towlower);
    return Result;
}

inline string8 toupper(const string8& str)
{
    string8 Result = str;
    std::transform(Result.begin(), Result.end(), Result.begin(), ::toupper);
    return Result;
}

inline string16 toupper(const string16& str)
{
    string16 Result = str;
    std::transform(Result.begin(), Result.end(), Result.begin(), ::towupper);
    return Result;
}

template<typename STR>
inline bool RemoveCharsT(const STR& input,
                         const typename STR::value_type remove_chars[],
                         STR* output)
{
    bool removed = false;
    size_t found;
    *output = input;
    found = output->find_first_of(remove_chars);

    while (found != STR::npos) {
        removed = true;
        output->replace(found, 1, STR());
        found = output->find_first_of(remove_chars, found);
    }

    return removed;
}

inline bool RemoveChars(const std::wstring& input,
                        const wchar_t remove_chars[],
                        std::wstring* output)
{
    return RemoveCharsT(input, remove_chars, output);
}

inline bool RemoveChars(const std::string& input,
                        const char remove_chars[],
                        std::string* output)
{
    return RemoveCharsT(input, remove_chars, output);
}


enum TrimPositions {
    TRIM_NONE     = 0,
    TRIM_LEADING  = 1 << 0,
    TRIM_TRAILING = 1 << 1,
    TRIM_ALL      = TRIM_LEADING | TRIM_TRAILING,
};


template<typename STR>
inline TrimPositions TrimStringT(const STR& input,
                                 const typename STR::value_type trim_chars[],
                                 TrimPositions positions,
                                 STR* output)
{
    // 根据移除选项positions查找两端边界.
    const typename STR::size_type last_char = input.length() - 1;
    const typename STR::size_type first_good_char = (positions & TRIM_LEADING) ?
            input.find_first_not_of(trim_chars) : 0;
    const typename STR::size_type last_good_char = (positions & TRIM_TRAILING) ?
            input.find_last_not_of(trim_chars) : last_char;

    // 当字符串所有字符都是空白, 根据调用传入的positions返回TrimPositions.
    // 对于空输入没有去除任何空白, 但仍需要对output串调用clear.
    if (input.empty() || (first_good_char == STR::npos) || (last_good_char == STR::npos)) {
        bool input_was_empty = input.empty();
        output->clear();
        return input_was_empty ? TRIM_NONE : positions;
    }

    // 移除空白.
    *output = input.substr(first_good_char, last_good_char - first_good_char + 1);
    // 返回两端哪边移除过.
    return static_cast<TrimPositions>(
               ((first_good_char == 0) ? TRIM_NONE : TRIM_LEADING) |
               ((last_good_char == last_char) ? TRIM_NONE : TRIM_TRAILING));
}


inline bool TrimString(const std::wstring& input,
                       const wchar_t trim_chars[],
                       std::wstring* output)
{
    return TrimStringT(input, trim_chars, TRIM_ALL, output) != TRIM_NONE;
}

inline bool TrimString(const std::string& input,
                       const char trim_chars[],
                       std::string* output)
{
    return TrimStringT(input, trim_chars, TRIM_ALL, output) != TRIM_NONE;
}

inline TrimPositions TrimWhitespaceASCII(const std::string& input,
        TrimPositions positions,
        std::string* output)
{
    return TrimStringT(input, kWhitespaceASCII, positions, output);
}

inline TrimPositions TrimWhitespace(const std::wstring& input,
                                    TrimPositions positions,
                                    std::wstring* output)
{
    return TrimStringT(input, kWhitespaceWide, positions, output);
}


inline TrimPositions TrimWhitespace(const std::string& input,
                                    TrimPositions positions,
                                    std::string* output)
{
    return TrimWhitespaceASCII(input, positions, output);
}



inline string8& trim(string8& s)
{
    if (s.empty()) {
        return s;
    }

    string8::iterator c;

    // Erase whitespace before the string
    for (c = s.begin(); c != s.end() && iswspace(*c++););

    s.erase(s.begin(), --c);

    // Erase whitespace after the string
    for (c = s.end(); c != s.begin() && iswspace(*--c););

    s.erase(++c, s.end());
    return s;
}

inline string8 trim(const string8& str)
{
    string8 Result = str;
    Result.erase(0, Result.find_first_not_of(" /t/n/r")).erase(Result.find_last_not_of(" /t/n/r") + 1);
    return Result;
}

inline string16 trim(const string16& str)
{
    string16 Result = str;
    Result.erase(0, Result.find_first_not_of(L" /t/n/r")).erase(Result.find_last_not_of(L" /t/n/r") + 1);
    return Result;
}

inline string8 trimleft(const string8& str)
{
    string8 Result = str;
    Result.erase(0, Result.find_first_not_of(" /t/n/r"));
    return Result;
}

inline string16 trimleft(const string16& str)
{
    string16 Result = str;
    Result.erase(0, Result.find_first_not_of(L" /t/n/r"));
    return Result;
}

inline string8 trimright(const string8& str)
{
    string8 Result = str;
    Result.erase(Result.find_last_not_of(" /t/n/r") + 1);
    return Result;
}


inline string16 trimright(const string16& str)
{
    string16 Result = str;
    Result.erase(Result.find_last_not_of(L" /t/n/r") + 1);
    return Result;
}

inline std::ostream& operator<<(std::ostream& out, const wchar_t* wstr)
{
    if (!wstr || !wstr[0])
        return out;

    // compute the length of the buffer we'll need
    int charcount = WideCharToMultiByte(CP_UTF8, 0, wstr, -1,
                                        NULL, 0, NULL, NULL);

    if (charcount == 0)
        return out;

    // convert
    std::vector<char> buf(charcount);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &buf[0], charcount, NULL, NULL);
    return out << &buf[0];
}



template<typename STR>
inline static STR JoinStringT(const std::vector<STR>& parts,
                              typename STR::value_type sep)
{
    if (parts.size() == 0) return STR();

    STR result(parts[0]);
    typename std::vector<STR>::const_iterator iter = parts.begin();
    ++iter;

    for (; iter != parts.end(); ++iter) {
        result += sep;
        result += *iter;
    }

    return result;
}

inline string8 JoinString(const std::vector<string8>& parts, char sep)
{
    return JoinStringT(parts, sep);
}

inline string16 JoinString(const std::vector<string16>& parts, char16 sep)
{
    return JoinStringT(parts, sep);
}


template<typename STR>
inline static void SplitStringT(const STR& str,
                                const typename STR::value_type s,
                                bool trim_whitespace,
                                std::vector<STR>* r)
{
    size_t last = 0;
    size_t i;
    size_t c = str.size();

    for (i = 0; i <= c; ++i) {
        if (i == c || str[i] == s) {
            size_t len = i - last;
            STR tmp = str.substr(last, len);

            if (trim_whitespace) {
                STR t_tmp;
                TrimWhitespace(tmp, TRIM_ALL, &t_tmp);
                r->push_back(t_tmp);
            } else {
                r->push_back(tmp);
            }

            last = i + 1;
        }
    }
}

inline void SplitString(const std::wstring& str,
                        wchar_t c, std::vector<std::wstring>* r)
{
    SplitStringT(str, c, true, r);
}

inline void SplitString(const std::string& str,
                        char c, std::vector<std::string>* r)
{
    //DCHECK(c>=0 && c<0x7F);
    SplitStringT(str, c, true, r);
}

inline bool SplitStringIntoKeyValues(
    const std::string& line,
    char key_value_delimiter,
    std::string* key,
    std::vector<std::string>* values)
{
    key->clear();
    values->clear();
    // 查找key.
    size_t end_key_pos = line.find_first_of(key_value_delimiter);

    if (end_key_pos == std::string::npos) {
        std::cout << "cannot parse key from line: " << line;
        return false; // 没有key.
    }

    key->assign(line, 0, end_key_pos);
    // 查找values.
    std::string remains(line, end_key_pos, line.size() - end_key_pos);
    size_t begin_values_pos = remains.find_first_not_of(key_value_delimiter);

    if (begin_values_pos == std::string::npos) {
        std::cout << "cannot parse value from line: " << line;
        return false; // 没有value.
    }

    std::string values_string(remains, begin_values_pos,
                              remains.size() - begin_values_pos);
    // 添加到vector.
    values->push_back(values_string);
    return true;
}

inline bool SplitStringIntoKeyValuePairs(
    const std::string& line,
    char key_value_delimiter,
    char key_value_pair_delimiter,
    std::vector<std::pair<std::string, std::string> >* kv_pairs)
{
    kv_pairs->clear();
    std::vector<std::string> pairs;
    SplitString(line, key_value_pair_delimiter, &pairs);
    bool success = true;

    for (size_t i = 0; i < pairs.size(); ++i) {
        // 空键值对. SplitStringIntoKeyValues不识别空行, 继续下一个键值对.
        if (pairs[i].empty()) {
            continue;
        }

        std::string key;
        std::vector<std::string> value;

        if (!SplitStringIntoKeyValues(pairs[i],
                                      key_value_delimiter, &key, &value)) {
            // 这里不返回, 允许键不关联值; 仅标记切分失败.
            success = false;
        }

        //DCHECK_LE(value.size(), 1U);
        kv_pairs->push_back(make_pair(key, value.empty() ? "" : value[0]));
    }

    return success;
}

template<typename STR>
inline static void SplitStringUsingSubstrT(const STR& str,
        const STR& s, std::vector<STR>* r)
{
    typename STR::size_type begin_index = 0;

    while (true) {
        const typename STR::size_type end_index = str.find(s, begin_index);

        if (end_index == STR::npos) {
            const STR term = str.substr(begin_index);
            STR tmp;
            TrimWhitespace(term, TRIM_ALL, &tmp);
            r->push_back(tmp);
            return;
        }

        const STR term = str.substr(begin_index, end_index - begin_index);
        STR tmp;
        TrimWhitespace(term, TRIM_ALL, &tmp);
        r->push_back(tmp);
        begin_index = end_index + s.size();
    }
}

inline void SplitStringUsingSubstr(const string16& str,
                                   const string16& s, std::vector<string16>* r)
{
    SplitStringUsingSubstrT(str, s, r);
}

inline void SplitStringUsingSubstr(const std::string& str,
                                   const std::string& s, std::vector<std::string>* r)
{
    SplitStringUsingSubstrT(str, s, r);
}

inline void SplitStringDontTrim(const string16& str,
                                char16 c, std::vector<string16>* r)
{
    //DCHECK(CBU16_IS_SINGLE(c));
    SplitStringT(str, c, false, r);
}

inline void SplitStringDontTrim(const std::string& str,
                                char c, std::vector<std::string>* r)
{
    //DCHECK(IsStringUTF8(str));
    //DCHECK(c>=0 && c<0x7F);
    SplitStringT(str, c, false, r);
}




inline bool startswith(const string8& str, const string8& substr)
{
    return str.find(substr) == 0;
}

inline bool startswith(const string16& str, const string16& substr)
{
    return str.find(substr) == 0;
}

inline bool endswith(const string8& str, const string8& substr)
{
    return str.rfind(substr) == (str.length() - substr.length());
}

inline bool endswith(const string16& str, const string16& substr)
{
    return str.rfind(substr) == (str.length() - substr.length());
}

inline bool equalsIgnoreCase(const string8& str1, const string8& str2)
{
    return toLower(str1) == toLower(str2);
}

inline bool equalsIgnoreCase(const string16& str1, const string16& str2)
{
    return toLower(str1) == toLower(str2);
}

inline bool icontains(const string8& str1, const string8& str2)
{
#ifdef stristr
    return stristr(str1.c_str(), str2.c_str()) != NULL;
#else
    return toLower(str1).find(toLower(str2)) != string16::npos;
#endif
}

inline bool icontains(const string16& str1, const string16& str2)
{
#ifdef wcsistr
    return wcsistr(str1.c_str(), str2.c_str()) != NULL;
#else
    return toLower(str1).find(toLower(str2)) != string16::npos;
#endif
}


void ExpandString(stringlist& string_list, const string8& string, const char delimiter = ',');

void JoinString(string8& string, const stringlist& string_list, const char delimiter = ',');


/*! @brief 分割字符串
 *
 * @param[in] text 待分割字符串
 * @param[in] separator 分隔符
 * @return 返回分割后字符串数组
 */
stringlist split(const std::string& text, char separator);

/*! @brief 分割字符串
 *
 * @param[in] text 待分割字符串
 * @param[in] separator 分隔符
 * @return 返回分割后字符串数组
 */
string16list split(const string16& text, char16 separator);

/*! @brief n个字符后换行
 *
 * @param[in] text 字符串
 * @param[in] wrapColumn n个字符
 * @return 返回字符串
 */
std::string wrap(const std::string& text, int wrapColumn);

/*! @brief n个字符后换行
 *
 * @param[in] text 字符串
 * @param[in] wrapColumn n个字符
 * @return 返回字符串
 */
string16 wrap(const string16& text, int wrapColumn);


inline int SafeStringLen(LPCSTR psz)
{
    // returns length in bytes
    return (psz != NULL) ? int(strlen(psz)) : 0;
}

inline int SafeStringLen(LPCWSTR psz)
{
    // returns length in wchar_ts
    return (psz != NULL) ? int(wcslen(psz)) : 0;
}




inline unsigned int CharCount(const std::string &str, const char c)
{
    unsigned int count = 0;

    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == c) {
            count++;
        }
    }

    return count;
}

inline std::string StringReplace(std::string str, const std::string &strReplace, const std::string &strWith, unsigned int count = 0)
{
    size_t pos = str.find(strReplace);

    for (unsigned int i = 0; pos != std::string::npos && (i < count || count == 0); i++) {
        str.replace(pos, strReplace.size(), strWith);
        pos = str.find(strReplace);
    }

    return str;
}

inline string16 StringReplace(string16 str, const string16& strReplace, const string16& strWith, unsigned int count = 0)
{
    size_t pos = str.find(strReplace);

    for (unsigned int i = 0; pos != std::string::npos && (i < count || count == 0); i++) {
        str.replace(pos, strReplace.size(), strWith);
        pos = str.find(strReplace);
    }

    return str;
}

LPTSTR lstrcasestr(LPCTSTR phaystack, LPCTSTR pneedle);
LPTSTR tcscasestr(LPCTSTR phaystack, LPCTSTR pneedle);


#ifdef UNICODE
    #define tmemcpy         wmemcpy
    #define tmemmove        wmemmove
    #define tmemset         wmemset
    #define tmemcmp         wmemcmp
    #define tmalloc(c)      ((LPTSTR) malloc((c) << 1))
    #define trealloc(p, c)  ((LPTSTR) realloc((p), (c) << 1))
    #define talloca(c)      ((LPTSTR) _alloca((c) << 1))
#else
    #define tmemcpy         (char*)memcpy
    #define tmemmove        memmove
    #define tmemset         memset
    #define tmemcmp         memcmp
    #define tmalloc(c)      ((LPTSTR) malloc(c))
    #define trealloc(p, c)  ((LPTSTR) realloc((p), (c)))
    #define talloca(c)      ((LPTSTR) _alloca(c))
#endif

enum StringCaseSenseType {SCS_INSENSITIVE, SCS_SENSITIVE, SCS_INSENSITIVE_LOCALE, SCS_INSENSITIVE_LOGICAL, SCS_INVALID};

#define ltolower(ch) (TBYTE)CharLower((LPTSTR)(TBYTE)(ch))  // "L" prefix stands for "locale", like lstrcpy.
#define ltoupper(ch) (TBYTE)CharUpper((LPTSTR)(TBYTE)(ch))

#define tcsstr2(haystack, needle, string_case_sense) ((string_case_sense) == SCS_INSENSITIVE ? tcscasestr(haystack, needle) \
        : ((string_case_sense) == SCS_INSENSITIVE_LOCALE ? lstrcasestr(haystack, needle) : _tcsstr(haystack, needle)))


UINT StrReplace(LPTSTR aHaystack, LPTSTR aOld, LPTSTR aNew, StringCaseSenseType aStringCaseSense
                , UINT aLimit = UINT_MAX, size_t aSizeLimit = -1, LPTSTR *aDest = NULL, size_t *aHaystackLength = NULL);
size_t PredictReplacementSize(ptrdiff_t aLengthDelta, int aReplacementCount, int aLimit, size_t aHaystackLength
                              , size_t aCurrentLength, size_t aEndOffsetOfCurrMatch);

void BSTR2CString(CString* pStr, BSTR bstr);

CString ToString(LPCTSTR fmt, ...);

CString ToString(UINT fmtid, ...);

string16 format_string(const char16* fmt, ...);
string8 format_string(const char* fmt, ...);


int vsnprintf(char* buffer, size_t size, const char* format, va_list arguments);

int vswprintf(char16* buffer, size_t size, const char16* format, va_list arguments);

CString Format(double dVal, int nDecPlaces);

CString Format(DWORD dwVal);

CString FormatCost(double dCost);

string16 FormatMessage(unsigned messageid);

string16 FormatLastWin32Error();

string8 HexEncode(const void* bytes, size_t size);

string8 toOctalString(uint n);




/*! @brief 格式化字节数
 *
 * @param[in] aBytes 字节数
 * @return 返回格式化字节数
 */
string16 formatBytesW(int64_t aBytes);

CString GetSizeStr(__int64 nSize);

CString CalcStrFormatByteSize(LONGLONG qdw, BOOL bLimit = FALSE);

string16 URLEncode2(const string16& url);

string8 EscapeUrl(const string8& s);

string8 char2hex(char dec);

/*! @brief URL编码
 *
 * @param[in] c url字符串
 * @return 返回URL编码后的字符串
 */
string8 urlencode(const string8 &c);

inline int IndexStr(const string16& s, const std::vector<string16> values)
{
    int result = -1;

    for (int i = 0; i < values.size(); ++i) {
        if (wcscmp(s.c_str(), values[i].c_str()) == 0) {
            result = i;
            break;
        }
    }

    return result;
}

inline string16 LeftStr(const string16& s, const int& count)
{
    return s.substr(0, count);
}

inline string16 RightStr(const string16& s, const int& count)
{
    return s.substr(s.length() + 1 - count, count);
}

inline string16 MidStr(const string16& s, const int& start, const int& count)
{
    return s.substr(start, count);
}

inline string16::size_type AnsiPos(const string16& str, const string16& substr)
{
    return str.find(substr);
}

inline bool ContainsStr(const string16& str, const string16& substr)
{
    return AnsiPos(str, substr) != string16::npos;
}


BOOL IsGuid(LPCTSTR szGuid);
BOOL SameGuids(const GUID &guid1, const GUID &guid2);
BOOL IsBeginGuiD(LPCWSTR lpStr);
BOOL GuidIsNull(const GUID &guid);
BOOL GuidFromString(const string16& data, GUID &guid);
BOOL GuidFromString(LPCTSTR szGuid, GUID &guid);
BOOL GuidToString(const GUID &guid, CString &sGuid);
string16 GuidToString(const GUID &guid);
int StringFromGUID2(REFGUID rguid, LPOLESTR lptsz, int cchMax);
BOOL wUUIDFromString(LPCWSTR lpsz, GUID * pguid);
BOOL wGUIDFromString(LPCWSTR lpsz, GUID * pguid);
BOOL HexStringToDword(LPCWSTR FAR& lpsz, DWORD FAR& Value,
                             int cDigits, WCHAR chDelim);



/*! @brief 二进制转十六进制(字符串)
 *
 * @param[in] sBIN_INPUT 二进制字符串
 * @return 返回16进制字符串
 */
CStringA BIN2HEX(CStringA sBIN_INPUT);

/*! @brief 十六进制转换为二进制(字符穿)
 *
 * @param[in] sHEX_INPUT 16进制字符串
 * @return 返回2进制字符串
 */
CStringA HEX2BIN(CStringA sHEX_INPUT);

CString ConvertStringToHex(CString sText, int iBlockSize = 1);
CString ConvertHexToString(CString sText);

string16 GetHiString();

string16 GetSysDateString();

ULARGE_INTEGER ParseVersion( LPCTSTR szVer );
BOOL EvalCompareResult( INT nRet, LPCTSTR sOpt, BOOL &bResult );
BOOL VersionInRange(ULARGE_INTEGER lVersion, LPCTSTR lpszLowerVersion, LPCTSTR lpszUpperVersion );
BOOL CompareVersion( const ULARGE_INTEGER &v1, const ULARGE_INTEGER &v2, LPCTSTR sOpt, BOOL &bResult );

int CompareVersion(const CString& strVer1, const CString& strVer2);
int CompareVersions(CString sVersion1, CString sVersion2);
//-----------------------------------------------------------------------------
// Name: CompareLargeInts()
// Desc: Returns 1 if ullParam1 > ullParam2
//       Returns 0 if ullParam1 = ullParam2
//       Returns -1 if ullParam1 < ullParam2
//-----------------------------------------------------------------------------
int CompareLargeInts(ULARGE_INTEGER ullParam1, ULARGE_INTEGER ullParam2);

ULARGE_INTEGER MakeInt64( WORD a, WORD b, WORD c, WORD d );

BOOL IsNumber( LPTSTR lpszName );


#if defined(_MSC_VER) && defined(_DEBUG)
    void OutputDebugStringFormat(LPCTSTR fmt, ...);
#endif

void CCLog(const char* pszFormat, ...);


class StringUtil
{
public:
    static const utf8string sEmpty;
    static const string16 sWEmpty;
    static const char* getNotNullString(const char* strValue)
    {
        return (strValue ? strValue : sEmpty.c_str());
    }

    static const char16* getNotNullString(const char16* strValue)
    {
        return (strValue ? strValue : sWEmpty.c_str());
    }


    template<typename StringPtrType>
    static bool NullOrEmpty(StringPtrType strPtr)
    {
        return !(strPtr && strPtr[0]);
    }

    template<typename StringPtrType>
    static bool NotNullOrEmpty(StringPtrType strPtr)
    {
        return (strPtr && strPtr[0]);
    }

    template<typename StringType>
    static void Trim(StringType& toTrim)
    {
        TrimRight(toTrim);
        TrimLeft(toTrim);
    }

    template<typename StringType>
    static void TrimLeft(StringType& toTrim)
    {
        while (::isspace(toTrim[0]))
            toTrim.erase(toTrim.begin(), toTrim.begin() + 1);
    }

    template<typename StringType>
    static void TrimRight(StringType& toTrim)
    {
        while (::isspace(toTrim[toTrim.length() - 1]))
            toTrim.erase(toTrim.length() - 1);
    }

    template<typename StringType>
    static void replace(StringType& strBig, const StringType& strSrc, const StringType& strDst)
    {
        typedef typename StringType::size_type size_type;
        size_type pos = 0;
        size_type srcLen = strSrc.size();
        size_type dstLen = strDst.size();

        while ((pos = strBig.find(strSrc, pos)) != StringType::npos) {
            strBig.replace(pos, srcLen, strDst);
            pos += dstLen;
        }
    }
};

__declspec(selectany) const string16 StringUtil::sWEmpty = L"";
__declspec(selectany) const utf8string StringUtil::sEmpty = "";

class Hash
{
public:
    static void MD5(const unsigned char* message,
                    size_t messageSize,
                    unsigned char digest[16]);

    static void SHA1(const unsigned char* message,
                     size_t messageSize,
                     unsigned char digest[20]);

    BOOL GetFileMD5(const wchar_t* filename,
        BOOL bUpperCase,
        TCHAR *pszResult
        );
};



namespace digit_conversion
{

std::string ToHexString(int i);

std::string ToOctalString(int i);

std::string ToBinaryString(int i);

} // namespace digit_conversion
} // namespace cactus


//------------------------------------------------
// convert.h
/*! @brief cactus命名空间
 *
 * cactus库
 */
namespace cactus
{

/*
template <typename T>
inline std::string to_string(const T& arg)
{
    try
    {
        return boost::lexical_cast<std::string>(arg);
    }
    catch(boost::bad_lexical_cast& e)
    {
        return "";
    }
}*/

template<typename To, typename From>
inline To convert_to(const From& from)
{
    enum { not_supported = 1 / (1 == 0) }; // static_assert(false)
}
/*! @brief 单字节字符串转宽字节字符串
 *
 * @param[in] from 单字节字符串
 * @return 返回宽字节字符串
 */
template<> inline string16 convert_to(const std::string& from)
{
    int len = static_cast<int>(from.size() + 1);
    std::vector<char16> wideBuf(len);
    int ret = MultiByteToWideChar(CP_ACP, 0, from.c_str(), len, &wideBuf[0], len);

    if (ret == 0)
        return StringUtil::sWEmpty;
    else
        return string16(&wideBuf[0]);
}

template<> inline string16 convert_to(const int& from)
{
    return format_string(L"%d", from);
}

template<> inline string16 convert_to(const long& from)
{
    return format_string(L"%ld", from);
}

template<> inline string16 convert_to(const unsigned int& from)
{
    return format_string(L"%u", from);
}

template<> inline string16 convert_to(const unsigned long& from)
{
    return format_string(L"%lu", from);
}

template<> inline string16 convert_to(const float& from)
{
    return format_string(L"%.16g", from);
}

template<> inline string16 convert_to(const double& from)
{
    return format_string(L"%.16g", from);
}

template<> inline string16 convert_to(const char* const& from)
{
    int len = static_cast<int>(strlen(from) + 1);
    std::vector<char16> wideBuf(len);
    int ret = MultiByteToWideChar(CP_ACP, 0, from, len, &wideBuf[0], len);

    if (ret == 0)
        return StringUtil::sWEmpty;
    else
        return string16(&wideBuf[0]);
}


template<> inline std::string convert_to(const char16* const& from)
{
    int len = static_cast<int>(wcslen(from));
    int required_size = WideCharToMultiByte(CP_ACP, 0, from, (DWORD)len, NULL, 0, NULL, NULL);

    if (required_size == 0)
        return StringUtil::sEmpty;

    std::vector<char> buf(++required_size);
    int ret = WideCharToMultiByte(CP_ACP, 0, from, len, &buf[0], required_size, NULL, NULL);

    if (ret == 0)
        return StringUtil::sEmpty;
    else
        return std::string(&buf[0]);
}

/*! @brief 双字节字符串转单字节字符串
 *
 * @param[in] from 双字节字符串
 * @return 返回单字节字符串
 */
template<> inline std::string convert_to(const string16& from)
{
    int len = static_cast<int>(from.size());
    int required_size = WideCharToMultiByte(CP_ACP, 0, from.c_str(), (DWORD)len, NULL, 0, NULL, NULL);

    if (required_size == 0)
        return StringUtil::sEmpty;

    std::vector<char> buf(++required_size);
    int ret = WideCharToMultiByte(CP_ACP, 0, from.c_str(), len, &buf[0], required_size, NULL, NULL);

    if (ret == 0)
        return StringUtil::sEmpty;
    else
        return std::string(&buf[0]);
}


template<> inline CStringW convert_to(const CStringA& from)
{
    int required_size = from.GetLength();
    std::vector<char16> buf(++required_size);
    int ret = MultiByteToWideChar(CP_ACP, 0, LPCSTR(from), from.GetLength(), &buf[0], required_size);

    if (ret == 0)
        return StringUtil::sWEmpty.c_str();
    else
        return CStringW(&buf[0]);
}

template<> inline CStringA convert_to(const CStringW& from)
{
    int len = from.GetLength();
    int required_size = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(from), (DWORD)len, NULL, 0, NULL, NULL);

    if (required_size == 0)
        return StringUtil::sEmpty.c_str();

    std::vector<char> buf(++required_size);
    int ret = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(from), len, &buf[0], required_size, NULL, NULL);

    if (ret == 0)
        return StringUtil::sEmpty.c_str();
    else
        return CStringA(&buf[0]);
}

template<> inline int convert_to(const string16& from)
{
    return (int)std::wcstol(from.c_str(), NULL, 10);
}

template<> inline long convert_to(const string16& from)
{
    return (long)std::wcstol(from.c_str(), NULL, 10);
}

template<> inline unsigned int convert_to(const string16& from)
{
    return (unsigned int)std::wcstoul(from.c_str(), NULL, 10);
}

template<> inline unsigned long convert_to(const string16& from)
{
    return (unsigned long)std::wcstoul(from.c_str(), NULL, 10);
}

template<> inline float convert_to(const string16& from)
{
    return (float)std::wcstod(from.c_str(), NULL);
}

template<> inline double convert_to(const string16& from)
{
    return std::wcstod(from.c_str(), NULL);
}

/*! @brief 双字节字符串转UTF8字符串
 *
 * @param[in] string 双字节字符串
 * @return 返回UTF8字符串
 */
inline std::string to_utf8(const string16& string)
{
    int required_size =
        WideCharToMultiByte(CP_UTF8, 0,
                            string.c_str(), (int)string.size(),
                            NULL, 0, NULL, NULL);

    if (required_size == 0)
        return StringUtil::sEmpty;

    std::vector<char> buf(++required_size);
    WideCharToMultiByte(CP_UTF8, 0,
                        string.c_str(), (int)string.size(),
                        &buf[0], required_size,
                        NULL, NULL);
    return std::string(&buf[0]);
}

/*! @brief UTF8字符串转双字节字符串
 *
 * @param[in] string UTF8字符串
 * @return 返回双字节字符串
 */
inline string16 from_utf8(const std::string& string)
{
    int required_size =
        MultiByteToWideChar(CP_UTF8, 0,
                            string.c_str(), (int)string.size(),
                            NULL, 0);

    if (required_size == 0)
        return StringUtil::sWEmpty;

    std::vector<char16> buf(++required_size);
    MultiByteToWideChar(CP_UTF8, 0,
                        string.c_str(), (int)string.size(),
                        &buf[0], required_size);
    return string16(&buf[0]);
}

inline CStringA UnicodeToAnsi(CStringW strValueW)
{
    int required_size = ::WideCharToMultiByte(CP_ACP, NULL, strValueW, (int)wcslen(strValueW.GetBuffer(0)), NULL, 0, NULL, NULL);
    std::vector<char> buf(++required_size);
    ::WideCharToMultiByte(CP_ACP, NULL, strValueW.GetBuffer(0), (int)wcslen(strValueW.GetBuffer(0)), (LPSTR)&buf[0], required_size, NULL, NULL);
    strValueW.ReleaseBuffer();
    return CStringA(&buf[0]);
}

inline CStringW AnsiToUnicode(CStringA strValueA)
{
    int required_size = ::MultiByteToWideChar(CP_ACP, NULL, strValueA.GetBuffer(0), (int)strlen(strValueA.GetBuffer(0)), NULL, 0);
    std::vector<char16> buf(++required_size);
    ::MultiByteToWideChar(CP_ACP, NULL, strValueA.GetBuffer(0), (int)strlen(strValueA.GetBuffer(0)), &buf[0], required_size);
    strValueA.ReleaseBuffer();
    return CStringW(&buf[0]);
}
}


//------------------------------------------------
// string_help.h

namespace cactus
{
/// \brief String helper functions.
class StringHelp
{
public:
    static std::vector<std::string> split_text(
        const std::string &text,
        const std::string &split_string,
        bool skip_empty = true);

    /// \brief Wide char to utf-8
    ///
    /// \param value = Wide char value
    /// \return The UTF-8 String
    static std::string wchar_to_utf8(wchar_t value);

    /// \brief unicode char to utf-8
    ///
    /// \param value = unicode char value
    /// \return The UTF-8 String
    static std::string unicode_to_utf8(unsigned int value);

    /// \brief Trim
    ///
    /// \param text = String Ref
    ///
    /// \return Temp String
    static std::string trim(const std::string &text);

    /// \brief Compare
    ///
    /// \param a = String Ref8
    /// \param b = String Ref8
    /// \param case_insensitive = bool
    ///
    /// \return int
    static int compare(const std::string &a, const std::string &b, bool case_insensitive = false);

    /// \brief Compare
    ///
    /// \param a = String Ref16
    /// \param b = String Ref16
    /// \param case_insensitive = bool
    ///
    /// \return int
    static int compare(const std::wstring &a, const std::wstring &b, bool case_insensitive = false);

    /// \brief Text to upper
    ///
    /// \param s = String Ref
    ///
    /// \return Temp String
    static std::string text_to_upper(const std::string &s);

    /// \brief Local8 to upper
    ///
    /// \param s = String Ref8
    ///
    /// \return Temp String8
    static std::string local8_to_upper(const std::string &s);

    /// \brief Ucs2 to upper
    ///
    /// \param s = String Ref16
    ///
    /// \return Temp String16
    static std::wstring ucs2_to_upper(const std::wstring &s);

    /// \brief Text to lower
    ///
    /// \param s = String Ref
    ///
    /// \return Temp String
    static std::string text_to_lower(const std::string &s);

    /// \brief Local8 to lower
    ///
    /// \param s = String Ref8
    ///
    /// \return Temp String8
    static std::string local8_to_lower(const std::string &s);

    /// \brief Ucs2 to lower
    ///
    /// \param s = String Ref16
    ///
    /// \return Temp String16
    static std::wstring ucs2_to_lower(const std::wstring &s);

    /// \brief Float to text
    ///
    /// \param value = value
    /// \param num_decimal_places = value
    ///
    /// \return Temp String
    static std::string float_to_text(float value, int num_decimal_places = 6);

    /// \brief Float to local8
    ///
    /// \param value = value
    /// \param num_decimal_places = value
    ///
    /// \return Temp String8
    static std::string float_to_local8(float value, int num_decimal_places = 6);

    /// \brief Float to ucs2
    ///
    /// \param value = value
    /// \param num_decimal_places = value
    ///
    /// \return Temp String16
    static std::wstring float_to_ucs2(float value, int num_decimal_places = 6);

    /// \brief Text to float
    ///
    /// \param value = String Ref
    ///
    /// \return float
    static float text_to_float(const std::string &value);

    /// \brief Local8 to float
    ///
    /// \param value = String Ref8
    ///
    /// \return float
    static float local8_to_float(const std::string &value);

    /// \brief Ucs2 to float
    ///
    /// \param value = String Ref16
    ///
    /// \return float
    static float ucs2_to_float(const std::wstring &value);

    /// \brief Double to text
    ///
    /// \param value = value
    /// \param num_decimal_places = value
    ///
    /// \return Temp String
    static std::string double_to_text(double value, int num_decimal_places = 6);

    /// \brief Double to local8
    ///
    /// \param value = value
    /// \param num_decimal_places = value
    ///
    /// \return Temp String8
    static std::string double_to_local8(double value, int num_decimal_places = 6);

    /// \brief Double to ucs2
    ///
    /// \param value = value
    /// \param num_decimal_places = value
    ///
    /// \return Temp String16
    static std::wstring double_to_ucs2(double value, int num_decimal_places = 6);

    /// \brief Text to double
    ///
    /// \param value = String Ref
    ///
    /// \return double
    static double text_to_double(const std::string &value);

    /// \brief Local8 to double
    ///
    /// \param value = String Ref8
    ///
    /// \return double
    static double local8_to_double(const std::string &value);

    /// \brief Ucs2 to double
    ///
    /// \param value = String Ref16
    ///
    /// \return double
    static double ucs2_to_double(const std::wstring &value);

    /// \brief Int to text
    ///
    /// \param value = value
    ///
    /// \return Temp String
    static std::string int_to_text(int value);

    /// \brief Int to local8
    ///
    /// \param value = value
    ///
    /// \return Temp String8
    static std::string int_to_local8(int value);

    /// \brief Int to ucs2
    ///
    /// \param value = value
    ///
    /// \return Temp String16
    static std::wstring int_to_ucs2(int value);

    /// \brief Text to int
    ///
    /// \param value = String Ref
    /// \param base = value
    ///
    /// \return int
    static int text_to_int(const std::string &value, int base = 10);

    /// \brief Local8 to int
    ///
    /// \param value = String Ref8
    /// \param base = value
    ///
    /// \return int
    static int local8_to_int(const std::string &value, int base = 10);

    /// \brief Ucs2 to int
    ///
    /// \param value = String Ref16
    /// \param base = value
    ///
    /// \return int
    static int ucs2_to_int(const std::wstring &value, int base = 10);

    /// \brief Uint to text
    ///
    /// \param value = value
    ///
    /// \return Temp String
    static std::string uint_to_text(unsigned int value);

    /// \brief Uint to local8
    ///
    /// \param value = value
    ///
    /// \return Temp String8
    static std::string uint_to_local8(unsigned int value);

    /// \brief Uint to ucs2
    ///
    /// \param value = value
    ///
    /// \return Temp String16
    static std::wstring uint_to_ucs2(unsigned int value);

    static unsigned int text_to_uint(const std::string &value, int base = 10);

    static unsigned int local8_to_uint(const std::string &value, int base = 10);

    static unsigned int ucs2_to_uint(const std::wstring &value, int base = 10);

    /// \brief Long long to text
    ///
    /// \param value = value
    ///
    /// \return Temp String
    static std::string ll_to_text(long long value);

    /// \brief Long long to local8
    ///
    /// \param value = value
    ///
    /// \return Temp String8
    static std::string ll_to_local8(long long value);

    /// \brief Long long to ucs2
    ///
    /// \param value = value
    ///
    /// \return Temp String16
    static std::wstring ll_to_ucs2(long long value);

    /// \brief Text to long long
    ///
    /// \param value = String Ref
    /// \param base = value
    ///
    /// \return long long
    static long long text_to_ll(const std::string &value, int base = 10);

    /// \brief Local8 to long long
    ///
    /// \param value = String Ref8
    /// \param base = value
    ///
    /// \return long long
    static long long local8_to_ll(const std::string &value, int base = 10);

    /// \brief Ucs2 to long long
    ///
    /// \param value = String Ref16
    /// \param base = value
    ///
    /// \return long long
    static long long ucs2_to_ll(const std::wstring &value, int base = 10);

    /// \brief Unsigned long long to text
    ///
    /// \param value = value
    ///
    /// \return Temp String
    static std::string ull_to_text(unsigned long long value);

    /// \brief Unsigned long long to local8
    ///
    /// \param value = value
    ///
    /// \return Temp String8
    static std::string ull_to_local8(unsigned long long value);

    /// \brief Unsigned long long to ucs2
    ///
    /// \param value = value
    ///
    /// \return Temp String16
    static std::wstring ull_to_ucs2(unsigned long long value);

    /// \brief Text to unsigned long long
    ///
    /// \param value = String Ref
    /// \param base = value
    ///
    /// \return unsigned long long
    static unsigned long long text_to_ull(const std::string &value, int base = 10);

    /// \brief Local8 to unsigned long long
    ///
    /// \param value = String Ref8
    /// \param base = value
    ///
    /// \return unsigned long long
    static unsigned long long local8_to_ull(const std::string &value, int base = 10);

    /// \brief Ucs2 to unsigned long long
    ///
    /// \param value = String Ref16
    /// \param base = value
    ///
    /// \return unsigned long long
    static unsigned long long ucs2_to_ull(const std::wstring &value, int base = 10);

    /// \brief Bool to text
    ///
    /// \param value = bool
    ///
    /// \return Temp String
    static std::string bool_to_text(bool value);

    /// \brief Bool to local8
    ///
    /// \param value = bool
    ///
    /// \return Temp String8
    static std::string bool_to_local8(bool value);

    /// \brief Bool to ucs2
    ///
    /// \param value = bool
    ///
    /// \return Temp String16
    static std::wstring bool_to_ucs2(bool value);

    /// \brief Text to bool
    ///
    /// \param value = String Ref
    ///
    /// \return bool
    static bool text_to_bool(const std::string &value);

    /// \brief Local8 to bool
    ///
    /// \param value = String Ref8
    ///
    /// \return bool
    static bool local8_to_bool(const std::string &value);

    /// \brief Ucs2 to bool
    ///
    /// \param value = String Ref16
    ///
    /// \return bool
    static bool ucs2_to_bool(const std::wstring &value);

    /// \brief Text to cp437
    ///
    /// \param text = String Ref
    ///
    /// \return Temp String8
    static std::string text_to_cp437(const std::string &text);

    /// \brief Ucs2 to cp437
    ///
    /// \param text = String Ref16
    ///
    /// \return Temp String8
    static std::string ucs2_to_cp437(const std::wstring &text);

    /// \brief Cp437 to text
    ///
    /// \param cp437 = String Ref8
    ///
    /// \return Temp String
    static std::string cp437_to_text(const std::string &cp437);

    /// \brief Cp437 to ucs2
    ///
    /// \param cp437 = String Ref8
    ///
    /// \return Temp String16
    static std::wstring cp437_to_ucs2(const std::string &cp437);

    /// \brief Text to local8
    ///
    /// \param text = String Ref
    ///
    /// \return Temp String8
    static std::string text_to_local8(const std::string &text);

    /// \brief Text to utf8
    ///
    /// \param text = String Ref
    ///
    /// \return Temp String8
    static std::string text_to_utf8(const std::string &text);

    /// \brief Ucs2 to latin1
    ///
    /// \param ucs2 = String Ref16
    ///
    /// \return Temp String8
    static std::string ucs2_to_latin1(const std::wstring &ucs2);

    /// \brief Ucs2 to latin9
    ///
    /// \param ucs2 = String Ref16
    ///
    /// \return Temp String8
    static std::string ucs2_to_latin9(const std::wstring &ucs2);

    /// \brief Ucs2 to local8
    ///
    /// \param ucs2 = String Ref16
    ///
    /// \return Temp String8
    static std::string ucs2_to_local8(const std::wstring &ucs2);

    /// \brief Ucs2 to utf8
    ///
    /// \param ucs2 = String Ref16
    ///
    /// \return Temp String8
    static std::string ucs2_to_utf8(const std::wstring &ucs2);

    /// \brief Local8 to text
    ///
    /// \param local8 = String Ref8
    ///
    /// \return Temp String
    static std::string local8_to_text(const std::string &local8);

    /// \brief Ucs2 to text
    ///
    /// \param ucs2 = String Ref16
    ///
    /// \return Temp String
    static std::string ucs2_to_text(const std::wstring &ucs2);

    /// \brief Latin1 to ucs2
    ///
    /// \param latin1 = String Ref8
    ///
    /// \return Temp String16
    static std::wstring latin1_to_ucs2(const std::string &latin1);

    /// \brief Latin9 to ucs2
    ///
    /// \param latin9 = String Ref8
    ///
    /// \return Temp String16
    static std::wstring latin9_to_ucs2(const std::string &latin9);

    /// \brief Local8 to ucs2
    ///
    /// \param local8 = String Ref8
    ///
    /// \return Temp String16
    static std::wstring local8_to_ucs2(const std::string &local8);

    /// \brief Utf8 to ucs2
    ///
    /// \param utf8 = String Ref8
    ///
    /// \return Temp String16
    static std::wstring utf8_to_ucs2(const std::string &utf8);

    /// \brief Utf8 to text
    ///
    /// \param utf8 = String Ref8
    ///
    /// \return Temp String
    static std::string utf8_to_text(const std::string &utf8);

    static std::string::size_type utf8_length(const std::string &str);

    enum BOMType {
        bom_none,
        bom_utf32_be,
        bom_utf32_le,
        bom_utf16_be,
        bom_utf16_le,
        bom_utf8
    };

    /// \brief Detect bom
    ///
    /// \param data = void
    /// \param size_type = String
    ///
    /// \return BOMType
    static BOMType detect_bom(const void *data, std::string::size_type length);





private:
    static const char trailing_bytes_for_utf8[256];

    static const unsigned char bitmask_leadbyte_for_utf8[6];

};

}




#endif // __CACTUS_CACTUS_STRING_H__
