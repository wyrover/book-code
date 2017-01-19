#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <iostream>

int main()
{
    boost::interprocess::managed_shared_memory managed_shm(boost::interprocess::open_or_create, "shm", 1024);
    int *i = managed_shm.find_or_construct<int>("Integer")();
    boost::interprocess::interprocess_mutex *mtx = managed_shm.find_or_construct<boost::interprocess::interprocess_mutex>("mtx")();
    mtx->lock();
    ++(*i);
    std::cout << *i << std::endl;
    mtx->unlock();
    
    return 0;
}