#ifndef string_h__
#define string_h__

#include <string>
#include <vector>
#include <list>

namespace cactus
{

// convert UTF8 to UNICODE
std::wstring UTF8string2wstring(const char * s);
std::wstring UTF8string2wstring(const std::string& s);
// convert ASCII to UNICODE
std::wstring string2wstring(const std::string& s);
std::wstring string2wstring(const char * s);
// convert UNICODE to ASCII (CP_ACP)
std::string wstring2string(const wchar_t * s);
std::string wstring2string(const std::wstring& s);
// convert UTF8 to ASCII
std::string UTF8string2string(const char * s);
std::string UTF8string2string(const std::string& s);

// convert a string representation of boolean
bool string2bool(const char *, bool defaultvalue = false);
bool string2bool(const std::string&, bool defaultvalue = false);
bool wstring2bool(const wchar_t *, bool defaultvalue = false);
bool wstring2bool(const std::wstring&, bool defaultvalue = false);

// convert a string representation of long
long string2long(const std::string& ss, int base = 10);
long wstring2long(const std::wstring& ss, int base = 10);

// convert a string representation of unsigned long
unsigned long string2ulong(const std::string& ss, int base = 10);
unsigned long wstring2ulong(const std::wstring& ss, int base = 10);

// perform a string replace on every instance
std::string replace(const std::string& s, const std::string& from, const std::string& to);
std::wstring replace(const std::wstring& s, const std::wstring& from, const std::wstring& to);

// tokenize a given string using a multi-byte delimiter
std::vector<std::string> split(const std::string& ss, const std::string& delims, int max = -1);
std::vector<std::wstring> split(const std::wstring& ss, const std::wstring& delims, int max = -1);

// join a vector using a multi-byte delimiter
std::string join(const std::vector<std::string>& tokens, const std::string& delims);
std::wstring join(const std::vector<std::wstring>& tokens, const std::wstring& delims);
std::string join(const std::list<std::string>& tokens, const std::string& delims);
std::wstring join(const std::list<std::wstring>& tokens, const std::wstring& delims);

// left trim
std::string trimleft(const std::string& s, const std::string& whitespaces = " \t\n\r");
std::wstring trimleft(const std::wstring& s, const std::wstring& whitespaces = L" \t\n\r");

// right trim
std::string trimright(const std::string& s, const std::string& whitespaces = " \t\n\r");
std::wstring trimright(const std::wstring& s, const std::wstring& whitespaces = L" \t\n\r");

// left and right trim
std::string trim(const std::string& s, const std::string& whitespaces = " \t\n\r");
std::wstring trim(const std::wstring& s, const std::wstring& whitespaces = L" \t\n\r");

// returns true if a string starts with (case-sensitive) another string
bool startswith(const std::string& ss, const std::string& what);
bool startswith(const std::wstring& ss, const std::wstring& what);

// returns true if a string ends with (case-sensitive) another string
bool endswith(const std::string& ss, const std::string& what);
bool endswith(const std::wstring& ss, const std::wstring& what);

// convert any streamable data into a UNICODE string
template<class T>
std::wstring towstring(const T& t)
{
    std::wstringstream ss;
    ss << t;
    return ss.str();
}

// convert any streamable data into an ANSI string
template<class T>
std::string tostring(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}


template<class T>
class StringUtilImpl
{
public:

    static std::basic_string<T> replace(const std::basic_string<T>& s_in, const std::basic_string<T>& from, const std::basic_string<T>& to)
    {
        std::basic_string<T> s(s_in);
        std::basic_string<T>::size_type ii = 0;

        if (s.length() != 0 && from.length() != 0) {
            int result = 0; // \todo: return number of replacements

            while ((ii = s.find(from, ii)) != s.npos) {
                s.replace(ii, from.length(), to);
                ii += (abs((int)(to.length() - from.length())) + 1);
                result ++;
            }
        }

        return s;
    }

    static std::vector<std::basic_string<T>> split(const std::basic_string<T>& ss, const std::basic_string<T>& delim, int max)
    {
        std::vector<std::basic_string<T>> tokens;
        std::basic_string<T>::size_type begin = 0, end = 0;
        std::basic_string<T> temp;

        if (max == 1 || delim.empty()) {
            throw std::exception("Invalid parameter");
        }

        temp = ss;

        while ((end = temp.find(delim)) != std::basic_string<T>::npos) {
            tokens.push_back(temp.substr(0, end));
            begin = end + delim.length();
            temp = temp.substr(begin, temp.length() - begin);

            if (static_cast<int>(tokens.size() + 1) == max)
                break;
        }

        tokens.push_back(temp);
        return tokens;
    }

    static std::basic_string<T> join(const std::vector<std::basic_string<T>>& tokens, const std::basic_string<T>& delim)
    {
        std::basic_string<T> ss;

        for (unsigned int i = 0; i < tokens.size(); i++) {
            if (i > 0) ss.append(delim);

            ss.append(tokens[i]);
        }

        return ss;
    }

    static std::basic_string<T> join(const std::list<std::basic_string<T>>& tokens, const std::basic_string<T>& delim)
    {
        std::basic_string<T> ss;
        int i = 0;

        for each(const std::basic_string<T>& token in tokens) {
            if (i > 0) ss.append(delim);

            ss.append(token);
            i++;
        }

        return ss;
    }

    static std::basic_string<T> trimleft(const std::basic_string<T>& ss_in, const std::basic_string<T>& whitespaces)
    {
        std::basic_string<T> ss(ss_in);
        ss.erase(0, ss.find_first_not_of(whitespaces));
        return ss;
    }

    static std::basic_string<T> trimright(const std::basic_string<T>& ss_in, const std::basic_string<T>& whitespaces)
    {
        std::basic_string<T> ss(ss_in);
        ss.erase(ss.find_last_not_of(whitespaces) + 1);
        return ss;
    }

    static std::basic_string<T> trim(const std::basic_string<T>& ss, const std::basic_string<T>& whitespaces)
    {
        return trimleft(trimright(ss, whitespaces), whitespaces);
    }

    static bool startswith(const std::basic_string<T>& what, const std::basic_string<T>& with)
    {
        if (what.length() < with.length())
            return false;

        if (what.substr(0, with.length()) == with)
            return true;

        return false;
    }

    static bool endswith(const std::basic_string<T>& what, const std::basic_string<T>& with)
    {
        if (what.length() < with.length())
            return false;

        if (what.substr(what.length() - with.length(), what.length()) == with)
            return true;

        return false;
    }
};

}
#endif // string_h__
