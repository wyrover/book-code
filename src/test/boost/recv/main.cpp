#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <string>
#include <stdio.h>
#include <cstdlib> //std::system

//using namespace boost::interprocess;
typedef boost::interprocess::allocator<float, boost::interprocess::managed_shared_memory::segment_manager>  ShmemAllocator;
typedef boost::interprocess::vector<float, ShmemAllocator> DataVector;

int main(int argc, char *argv[])
{
    boost::interprocess::managed_shared_memory segment(boost::interprocess::open_only, "MySharedMemory");
    DataVector *myvector = segment.find<DataVector>("MyVector").first;

    for (int i = 0; i < 6; ++i) { //Insert data in the vector
        printf("%f ", (float)myvector->at(i));
    }

    return 0;
};