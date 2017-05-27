#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <stdio.h>
#include <cstdlib> //std::system

//using namespace boost::interprocess;
typedef boost::interprocess::allocator<float, boost::interprocess::managed_shared_memory::segment_manager>  ShmemAllocator;
typedef boost::interprocess::vector<float, ShmemAllocator> DataVector;
int main(int argc, char *argv[])
{
    struct shm_remove {
        shm_remove()
        {
            boost::interprocess::shared_memory_object::remove("MySharedMemory");
        }
        ~shm_remove()
        {
            boost::interprocess::shared_memory_object::remove("MySharedMemory");
        }
    } remover;
    boost::interprocess::managed_shared_memory segment(boost::interprocess::create_only, "MySharedMemory", 65536);
    const ShmemAllocator alloc_inst(segment.get_segment_manager());
    DataVector *myvector = segment.construct<DataVector>("MyVector")(alloc_inst);

    for (int i = 0; i < 6; ++i) { //Insert data in the vector
        printf("%d ", i);
        myvector->push_back(i * 3);
    }

	system("pause");

    while (1) {
        for (int i = 0; i < 6; ++i) { //Insert data in the vector
            printf("%d ", i);
            myvector->at(i) = rand() * rand();
            printf("%f ", myvector->at(i));
        }

        printf("\n");
    }



    return 0;
};