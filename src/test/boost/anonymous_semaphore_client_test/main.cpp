#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include "doc_anonymous_semaphore_shared_data.h"

using namespace boost::interprocess;

int main()
{
    //Remove shared memory on destruction
    struct shm_remove {
        ~shm_remove()
        {
            shared_memory_object::remove("MySharedMemory");
        }
    } remover;
    //Create a shared memory object.
    shared_memory_object shm
    (open_only                    //only create
     , "MySharedMemory"             //name
     , read_write //read-write mode
    );
    //Map the whole shared memory in this process
    mapped_region region
    (shm                       //What to map
     , read_write //Map it as read-write
    );
    //Get the address of the mapped region
    void * addr       = region.get_address();
    //Obtain the shared structure
    shared_memory_buffer * data = static_cast<shared_memory_buffer*>(addr);
    const int NumMsg = 100;
    int extracted_data [NumMsg];

    //Extract the data
    for (int i = 0; i < NumMsg; ++i) {
        data->nstored.wait();
        data->mutex.wait();
        extracted_data[i] = data->items[i % shared_memory_buffer::NumItems];

		std::cout << extracted_data[i] << std::endl;

        data->mutex.post();
        data->nempty.post();
    }

    return 0;
}