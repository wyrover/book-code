#include <boost/move/move.hpp>
#include <boost/swap.hpp>
#include <cstring>

class String
{
private:
    BOOST_MOVABLE_BUT_NOT_COPYABLE(String)

public:
    String(const char *s = nullptr) : str(nullptr), sz(0)
    {
        str = heapcpy(s, sz);
    }

    ~String()
    {
        delete[] str;
        sz = 0;
    }

    String(BOOST_RV_REF(String) that) : str(nullptr), sz(0)
    {
        swap(that);
    }

    String& operator = (BOOST_RV_REF(String) rhs)
    {
        String tmp(boost::move(rhs));
        return *this;
    }

    void swap(String& rhs)
    {
        boost::swap(this->sz, rhs.sz);
        boost::swap(this->str, rhs.str);
    }

    const char *get() const
    {
        return str;
    }

private:
    char *str;
    size_t sz;

    static char *heapcpy(const char *str, size_t& sz)
    {
        char *ret = nullptr;

        if (str) {
            sz = std::strlen(str) + 1;
            ret = new char[sz];
            std::strncpy(ret, str, sz);
        }

        return ret;
    }
};

int main()
{
}
