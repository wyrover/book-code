#include "Updater.h"

#include <iostream>

void Updater::updater_thread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mtx);

		if (external_brightness != internal_brightness)
		{
			external_brightness += step_size;
			if ((step_size > 0 && external_brightness > target_ext_bright) || (step_size < 0 && external_brightness < target_ext_bright))
				external_brightness = internal_brightness;
			lg.set_brightness(external_brightness);
		}
		else
		{
			lock.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

Updater::Updater(const std::wstring MONITOR_NAME, const int DIM_STEPS) :
	MONITOR_NAME(MONITOR_NAME),
	DIM_STEPS(DIM_STEPS),
	lg(ExternalMonitor(MONITOR_NAME))
{
	internal_brightness = surface.getBrightness();
	external_brightness = internal_brightness;
	target_ext_bright = external_brightness;

	lg.set_brightness(target_ext_bright);

	std::thread t(&Updater::updater_thread, this);

	while (true)
	{
		std::unique_lock<std::mutex> lock(mtx);

		internal_brightness = surface.getBrightness();

		if (target_ext_bright != internal_brightness)
		{
			target_ext_bright = internal_brightness;
			double s = (target_ext_bright - external_brightness) / (double) DIM_STEPS;
			if (s > 0)
				step_size = static_cast<int>(std::ceil(s));
			else
				step_size = static_cast<int>(std::floor(s));
			//std::cout << "step size " << s << " --> " << step_size << std::endl;
		}

		lock.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	t.join();
}


Updater::~Updater()
{
}
