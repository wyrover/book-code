/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
/**
 * \file   autotimer.c
 * \author Martin Reddy
 * \brief  An object to time the lifetime of a scope.
 *
 * Copyright (c) 2010, Martin Reddy. All rights reserved.
 * Distributed under the X11/MIT License. See LICENSE.txt.
 * See http://APIBook.com/ for the latest
 */

#include "autotimer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

struct AutoTimer
{
	char *mName;
#ifdef _WIN32
	DWORD mStartTime;
#else
	struct timeval mStartTime;
#endif
} AutoTimer;

AutoTimerPtr AutoTimerCreate(const char *name)
{
	AutoTimerPtr ptr = malloc(sizeof(AutoTimer));
	if (ptr)
	{
		ptr->mName = strdup(name);
#ifdef _WIN32
		ptr->mStartTime = GetTickCount();
#else
		gettimeofday(&ptr->mStartTime, NULL);
#endif
	}
	return ptr;
}

/// Return how long the object has been alive
static double GetElapsed(AutoTimerPtr ptr)
{
#ifdef _WIN32
	return (GetTickCount() - ptr->mStartTime) / 1e3;
#else
	struct timeval end_time;
	gettimeofday(&end_time, NULL);
	double t1 = ptr->mStartTime.tv_usec / 1e6 + ptr->mStartTime.tv_sec;
	double t2 = end_time.tv_usec / 1e6 + end_time.tv_sec;
	return t2 - t1;
#endif
}

void AutoTimerDestroy(AutoTimerPtr ptr)
{
	if (ptr)
	{
		printf("%s: took %f secs\n", ptr->mName, GetElapsed(ptr));
		free(ptr);
	}
}

