#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include "doc_anonymous_semaphore_shared_data.h"

#include <tracetool/tracetool.h>

using namespace boost::interprocess;

int main()
{
    //Remove shared memory on construction and destruction
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
    //Create a shared memory object.
    shared_memory_object shm
    (create_only                  //only create
     , "MySharedMemory"             //name
     , read_write //read-write mode
    );
    //Set size
    shm.truncate(sizeof(shared_memory_buffer));
    //Map the whole shared memory in this process
    mapped_region region
    (shm                       //What to map
     , read_write //Map it as read-write
    );
    //Get the address of the mapped region
    void * addr       = region.get_address();
    //Construct the shared structure in memory
    shared_memory_buffer * data = new(addr) shared_memory_buffer;
    const int NumMsg = 100;

    //Insert data in the array
    for (int i = 0; i < NumMsg; ++i) {
		TRACE_SEND_A("data->nempty.wait()");
        data->nempty.wait();
		TRACE_SEND_A("data->mutex.wait()");
        data->mutex.wait();
        data->items[i % shared_memory_buffer::NumItems] = i;
        data->mutex.post();
        data->nstored.post();
    }

    return 0;
}