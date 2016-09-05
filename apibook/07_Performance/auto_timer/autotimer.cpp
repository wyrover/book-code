/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   autotimer.cpp
/// \author Martin Reddy
/// \brief  An object to time the lifetime of a scope.
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

class AutoTimer::Impl
{
public:
	std::string mName;
#ifdef _WIN32
	DWORD mStartTime;
#else
	struct timeval mStartTime;	
#endif
};

AutoTimer::AutoTimer(const std::string &name) :
	mImpl(new Impl)
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
	double elapsed = GetElapsed();
	std::cout << mImpl->mName << ": took " << elapsed
			  << " secs" << std::endl;
	delete mImpl;
}

double AutoTimer::GetElapsed() const
{
#ifdef _WIN32
	return (GetTickCount() - mImpl->mStartTime) / 1e3;
#else
	struct timeval end_time;
	gettimeofday(&end_time, NULL);
	double start_secs = (double)mImpl->mStartTime.tv_sec +
		(mImpl->mStartTime.tv_usec / 1000000.0);
	double end_secs = (double)end_time.tv_sec +
		(end_time.tv_usec / 1000000.0);
	return (end_secs - start_secs);
#endif
}

