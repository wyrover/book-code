#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib> // std::system
#include <sstream>

int main(int argc, char* argv[])
{
	using namespace boost::interprocess;

	if (argc == 1) // parent process
	{
		// remove shared memory on construction and destruction
		struct shm_remove
		{
			shm_remove()  { shared_memory_object::remove("MySharedMemory"); }
			~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		} remover;

		// create a managed shared memory segment
		managed_shared_memory segment(create_only, "MySharedMemory", 65536);

		// allocate a portion of the segment (raw memory)
		managed_shared_memory::size_type free_memory = segment.get_free_memory();
		void *shptr = segment.allocate(1024);

		// check invariant
		if (free_memory <= segment.get_free_memory())
			return 1;

		// an handle from the base address can identify any byte of the shared
		// memory segment even if it is mapped in different base addresses
		managed_shared_memory::handle_t handle = segment.get_handle_from_address(shptr);
		std::stringstream ss;
		ss << argv[0] << " " << handle;
		ss << std::ends;

		// launch child process
		if (0 != std::system(ss.str().c_str()))
			return 1;

		// check memory has been freed
		if (free_memory != segment.get_free_memory())
			return 1;
	}
	else
	{
		// open managed segment
		managed_shared_memory segment(open_only, "MySharedMemory");

		managed_shared_memory::handle_t handle = 0;
		std::stringstream ss; ss << argv[1]; ss >> handle;

		// get buffer local address from handle
		void *msg = segment.get_address_from_handle(handle);

		// deallocate previously allocated memory
		segment.deallocate(msg);
	}
	
	return 0;
}

