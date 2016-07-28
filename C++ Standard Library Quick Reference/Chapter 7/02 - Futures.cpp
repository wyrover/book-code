#include <iostream>
#include <future>

int gcd(int x, int y) { return y ? gcd(y, x % y) : x; } // Euclid's algorithm

int main()
{
	{
		// std::async
		std::future<int> answer = std::async(gcd, 123, 6);
		// ...
		std::cout << answer.get() << '\n'; // 3 (greatest common divisor of 123 and 6)
	}


	{
		// std::packaged_task
		std::packaged_task<int(int, int)> gcd_task(gcd);
		std::future<int> gcd_future = gcd_task.get_future();
		std::thread worker(std::move(gcd_task), 8, 12);
		worker.detach();
		// ...
		const int four = gcd_future.get();
		std::cout << four << '\n';
	}


	{
		// make_ready_at_thread_exit
		std::packaged_task<int(int, int)> gcd_task(gcd);
		std::thread worker([&] { gcd_task.make_ready_at_thread_exit(8, 12); });
		worker.detach();
		// ...
		const int four = gcd_task.get_future().get();
		std::cout << four << '\n';
	}


	{
		// std::promise
		std::promise<int> gcd_promise;
		std::thread worker([&] { gcd_promise.set_value(gcd(121, 22)); });
		worker.detach();
		// ...
		const int eleven = gcd_promise.get_future().get();
		std::cout << eleven << '\n';
	}


	std::cout << std::endl;
}
