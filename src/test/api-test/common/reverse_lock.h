#ifndef reverse_lock_h__
#define reverse_lock_h__

#include <exception>
#include <boost/noncopyable.hpp>

namespace cactus
{

class reverse_lock_unlocked_exception : public std::exception
{
public:
    reverse_lock_unlocked_exception()
    {
    }

    virtual const char* what() const throw()
    {
        return "lock did not locked in reverse lock";
    }
};

template<class Lock>
class reverse_lock : public boost::noncopyable
{
public:
    explicit reverse_lock(Lock & lock)
        : lock_(lock)
    {
        if (lock_.owns_lock())
            lock_.unlock();
        else
            throw reverse_lock_unlocked_exception();
    }

    ~reverse_lock()
    {
        lock_.lock();
    }
private:
    Lock & lock_;
};

}

#endif // reverse_lock_h__
