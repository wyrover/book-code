#define _CRT_SECURE_NO_WARNINGS	// To make sure ctime() works with Visual C++.

#include <iostream>
#include <chrono>
#include <ctime>

int main()
{
	using std::chrono::steady_clock;
	const steady_clock::time_point before = steady_clock::now();
	std::cout << steady_clock::period::num << '/'    /* Possible output: */
		<< steady_clock::period::den << '\n';         // 1/1000000000
	std::cout << (steady_clock::now() - before).count() << '\n';    // 34721


	using std::chrono::system_clock;
	const auto now = system_clock::now();             /* Possible output: */
	const time_t now_time_t = system_clock::to_time_t(now);
	std::cout << now.time_since_epoch().count() << '\n';    // 1445470140000
	std::cout << ctime(&now_time_t) << '\n';     // Wed Oct 21 16:29:00 2015


	std::cout << std::endl;
}
