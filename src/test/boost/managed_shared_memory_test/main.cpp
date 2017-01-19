#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <string>
#include <stdio.h>
#include <cstdlib> //std::system
#include <tracetool/tracetool.h>

int funnyfunc(int a)
{
    return a * 4;
}

using namespace boost::interprocess;
typedef boost::interprocess::allocator<int, managed_shared_memory::segment_manager>  ShmemAllocator;
typedef boost::container::vector<int, ShmemAllocator> MyVector;

int main(int argc, char *argv[])
{
    if (argc == 1) {
        struct shm_remove {
            shm_remove()
            {
                shared_memory_object::remove("MySharedMemory");
            }
            ~shm_remove()
            {
                shared_memory_object::remove("MySharedMemory");
            }
        } remover;
        managed_shared_memory segment(create_only, "MySharedMemory", 65536);
        const ShmemAllocator alloc_inst(segment.get_segment_manager());
        MyVector *myvector = segment.construct<MyVector>("MyVector")(alloc_inst);

        for (int i = 0; i < 100; ++i) //Insert data in the vector
            myvector->push_back(i);

        std::string s(argv[0]);
        s += " child ";

        if (0 != std::system(s.c_str()))
            return 1;

        if (segment.find<MyVector>("MyVector").first)
            return 1;
    } else {
		
		TRACE_SEND_A("child process");
		
        managed_shared_memory segment(open_only, "MySharedMemory");
        MyVector *myvector = segment.find<MyVector>("MyVector").first;
        std::sort(myvector->rbegin(), myvector->rend());

        for (int i = 0; i < 100; ++i) { //Insert data in the vector
            printf("%d ", myvector->at(i));
        }

        segment.destroy<MyVector>("MyVector");
    }

    return 0;
};