#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/atomic.hpp>
#include <iostream>

typedef boost::atomic<int> atomic_t;
#if (BOOST_ATOMIC_INT_LOCK_FREE != 2)
    // 检查atomic_t不会使用互斥量。因为通常情况下，互斥量不会在共享内存中工作。
    // 因此，如果BOOST_ATOMIC_INT_LOCK_FREE不等于2，会得到一个未定义的行为。
    #error "This code requires lock-free boost::atomic<int>"
#endif

//#include <atomic>
//typedef std::atomic<int> atomic_t;

int main()
{
    boost::interprocess::managed_shared_memory
    segment(boost::interprocess::open_or_create, "shm1-cache", 1024);
    atomic_t& atomic
        = *segment.find_or_construct<atomic_t> //1
          ("shm-counter") //2
          (0)             //3
          ;
    std::cout << "I have index " << ++atomic << "\nPress any key...";
    std::cin.get();
    int snapshot = --atomic;

    if (!snapshot) {
        segment.destroy<atomic_t>("shm-counter");
        boost::interprocess::shared_memory_object::remove("shm1-cache");
    }

    system("pause");
    return 0;
}