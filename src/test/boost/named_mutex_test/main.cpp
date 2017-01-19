#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>

using namespace boost::interprocess;



int main(int argc, char *argv[])
{
    boost::interprocess::managed_shared_memory managed_shm(open_or_create, "shm", 1024);
    int *i = managed_shm.find_or_construct<int>("Integer")();
    named_mutex named_mtx(open_or_create, "mtx");
    named_mtx.lock();
    ++(*i);
    std::cout << *i << '\n';
    named_mtx.unlock();

	return 0;
}

