#pragma once

#include <thread>
#include <mutex>

#include "ExternalMonitor.h"
#include "InternalMonitor.h"

class Updater
{
public:
	Updater(const std::wstring MONITOR_NAME, const int DIM_STEPS);
	~Updater();

	const std::wstring MONITOR_NAME = L"LG ULTRA HD(DisplayPort)";
	const int DIM_STEPS = 5;

private:
	ExternalMonitor lg;
	InternalMonitor surface;

	std::mutex mtx;
	std::condition_variable cv;

	int internal_brightness;
	int external_brightness;
	int target_ext_bright;
	int step_size;

	void updater_thread();
};

