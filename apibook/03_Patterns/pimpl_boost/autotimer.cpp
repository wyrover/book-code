/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   autotimer.cpp
/// \author Martin Reddy
/// \brief  An example of Pimpl with a smart pointer.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "autotimer.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace apibook {

class AutoTimer::Impl
{
public:

	/// Return how long the object has been alive
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

	std::string mName;
#ifdef _WIN32
	DWORD mStartTime;
#else
	struct timeval mStartTime;
#endif
};

AutoTimer::AutoTimer(const std::string &name) :
	mImpl(new AutoTimer::Impl())
{
	mImpl->mName = name;

#ifdef _WIN32
	mImpl->mStartTime = GetTickCount();
#else
	gettimeofday(&mImpl->mStartTime, NULL);
#endif
}

AutoTimer::~AutoTimer()
{
	std::cout << mImpl->mName << ": took " << mImpl->GetElapsed()
			  << " secs" << std::endl;
}

}
