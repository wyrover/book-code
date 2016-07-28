#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>

int main()
{
	int counter = 0;
	std::mutex m;
	std::vector<std::thread> threads;        // Needs <vector> and <thread>.
	for (int t = 0; t < 4; ++t)              // Launch 4 counting threads.
		threads.emplace_back([&] {
		for (int i = 0; i < 500; ++i) {    // Count to 500 in each thread.
			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(1ms);
			std::lock_guard<std::mutex> lock(m);
			++counter;
		}
	});
	for (auto& t : threads) { t.join(); }    // Wait for all threads to finish.
	std::cout << counter << std::endl;       // 2000


	std::cout << std::endl;
}
