#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib> // std::system
#include <cstddef>
#include <cassert>
#include <utility>

int main(int argc, char *argv[])
{
	using namespace boost::interprocess;
	typedef std::pair<double, int> MyType;

	if (argc == 1) { // Parent process
		// Remove shared memory on construction and destruction
		struct shm_remove
		{
			shm_remove()  { shared_memory_object::remove("MySharedMemory"); }
			~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
		} remover;

		// Construct managed shared memory
		managed_shared_memory segment(create_only, "MySharedMemory", 65536);

		// Create an object of MyType initialized to {0.0, 0}
		MyType *instance = segment.construct<MyType>
			("MyType instance")  // name of the object
			(0.0, 0);            // ctor first argument

		// Create an array of 10 elements of MyType initialized to {0.0, 0}
		MyType *array = segment.construct<MyType>
			("MyType array")     // name of the object
			[10]                 // number of elements
			(0.0, 0);            // Same two ctor arguments for all objects

		// Create an array of 3 elements of MyType initializing each one
		// to a different value {0.0, 0}, {1.0, 1}, {2.0, 2} ...
		float float_initializer[3] = {0.0, 1.0, 2.0};
		int int_initializer[3] = {0, 1, 2};

		MyType *array_it = segment.construct_it<MyType>
			("MyType array from it")  // name of the object
			[3]                       // number of elements
			(&float_initializer[0], &int_initializer[0]);

		// Launch child process
		std::string s(argv[0]); s += " child ";
		if (0 != std::system(s.c_str()))
			return 1;

		// Check child has destroyed all objects
		if (segment.find<MyType>("MyType array").first ||
			segment.find<MyType>("MyType instance").first ||
			segment.find<MyType>("MyType array from it").first)
			return 1;
	}
	else
	{
		managed_shared_memory segment(open_only, "MySharedMemory");

		std::pair<MyType*, managed_shared_memory::size_type> res;

		res = segment.find<MyType>("MyType array");
		if (res.second != 10) return 1;

		res = segment.find<MyType>("MyType instance");
		if (res.second != 1) return 1;

		res = segment.find<MyType>("MyType array from it");
		if (res.second != 3) return 1;

		segment.destroy<MyType>("MyType array");
		segment.destroy<MyType>("MyType instance");
		segment.destroy<MyType>("MyType array from it");
	}

	return 0;
}