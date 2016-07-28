#include <iostream>
#include <condition_variable>
#include <thread>

// Shared between threads:
std::mutex m;
std::condition_variable cv;
bool ready = false;

int main()
{
	std::thread t([] {
		std::unique_lock<std::mutex> lock(m);
		while (!ready) cv.wait(lock);
		// Or:
		// cv.wait(lock, [&]{ return ready; });

		std::cout << "Finished waiting in a thread.\n";

		//... access to other resources guarded by m, if any
	});

	{	// Set ready = true in the main thread:
		std::lock_guard<std::mutex> lock(m);
		ready = true;
	}
	cv.notify_all();


	t.join();


	std::cout << std::endl;
}
