#include <boost/move/move.hpp>
#include <boost/swap.hpp>
#include <cstring>

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
private:
    BOOST_COPYABLE_AND_MOVABLE(String);

public:
    // Constructor
    String(const char *str = 0)
    {
        buffer_ = dupstr(str, size_);
    }

    // Destructor
    ~String();

    // Copy constructor
    String(const String& that) : buffer_(0), size_(0)
    {
        buffer_ = dupstr(that.buffer_, size_);
    }

    // Copy assignment operator
    String& operator=(BOOST_COPY_ASSIGN_REF(String) rhs);

    // Move constructor
    String(BOOST_RV_REF(String) that);

    // Move assignment
    String& operator=(BOOST_RV_REF(String) rhs);

    void swap(String& that);

private:
    char *buffer_;
    size_t size_;
};

// Copy assignment operator
String& String::operator=(BOOST_COPY_ASSIGN_REF(String)rhs)
{
    String tmp(rhs);
    swap(tmp);        // calls String::swap member
    return *this;
}

// Move constructor
String::String(BOOST_RV_REF(String) that) : buffer_(0),
    size_(0)
{
    swap(that);      // calls String::swap member
}
// Move assignment operator
String& String::operator=(BOOST_RV_REF(String)rhs)
{
    swap(rhs);
    String tmp;
    rhs.swap(tmp);
    return *this;
}

void String::swap(String& that)
{
    boost::swap(buffer_, that.buffer_);
    boost::swap(size_, that.size_);
}

String::~String()
{
    delete [] buffer_;
}

int main()
{
}
