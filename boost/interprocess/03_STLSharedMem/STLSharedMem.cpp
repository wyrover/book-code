#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <string>
#include <cstdlib> // std::system

using namespace boost::interprocess;

// Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
// This allocator will allow placing containers in the segment
typedef allocator<int, managed_shared_memory::segment_manager> ShmemAllocator;

// Alias a vector that uses the previous STL-like allocator so that allocates
// its values from the segment
typedef vector<int, ShmemAllocator> MyVector;

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		struct shm_remove
		{
			shm_remove()  { shared_memory_object::remove("MySharedMemory"); }
			~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		} remover;

		// Create a new segment with given name and size
		managed_shared_memory segment(create_only, "MySharedMemory", 65536);

		// Initialize shared memory STL-compatible allocator
		const ShmemAllocator alloc_inst(segment.get_segment_manager());

		// Construct a vector named "MyVector" in shared memory with argument alloc_inst
		MyVector *myvector = segment.construct<MyVector>("MyVector")(alloc_inst);

		for (int i = 0; i < 100; ++i)
		{
			myvector->push_back(i);
		}

		// Launch child process
		std::string s(argv[0]); s += " child ";
		if (0 != std::system(s.c_str()))
			return 1;

		if (segment.find<MyVector>("MyVector").first)
			return 1;
	}
	else
	{
		managed_shared_memory segment(open_only, "MySharedMemory");

		MyVector *myvector = segment.find<MyVector>("MyVector").first;

		std::sort(myvector->rbegin(), myvector->rend());

		segment.destroy<MyVector>("MyVector");
	}

	return 0;
}
