/*!
* \file cactus_pattern.h
* \brief 模式声明
* 
* 模式声明
* 
* \author wangyang 
* \date 2015/02/04 
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_PATTERN_H__
#define __CACTUS_CACTUS_PATTERN_H__

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <stdlib.h> // atexit

namespace cactus
{

template<typename T>
class Singleton : boost::noncopyable
{
public:
    static T& instance()
    {
		if (!value_)
		{
			boost::mutex::scoped_lock lock(mutex_);			
			Singleton::init();			
		}
		return *value_;       
    }

private:
    Singleton();
    ~Singleton();

    static void init()
    {
        value_ = new T();
        ::atexit(destroy);
    }

    static void destroy()
    {
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy;
        (void) dummy;
        delete value_;
    }

private:
    static long           once_init_;
    static T*             value_;
	static boost::mutex   mutex_;
};

template<typename T>
long Singleton<T>::once_init_ = 0;

template<typename T>
T* Singleton<T>::value_ = NULL;

template<typename T>
boost::mutex Singleton<T>::mutex_;


} // namespace cactus

#endif // __CACTUS_CACTUS_PATTERN_H__
