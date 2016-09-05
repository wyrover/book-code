/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   autotimer.h
/// \author Martin Reddy
/// \brief  A poor API that can be improved by Pimpl.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef AUTOTIMER_H
#define AUTOTIMER_H

#include <string>
// Bad: only need iostream because of implementation code
#include <iostream>

// Bad: exposes platform specifics in your header file
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace apibook {

class AutoTimer
{
public:
	// Bad: exposes (and inlines) implementation details
	explicit AutoTimer(const std::string &name) :
		mName(name)
	{
#ifdef _WIN32
		mStartTime = GetTickCount();
#else
		gettimeofday(&mStartTime, NULL);
#endif
	}

	~AutoTimer()
	{
		std::cout << mName << ": took " << GetElapsed()
				  << " secs" << std::endl;
	}

	// Bad: no need to expose this function publicly
	double GetElapsed() const
	{
#ifdef _WIN32
		return (GetTickCount() - mStartTime) / 1e3;
#else
		struct timeval end_time;
		gettimeofday(&end_time, NULL);
		double t1 = mStartTime.tv_usec / 1e6 + mStartTime.tv_sec;
		double t2 = end_time.tv_usec / 1e6 + end_time.tv_sec;
		return t2 - t1;
#endif
	}

	// Bad: data members should always be private
	std::string mName;
#ifdef _WIN32
	DWORD mStartTime;
#else
	struct timeval mStartTime;
#endif
};

}

#endif
