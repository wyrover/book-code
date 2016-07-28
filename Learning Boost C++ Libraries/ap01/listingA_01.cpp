#include <cstring>
#include <utility> // for std::swap

// dupstr returns a copy of s, allocated dynamically.
//   Sets len to the length of s.
char *dupstr(const char *str, size_t& len)
{
    char *ret = nullptr;

    if (!str) {
        len = 0;
        return ret;
    }

    len = std::strlen(str);
    ret = new char[len + 1];
    strncpy(ret, str, len + 1);
    return ret;
}

class String
{
public:
    String(const char *str = 0)
    {
        buffer_ = dupstr(str, len_);
    }

    ~String()
    {
        delete [] buffer_;
    }

    String(const String &str) : buffer_(0), len_(0)
    {
        buffer_ = dupstr(str.buffer_, len_);
    }

    String& operator=(const String& rhs)
    {
        String tmp(rhs);   // copy the rhs in a temp variable
        swap(tmp);         // swap tmp's state with this' state.
        return *this;      // tmp goes out of scope, releases this'
        // old state
    }

    void swap(String&rhs) /* noexcept */
    {
        using std::swap;
        swap(buffer_, rhs.buffer_);
        swap(len_, rhs.len_);
    }


private:
    char *buffer_;
    size_t len_;
};

int main()
{
}
