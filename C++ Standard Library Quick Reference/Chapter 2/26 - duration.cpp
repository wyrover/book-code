#include <iostream>
#include <chrono>

int main()
{
	using namespace std::chrono;
	typedef duration<int, std::ratio<3600>> hours_t;
	typedef duration<int64_t, std::milli> millisecs_t;		// milli == ratio<1,1000>
	const hours_t one_hour(1);
	const millisecs_t ms(one_hour);
	std::cout << "1h = " << ms.count() << "ms" << '\n';		// 1h = 3600000ms


	// const hours_t back_to_hours(ms);  <-- error (int64_t would be truncated)
	const auto back_to_hours = duration_cast<hours_t>(ms);


	const auto secs = duration_cast<seconds>(0.5h);
	std::cout << "0.5h = " << secs.count() << "s" << '\n';	// 0.5h = 1800s


	const auto result = duration_cast<minutes>((12min + .5h) / 2 + (100ns >= 1ms ? -3h : ++59s));
	std::cout << result.count();


	std::cout << std::endl;
}
