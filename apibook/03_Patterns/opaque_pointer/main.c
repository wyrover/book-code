/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
/**
 * \file   main.c
 * \author Martin Reddy
 * \brief  An opaque pointer example in plain C.
 *
 * Copyright (c) 2010, Martin Reddy. All rights reserved.
 * Distributed under the X11/MIT License. See LICENSE.txt.
 * See http://APIBook.com/ for the latest
 */

#include "autotimer.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	// create a new auto timer
	AutoTimerPtr timer = AutoTimerCreate("MyTimer");
	int i;

	for (i = 0; i < 1000; ++i)
	{
		printf(".");
	}
	printf("\n");

	// destroy the auto timer - will output its time alive
	AutoTimerDestroy(timer);

	return 0;
}
