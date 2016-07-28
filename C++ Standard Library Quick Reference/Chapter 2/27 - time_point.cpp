#include <iostream>
#include <chrono>

int main()
{
	using namespace std::chrono;

	time_point<system_clock, hours> one_hour(1h);     // 1h since epoch
	time_point<system_clock, minutes> sixty_minutes = one_hour;
	std::cout << (one_hour - sixty_minutes).count() << std::endl;  // 0


	auto one_hour2 = time_point_cast<hours>(sixty_minutes);


	std::cout << std::endl;
}
